#pragma once
class GameObject;
class LevelBorder
{
public:
	LevelBorder(DirectX::XMFLOAT2 startPos, float width, float height, float blockOffset, UINT IndestructMat);
	virtual ~LevelBorder() = default;

	LevelBorder(const LevelBorder& other) = delete;
	LevelBorder(LevelBorder&& other) noexcept = delete;
	LevelBorder& operator=(const LevelBorder& other) = delete;
	LevelBorder& operator=(LevelBorder&& other) noexcept = delete;

	std::vector<GameObject*>& GetObjects() { return m_VecBoxes; }

private:
	DirectX::XMFLOAT2 m_StartPos;
	float m_Width, m_Height, m_Offset;
	std::vector<GameObject*> m_VecBoxes{};
	UINT m_IndestructableMat;
};

