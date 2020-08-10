#include "stdafx.h"
#include "ParticleEmitterComponent.h"
 #include <utility>
#include "EffectHelper.h"
#include "GameObject.h"
#include "ContentManager.h"
#include "TextureDataLoader.h"
#include "Particle.h"
#include "TransformComponent.h"

ParticleEmitterComponent::ParticleEmitterComponent(std::wstring  assetFile, int particleCount):
	m_pVertexBuffer(nullptr),
	m_pEffect(nullptr),
	m_pParticleTexture(nullptr),
	m_pInputLayout(nullptr),
	m_pInputLayoutSize(0),
	m_Settings(ParticleEmitterSettings()),
	m_ParticleCount(particleCount),
	m_ActiveParticles(0),
	m_LastParticleInit(0.0f),
	m_AssetFile(std::move(assetFile))
{
	//TODO: See Lab9_2
	for (int currNr{}; currNr < m_ParticleCount; ++currNr)
	{
		Particle* pParticle = new Particle(m_Settings);
		m_Particles.push_back(pParticle);
	}
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	//TODO: See Lab9_2
	for (Particle* pParticle : m_Particles)
	{
		delete pParticle;
		pParticle = nullptr;
	}
	m_Particles.clear();
	m_pInputLayout->Release();
	m_pVertexBuffer->Release();
}

void ParticleEmitterComponent::Initialize(const GameContext& gameContext)
{
	//TODO: See Lab9_2
	LoadEffect(gameContext);
	CreateVertexBuffer(gameContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::LoadEffect(const GameContext& gameContext)
{
	//TODO: See Lab9_2
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Resources/Effects/ParticleRenderer.fx");
	m_pDefaultTechnique = m_pEffect->GetTechniqueByIndex(0);
	m_pWvpVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pViewInverseVariable = m_pEffect->GetVariableByName("gViewInverse")->AsMatrix();
	m_pTextureVariable = m_pEffect->GetVariableByName("gParticleTexture")->AsShaderResource();
	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pDefaultTechnique, &m_pInputLayout, m_pInputLayoutSize);
}

void ParticleEmitterComponent::CreateVertexBuffer(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//TODO: See Lab9_2
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();

	//Vertexbuffer
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { nullptr };
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ParticleVertex) * m_ParticleCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	initData.pSysMem = m_Particles.data();
	const HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Vertexbuffer");
}

void ParticleEmitterComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//TODO: See Lab9_2
	float averageEnergy = (m_Settings.MaxEnergy + m_Settings.MinEnergy) / 2;
	float particleInterval = averageEnergy / m_ParticleCount;
	m_LastParticleInit += gameContext.pGameTime->GetElapsed();
	m_ActiveParticles = 0;

	//BUFFER MAPPING CODE [PARTIAL :)]
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ParticleVertex* pBuffer = (ParticleVertex*)mappedResource.pData;

	for (Particle* pParticle : m_Particles)
	{
		pParticle->Update(gameContext);
		if (pParticle->IsActive())
		{
			pBuffer[m_ActiveParticles] = pParticle->GetVertexInfo();
			++m_ActiveParticles;
		}
		else if (m_LastParticleInit >= particleInterval)
		{
			pParticle->Init(GetGameObject()->GetTransform()->GetPosition());
			pBuffer[m_ActiveParticles] = pParticle->GetVertexInfo();
			++m_ActiveParticles;
			m_LastParticleInit = 0;
		}
	}
	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::Draw(const GameContext& )
{}

void ParticleEmitterComponent::PostDraw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//TODO: See Lab9_2
	const float* pVPMatrix = &gameContext.pCamera->GetViewProjection()._11;
	m_pWvpVariable->SetMatrix(pVPMatrix);

	const float* pVIMatrix = &gameContext.pCamera->GetViewInverse()._11;
	m_pViewInverseVariable->SetMatrix(pVIMatrix);

	m_pTextureVariable->SetResource(m_pParticleTexture->GetShaderResourceView());

	// Set the input layout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	// Set primitive topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Set vertex buffer
	const unsigned int stride = sizeof(ParticleVertex);
	unsigned int offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pDefaultTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pDefaultTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}
