#include "stdafx.h"
#include "PostColorGrading.h"
#include "RenderTarget.h"
#include "ContentManager.h"
#include "TextureData.h"

PostColorGrading::PostColorGrading() : PostProcessingMaterial(L"./Resources/Effects/Post/ColorGrading.fx",1)
,m_pLUTTextureVariable(nullptr)
,m_pTextureMapVariable(nullptr)
,m_pLUTTexture(nullptr)
{
}

void PostColorGrading::SetLUTTexture(const std::wstring& assetFile)
{
	m_pLUTTexture = ContentManager::Load<TextureData>(assetFile);
}

void PostColorGrading::LoadEffectVariables()
{
	m_pTextureMapVariable = GetEffect()->GetVariableByName("InputTexture")->AsShaderResource();
	if (!m_pTextureMapVariable->IsValid())
	{
		Logger::LogWarning(L"PostColorGrading::LoadEffectVariables() > \'InputTexture\' variable not found!");
		m_pTextureMapVariable = nullptr;
	}

	m_pLUTTextureVariable = GetEffect()->GetVariableByName("LUT")->AsShaderResource();
	if (!m_pLUTTextureVariable->IsValid())
	{
		Logger::LogWarning(L"PostColorGrading::LoadEffectVariables() > \'LUT\' variable not found!");
		m_pLUTTextureVariable = nullptr;
	}
}

void PostColorGrading::UpdateEffectVariables(RenderTarget* pRendertarget)
{
	if (m_pTextureMapVariable)
	{
		m_pTextureMapVariable->SetResource(pRendertarget->GetShaderResourceView());
	}

	if (m_pLUTTextureVariable)
	{
		m_pLUTTextureVariable->SetResource(m_pLUTTexture->GetShaderResourceView());
	}
}
