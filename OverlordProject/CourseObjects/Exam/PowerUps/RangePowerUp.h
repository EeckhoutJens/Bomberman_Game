#pragma once
#include <GameObject.h>
class RangePowerUp final : public GameObject
{
public:
	RangePowerUp(DirectX::XMFLOAT3 pos, UINT matID = 0);
	virtual ~RangePowerUp() = default;

	RangePowerUp(const RangePowerUp& other) = delete;
	RangePowerUp(RangePowerUp&& other) noexcept = delete;
	RangePowerUp& operator=(const RangePowerUp& other) = delete;
	RangePowerUp& operator=(RangePowerUp&& other) noexcept = delete;

	void Initialize(const GameContext & gameContext) override;
	void Update(const GameContext & gameContext) override;
	void SetHit() { m_WasHit = true; }
	bool GetHit() { return m_WasHit; }

private:
	UINT m_RangePUMat;
	bool m_WasHit{ false };
	DirectX::XMFLOAT3 m_Pos;
	const float m_YPosOffset{ -7.75f };
	const float m_Scale{ 0.2f };
};

