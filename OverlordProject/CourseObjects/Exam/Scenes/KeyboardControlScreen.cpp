#include "stdafx.h"
#include "KeyboardControlScreen.h"
#include "GameObject.h"
#include "Components.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ExamScene.h"

KeyboardControlScreen::KeyboardControlScreen() : GameScene(L"KeyboardControlScreen")
, m_pControlScreenSprite{nullptr}
{
}

void KeyboardControlScreen::Initialize()
{
	m_pControlScreenSprite = new GameObject();
	m_pControlScreenSprite->AddComponent(new SpriteComponent(L"./Resources/Textures/KeyboardControls.png", DirectX::XMFLOAT2(0.5f, 0.5f)));
	AddChild(m_pControlScreenSprite);
	m_pControlScreenSprite->GetTransform()->Translate(640, 360, 0);
	InputAction iaStart{ 0,InputTriggerState::Pressed,VK_SPACE,-1,XINPUT_GAMEPAD_START };
	GetGameContext().pInput->AddInputAction(iaStart);
}

void KeyboardControlScreen::Update()
{
	GetGameContext().pInput->RefreshControllerConnections();
	if (GetGameContext().pInput->IsActionTriggered(0))
	{
		SceneManager::GetInstance()->NextScene();
	}
}

void KeyboardControlScreen::Draw()
{
}
