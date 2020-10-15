#include "stdafx.h"
#include "DestructableBox.h"
#include "Components.h"
#include "PhysxManager.h"

DestructableBox::DestructableBox(DirectX::XMFLOAT3 pos, UINT matID)
	:m_MatID{ matID }
	, m_Pos{pos}
{
}

void DestructableBox::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto boxPhysicsMat = physX->createMaterial(0, 0, 0);

	auto pDestructableModel = new ModelComponent(L"./Resources/Meshes/WoodenBox.ovm");
	auto rigidBodyBox = new RigidBodyComponent(true);
	rigidBodyBox->SetCollisionGroup(CollisionGroupFlag::Group1);
	std::shared_ptr<physx::PxGeometry> boxGeom(new physx::PxBoxGeometry(m_GeomScale, m_GeomScale, m_GeomScale));
	auto pBoxCollider = new ColliderComponent(boxGeom, *boxPhysicsMat);

	pDestructableModel->SetMaterial(m_MatID);

	GetTransform()->Translate(m_Pos.x, m_Pos.y, m_Pos.z);
	GetTransform()->Scale(m_Scale, m_Scale, m_Scale);
	AddComponent(pDestructableModel);
	AddComponent(rigidBodyBox);
	AddComponent(pBoxCollider);
}

void DestructableBox::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
