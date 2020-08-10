#include "stdafx.h"
#include "ExplosionEmitter.h"
#include "Components.h"

ExplosionEmitter::ExplosionEmitter(DirectX::XMFLOAT3 pos, std::vector<float> ranges)
	:m_Ranges{ranges}
	,m_Pos{pos}
	, m_pDownEmitter{nullptr}
	, m_pLeftEmitter{nullptr}
	, m_pUpEmitter{nullptr}
	, m_pRightEmitter{nullptr}
{
}

void ExplosionEmitter::Initialize(const GameContext&)
{

	GetTransform()->Translate(m_Pos);

	//RIGHT EMITTER
	m_pRightEmitter = new ParticleEmitterComponent(L"./Resources/Textures/FireBall.png", 50);
	m_pRightEmitter->SetVelocity(DirectX::XMFLOAT3(m_Ranges[0] * m_VelocityPercentage * 5.f, 0, 0));
	float Energy = m_Ranges[0] * (m_EnergyPercentage / 10.f);
	m_pRightEmitter->SetMaxEnergy(Energy);
	m_pRightEmitter->SetMinEnergy(Energy);
	m_pRightEmitter->SetMinSize(5.5f);
	m_pRightEmitter->SetMaxSize(5.5f);
	m_pRightEmitter->SetMinSizeGrow(2.25f);
	m_pRightEmitter->SetMaxSizeGrow(2.25f);
	m_pRightEmitter->SetMinEmitterRange(0.f);
	m_pRightEmitter->SetMaxEmitterRange(0.f);
	m_pRightEmitter->SetColor(DirectX::XMFLOAT4(1.f, 0.5f, 0.3f, 0.6f));

	//LEFT EMITTER
	m_pLeftEmitter = new ParticleEmitterComponent(L"./Resources/Textures/FireBall.png", 50);
	m_pLeftEmitter->SetVelocity(DirectX::XMFLOAT3(-(m_Ranges[1] * m_VelocityPercentage * 5.f), 0, 0));
	Energy = m_Ranges[1] * (m_EnergyPercentage / 10.f);
	m_pLeftEmitter->SetMaxEnergy(Energy);
	m_pLeftEmitter->SetMinEnergy(Energy);
	m_pLeftEmitter->SetMinSize(5.5f);
	m_pLeftEmitter->SetMaxSize(5.5f);
	m_pLeftEmitter->SetMinSizeGrow(2.25f);
	m_pLeftEmitter->SetMaxSizeGrow(2.25f);
	m_pLeftEmitter->SetMinEmitterRange(0.f);
	m_pLeftEmitter->SetMaxEmitterRange(0.f);
	m_pLeftEmitter->SetColor(DirectX::XMFLOAT4(1.f, 0.5f, 0.3f, 0.6f));

	//UP EMITTER
	m_pUpEmitter = new ParticleEmitterComponent(L"./Resources/Textures/FireBall.png", 50);
	m_pUpEmitter->SetVelocity(DirectX::XMFLOAT3(0, 0, m_Ranges[2] * m_VelocityPercentage * 5.f));
	Energy = m_Ranges[2] * (m_EnergyPercentage / 10.f);
	m_pUpEmitter->SetMaxEnergy(Energy);
	m_pUpEmitter->SetMinEnergy(Energy);
	m_pUpEmitter->SetMinSize(5.5f);
	m_pUpEmitter->SetMaxSize(5.5f);
	m_pUpEmitter->SetMinSizeGrow(2.25f);
	m_pUpEmitter->SetMaxSizeGrow(2.25f);
	m_pUpEmitter->SetMinEmitterRange(0.f);
	m_pUpEmitter->SetMaxEmitterRange(0.f);
	m_pUpEmitter->SetColor(DirectX::XMFLOAT4(1.f, 0.5f, 0.3f, 0.6f));

	//DOWN EMITTER
	m_pDownEmitter = new ParticleEmitterComponent(L"./Resources/Textures/FireBall.png", 50);
	m_pDownEmitter->SetVelocity(DirectX::XMFLOAT3(0, 0, -(m_Ranges[3] * m_VelocityPercentage * 5.f)));
	Energy = m_Ranges[3] * (m_EnergyPercentage / 10.f);
	m_pDownEmitter->SetMaxEnergy(Energy);
	m_pDownEmitter->SetMinEnergy(Energy);
	m_pDownEmitter->SetMinSize(5.5f);
	m_pDownEmitter->SetMaxSize(5.5f);
	m_pDownEmitter->SetMinSizeGrow(2.25f);
	m_pDownEmitter->SetMaxSizeGrow(2.25f);
	m_pDownEmitter->SetMinEmitterRange(0.f);
	m_pDownEmitter->SetMaxEmitterRange(0.f);
	m_pDownEmitter->SetColor(DirectX::XMFLOAT4(1.f, 0.5f, 0.3f, 0.6f));

	AddComponent(m_pRightEmitter);
	AddComponent(m_pLeftEmitter);
	AddComponent(m_pUpEmitter);
	AddComponent(m_pDownEmitter);
}

void ExplosionEmitter::Update(const GameContext& gameContext)
{
	m_CurrTimer += gameContext.pGameTime->GetElapsed();
	if (m_CurrTimer >= m_MaxLifeTimer)
	{
		m_ShouldDestroy = true;
	}
}
