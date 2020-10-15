#include "stdafx.h"
#include "Bomb.h"
#include "Components.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "../Scenes/ExamScene.h"
#include "../Level/IndestructableBox.h"

Bomb::Bomb(DirectX::XMFLOAT3 pos, float range, UINT matID)
	:m_CurrTime{0}
	,m_Pos{pos}
	,m_MatID{matID}
	,m_Range{range}
	,m_pRigid{nullptr}
	,m_pSparkEmitter{nullptr}
{
}

void Bomb::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	const auto bombPhysicsMat = physX->createMaterial(1, 1, 0);

	//SETUP MODEL
	ModelComponent* bombModel = new ModelComponent{ L"Resources/Meshes/Bomb.ovm" };
	bombModel->SetMaterial(m_MatID);
	GameObject* pModelObject = new GameObject();
	pModelObject->AddComponent(bombModel);

	//SETUP RIGID
	m_pRigid = new RigidBodyComponent(false);
	std::shared_ptr<physx::PxGeometry> bombGeom(new physx::PxSphereGeometry(3.5f));
	ColliderComponent* pBombCollider = new ColliderComponent(bombGeom, *bombPhysicsMat);
	m_pRigid->SetKinematic(true);
	m_pRigid->SetCollisionGroup(CollisionGroupFlag::Group1);

	//SETUP SPARK EMITTER
	m_pSparkObject = new GameObject();
	m_pSparkEmitter = new ParticleEmitterComponent(L"./Resources/Textures/SparkParticle.png",10);
	m_pSparkEmitter->SetVelocity(DirectX::XMFLOAT3(0.5f, 1, 0));
	m_pSparkEmitter->SetMinSize(1.f);
	m_pSparkEmitter->SetMaxSize(1.5f);
	m_pSparkEmitter->SetMinSizeGrow(1.75f);
	m_pSparkEmitter->SetMaxSizeGrow(2.25f);
	m_pSparkEmitter->SetMinEmitterRange(0.4f);
	m_pSparkEmitter->SetMaxEmitterRange(1.f);
	m_pSparkEmitter->SetColor(DirectX::XMFLOAT4(1.f, 0.5f, 0.3f, 0.6f));
	m_pSparkObject->AddComponent(m_pSparkEmitter);


	//FINISH UP INITIALIZE
	AddChild(pModelObject);
	AddChild(m_pSparkObject);
	AddComponent(m_pRigid);
	AddComponent(pBombCollider);
	GetTransform()->Scale(0.5f, 0.5f, 0.5f);
	GetTransform()->Translate(m_Pos);
	pModelObject->GetTransform()->Translate(0, -7.75, 0);
	m_pSparkObject->GetTransform()->Translate(m_Pos.x, m_Pos.y + 5.75f,m_Pos.z);
}

void Bomb::Update(const GameContext& gameContext)
{
	//UPDATE PARTICLE
	m_pSparkObject->GetTransform()->Translate(m_Pos.x, m_Pos.y + m_ParticleOffset, m_Pos.z);
	
	//CHECK IF BOMBPUSH HAS BEEN FIRED FROM PLAYER CHARACTER
	if (m_MoveBomb && !m_WasAlreadyPushed)
	{
		ExecuteBombPush();
	}

	//UPDATE BOMB
	m_Pos = GetTransform()->GetPosition();
	m_CurrTime += gameContext.pGameTime->GetElapsed();
	if (m_CurrTime >= m_MaxTime)
		ExecuteRaycast();
}

void Bomb::ExecuteRaycast()
{

	physx::PxVec3 dirUp{ 0,0,1 }, dirDown{ 0,0,-1 }, dirLeft{ -1,0,0 }, dirRight{ 1,0,0 };
	physx::PxVec3 pos{ m_Pos.x + m_PosOffset,m_Pos.y,m_Pos.z };
	physx::PxVec3 pos2{ m_Pos.x - m_PosOffset,m_Pos.y,m_Pos.z };
	physx::PxVec3 pos3{ m_Pos.x,m_Pos.y,m_Pos.z + m_PosOffset };
	physx::PxVec3 pos4{ m_Pos.x,m_Pos.y,m_Pos.z - m_PosOffset };

	XRayCast(pos, dirRight);
	XRayCast(pos2, dirLeft);
	ZRayCast(pos3, dirUp);
	ZRayCast(pos4, dirDown);
	m_DestroyBomb = true;
}

