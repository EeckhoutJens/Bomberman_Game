#include "stdafx.h"
#include "BombPowerUp.h"
#include "Components.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "GameScene.h"

BombPowerUp::BombPowerUp(DirectX::XMFLOAT3 pos, UINT matID)
	:m_bombPUMat(matID)
	,m_Pos(pos)
{}

void BombPowerUp::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto powerUpPhysicsMat = physX->createMaterial(1, 1, 0);

	auto powerUpModel = new ModelComponent{ L"Resources/Meshes/PowerUp.ovm" };
	powerUpModel->SetMaterial(m_bombPUMat);
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
	pModelObject->GetTransform()->Translate(0, m_YPosOffset, 0);
	pModelObject->GetTransform()->Rotate(90, -90, 0);
	GetTransform()->Translate(m_Pos);
	GetTransform()->Scale(m_Scale, m_Scale, m_Scale);

}

void BombPowerUp::Update(const GameContext& gameContext)
{
	auto prevRot = GetChild<GameObject>()->GetTransform()->GetRotation();
	GetChild<GameObject>()->GetTransform()->Rotate(90, 40 * gameContext.pGameTime->GetTotal(),prevRot.z, true);
}
