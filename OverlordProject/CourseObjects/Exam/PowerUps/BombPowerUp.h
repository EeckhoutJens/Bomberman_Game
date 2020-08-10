#pragma once
#include <GameObject.h>
class BombPowerUp final : public GameObject
{
public:
	BombPowerUp(DirectX::XMFLOAT3 pos, UINT matID = 0);
	virtual ~BombPowerUp() = default;

	BombPowerUp(const BombPowerUp& other) = delete;
	BombPowerUp(BombPowerUp&& other) noexcept = delete;
	BombPowerUp& operator=(const BombPowerUp& other) = delete;
	BombPowerUp& operator=(BombPowerUp&& other) noexcept = delete;

	void Initialize(const GameContext & gameContext) override;
	void Update(const GameContext & gameContext) override;
	void SetHit() { m_WasHit = true; }
	bool GetHit() { return m_WasHit; }

private:
	UINT m_bombPUMat;
	bool m_WasHit{false};
	DirectX::XMFLOAT3 m_Pos;
};

