#include "stdafx.h"
#include "ControllerComponent.h"
#include "TransformComponent.h"
#include "PhysxProxy.h"
#include "GameObject.h"
#include "GameScene.h"

#pragma warning(push)
#pragma warning(disable: 26812)

int ControllerComponent::instanceCount = 0;
ControllerComponent::ControllerComponent(physx::PxMaterial* material, float radius, float height, float stepOffset, std::wstring name,
                                         physx::PxCapsuleClimbingMode::Enum climbingMode)
	: m_Radius(radius),
	  m_Height(height),
	  m_StepOffset(stepOffset),
	  m_Name(std::move(name)),
	  m_Controller(nullptr),
	  m_ClimbingMode(climbingMode),
	  m_pMaterial(material),
	  m_CollisionFlag(physx::PxControllerCollisionFlags()),
	  m_CollisionGroups(physx::PxFilterData(static_cast<UINT32>(CollisionGroupFlag::Group0), 0, 0, 0))
{
	controllerNr = instanceCount;
	std::cout << "Controller nr is " << controllerNr << std::endl;
	instanceCount++;
}
#pragma warning(pop)

ControllerComponent::~ControllerComponent()
{
	if (m_Controller)
	{
		std::cout << "Deleting controller nr " << controllerNr << std::endl;
		m_Controller->release();
		m_Controller = nullptr;
	}
}

void ControllerComponent::Initialize(const GameContext&)
{
	if (m_Controller != nullptr)
	{
		Logger::LogError(L"[ControllerComponent] Cannot initialize a controller twice");
		return;
	}

	//TODO: 1. Retrieve the ControllerManager from the PhysX Proxy (PhysxProxy::GetControllerManager();)
	auto controllerManager = this->GetGameObject()->GetScene()->GetPhysxProxy()->GetControllerManager();
	//TODO: 2. Create a PxCapsuleControllerDesc (Struct)
	physx::PxCapsuleControllerDesc controllerDesc{};
	//  > Call the "setToDefault()" method of the PxCapsuleControllerDesc
	controllerDesc.setToDefault();
	//	> Fill in all the required fields
	//  > Radius, Height, ClimbingMode, UpDirection (PxVec3(0,1,0)), ContactOffset (0.1f), Material [See Initializer List]
	//  > Position -> Use the position of the parent GameObject
	//  > UserData -> This component
	controllerDesc.radius = m_Radius;
	controllerDesc.height = m_Height;
	controllerDesc.climbingMode = m_ClimbingMode;
	controllerDesc.stepOffset = m_StepOffset;
	controllerDesc.upDirection = physx::PxVec3(0, 1, 0);
	DirectX::XMFLOAT3 tempPos = this->GetGameObject()->GetTransform()->GetPosition();
	controllerDesc.position = physx::PxExtendedVec3(tempPos.x, tempPos.y, tempPos.z);
	controllerDesc.material = m_pMaterial;
	controllerDesc.userData = this;
	//3. Create the controller object (m_pController), use the ControllerManager to do that (CHECK IF VALID!!)
	controllerManager->createController(controllerDesc);
	m_Controller = controllerManager->getController(controllerNr);
	if (m_Controller == nullptr)
	{
		Logger::LogError(L"[ControllerComponent] Failed to create controller");
		return;
	}

	//TODO: 4. Set the controller's name (use the value of m_Name) [PxController::setName]
	//   > Converting 'wstring' to 'string' > Use one of the constructor's of the string class
	//	 > Converting 'string' to 'char *' > Use one of the string's methods ;)
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string str = converter.to_bytes(m_Name);
	m_Controller->getActor()->setName(str.c_str());

	//TODO: 5. Set the controller's actor's userdata > This Component
	m_Controller->getActor()->userData = this;
	SetCollisionGroup(static_cast<CollisionGroupFlag>(m_CollisionGroups.word0));
	SetCollisionIgnoreGroups(static_cast<CollisionGroupFlag>(m_CollisionGroups.word1));
}

void ControllerComponent::Update(const GameContext&)
{
}

void ControllerComponent::Draw(const GameContext&)
{
}

void ControllerComponent::Translate(const DirectX::XMFLOAT3& position) const
{
	Translate(position.x, position.y, position.z);
}

void ControllerComponent::Translate(const float x, const float y, const float z) const
{
	if (m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot Translate. No controller present");
	else
		m_Controller->setPosition(physx::PxExtendedVec3(x, y, z));
}

void ControllerComponent::Move(const DirectX::XMFLOAT3 displacement, const float minDist)
{
	if (m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot Move. No controller present");
	else
		m_CollisionFlag = m_Controller->move(ToPxVec3(displacement), minDist, 0, nullptr, nullptr);
}

DirectX::XMFLOAT3 ControllerComponent::GetPosition() const
{
	if (m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot get position. No controller present");
	else
		return ToXMFLOAT3(m_Controller->getPosition());

	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 ControllerComponent::GetFootPosition() const
{
	if (m_Controller == nullptr)
		Logger::LogError(L"[ControllerComponent] Cannot get footposition. No controller present");
	else
		return ToXMFLOAT3(m_Controller->getFootPosition());

	return DirectX::XMFLOAT3();
}

void ControllerComponent::SetCollisionIgnoreGroups(const CollisionGroupFlag ignoreGroups)
{
	using namespace physx;

	m_CollisionGroups.word1 = static_cast<PxU32>(ignoreGroups);

	if (m_Controller != nullptr)
	{
		const auto actor = m_Controller->getActor();
		const auto numShapes = actor->getNbShapes();
		const auto shapes = new PxShape*[numShapes];

		const auto numPointers = actor->getShapes(shapes, numShapes);
		for (PxU32 i = 0; i < numPointers; i++)
		{
#pragma warning (push)
#pragma warning (disable: 6385)
			auto shape = shapes[i];
#pragma warning (pop)
			shape->setSimulationFilterData(m_CollisionGroups);
			//TODO: shouldn't the query filter data be set as well?
		}
		delete[] shapes;
	}
}

void ControllerComponent::SetCollisionGroup(const CollisionGroupFlag group)
{
	using namespace physx;

	m_TestCollisionFlag = group;
	m_CollisionGroups.word0 = static_cast<UINT32>(group);

	if (m_Controller != nullptr)
	{
		const auto actor = m_Controller->getActor();
		const auto numShapes = actor->getNbShapes();
		const auto shapes = new PxShape*[numShapes];

		const auto numPointers = actor->getShapes(shapes, numShapes);
		for (PxU32 i = 0; i < numPointers; i++)
		{
#pragma warning (push)
#pragma warning (disable: 6385)
			auto shape = shapes[i];
#pragma warning (pop)
			shape->setSimulationFilterData(m_CollisionGroups);
			shape->setQueryFilterData(m_CollisionGroups);
		}
		delete[] shapes;
	}
}
