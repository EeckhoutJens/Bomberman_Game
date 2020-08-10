#include "stdafx.h"
#include "ControlScreen.h"
#include "GameObject.h"
#include "Components.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ExamScene.h"

ControlScreen::ControlScreen() : GameScene(L"ControlScreen")
,m_pControlScreenSprite(nullptr)
{
}

void ControlScreen::Initialize()
{
	m_pControlScreenSprite = new GameObject();
	m_pControlScreenSprite->AddComponent(new SpriteComponent(L"./Resources/Textures/ControlScreen.png", DirectX::XMFLOAT2(0.5f, 0.5f)));
	AddChild(m_pControlScreenSprite);
	m_pControlScreenSprite->GetTransform()->Translate(640, 360, 0);
	InputAction iaStart{ 0,InputTriggerState::Pressed,VK_SPACE,-1,XINPUT_GAMEPAD_START };
	GetGameContext().pInput->AddInputAction(iaStart);
}

void ControlScreen::Update()
{
	GetGameContext().pInput->RefreshControllerConnections();
	if (GetGameContext().pInput->IsActionTriggered(0))
	{
		SceneManager::GetInstance()->NextScene();
	}
}

void ControlScreen::Draw()
{
}
