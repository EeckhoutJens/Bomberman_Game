//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ShadowMapMaterial.h"
#include "ContentManager.h"

ShadowMapMaterial::~ShadowMapMaterial()
{
	//TODO: make sure you don't have memory leaks and/or resource leaks :) -> Figure out if you need to do something here
	for (int i{}; i < NUM_TYPES; ++i)
	{
		SafeRelease(m_pInputLayouts[i]);
	}
}

void ShadowMapMaterial::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (!m_IsInitialized)
	{
		//TODO: initialize the effect, techniques, shader variables, input layouts (hint use EffectHelper::BuildInputLayout), etc.

		m_pShadowEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/ShadowMapGenerator.fx");

		m_pShadowTechs[0] = m_pShadowEffect->GetTechniqueByIndex(0);
		m_pShadowTechs[1] = m_pShadowEffect->GetTechniqueByIndex(1);

		m_pWorldMatrixVariable = m_pShadowEffect->GetVariableByName("gWorld")->AsMatrix();
		if (!m_pWorldMatrixVariable->IsValid())
		{
			Logger::LogError(L"ShadowMapMaterial gWorld variable is invalid");
			return;
		}
		m_pBoneTransforms = m_pShadowEffect->GetVariableByName("gBones")->AsMatrix();
		if (!m_pBoneTransforms->IsValid())
		{
			Logger::LogError(L"ShadowMapMaterial gBones variable is invalid");
			return;
		}
		m_pLightVPMatrixVariable = m_pShadowEffect->GetVariableByName("gLightViewProj")->AsMatrix();
		if (!m_pLightVPMatrixVariable->IsValid())
		{
			Logger::LogError(L"ShadowMapMaterial gLightViewProj variable is invalid");
			return;
		}
		//InputLayout
		EffectHelper::BuildInputLayout(gameContext.pDevice, m_pShadowTechs[0], &m_pInputLayouts[0], m_InputLayoutDescriptions[0], m_InputLayoutSizes[0], m_InputLayoutIds[0]);
		EffectHelper::BuildInputLayout(gameContext.pDevice, m_pShadowTechs[1], &m_pInputLayouts[1], m_InputLayoutDescriptions[1], m_InputLayoutSizes[1], m_InputLayoutIds[1]);

	}
}

void ShadowMapMaterial::SetLightVP(DirectX::XMFLOAT4X4 lightVP) const
{
	//TODO: set the correct shader variable
	m_pLightVPMatrixVariable->SetMatrix(&lightVP._11);
}

void ShadowMapMaterial::SetWorld(DirectX::XMFLOAT4X4 world) const
{
	//TODO: set the correct shader variable
	m_pWorldMatrixVariable->SetMatrix(&world._11);
}

void ShadowMapMaterial::SetBones(const float* pData, int count) const
{
	//TODO: set the correct shader variable
	m_pBoneTransforms->SetMatrixArray(pData, 0, count);
}
