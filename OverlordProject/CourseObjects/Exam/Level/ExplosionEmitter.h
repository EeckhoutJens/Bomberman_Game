#pragma once
#include <GameObject.h>
class ParticleEmitterComponent;
class ExplosionEmitter final : public GameObject
{
public:
	ExplosionEmitter(DirectX::XMFLOAT3 pos, std::vector<float> ranges);
	virtual ~ExplosionEmitter() = default;

	ExplosionEmitter(const ExplosionEmitter& other) = delete;
	ExplosionEmitter(ExplosionEmitter&& other) noexcept = delete;
	ExplosionEmitter& operator=(const ExplosionEmitter& other) = delete;
	ExplosionEmitter& operator=(ExplosionEmitter&& other) noexcept = delete;
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	bool ShouldDestroy() { return m_ShouldDestroy; }

private:
	bool m_ShouldDestroy{ false };
	float m_CurrTimer{0.f};
	DirectX::XMFLOAT3 m_Pos{};
	const float m_MaxLifeTimer{1.f};
	const float m_VelocityPercentage{0.95f};
	const float m_EnergyPercentage{ 0.05f };
	std::vector<float> m_Ranges;

	ParticleEmitterComponent* m_pUpEmitter;
	ParticleEmitterComponent* m_pDownEmitter;
	ParticleEmitterComponent* m_pLeftEmitter;
	ParticleEmitterComponent* m_pRightEmitter;

};

