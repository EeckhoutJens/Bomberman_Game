#include "stdafx.h"
#include "Particle.h"

// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
using namespace DirectX;

Particle::Particle(const ParticleEmitterSettings& emitterSettings):
	m_VertexInfo(ParticleVertex()),
	m_EmitterSettings(emitterSettings),
	m_IsActive(false),
	m_TotalEnergy(0),
	m_CurrentEnergy(0),
	m_SizeGrow(0),
	m_InitSize(0)
{}

void Particle::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//TODO: See Lab9_2
	if (!m_IsActive)
		return;

	m_CurrentEnergy -= gameContext.pGameTime->GetElapsed();
	if (m_CurrentEnergy <= 0)
	{
		m_IsActive = false;
		return;
	}

	//UPDATE POSITION
	m_VertexInfo.Position.x += m_EmitterSettings.Velocity.x * gameContext.pGameTime->GetElapsed();
	m_VertexInfo.Position.y += m_EmitterSettings.Velocity.y * gameContext.pGameTime->GetElapsed();
	m_VertexInfo.Position.z += m_EmitterSettings.Velocity.z * gameContext.pGameTime->GetElapsed();

	//UPDATE COLOR
	m_VertexInfo.Color = m_EmitterSettings.Color;
	float particleLifePercent = m_CurrentEnergy / m_TotalEnergy;
	m_VertexInfo.Color.w = particleLifePercent * 2;

	//UPDATE SIZE
	if (m_SizeGrow < 1)
	{
		m_VertexInfo.Size = m_InitSize - (1 - m_SizeGrow) * (1 - particleLifePercent) * m_InitSize;
	}
	else if (m_SizeGrow > 1)
	{
		m_VertexInfo.Size = m_InitSize + (m_SizeGrow - 1) * (1 - particleLifePercent) * m_InitSize;
	}


}

void Particle::Init(XMFLOAT3 initPosition)
{
	UNREFERENCED_PARAMETER(initPosition);
	//TODO: See Lab9_2
	m_IsActive = true;

	//ENERGY INITIALISATION
	float randFloat = randF(m_EmitterSettings.MinEnergy, m_EmitterSettings.MaxEnergy);
	m_TotalEnergy = randFloat;
	m_CurrentEnergy = randFloat;

	//POSITION INITIALISATION
	DirectX::XMVECTOR randDirection{ 1,0,0 };
	auto randRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI));
	randDirection = XMVector3TransformNormal(randDirection, randRotationMatrix);
	float randomRange = randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange);
	DirectX::XMFLOAT3 receivedDirection;
	XMStoreFloat3(&receivedDirection, randDirection);
	m_VertexInfo.Position.x = initPosition.x + receivedDirection.x * randomRange;
	m_VertexInfo.Position.y = initPosition.y + receivedDirection.y * randomRange;
	m_VertexInfo.Position.z = initPosition.z + receivedDirection.z * randomRange;

	//SIZE INITIALISATION
	float randSize = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);
	float randGrow = randF(m_EmitterSettings.MinSizeGrow, m_EmitterSettings.MaxSizeGrow);
	m_VertexInfo.Size = randSize;
	m_InitSize = randSize;
	m_SizeGrow = randGrow;

	//ROTATION INITIALISATION
	float randRotation = randF(-XM_PI, XM_PI);
	m_VertexInfo.Rotation = randRotation;

}
