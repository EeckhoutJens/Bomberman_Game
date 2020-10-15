#include "stdafx.h"
#include "IndestructableBox.h"
#include "PhysxManager.h"
#include "Components.h"

IndestructableBox::IndestructableBox(DirectX::XMFLOAT3 pos, UINT matID)
	:m_MatID{ matID }
	, m_Pos{ pos }
{
}

void IndestructableBox::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto boxPhysicsMat = physX->createMaterial(0, 0, 0);

	auto pIndestructableModel = new ModelComponent(L"./Resources/Meshes/WoodenBox.ovm");
	auto rigidBodyBox = new RigidBodyComponent(true);
	rigidBodyBox->SetCollisionGroup(CollisionGroupFlag::Group1);
	std::shared_ptr<physx::PxGeometry> boxGeom(new physx::PxBoxGeometry(m_GeomScale, m_GeomScale, m_GeomScale));
	auto pBoxCollider = new ColliderComponent(boxGeom, *boxPhysicsMat);

	pIndestructableModel->SetMaterial(m_MatID);

	GetTransform()->Translate(m_Pos.x, m_Pos.y, m_Pos.z);
	GetTransform()->Scale(m_Scale, m_Scale, m_Scale);
	AddComponent(pIndestructableModel);
	AddComponent(rigidBodyBox);
	AddComponent(pBoxCollider);
}

void IndestructableBox::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
