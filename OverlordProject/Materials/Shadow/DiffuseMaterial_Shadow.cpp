//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "DiffuseMaterial_Shadow.h"
#include "GeneralStructs.h"
#include "Logger.h"
#include "ContentManager.h"
#include "TextureData.h"
#include "Components.h"

ID3DX11EffectShaderResourceVariable* DiffuseMaterial_Shadow::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* DiffuseMaterial_Shadow::m_pShadowSRVvariable = nullptr;
ID3DX11EffectVectorVariable* DiffuseMaterial_Shadow::m_pLightDirectionVariable = nullptr;
ID3DX11EffectMatrixVariable* DiffuseMaterial_Shadow::m_pLightWVPvariable = nullptr;

DiffuseMaterial_Shadow::DiffuseMaterial_Shadow() : Material(L"./Resources/Effects/Shadow/PosNormTex3D_Shadow.fx"),
m_pDiffuseTexture(nullptr)
{}

void DiffuseMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	//TODO: store the diffuse texture in the appropriate member
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void DiffuseMaterial_Shadow::SetLightDirection(DirectX::XMFLOAT3 dir)
{
	//TODO: store the light direction in the appropriate member
	m_LightDirection = dir;
}

void DiffuseMaterial_Shadow::LoadEffectVariables()
{
	//TODO: load all the necessary shader variables
	m_pDiffuseSRVvariable = GetEffect()->GetVariableByName("gDiffuseMap")->AsShaderResource();
	m_pShadowSRVvariable = GetEffect()->GetVariableByName("gShadowMap")->AsShaderResource();
	m_pLightDirectionVariable = GetEffect()->GetVariableByName("gLightDirection")->AsVector();
	m_pLightWVPvariable = GetEffect()->GetVariableByName("gWorldViewProj_Light")->AsMatrix();
}

void DiffuseMaterial_Shadow::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	//TODO: update all the necessary shader variables
	m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	m_pLightDirectionVariable->SetFloatVector((float*)&m_LightDirection);
	m_pShadowSRVvariable->SetResource(gameContext.pShadowMapper->GetShadowMap());

	auto world = pModelComponent->GetTransform()->GetWorld();
	auto worldMatrix = DirectX::XMLoadFloat4x4(&world);
	auto lightVP = gameContext.pShadowMapper->GetLightVP();
	auto lightVPMatrix = DirectX::XMLoadFloat4x4(&lightVP);;
	auto lightWVP = DirectX::XMMatrixMultiply(worldMatrix, lightVPMatrix);
	m_pLightWVPvariable->SetMatrix((float*)&lightWVP);
}