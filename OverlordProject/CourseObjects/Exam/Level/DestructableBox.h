#pragma once
#include <GameObject.h>
class DestructableBox final : public GameObject
{
public:
	DestructableBox(DirectX::XMFLOAT3 pos, UINT matID = 0);
	virtual ~DestructableBox() = default;

	DestructableBox(const DestructableBox& other) = delete;
	DestructableBox(DestructableBox&& other) noexcept = delete;
	DestructableBox& operator=(const DestructableBox& other) = delete;
	DestructableBox& operator=(DestructableBox&& other) noexcept = delete;

	void Initialize(const GameContext & gameContext) override;
	void Update(const GameContext & gameContext) override;

private:
	UINT m_MatID;
	DirectX::XMFLOAT3 m_Pos;
	const float m_GeomScale{4.5f};
	const float m_Scale{0.3f};
};

