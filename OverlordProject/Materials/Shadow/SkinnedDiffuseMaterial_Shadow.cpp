//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SkinnedDiffuseMaterial_Shadow.h"
#include "GeneralStructs.h"
#include "Logger.h"
#include "ContentManager.h"
#include "TextureData.h"
#include "ModelComponent.h"
#include "ModelAnimator.h"
#include "Components.h"

ID3DX11EffectShaderResourceVariable* SkinnedDiffuseMaterial_Shadow::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* SkinnedDiffuseMaterial_Shadow::m_pNormalSRVvariable = nullptr;
ID3DX11EffectMatrixVariable* SkinnedDiffuseMaterial_Shadow::m_pBoneTransforms = nullptr;
ID3DX11EffectVectorVariable* SkinnedDiffuseMaterial_Shadow::m_pLightDirectionVariable = nullptr;
ID3DX11EffectShaderResourceVariable* SkinnedDiffuseMaterial_Shadow::m_pShadowSRVvariable = nullptr;
ID3DX11EffectMatrixVariable* SkinnedDiffuseMaterial_Shadow::m_pLightWVPvariable = nullptr;

SkinnedDiffuseMaterial_Shadow::SkinnedDiffuseMaterial_Shadow() : Material(L"./Resources/Effects/Shadow/PosNormTex3D_Skinned_Shadow.fx"),
	m_pDiffuseTexture(nullptr)
	,m_pNormalTexture(nullptr)
{}

void SkinnedDiffuseMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	//TODO: store the diffuse texture in the appropriate member
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void SkinnedDiffuseMaterial_Shadow::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);
}

void SkinnedDiffuseMaterial_Shadow::SetLightDirection(DirectX::XMFLOAT3 dir)
{
	//TODO: store the light direction in the appropriate member
	m_LightDirection = dir;
}

void SkinnedDiffuseMaterial_Shadow::LoadEffectVariables()
{
	//TODO: load all the necessary shader variables
	m_pDiffuseSRVvariable = GetEffect()->GetVariableByName("gDiffuseMap")->AsShaderResource();
	m_pNormalSRVvariable = GetEffect()->GetVariableByName("gTextureNormal")->AsShaderResource();
	m_pShadowSRVvariable = GetEffect()->GetVariableByName("gShadowMap")->AsShaderResource();
	m_pLightDirectionVariable = GetEffect()->GetVariableByName("gLightDirection")->AsVector();
	m_pLightWVPvariable = GetEffect()->GetVariableByName("gWorldViewProj_Light")->AsMatrix();
	m_pBoneTransforms = GetEffect()->GetVariableByName("gBones")->AsMatrix();
}

void SkinnedDiffuseMaterial_Shadow::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	//TODO: update all the necessary shader variables
	m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	m_pNormalSRVvariable->SetResource(m_pNormalTexture->GetShaderResourceView());
	m_pLightDirectionVariable->SetFloatVector((float*)&m_LightDirection);
	m_pShadowSRVvariable->SetResource(gameContext.pShadowMapper->GetShadowMap());

	auto world = pModelComponent->GetTransform()->GetWorld();
	auto worldMatrix = DirectX::XMLoadFloat4x4(&world);
	auto lightVP = gameContext.pShadowMapper->GetLightVP();
	auto lightVPMatrix = DirectX::XMLoadFloat4x4(&lightVP);
	auto lightWVP = DirectX::XMMatrixMultiply(worldMatrix, lightVPMatrix);
	m_pLightWVPvariable->SetMatrix((float*)&lightWVP);

	m_pBoneTransforms->SetMatrixArray((float*)pModelComponent->GetAnimator()->GetBoneTransforms().data(),
		0, pModelComponent->GetAnimator()->GetBoneTransforms().size());
}