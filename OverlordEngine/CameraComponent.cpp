#include "stdafx.h"
#include "CameraComponent.h"
#include "OverlordGame.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "PhysxProxy.h"
#include "GameObject.h"
#include "GameScene.h"

CameraComponent::CameraComponent() :
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(DirectX::XM_PIDIV4),
	m_Size(25.0f),
	m_IsActive(true),
	m_PerspectiveProjection(true)
{
	XMStoreFloat4x4(&m_Projection, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, DirectX::XMMatrixIdentity());
}

void CameraComponent::Initialize(const GameContext&) {}

void CameraComponent::Update(const GameContext&)
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	const auto windowSettings = OverlordGame::GetGameSettings().Window;
	DirectX::XMMATRIX projection;

	if (m_PerspectiveProjection)
	{
		projection = DirectX::XMMatrixPerspectiveFovLH(m_FOV, windowSettings.AspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size > 0) ? m_Size * windowSettings.AspectRatio : windowSettings.Width;
		const float viewHeight = (m_Size > 0) ? m_Size : windowSettings.Height;
		projection = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const DirectX::XMVECTOR worldPosition = DirectX::XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const DirectX::XMVECTOR lookAt = DirectX::XMLoadFloat3(&GetTransform()->GetForward());
	const DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&GetTransform()->GetUp());

	const DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const DirectX::XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const DirectX::XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::Draw(const GameContext&) {}

void CameraComponent::SetActive()
{
	auto gameObject = GetGameObject();
	if (gameObject == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game object is null");
		return;
	}

	auto gameScene = gameObject->GetScene();
	if (gameScene == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game scene is null");
		return;
	}

	gameScene->SetActiveCamera(this);
}

GameObject* CameraComponent::Pick(const GameContext& gameContext, CollisionGroupFlag ignoreGroups) const
{
	//TODO implement
	//Transform coordinates to NDC
	auto tempPos = gameContext.pInput->GetMousePosition();
	DirectX::XMFLOAT2 MousePos{ float(tempPos.x),float(tempPos.y) };
	DirectX::XMVECTOR NDCMouse{};
	NDCMouse = DirectX::XMLoadFloat2(&MousePos);
	DirectX::XMMATRIX viewProjectionInverseMatrix = DirectX::XMLoadFloat4x4(&GetViewProjectionInverse());
	DirectX::XMVector2TransformCoord(NDCMouse, viewProjectionInverseMatrix);
	DirectX::XMStoreFloat2(&MousePos, NDCMouse);

	//calculate halfwidth / halfheight
	float halfWidth = OverlordGame::GetGameSettings().Window.Width / 2.f;
	float halfHeight = OverlordGame::GetGameSettings().Window.Height / 2.f;
	MousePos.x = (MousePos.x - halfWidth) / halfWidth;
	MousePos.y = (halfHeight - MousePos.y) / halfHeight;

	//calculate near and far point
	DirectX::XMFLOAT2 nearPoint;
	DirectX::XMFLOAT2 farPoint;
	DirectX::XMFLOAT3* floatNear{ new DirectX::XMFLOAT3(float(MousePos.x), float(MousePos.y), 0) };
	DirectX::XMFLOAT3* floatFar{ new DirectX::XMFLOAT3(float(MousePos.x),float(MousePos.y),1) };
	DirectX::XMVECTOR vectorNear = DirectX::XMLoadFloat3(floatNear);
	DirectX::XMVECTOR vectorFar = DirectX::XMLoadFloat3(floatFar);
	DirectX::XMMATRIX viewProjectionMatrix = DirectX::XMLoadFloat4x4(&GetViewProjectionInverse());
	vectorNear = DirectX::XMVector3TransformCoord(vectorNear, viewProjectionMatrix);
	vectorFar = DirectX::XMVector3TransformCoord(vectorFar, viewProjectionMatrix);
	DirectX::XMStoreFloat3(floatNear, vectorNear);
	DirectX::XMStoreFloat3(floatFar, vectorFar);
	nearPoint.x = floatNear->x;
	nearPoint.y = floatNear->y;
	farPoint.x = floatFar->x;
	farPoint.y = floatFar->y;

	//raycast
	physx::PxQueryFilterData filterData;
	filterData.data.word0 = physx::PxU32(ignoreGroups);

	physx::PxRaycastBuffer hit;

	physx::PxVec3 finalNear{};
	physx::PxVec3 finalFar{};
	physx::PxVec3 finalDir{};

	finalNear.x = float(nearPoint.x);
	finalNear.y = float(nearPoint.y);
	finalNear.z = 0;

	finalFar.x = float(farPoint.x);
	finalFar.y = float(farPoint.y);
	finalFar.z = 1;

	finalDir = finalFar - finalNear;

	finalDir.normalize();
	delete floatNear;
	delete floatFar;
	if (this->GetGameObject()->GetScene()->GetPhysxProxy()->Raycast(finalNear, finalDir, PX_MAX_F32, hit, physx::PxHitFlag::eDEFAULT, filterData))
	{
		auto actor = &hit.getAnyHit(0).actor->userData;
		RigidBodyComponent* component = static_cast<RigidBodyComponent*>(*actor);
		return component->GetGameObject();
	}
	return nullptr;
}