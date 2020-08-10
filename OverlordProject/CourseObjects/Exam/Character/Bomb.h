#pragma once
#include <GameObject.h>
class RigidBodyComponent;
class ParticleEmitterComponent;
enum class  MoveDirection
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
class Bomb : public GameObject
{
public:
	Bomb(DirectX::XMFLOAT3 pos, float range,  UINT matID = 0);
	virtual ~Bomb() = default;

	Bomb(const Bomb& other) = delete;
	Bomb(Bomb&& other) noexcept = delete;
	Bomb& operator=(const Bomb& other) = delete;
	Bomb& operator=(Bomb&& other) noexcept = delete;

	void Initialize(const GameContext & gameContext) override;
	void Update(const GameContext & gameContext) override;
	bool ShouldDestroy() { return m_DestroyBomb; }
	void SetDestroy() { m_DestroyBomb = true; }
	void ExecuteRaycast();
	void PushBomb(MoveDirection direction) { m_MoveBomb = true;  m_Direction = direction; }
	std::vector<GameObject*>* GetHitObjects() { return &m_VecHitObjects; }
	std::vector<float> GetRangeVector() { return m_VecRanges; }

private:
	DirectX::XMFLOAT3 m_Pos;
	float m_Range, m_CurrTime;
	const float m_MaxTime{ 2.f };
	const float m_PosOffset{ 4.f };
	UINT m_MatID;
	bool m_DestroyBomb{ false };
	bool m_MoveBomb{};
	bool m_WasAlreadyPushed{ false };
	RigidBodyComponent* m_pRigid;
	ParticleEmitterComponent* m_pSparkEmitter;
	std::vector<GameObject*> m_VecHitObjects{};
	std::vector<float> m_VecRanges{};
	MoveDirection m_Direction{MoveDirection::FORWARD};
	GameObject* m_pSparkObject = nullptr;
};

