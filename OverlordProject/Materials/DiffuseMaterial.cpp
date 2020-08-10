#include "stdafx.h"
#include "DiffuseMaterial.h"
#include "ContentManager.h"
#include "TextureData.h"

ID3DX11EffectShaderResourceVariable* DiffuseMaterial::m_pDiffuseSRVvariable = nullptr;

DiffuseMaterial::DiffuseMaterial(bool enableTransparency) : Material(L"./Resources/Effects/PosNormTex3D.fx", enableTransparency ? L"TransparencyTech" : L"")
{
}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void DiffuseMaterial::LoadEffectVariables()
{
	m_pDiffuseSRVvariable = GetEffect()->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

void DiffuseMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(pModelComponent);
	UNREFERENCED_PARAMETER(gameContext);
	m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
}
