#include "stdafx.h"
#include "PostProcessingMaterial.h"
#include "RenderTarget.h"
#include "OverlordGame.h"
#include "ContentManager.h"

PostProcessingMaterial::PostProcessingMaterial(std::wstring effectFile, unsigned int renderIndex,
                                               std::wstring technique)
	: m_IsInitialized(false), 
	  m_pInputLayout(nullptr),
	  m_pInputLayoutSize(0),
	  m_effectFile(std::move(effectFile)),
	  m_InputLayoutID(0),
	  m_RenderIndex(renderIndex),
	  m_pRenderTarget(nullptr),
	  m_pVertexBuffer(nullptr),
	  m_pIndexBuffer(nullptr),
	  m_NumVertices(0),
	  m_NumIndices(0),
	  m_VertexBufferStride(0),
	  m_pEffect(nullptr),
	  m_pTechnique(nullptr),
	  m_TechniqueName(std::move(technique))
{
}

PostProcessingMaterial::~PostProcessingMaterial()
{
	//TODO: delete and/or release necessary objects and/or resources
	m_pInputLayout->Release();
	m_pInputLayout = nullptr;

	m_pVertexBuffer->Release();
	m_pVertexBuffer = nullptr;

	m_pIndexBuffer->Release();
	m_pIndexBuffer = nullptr;

	delete m_pRenderTarget;
	m_pRenderTarget = nullptr;
}

void PostProcessingMaterial::Initialize(const GameContext& gameContext)
{
	if (!m_IsInitialized)
	{
		//TODO: complete
		//1. LoadEffect (LoadEffect(...))
		LoadEffect(gameContext, m_effectFile);
		//2. CreateInputLaytout (CreateInputLayout(...))
		CreateIndexBuffer(gameContext);
		//   CreateVertexBuffer (CreateVertexBuffer(...)) > As a TriangleStrip (FullScreen Quad)
		CreateVertexBuffer(gameContext);
		//3. Create RenderTarget (m_pRenderTarget)
		//		Take a look at the class, figure out how to initialize/create a RenderTarget Object
		//		GameSettings > OverlordGame::GetGameSettings()
		const auto windowSettings = OverlordGame::GetGameSettings().Window;
		RENDERTARGET_DESC RTDesc{};
		RTDesc.Height = windowSettings.Height;
		RTDesc.Width = windowSettings.Width;
		RTDesc.EnableColorSRV = true;
		m_pRenderTarget = new RenderTarget(gameContext.pDevice);
		m_pRenderTarget->Create(RTDesc);



		m_IsInitialized = true;
	}
}

bool PostProcessingMaterial::LoadEffect(const GameContext& gameContext, const std::wstring& effectFile)
{
	//TODO: complete
	//Load Effect through ContentManager
	m_pEffect = ContentManager::Load<ID3DX11Effect>(effectFile);
	//Check if m_TechniqueName (default constructor parameter) is set
	// If SET > Use this Technique (+ check if valid)
	// If !SET > Use Technique with index 0
	if (m_TechniqueName != L"")
		m_pTechnique = m_pEffect->GetTechniqueByName(reinterpret_cast<LPCSTR>(m_TechniqueName.c_str()));
	else
		m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, m_pInputLayoutDescriptions, m_pInputLayoutSize, m_InputLayoutID);

	//Call LoadEffectVariables
	LoadEffectVariables();

	return true;
}

void PostProcessingMaterial::Draw(const GameContext& gameContext, RenderTarget* previousRendertarget)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(previousRendertarget);

	//TODO: complete
	//1. Clear the object's RenderTarget (m_pRenderTarget) [Check RenderTarget Class]
	float color[4]{ 0.5f,0.5f,0.5f,1.f };
	m_pRenderTarget->Clear(gameContext, color);

	//2. Call UpdateEffectVariables(...)
	UpdateEffectVariables(previousRendertarget);

	//3. Set InputLayout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//4. Set VertexBuffer
	m_VertexBufferStride = sizeof(VertexPosTex);
	UINT offset{ 0 };
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &offset);

	//5. Set PrimitiveTopology (TRIANGLELIST)
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//6. Draw 
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT currP{}; currP < techDesc.Passes; ++currP)
	{
		m_pTechnique->GetPassByIndex(currP)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}

	// Generate Mips
	//gameContext.pDeviceContext->GenerateMips(m_pRenderTarget->GetShaderResourceView());

	ID3D11ShaderResourceView* const pNullSRV{ nullptr };
	gameContext.pDeviceContext->PSSetShaderResources(0, 1, &pNullSRV);
}

void PostProcessingMaterial::CreateVertexBuffer(const GameContext& gameContext)
{
	m_NumVertices = 4;

	UNREFERENCED_PARAMETER(gameContext);
	//TODO: complete
	//Create vertex array containing three elements in system memory
	VertexPosTex vertices[]
	{
		VertexPosTex{{-1.f, -1.f, 0.f}, {0.f,1.0f}},
		VertexPosTex{{-1.f, 1.f, 0.f}, {0.f,0.0f}},
		VertexPosTex{{1.f, 1.f, 0.f}, {1.f,0.0f}},
		VertexPosTex{{1.f, -1.f, 0.f}, {1.f,1.0f}},
	};

	//fill a buffer description to copy the vertexdata into graphics memory
	D3D11_BUFFER_DESC buffDesc;
	ZeroMemory(&buffDesc, sizeof(buffDesc));
	buffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	buffDesc.ByteWidth = sizeof(VertexPosTex) * m_NumVertices;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
	vertexSubResourceData.pSysMem = vertices;

	//create a ID3D10Buffer in graphics memory containing the vertex info
	auto hr = gameContext.pDevice->CreateBuffer(&buffDesc, &vertexSubResourceData, &m_pVertexBuffer);
	if (Logger::LogHResult(hr, L"PostProcessingMaterial::CreateVertexBuffer"))
		return;
}

void PostProcessingMaterial::CreateIndexBuffer(const GameContext& gameContext)
{
	m_NumIndices = 6;

	UNREFERENCED_PARAMETER(gameContext);
	//TODO: complete
	// Create index buffer
	m_NumIndices = 6;
	UINT indices[] =
	{
		0,1,2,
		0,2,3
	};
	UNREFERENCED_PARAMETER(gameContext);
	if (m_pIndexBuffer != nullptr)
		return;

	D3D11_BUFFER_DESC bd = {};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexSubResourceData;
	ZeroMemory(&indexSubResourceData, sizeof(indexSubResourceData));
	indexSubResourceData.pSysMem = indices;

	auto hr = gameContext.pDevice->CreateBuffer(&bd, &indexSubResourceData, &m_pIndexBuffer);
	Logger::LogHResult(hr, L"PostProcessingMaterial::CreateIndexBuffer()");
}
