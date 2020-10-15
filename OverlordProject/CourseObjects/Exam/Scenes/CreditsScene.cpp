#include "stdafx.h"
#include "CreditsScene.h"
#include "GameObject.h"
#include "Components.h"
#include "SceneManager.h"
CreditsScene::CreditsScene() : GameScene(L"CreditsScene")
,m_pCreditsScreenSprite(nullptr)
{
}

void CreditsScene::Initialize()
{
	//ADD SPRITE, SET PIVOT AND PUT IT TO CENTER OF THE SCREEN
	m_pCreditsScreenSprite = new GameObject();
	m_pCreditsScreenSprite->AddComponent(new SpriteComponent(L"./Resources/Textures/CreditsScreen.png", DirectX::XMFLOAT2(0.5f, 0.5f)));
	AddChild(m_pCreditsScreenSprite);
	m_pCreditsScreenSprite->GetTransform()->Translate(640, 360, 0);
	InputAction iaStart{ 0,InputTriggerState::Pressed,VK_RETURN,-1,XINPUT_GAMEPAD_START };
	GetGameContext().pInput->AddInputAction(iaStart);
}

void CreditsScene::Update()
{
	GetGameContext().pInput->RefreshControllerConnections();
	if (GetGameContext().pInput->IsActionTriggered(0))
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"TitleScreen");
	}
}

void CreditsScene::Draw()
{
}
