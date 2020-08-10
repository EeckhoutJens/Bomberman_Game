#include "stdafx.h"
#include "ExamCharacter.h"
#include "Components.h"
#include "Prefabs.h"
#include "GameScene.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "ModelAnimator.h"
#include "Bomb.h"
#include "../../../Materials/SkinnedDiffuseMaterial.h"

int ExamCharacter::instanceCount = 0;

ExamCharacter::ExamCharacter(float radius, float height, float moveSpeed, CameraComponent* pCamera, UINT matID, UINT bombMatID, bool firstRound) :
m_Radius(radius),
m_Height(height),
m_MoveSpeed(moveSpeed),
m_pController(nullptr),
m_TotalPitch(0),
m_TotalYaw(0),
m_RotationSpeed(90.f),
m_pCamera{pCamera},
m_pModel{nullptr},
m_MatID{matID},
m_BombMatID{bombMatID},

//Running
m_MaxRunVelocity(30.0f),
m_TerminalVelocity(1),
m_Gravity(9.81f),
m_RunAccelerationTime(0.3f),
m_JumpAccelerationTime(0.8f),
m_RunAcceleration(m_MaxRunVelocity / m_RunAccelerationTime),
m_JumpAcceleration(m_Gravity / m_JumpAccelerationTime),
m_RunVelocity(0),
m_JumpVelocity(0),
m_Velocity(0, 0, 0),
isFirstRound{firstRound}
{
	CharacterNr = instanceCount;
	instanceCount++;
}

