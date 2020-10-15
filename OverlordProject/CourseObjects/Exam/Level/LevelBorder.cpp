#include "stdafx.h"
#include "LevelBorder.h"
#include "IndestructableBox.h"

LevelBorder::LevelBorder(DirectX::XMFLOAT2 startPos, float width, float height, float blockOffset, UINT IndestructMat)
	:m_StartPos{startPos}
	,m_Width{width}
	,m_Height{height}
	,m_Offset{blockOffset}
	,m_IndestructableMat{IndestructMat}
{
	int nrOfBlocks{};
	for (float currWidth = 0; currWidth <= m_Width; currWidth += m_Offset)
	{
		GameObject* objectToAdd = new IndestructableBox(DirectX::XMFLOAT3(m_StartPos.x + currWidth, m_DefaultYValue, m_StartPos.y), m_IndestructableMat);
		m_VecBoxes.push_back(objectToAdd);
		++nrOfBlocks;
	}

	for (float currWidth = 0; currWidth <= m_Width; currWidth += m_Offset)
	{
		if (currWidth != 0)
		{
			GameObject* objectToAdd = new IndestructableBox(DirectX::XMFLOAT3(m_StartPos.x + currWidth, m_DefaultYValue, m_StartPos.y + m_Height), m_IndestructableMat);
			m_VecBoxes.push_back(objectToAdd);
			++nrOfBlocks;
		}
	}

	for (float currHeight = 0; currHeight <= m_Height; currHeight += m_Offset)
	{
		if (currHeight != 0)
		{
			GameObject* objectToAdd = new IndestructableBox(DirectX::XMFLOAT3(m_StartPos.x, m_DefaultYValue, m_StartPos.y + currHeight), m_IndestructableMat);
			m_VecBoxes.push_back(objectToAdd);
			++nrOfBlocks;
		}
	}

	for (float currHeight = 0; currHeight <= m_Height; currHeight += m_Offset)
	{
		if (currHeight != 0 && currHeight != m_Height)
		{
			GameObject* objectToAdd = new IndestructableBox(DirectX::XMFLOAT3(m_StartPos.x + m_Width, m_DefaultYValue, m_StartPos.y + currHeight), m_IndestructableMat);
			m_VecBoxes.push_back(objectToAdd);
			++nrOfBlocks;
		}
	}	
}
