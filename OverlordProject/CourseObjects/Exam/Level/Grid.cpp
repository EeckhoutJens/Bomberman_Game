#include "stdafx.h"
#include "Grid.h"
#include "DestructableBox.h"
#include "IndestructableBox.h"
#include "GameObject.h"

Grid::Grid(DirectX::XMFLOAT2 startPos, float width, float height, int rows, int cols, float blockOffset, UINT DestructMat, UINT IndestructMat)
	:m_StartPos{startPos}
	,m_Width{width}
	,m_Height{height}
	,m_Rows{rows}
	,m_Cols{cols}
	,m_Offset{blockOffset}
	,m_DestructableMat{DestructMat}
	,m_IndestructableMat{IndestructMat}
{
	int createdBlocks{};
	float currHeight{};
	std::srand(unsigned int(std::time(0)));
	for (int currRow{}; currRow < m_Rows; ++currRow)
	{
		float currWidth{};
		for (int currCol{}; currCol < m_Cols; ++currCol)
		{
			BlockType currBlockType{BlockType::Indestructable};
			int rand = std::rand() % (100+1);
			if (rand  < 85 )
			{
				currBlockType = BlockType::Destructable;
			}
			GameObject* objectToAdd;
			switch (currBlockType)
			{

			case BlockType::Destructable:
				objectToAdd = new DestructableBox(DirectX::XMFLOAT3(m_StartPos.x + currWidth, m_DefaultYValue, m_StartPos.y + currHeight), m_DestructableMat);
				break;

			case BlockType::Indestructable:
				objectToAdd = new IndestructableBox(DirectX::XMFLOAT3(m_StartPos.x + currWidth, m_DefaultYValue, m_StartPos.y + currHeight), m_IndestructableMat);
				break;

			default:
				break;
			}
			m_VecBoxes.push_back(objectToAdd);
			currWidth += m_Offset;
			++createdBlocks;
		}
		currHeight += m_Offset;
	}

	//CLEAR PLAYER1 SPAWN
	delete m_VecBoxes[0];
	m_VecBoxes[0] = nullptr;
	delete m_VecBoxes[1];
	m_VecBoxes[1] = nullptr;
	delete m_VecBoxes[0 + m_Cols];
	m_VecBoxes[0 + m_Cols] = nullptr;

	//CLEAR PLAYER2 SPAWN
	delete m_VecBoxes[m_VecBoxes.size() - 1];
	m_VecBoxes[m_VecBoxes.size() - 1] = nullptr;
	delete m_VecBoxes[m_VecBoxes.size() - 2];
	m_VecBoxes[m_VecBoxes.size() - 2] = nullptr;
	delete m_VecBoxes[m_VecBoxes.size() - 1 - m_Cols];
	m_VecBoxes[m_VecBoxes.size() - 1 - m_Cols] = nullptr;
	
	//CLEAR PLAYER3 SPAWN
	delete m_VecBoxes[m_Cols - 1];
	m_VecBoxes[m_Cols - 1] = nullptr;
	delete m_VecBoxes[m_Cols - 2];
	m_VecBoxes[m_Cols - 2] = nullptr;
	delete m_VecBoxes[m_Cols + m_Cols - 1];
	m_VecBoxes[m_Cols + m_Cols - 1] = nullptr;

	//CLEAR PLAYER4 SPAWN
	delete m_VecBoxes[m_VecBoxes.size() - m_Cols];
	m_VecBoxes[m_VecBoxes.size() - m_Cols] = nullptr;
	delete m_VecBoxes[m_VecBoxes.size() - m_Cols + 1];
	m_VecBoxes[m_VecBoxes.size() - m_Cols + 1] = nullptr;
	delete m_VecBoxes[m_VecBoxes.size() - m_Cols * 2];
	m_VecBoxes[m_VecBoxes.size() - m_Cols * 2] = nullptr;


	//CLEAR X-AMOUNT OF RANDOM BLOCKS
	for (int i{}; i < m_NrOfBlocksToDelete; ++i)
	{
		int rand = std::rand() % (m_VecBoxes.size());
		if (m_VecBoxes[rand])
		{
			delete m_VecBoxes[rand];
			m_VecBoxes[rand] = nullptr;
		}
	}




}