void ExamCharacter::Initialize(const GameContext& gameContext)
{
	if (CharacterNr == 0)
	{
		ControllerComponent::ResetInstanceCount();
	}
	//TODO: Create controller
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetInstance()->GetPhysics();
	auto pControllerMat = physX->createMaterial(0, 0, 0);
	m_pController = new ControllerComponent{ pControllerMat,m_Radius,m_Height,0 };
	m_pController->SetCollisionGroup(CollisionGroupFlag::Group1);
	AddComponent(m_pController);

	//TODO: Register all Input Actions
	if (CharacterNr == 0 && isFirstRound)
	{
		InputAction forward{ CharacterInput::FORWARD,InputTriggerState::Down,'W',-1,0,(GamepadIndex)CharacterNr };
		InputAction backward{ CharacterInput::BACKWARD,InputTriggerState::Down,'S',-1,0,(GamepadIndex)CharacterNr };
		InputAction left{ CharacterInput::LEFT,InputTriggerState::Down,'A', -1,0,(GamepadIndex)CharacterNr };
		InputAction right{ CharacterInput::RIGHT,InputTriggerState::Down,'D', -1,0,(GamepadIndex)CharacterNr };
		InputAction bomb{ CharacterInput::BOMB,InputTriggerState::Pressed,' ', -1,XINPUT_GAMEPAD_A,(GamepadIndex)CharacterNr };
		InputAction push{ CharacterInput::PUSH,InputTriggerState::Released,'Q', -1,XINPUT_GAMEPAD_B,(GamepadIndex)CharacterNr };
		InputAction pause{ CharacterInput::PAUSE,InputTriggerState::Pressed, VK_ESCAPE,-1,XINPUT_GAMEPAD_START,(GamepadIndex)CharacterNr };
		gameContext.pInput->AddInputAction(forward);
		gameContext.pInput->AddInputAction(backward);
		gameContext.pInput->AddInputAction(left);
		gameContext.pInput->AddInputAction(right);
		gameContext.pInput->AddInputAction(bomb);
		gameContext.pInput->AddInputAction(push);
		gameContext.pInput->AddInputAction(pause);
	}
	if (CharacterNr == 1 && isFirstRound)
	{
		InputAction forward2{ int(CharacterInput::FORWARD + m_nrOfInputElements * CharacterNr),InputTriggerState::Down,VK_UP,-1,0,(GamepadIndex)CharacterNr };
		InputAction backward2{ int(CharacterInput::BACKWARD + m_nrOfInputElements * CharacterNr),InputTriggerState::Down,VK_DOWN,-1,0,(GamepadIndex)CharacterNr };
		InputAction left2{ int(CharacterInput::LEFT + m_nrOfInputElements * CharacterNr),InputTriggerState::Down,VK_LEFT, -1,0,(GamepadIndex)CharacterNr };
		InputAction right2{ int(CharacterInput::RIGHT + m_nrOfInputElements * CharacterNr),InputTriggerState::Down,VK_RIGHT, -1,0,(GamepadIndex)CharacterNr };
		InputAction bomb2{ int(CharacterInput::BOMB + m_nrOfInputElements * CharacterNr),InputTriggerState::Released,VK_RCONTROL, -1,XINPUT_GAMEPAD_A,(GamepadIndex)CharacterNr };
		InputAction push2{ int(CharacterInput::PUSH + m_nrOfInputElements * CharacterNr),InputTriggerState::Released,VK_RSHIFT,-1,XINPUT_GAMEPAD_B,(GamepadIndex)CharacterNr };
		InputAction pause2{ int(CharacterInput::PAUSE + m_nrOfInputElements * CharacterNr),InputTriggerState::Released,-1,-1,XINPUT_GAMEPAD_START,(GamepadIndex)CharacterNr };

		gameContext.pInput->AddInputAction(forward2);
		gameContext.pInput->AddInputAction(backward2);
		gameContext.pInput->AddInputAction(left2);
		gameContext.pInput->AddInputAction(right2);
		gameContext.pInput->AddInputAction(bomb2);
		gameContext.pInput->AddInputAction(push2);
		gameContext.pInput->AddInputAction(pause2);
	}
	if(isFirstRound && CharacterNr > 1)
	{
		InputAction forward2{ int(CharacterInput::FORWARD + m_nrOfInputElements * CharacterNr),InputTriggerState::Down,-1,-1,0,(GamepadIndex)CharacterNr };
		InputAction backward2{ int(CharacterInput::BACKWARD + m_nrOfInputElements * CharacterNr),InputTriggerState::Down,-1,-1,0,(GamepadIndex)CharacterNr };
		InputAction left2{ int(CharacterInput::LEFT + m_nrOfInputElements * CharacterNr),InputTriggerState::Down,-1, -1,0,(GamepadIndex)CharacterNr };
		InputAction right2{ int(CharacterInput::RIGHT + m_nrOfInputElements * CharacterNr),InputTriggerState::Down,-1, -1,0,(GamepadIndex)CharacterNr };
		InputAction bomb2{ int(CharacterInput::BOMB + m_nrOfInputElements * CharacterNr),InputTriggerState::Released,-1, -1,XINPUT_GAMEPAD_A,(GamepadIndex)CharacterNr };
		InputAction push2{ int(CharacterInput::PUSH + m_nrOfInputElements * CharacterNr),InputTriggerState::Released,-1, -1,XINPUT_GAMEPAD_B,(GamepadIndex)CharacterNr };
		InputAction pause2{ int(CharacterInput::PAUSE + m_nrOfInputElements * CharacterNr),InputTriggerState::Released,-1,-1,XINPUT_GAMEPAD_START,(GamepadIndex)CharacterNr };

		gameContext.pInput->AddInputAction(forward2);
		gameContext.pInput->AddInputAction(backward2);
		gameContext.pInput->AddInputAction(left2);
		gameContext.pInput->AddInputAction(right2);
		gameContext.pInput->AddInputAction(bomb2);
		gameContext.pInput->AddInputAction(push2);
		gameContext.pInput->AddInputAction(pause2);
	}

	m_pModel = new ModelComponent{ L"Resources/Meshes/CharacterTest.ovm" };
	
	m_pModel->SetMaterial(m_MatID);

	auto pModelObject = new GameObject();
	pModelObject->AddComponent(m_pModel);
	AddChild(pModelObject);
	pModelObject->GetTransform()->Translate(0, -m_Height / 1.25f, 0);
}

void ExamCharacter::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	GetChild<GameObject>()->GetTransform()->Scale(0.25f, 0.25f, 0.25f);
}

