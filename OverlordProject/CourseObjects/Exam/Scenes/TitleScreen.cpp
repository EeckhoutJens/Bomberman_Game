#include "stdafx.h"
#include "TitleScreen.h"
#include "GameObject.h"
#include "Components.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ControlScreen.h"
#include "OverlordGame.h"

TitleScreen::TitleScreen() : GameScene(L"TitleScreen")
,m_pTitleScreenSprite(nullptr)
,m_pCursorSprite(nullptr)
{
}

void TitleScreen::Initialize()
{
	m_pTitleScreenSprite = new GameObject();
	m_pTitleScreenSprite->AddComponent(new SpriteComponent(L"./Resources/Textures/TitleScreen.png",DirectX::XMFLOAT2(0.5f,0.5f)));
	AddChild(m_pTitleScreenSprite);
	m_pTitleScreenSprite->GetTransform()->Translate(640, 360, 0);

	m_pCursorSprite = new GameObject();
	m_pCursorSprite->AddComponent(new SpriteComponent(L"./Resources/Textures/BombCursorP3.png",DirectX::XMFLOAT2(0.5f,0.5f)));
	AddChild(m_pCursorSprite);
	m_pCursorSprite->GetTransform()->Scale(0.3f, 0.3f, 0.3f);
	m_pCursorSprite->GetTransform()->Translate(m_CursorPosStart);



	InputAction iaStart{ 0,InputTriggerState::Pressed,VK_SPACE,-1,XINPUT_GAMEPAD_START,GamepadIndex::PlayerOne };
	InputAction iaUp{ 1,InputTriggerState::Released,'W',-1,XINPUT_GAMEPAD_DPAD_UP,GamepadIndex::PlayerOne };
	InputAction iaDown{ 2,InputTriggerState::Released,'S',-1,XINPUT_GAMEPAD_DPAD_DOWN,GamepadIndex::PlayerOne };
	GetGameContext().pInput->AddInputAction(iaStart);
	GetGameContext().pInput->AddInputAction(iaUp);
	GetGameContext().pInput->AddInputAction(iaDown);
}

void TitleScreen::Update()
{
	GetGameContext().pInput->RefreshControllerConnections();
	if (GetGameContext().pInput->IsActionTriggered(0))
	{
		if (m_SelectedOption == TSOptions::EXIT)
		{
			PostQuitMessage(0);
		}
		else if (m_SelectedOption == TSOptions::START)
		{
			SceneManager::GetInstance()->NextScene();
		}
		else if (m_SelectedOption == TSOptions::CREDITS)
		{
			SceneManager::GetInstance()->SetActiveGameScene(L"CreditsScene");
		}
	}

	if (GetGameContext().pInput->IsActionTriggered(1))
	{
		if (m_SelectedOption == TSOptions::EXIT)
		{
			m_SelectedOption = TSOptions::START;
			m_pCursorSprite->GetTransform()->Translate(m_CursorPosStart);
		}

		else if (m_SelectedOption == TSOptions::CREDITS)
		{
			m_SelectedOption = TSOptions::EXIT;
			m_pCursorSprite->GetTransform()->Translate(m_CursorPosExit);
		}
	}

	if (GetGameContext().pInput->IsActionTriggered(2))
	{
		if (m_SelectedOption == TSOptions::START)
		{
			m_SelectedOption = TSOptions::EXIT;
			m_pCursorSprite->GetTransform()->Translate(m_CursorPosExit);
		}
		else if (m_SelectedOption == TSOptions::EXIT)
		{
			m_SelectedOption = TSOptions::CREDITS;
			m_pCursorSprite->GetTransform()->Translate(m_CursorPosCredits);
		}
	}
}

void TitleScreen::Draw()
{
}
