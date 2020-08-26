#pragma once
class GameObject;
class Grid
{
public:
	Grid(DirectX::XMFLOAT2 startPos, float width, float height, int rows, int cols, float blockOffset, UINT DestructMat, UINT IndestructMat);
	virtual ~Grid() = default;

	Grid(const Grid& other) = delete;
	Grid(Grid&& other) noexcept = delete;
	Grid& operator=(const Grid& other) = delete;
	Grid& operator=(Grid&& other) noexcept = delete;

	std::vector<GameObject*>* GetGameObjects() { return &m_VecBoxes; }

private:
	enum class BlockType
	{
		Destructable,
		Indestructable
	};

	DirectX::XMFLOAT2 m_StartPos;
	float m_Width, m_Height, m_Offset;
	int m_Rows, m_Cols;
	std::vector<GameObject*> m_VecBoxes{};
	UINT m_DestructableMat, m_IndestructableMat;
};

