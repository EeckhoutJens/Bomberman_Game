#include "stdafx.h"
#include "RangePowerUp.h"
#include "Components.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "GameScene.h"

RangePowerUp::RangePowerUp(DirectX::XMFLOAT3 pos, UINT matID)
	:m_RangePUMat(matID)
	,m_Pos(pos)
{
}

void RangePowerUp::Initialize(const GameContext&)
{
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto powerUpPhysicsMat = physX->createMaterial(1, 1, 0);

	auto powerUpModel = new ModelComponent{ L"Resources/Meshes/PowerUp.ovm" };
	powerUpModel->SetMaterial(m_RangePUMat);
	auto rigidBody = new RigidBodyComponent(true);
	rigidBody->SetCollisionGroup(CollisionGroupFlag::Group5);
	AddComponent(rigidBody);
	std::shared_ptr<physx::PxGeometry> powerUpGeom(new physx::PxBoxGeometry(3.5f, 3.5f, 3.5f));
	auto pPowerUpCollider = new ColliderComponent(powerUpGeom, *powerUpPhysicsMat);
	pPowerUpCollider->EnableTrigger(true);
	AddComponent(pPowerUpCollider);
	auto pModelObject = new GameObject();
	pModelObject->AddComponent(powerUpModel);
	AddChild(pModelObject);
	pModelObject->GetTransform()->Translate(0, -7.75, 0);
	pModelObject->GetTransform()->Rotate(90, -90, 0);
	GetTransform()->Translate(m_Pos);
	GetTransform()->Scale(0.2f, 0.2f, 0.2f);
}

void RangePowerUp::Update(const GameContext& gameContext)
{
	auto prevRot = GetChild<GameObject>()->GetTransform()->GetRotation();
	GetChild<GameObject>()->GetTransform()->Rotate(90, 40 * gameContext.pGameTime->GetTotal(), prevRot.z, true);
}
