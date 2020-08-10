#include "stdafx.h"
#include "ShadowMapRenderer.h"
#include "ContentManager.h"
#include "ShadowMapMaterial.h"
#include "RenderTarget.h"
#include "MeshFilter.h"
#include "SceneManager.h"
#include "OverlordGame.h"

ShadowMapRenderer::~ShadowMapRenderer()
{
	//TODO: make sure you don't have memory leaks and/or resource leaks :) -> Figure out if you need to do something here
	delete m_pShadowRT;
	m_pShadowRT = nullptr;
	delete m_pShadowMat;
	m_pShadowMat = nullptr;
}

void ShadowMapRenderer::Initialize(const GameContext& gameContext)
{
	if (m_IsInitialized)
		return;

	//TODO: create shadow generator material + initialize it
	m_pShadowMat = new ShadowMapMaterial{};
	m_pShadowMat->Initialize(gameContext);

	//TODO: create a rendertarget with the correct settings (hint: depth only) for the shadow generator using a RENDERTARGET_DESC
	auto renderDesc = RENDERTARGET_DESC{};
	renderDesc.Width = OverlordGame::GetGameSettings().Window.Width;
	renderDesc.Height = OverlordGame::GetGameSettings().Window.Height;
	renderDesc.EnableDepthSRV = true;
	renderDesc.EnableDepthBuffer = true;
	renderDesc.EnableColorSRV = false;
	renderDesc.EnableColorBuffer = false;
	m_pShadowRT = new RenderTarget(gameContext.pDevice);
	m_pShadowRT->Create(renderDesc);
	m_IsInitialized = true;
}

void ShadowMapRenderer::SetLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction)
{
	//TODO: store the input parameters in the appropriate datamembers
	m_LightPosition = position;
	m_LightDirection = direction;
	//TODO: calculate the Light VP matrix (Directional Light only ;)) and store it in the appropriate datamember
	DirectX::XMVECTOR posLight = DirectX::XMLoadFloat3(&position);
	DirectX::XMVECTOR dirLight = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR upVector = { 0,1,0 };
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(posLight, dirLight, upVector);
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixOrthographicLH(m_Size, m_Size, 0.1f, 350.f);
	DirectX::XMStoreFloat4x4(&m_LightVP, (viewMatrix * projectionMatrix));
}

void ShadowMapRenderer::Begin(const GameContext& gameContext) const
{
	//Reset Texture Register 5 (Unbind)
	ID3D11ShaderResourceView *const pSRV[] = { nullptr };
	gameContext.pDeviceContext->PSSetShaderResources(1, 1, pSRV);

	//TODO: set the appropriate render target that our shadow generator will write to (hint: use the OverlordGame::SetRenderTarget function through SceneManager)
	SceneManager::GetInstance()->GetGame()->SetRenderTarget(m_pShadowRT);
	//TODO: clear this render target
	const float clearColor[4]{ 0,0,0,1 };
	SceneManager::GetInstance()->GetGame()->GetRenderTarget()->Clear(gameContext, clearColor);
	//TODO: set the shader variables of this shadow generator material
	m_pShadowMat->SetLightVP(m_LightVP);
}

void ShadowMapRenderer::End(const GameContext& gameContext) const
{
	UNREFERENCED_PARAMETER(gameContext);
	//TODO: restore default render target (hint: passing nullptr to OverlordGame::SetRenderTarget will do the trick)
	SceneManager::GetInstance()->GetGame()->SetRenderTarget(nullptr);
}

void ShadowMapRenderer::Draw(const GameContext& gameContext, MeshFilter* pMeshFilter, DirectX::XMFLOAT4X4 world, const std::vector<DirectX::XMFLOAT4X4>& bones) const
{
	//TODO: update shader variables in material
	m_pShadowMat->SetBones(reinterpret_cast<const float*>(bones.data()), bones.size());
	m_pShadowMat->SetWorld(world);
	m_pShadowMat->SetLightVP(m_LightVP);
	//TODO: set the correct inputlayout, buffers, topology (some variables are set based on the generation type Skinned or Static)
	ShadowMapMaterial::ShadowGenType shadowType{};
	if (pMeshFilter->m_BoneCount == 0)
		shadowType = ShadowMapMaterial::ShadowGenType::Static;
	else
		shadowType = ShadowMapMaterial::ShadowGenType::Skinned;

	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gameContext.pDeviceContext->IASetInputLayout(m_pShadowMat->m_pInputLayouts[shadowType]);
	auto bufferData = pMeshFilter->GetVertexBufferData(gameContext, m_pShadowMat->m_InputLayoutIds[shadowType]);
	const unsigned int offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &bufferData.pVertexBuffer, &bufferData.VertexStride, &offset);
	gameContext.pDeviceContext->IASetIndexBuffer(pMeshFilter->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//TODO: invoke draw call
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pShadowMat->m_pShadowTechs[shadowType]->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pShadowMat->m_pShadowTechs[shadowType]->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(pMeshFilter->m_IndexCount, 0, 0);
	}
}

void ShadowMapRenderer::UpdateMeshFilter(const GameContext& gameContext, MeshFilter* pMeshFilter)
{
	//TODO: based on the type (Skinned or Static) build the correct vertex buffers for the MeshFilter (Hint use MeshFilter::BuildVertexBuffer)
	ShadowMapMaterial::ShadowGenType shadowType;
	if (pMeshFilter->m_BoneCount == 0)
		shadowType = ShadowMapMaterial::ShadowGenType::Static;
	else
		shadowType = ShadowMapMaterial::ShadowGenType::Skinned;

	pMeshFilter->BuildVertexBuffer(gameContext, m_pShadowMat->m_InputLayoutIds[shadowType],
		m_pShadowMat->m_InputLayoutSizes[shadowType], m_pShadowMat->m_InputLayoutDescriptions[shadowType]);
}

ID3D11ShaderResourceView* ShadowMapRenderer::GetShadowMap() const
{
	//TODO: return the depth shader resource view of the shadow generator render target
	return m_pShadowRT->GetDepthShaderResourceView();
}
