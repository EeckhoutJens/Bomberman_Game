#pragma once
#include <GameObject.h>
class IndestructableBox : public GameObject
{
public:
	IndestructableBox(DirectX::XMFLOAT3 pos, UINT matID = 0);
	virtual ~IndestructableBox() = default;

	IndestructableBox(const IndestructableBox& other) = delete;
	IndestructableBox(IndestructableBox&& other) noexcept = delete;
	IndestructableBox& operator=(const IndestructableBox& other) = delete;
	IndestructableBox& operator=(IndestructableBox&& other) noexcept = delete;

	void Initialize(const GameContext & gameContext) override;
	void Update(const GameContext & gameContext) override;

private:
	UINT m_MatID;
	DirectX::XMFLOAT3 m_Pos;
	const float m_GeomScale{ 4.5f };
	const float m_Scale{ 0.3f };
};

