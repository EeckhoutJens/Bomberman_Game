#pragma once
#include "GameObject.h"

class ControllerComponent;
class CameraComponent;
class ModelComponent;
class Bomb;

class ExamCharacter final : public GameObject
{

public:

	enum  CharacterInput : UINT
	{
		LEFT = 0,
		RIGHT,
		FORWARD,
		BACKWARD,
		BOMB,
		PUSH,
		PAUSE
	};

	enum class CharacterDirection : UINT
	{
		LEFT,
		RIGHT,
		FORWARD,
		BACKWARD
	};

	ExamCharacter(float radius = 2, float height = 5, float moveSpeed = 100, CameraComponent* pCamera = nullptr, UINT matID = 0, UINT bombMatID = 0, bool firstRound = true);
	virtual ~ExamCharacter() = default;

	ExamCharacter(const ExamCharacter& other) = delete;
	ExamCharacter(ExamCharacter&& other) noexcept = delete;
	ExamCharacter& operator=(const ExamCharacter& other) = delete;
	ExamCharacter& operator=(ExamCharacter&& other) noexcept = delete;

	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void PausePlayer();
	void UnpausePlayer();
	bool GetSetPaused(){ return m_SetPaused; }
	void IncrementNrOfBombs()
	{
		if (m_MaxNrOfBombs < 4)
		{
			++m_MaxNrOfBombs;
		}
	}
	void IncrementRangeMultiplier()
	{
		if (m_RangeMultiplier < 5)
		{
			++m_RangeMultiplier;
		}
	}
	void SetBombRange(float range) { m_BombRange = range; }
	void SetDead();
	std::vector<Bomb*>* GetBombs() { return &m_VecBombs; }
	const bool& GetShouldDestroy() const { return m_ShouldDestroy; }
	const int& GetCharacterNr() const { return m_CharacterNr; }
	const int& GetNrOfBombs() const { return m_MaxNrOfBombs; }
	const int& GetRangeMultiplier() const { return m_RangeMultiplier; }
	const int& GetNrOfInputElements() const { return m_nrOfInputElements; }
	static void ResetCharacterNr() { m_InstanceCount = 0; }

private:
	//VARIABLES
	ControllerComponent* m_pController;
	const int m_nrOfInputElements{7};
	const float m_PushRange{ 11.5f };
	const float m_BombOffset{10.f};
	const float m_Scale{0.25f};
	const float m_HeightDiv{1.25f};
	CameraComponent* m_pCamera;
	ModelComponent* m_pModel;
	float m_TotalPitch, m_TotalYaw;
	float m_MoveSpeed, m_RotationSpeed;
	float m_Radius, m_Height;
	float m_currTimer{};
	const float m_MaxTimer{ 10.f };
	UINT m_MatID, m_BombMatID;
	static int m_InstanceCount;
	int m_CharacterNr{};
	CharacterDirection m_DirectionState{};
	std::vector<Bomb*> m_VecBombs{};
	int m_MaxNrOfBombs{2};
	float m_BombRange{9.f};
	int m_RangeMultiplier{3};
	bool m_IsDead{false};
	bool m_ShouldDestroy{false};
	bool m_IsFirstRound{};
	bool m_SetPaused{ false };
	float m_PauseTimer{};
	float m_MaxPauseTimer{ 0.2f };

	//Running
	float m_MaxRunVelocity,
		m_TerminalVelocity,
		m_Gravity,
		m_RunAccelerationTime,
		m_JumpAccelerationTime,
		m_RunAcceleration,
		m_RunVelocity,
		m_JumpVelocity;

	DirectX::XMFLOAT3 m_Velocity;
};