void ExamCharacter::Update(const GameContext& gameContext)
{
	//TODO: Update the character (Camera rotation, Character Movement, Character Gravity)

	if (IsDead)
	{
		if (!m_pModel->GetAnimator()->GetPlayOnce())
		{
			m_pModel->GetAnimator()->Pause();
			shouldDestroy = true;
		}
			
		return;
	}

	//UPDATE PAUSE TIMER
	pauseTimer += gameContext.pGameTime->GetElapsed();


	//UPDATE CAMERA
	using namespace DirectX;
	auto move = XMFLOAT2(0, 0);
	auto forward = XMVECTOR{};
	auto right = XMVECTOR{};

		move.y = gameContext.pInput->IsActionTriggered(CharacterInput::FORWARD + m_nrOfInputElements * CharacterNr) ? 1.0f : 0.0f;
		if (move.y == 0) move.y = -(gameContext.pInput->IsActionTriggered(CharacterInput::BACKWARD + m_nrOfInputElements * CharacterNr) ? 1.0f : 0.0f);
		if (move.y == 0) move.y = InputManager::GetThumbstickPosition(true,GamepadIndex(CharacterNr)).y;

		move.x = gameContext.pInput->IsActionTriggered(CharacterInput::RIGHT + m_nrOfInputElements * CharacterNr) ? 1.0f : 0.0f;
		if (move.x == 0) move.x = -(gameContext.pInput->IsActionTriggered(CharacterInput::LEFT + m_nrOfInputElements * CharacterNr) ? 1.0f : 0.0f);
		if (move.x == 0) move.x = InputManager::GetThumbstickPosition(true,GamepadIndex(CharacterNr)).x;

		auto currSpeed = m_MoveSpeed;
		if (InputManager::IsKeyboardKeyDown(VK_LSHIFT))
			currSpeed *= m_RotationSpeed;

		auto look = DirectX::XMFLOAT2(0, 0);
		if (InputManager::IsMouseButtonDown(VK_LBUTTON))
		{
			const auto mouseMove = InputManager::GetMouseMovement();
			look.x = static_cast<float>(mouseMove.x);
			look.y = static_cast<float>(mouseMove.y);
		}

		if (look.x == 0 && look.y == 0)
		{
			look = InputManager::GetThumbstickPosition(false);
		}

		//CALCULATE TRANSFORMS
		forward = XMLoadFloat3(&GetTransform()->GetForward());
		right = XMLoadFloat3(&GetTransform()->GetRight());
		auto currPos = XMLoadFloat3(&GetTransform()->GetPosition());

		currPos += forward * move.y * currSpeed * gameContext.pGameTime->GetElapsed();
		currPos += right * move.x * currSpeed * gameContext.pGameTime->GetElapsed();
	

	//UPDATE MOVEMENT AND GRAVITY

	//Calculate Direction
	auto Direction = forward * move.y + right * move.x;

	//Movement
	if (move.x != 0 || move.y != 0)
	{
		m_RunVelocity += m_RunAcceleration * gameContext.pGameTime->GetElapsed();
		if (m_RunVelocity > m_MaxRunVelocity)
		{
			m_RunVelocity = m_MaxRunVelocity;
		}

		auto yVelocity = m_Velocity.y;
		auto temp = Direction * m_RunVelocity;
		XMFLOAT3 tempFloat{ 0,0,0 };
		XMStoreFloat3(&tempFloat, temp);
		m_Velocity.x = tempFloat.x;
		m_Velocity.y = yVelocity;
		m_Velocity.z = tempFloat.z;
	}

	else
	{
		m_Velocity.x = 0;
		m_Velocity.z = 0;
	}


	if (!m_pController->GetCollisionFlags().isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN))
	{
		m_JumpVelocity -= m_JumpAccelerationTime;
		if (m_JumpVelocity < -m_TerminalVelocity)
		{
			m_JumpVelocity = -m_TerminalVelocity;
		}
	}

	else
	{
		m_Velocity.y = 0;
	}

	m_Velocity.y += m_JumpVelocity;

	auto finalVelocity = m_Velocity;
	finalVelocity.x *= gameContext.pGameTime->GetElapsed();
	finalVelocity.y *= gameContext.pGameTime->GetElapsed();
	finalVelocity.z *= gameContext.pGameTime->GetElapsed();
	m_pController->Move(finalVelocity);

	XMFLOAT4 forwardFloat{};
	XMFLOAT4 rightFloat{};
	DirectX::XMStoreFloat4(&forwardFloat, forward);
	DirectX::XMStoreFloat4(&rightFloat, right);

	//SET STATE
	if (finalVelocity.z > 0)
		m_DirectionState = CharacterDirection::FORWARD;

	if (finalVelocity.z < 0)
		m_DirectionState = CharacterDirection::BACKWARD;

	if (finalVelocity.x < 0)
		m_DirectionState = CharacterDirection::LEFT;

	if (finalVelocity.x > 0)
		m_DirectionState = CharacterDirection::RIGHT;


	//SET PAUSED
	if (GetScene()->GetGameContext().pInput->IsActionTriggered(CharacterInput::PAUSE + m_nrOfInputElements * CharacterNr))
	{
		if (pauseTimer >= maxPauseTimer)
		{
			setPaused = true;
		}
	}


	//PLACE BOMB
	if (GetScene()->GetGameContext().pInput->IsActionTriggered(CharacterInput::BOMB + m_nrOfInputElements * CharacterNr))
	{
		if (pauseTimer >= maxPauseTimer)
		{
			if (int(m_VecBombs.size()) < m_MaxNrOfBombs)
			{
				bool canPlaceBomb{ true };
				auto characterPos = this->GetTransform()->GetPosition();
				for (Bomb* pBomb : m_VecBombs)
				{
					auto bombPos = pBomb->GetTransform()->GetPosition();
					auto distance = hypot(hypot(bombPos.x - characterPos.x, bombPos.y - characterPos.y), bombPos.z - characterPos.z);
					if (distance < 10)
					{
						canPlaceBomb = false;
					}
				}

				if (canPlaceBomb)
				{
					Bomb* newBomb{};

					newBomb = new Bomb{ DirectX::XMFLOAT3{characterPos.x, 4, characterPos.z}, m_BombRange * m_RangeMultiplier,m_BombMatID };

					m_VecBombs.push_back(newBomb);
				}

			}
		}
	}


	//PUSH BOMB
	if (GetScene()->GetGameContext().pInput->IsActionTriggered(CharacterInput::PUSH + m_nrOfInputElements * CharacterNr))
	{
		auto charPos = GetTransform()->GetPosition();
		for (Bomb* pBomb : m_VecBombs)
		{
			auto bombPos = pBomb->GetTransform()->GetPosition();
			auto distanceToBomb = hypot(hypot(bombPos.x - charPos.x, bombPos.y - charPos.y), bombPos.z - charPos.z);
			if (distanceToBomb < m_PushRange)
			{
				switch (m_DirectionState)
				{
				case ExamCharacter::CharacterDirection::LEFT:
					pBomb->PushBomb(MoveDirection::LEFT);
					break;

				case ExamCharacter::CharacterDirection::RIGHT:

						pBomb->PushBomb(MoveDirection::RIGHT);

					break;
				case ExamCharacter::CharacterDirection::FORWARD:

						pBomb->PushBomb(MoveDirection::FORWARD);
					
					break;
				case ExamCharacter::CharacterDirection::BACKWARD:

						pBomb->PushBomb(MoveDirection::BACKWARD);
					
					break;
				default:
					break;
				}
			}
			
		}
	}







	if (finalVelocity.x != 0 || finalVelocity.z != 0)
	{
		if (forwardFloat.z > 0.5)
		{
			float testRot{};
			testRot = atan2(-finalVelocity.x, -finalVelocity.z);
			GetChild<GameObject>()->GetTransform()->Rotate(0, testRot, 0, false);
		}

		if (forwardFloat.z < -0.5)
		{
			float testRot{};
			testRot = atan2(finalVelocity.x, finalVelocity.z);
			GetChild<GameObject>()->GetTransform()->Rotate(0, testRot, 0, false);
		}

		if (forwardFloat.x > 0.5)
		{
			float testRot{};
			testRot = atan2(finalVelocity.z, -finalVelocity.x);
			GetChild<GameObject>()->GetTransform()->Rotate(0, testRot, 0, false);
		}


		if (forwardFloat.x < -0.5)
		{
			float testRot{};
			testRot = atan2(-finalVelocity.z, finalVelocity.x);
			GetChild<GameObject>()->GetTransform()->Rotate(0, testRot, 0, false);
		}

			if (!(m_pModel->GetAnimator()->GetClipName() == L"Running"))
			{
				m_pModel->GetAnimator()->SetAnimationSpeed(1.0f);
				m_pModel->GetAnimator()->SetAnimation(L"Running");
			}
	}

	else
	{
		if (!(m_pModel->GetAnimator()->GetClipName() == L"Idle"))
		{
			if (m_currTimer >= m_MaxTimer)
			{
				m_currTimer = 0;
				m_pModel->GetAnimator()->SetAnimationSpeed(1.0f);
				m_pModel->GetAnimator()->SetAnimation(L"Idle",true);
			}
		}

		if (!(m_pModel->GetAnimator()->GetClipName() == L"IdleStance") && !m_pModel->GetAnimator()->GetPlayOnce())
		{
			m_pModel->GetAnimator()->SetAnimationSpeed(1.0f);
			m_pModel->GetAnimator()->SetAnimation(L"IdleStance");
		}

		m_currTimer += gameContext.pGameTime->GetElapsed();
	}

	if (!m_pModel->GetAnimator()->IsPlaying())
	{
		m_pModel->GetAnimator()->Play();
	}
	
}

void ExamCharacter::PausePlayer()
{
	for (Bomb* bomb : m_VecBombs)
	{
		bomb->SetActive(false);
	}

	this->SetActive(false);
}

void ExamCharacter::UnpausePlayer()
{
	for (Bomb* bomb : m_VecBombs)
	{
		bomb->SetActive(true);
	}
	setPaused = false;
	this->SetActive(true);
	pauseTimer = 0;
}

void ExamCharacter::SetDead()
{
	IsDead = true;
	if (!(m_pModel->GetAnimator()->GetClipName() == L"Death"))
	{
		m_pModel->GetAnimator()->SetAnimationSpeed(1.0f);
		m_pModel->GetAnimator()->SetAnimation(L"Death",true);
	}
}
