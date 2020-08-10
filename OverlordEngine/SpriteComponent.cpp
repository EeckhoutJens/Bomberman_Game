#include "stdafx.h"

#include "SpriteComponent.h"
 #include <utility>

#include "GameObject.h"
#include "TextureData.h"
#include "ContentManager.h"
#include "SpriteRenderer.h"
#include "TransformComponent.h"

SpriteComponent::SpriteComponent(std::wstring spriteAsset, DirectX::XMFLOAT2 pivot, DirectX::XMFLOAT4 color):
	m_pTexture(nullptr),
	m_SpriteAsset(std::move(spriteAsset)),
	m_Pivot(pivot),
	m_Color(color)
{
}

void SpriteComponent::Initialize(const GameContext& )
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Update(const GameContext& )
{
}

void SpriteComponent::Draw(const GameContext&)
{
	if (!m_pTexture)
		return;

	//TODO: Here you need to draw the SpriteComponent using the Draw of the sprite renderer
	// The sprite renderer is a singleton
	DirectX::XMFLOAT3 position = GetTransform()->GetWorldPosition();
	DirectX::XMFLOAT3 scale = GetTransform()->GetWorldScale();
	float rotation = QuaternionToEuler(GetTransform()->GetWorldRotation()).z;
	auto spriteRenderer = SpriteRenderer::GetInstance();
	spriteRenderer->Draw(m_pTexture, DirectX::XMFLOAT2(position.x, position.y), m_Color, m_Pivot, DirectX::XMFLOAT2(scale.x, scale.y), rotation, this->GetTransform()->GetPosition().z);
	// you will need to position, the rotation and the scale
	// You can use the QuaternionToEuler function to help you with the z rotation 
}