void Bomb::ExecuteBombPush()
{
	//GET SCENE AND PLAY SFX
	GameScene* scene = this->GetScene();
	ExamScene* examScene = dynamic_cast<ExamScene*>(scene);
	if (examScene)
	{
		auto* channelGroup = examScene->GetSFXChannel();
		auto* pushSound = examScene->GetBombSound();
		FMOD::Channel* m_pSFXChannel;
		SoundManager::GetInstance()->GetSystem()->playSound(pushSound, nullptr, false, &m_pSFXChannel);
		m_pSFXChannel->setChannelGroup(channelGroup);
		m_pSFXChannel->setVolume(5.f);
	}

	//ADD FORCE ACCORDING TO DIRECTION
	switch (m_Direction)
	{
	case MoveDirection::FORWARD:
		m_pRigid->SetKinematic(false);
		m_pRigid->AddForce(physx::PxVec3(0, 0, m_BombForce), physx::PxForceMode::eIMPULSE);
		m_MoveBomb = false;
		m_WasAlreadyPushed = true;
		break;

	case MoveDirection::BACKWARD:
		m_pRigid->SetKinematic(false);
		m_pRigid->AddForce(physx::PxVec3(0, 0, -m_BombForce), physx::PxForceMode::eIMPULSE);
		m_MoveBomb = false;
		m_WasAlreadyPushed = true;
		break;

	case MoveDirection::LEFT:
		m_pRigid->SetKinematic(false);
		m_pRigid->AddForce(physx::PxVec3(-m_BombForce, 0, 0), physx::PxForceMode::eIMPULSE);
		m_MoveBomb = false;
		m_WasAlreadyPushed = true;
		break;

	case MoveDirection::RIGHT:
		m_pRigid->SetKinematic(false);
		m_pRigid->AddForce(physx::PxVec3(m_BombForce, 0, 0), physx::PxForceMode::eIMPULSE);
		m_MoveBomb = false;
		m_WasAlreadyPushed = true;
		break;
	}
}

void Bomb::XRayCast(const physx::PxVec3& pos, const physx::PxVec3& dir)
{
	physx::PxQueryFilterData filterData;
	filterData.data.word0 = physx::PxU32(CollisionGroupFlag::Group1);
	filterData.data.word1 = physx::PxU32(CollisionGroupFlag::Group5);

	physx::PxRaycastBuffer hit;
	
	if (this->GetScene()->GetPhysxProxy()->Raycast(pos, dir, m_Range, hit, physx::PxHitFlag::eDEFAULT, filterData))
	{
		auto* actor = &hit.getAnyHit(0).actor->userData;
		RigidBodyComponent* component = static_cast<RigidBodyComponent*>(*actor);
		ControllerComponent* test = static_cast<ControllerComponent*>(*actor);
		GameObject* obj = component->GetGameObject();
		IndestructableBox* convertObj = dynamic_cast<IndestructableBox*>(obj);
		if (!convertObj)
		{
			if (test->GetGroupFlag() == CollisionGroupFlag::Group1)
			{
				m_VecRanges.push_back(std::abs(obj->GetTransform()->GetPosition().x - GetTransform()->GetPosition().x) + m_BombRange);
				m_VecHitObjects.push_back(obj);
			}

			else if ((component->GetCollisionFlag() == CollisionGroupFlag::Group1))
			{
				m_VecRanges.push_back(std::abs(obj->GetTransform()->GetPosition().x - GetTransform()->GetPosition().x) + m_BombRange);
				m_VecHitObjects.push_back(obj);
			}
		}
		else
			m_VecRanges.push_back(std::abs(obj->GetTransform()->GetPosition().x - GetTransform()->GetPosition().x) + m_BombRange);
	}
	else
		m_VecRanges.push_back(m_Range);
}

void Bomb::ZRayCast(const physx::PxVec3& pos, const physx::PxVec3& dir)
{
	physx::PxQueryFilterData filterData;
	filterData.data.word0 = physx::PxU32(CollisionGroupFlag::Group1);
	filterData.data.word1 = physx::PxU32(CollisionGroupFlag::Group5);

	physx::PxRaycastBuffer hit;
	
	if (this->GetScene()->GetPhysxProxy()->Raycast(pos, dir, m_Range, hit, physx::PxHitFlag::eDEFAULT, filterData))
	{
		auto actor = &hit.getAnyHit(0).actor->userData;
		RigidBodyComponent* component = static_cast<RigidBodyComponent*>(*actor);
		ControllerComponent* test = static_cast<ControllerComponent*>(*actor);
		auto obj = component->GetGameObject();
		auto convertObj = dynamic_cast<IndestructableBox*>(obj);
		if (!convertObj)
		{
			if (test->GetGroupFlag() == CollisionGroupFlag::Group1)
			{
				m_VecRanges.push_back(std::abs(obj->GetTransform()->GetPosition().z - GetTransform()->GetPosition().z) + m_BombRange);
				m_VecHitObjects.push_back(obj);
			}

			else if ((component->GetCollisionFlag() == CollisionGroupFlag::Group1))
			{
				m_VecRanges.push_back(std::abs(obj->GetTransform()->GetPosition().z - GetTransform()->GetPosition().z) + m_BombRange);
				m_VecHitObjects.push_back(obj);
			}
		}
		else
			m_VecRanges.push_back(std::abs(obj->GetTransform()->GetPosition().z - GetTransform()->GetPosition().z) + m_BombRange);
	}
	else
		m_VecRanges.push_back(m_Range);
}

