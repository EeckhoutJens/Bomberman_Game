#include "stdafx.h"
#include "vld.h"
#include "PhysxManager.h"
#include "GameObject.h"
#include "Components.h"
#include "ContentManager.h"
#include "ExamScene.h"
#include "SpriteFont.h"
#include "TextRenderer.h"
#include "../Character/ExamCharacter.h"
#include "FixedCamera.h"
#include "../Character/Bomb.h"
#include "../Level/Grid.h"
#include "../Level/LevelBorder.h"
#include "../PowerUps/BombPowerUp.h"
#include "../PowerUps/RangePowerUp.h"
#include "OverlordGame.h"
#include "SceneManager.h"
#include "../Level/ExplosionEmitter.h"
#include "../OverlordProject/Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"
#include "../OverlordProject/Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "DebugRenderer.h"

ExamScene::ExamScene() : GameScene(L"ExamScene") 
{
}

ExamScene::~ExamScene()
{
	delete m_pGrid;
	m_pGrid = nullptr;

	delete m_pBorder;
	m_pBorder = nullptr;
}

void ExamScene::Initialize()
{
	//SET UP LIGHT
	//*************
	std::srand(unsigned int(std::time(0)));
	GameContext context = GetGameContext();
	context.pShadowMapper->SetLight({ 0,150.f,-40.f }, { 0.740129888f, -0.497205281f, -0.609117377f });
	DebugRenderer::ToggleDebugRenderer();

	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	physx::PxMaterial* pGroundMat = physX->createMaterial(0, 0, 0);
	GameObject* pGround = new GameObject();
	pGround->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<physx::PxGeometry> geom(new physx::PxPlaneGeometry());
	pGround->AddComponent(new ColliderComponent(geom, *pGroundMat, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));
	AddChild(pGround);

	//CAMERA
	//******
	m_pCamera = new FixedCamera{};
	m_pCamera->AddComponent(new CameraComponent{});
	AddChild(m_pCamera);
	CameraComponent* pCameraComponent = m_pCamera->GetComponent<CameraComponent>();
	pCameraComponent->SetActive();
	m_pCamera->GetTransform()->Translate(0, 175, -120);
	m_pCamera->GetTransform()->Rotate(55, 0, 0, true);


	//MATERIALS
	//*********
	SkinnedDiffuseMaterial_Shadow* diffMatP1 = new SkinnedDiffuseMaterial_Shadow();
	diffMatP1->SetDiffuseTexture(L"./Resources/Textures/Character_diffuse.png");
	diffMatP1->SetNormalTexture(L"./Resources/Textures/Character_normal.png");
	diffMatP1->SetLightDirection(context.pShadowMapper->GetLightDirection());
	context.pMaterialManager->AddMaterial(diffMatP1, 0);

	SkinnedDiffuseMaterial_Shadow* diffMatP2 = new SkinnedDiffuseMaterial_Shadow(); 
	diffMatP2->SetDiffuseTexture(L"./Resources/Textures/Character2_diffuse.png");
	diffMatP2->SetNormalTexture(L"./Resources/Textures/Character_normal.png");
	diffMatP2->SetLightDirection(context.pShadowMapper->GetLightDirection());
	context.pMaterialManager->AddMaterial(diffMatP2, 1);

	SkinnedDiffuseMaterial_Shadow*  diffMatP3 = new SkinnedDiffuseMaterial_Shadow();
	diffMatP3->SetDiffuseTexture(L"./Resources/Textures/Character3_diffuse.png");
	diffMatP3->SetNormalTexture(L"./Resources/Textures/Character_normal.png");
	diffMatP3->SetLightDirection(context.pShadowMapper->GetLightDirection());
	context.pMaterialManager->AddMaterial(diffMatP3, 2);

	SkinnedDiffuseMaterial_Shadow* diffMatP4 = new SkinnedDiffuseMaterial_Shadow();
	diffMatP4->SetDiffuseTexture(L"./Resources/Textures/Character4_diffuse.png");
	diffMatP4->SetNormalTexture(L"./Resources/Textures/Character_normal.png");
	diffMatP4->SetLightDirection(context.pShadowMapper->GetLightDirection());
	context.pMaterialManager->AddMaterial(diffMatP4, 3);

	DiffuseMaterial_Shadow* diffMatFloor = new DiffuseMaterial_Shadow();
	diffMatFloor->SetDiffuseTexture(L"./Resources/Textures/Floor.png");
	diffMatFloor->SetLightDirection(context.pShadowMapper->GetLightDirection());
	context.pMaterialManager->AddMaterial(diffMatFloor, 4);

	DiffuseMaterial_Shadow* diffMatMetalBox = new DiffuseMaterial_Shadow();
	diffMatMetalBox->SetDiffuseTexture(L"./Resources/Textures/MetalBox_diffuse.png");
	diffMatMetalBox->SetLightDirection(context.pShadowMapper->GetLightDirection());
	context.pMaterialManager->AddMaterial(diffMatMetalBox, 5);

	DiffuseMaterial_Shadow* diffMatWoodBox = new DiffuseMaterial_Shadow();
	diffMatWoodBox->SetDiffuseTexture(L"./Resources/Textures/WoodBox_diffuse.png");
	diffMatWoodBox->SetLightDirection(context.pShadowMapper->GetLightDirection());
	context.pMaterialManager->AddMaterial(diffMatWoodBox, 6);

	DiffuseMaterial_Shadow* diffMatBomb = new DiffuseMaterial_Shadow();
	diffMatBomb->SetLightDirection(context.pShadowMapper->GetLightDirection());
	diffMatBomb->SetDiffuseTexture(L"./Resources/Textures/Bomb_diffuseP1.png");
	context.pMaterialManager->AddMaterial(diffMatBomb, 7);

	DiffuseMaterial_Shadow* diffMatBomb2 = new DiffuseMaterial_Shadow();
	diffMatBomb2->SetLightDirection(context.pShadowMapper->GetLightDirection());
	diffMatBomb2->SetDiffuseTexture(L"./Resources/Textures/Bomb_diffuseP2.png");
	context.pMaterialManager->AddMaterial(diffMatBomb2, 10);

	DiffuseMaterial_Shadow* diffMatBomb3 = new DiffuseMaterial_Shadow();
	diffMatBomb3->SetLightDirection(context.pShadowMapper->GetLightDirection());
	diffMatBomb3->SetDiffuseTexture(L"./Resources/Textures/Bomb_diffuseP3.png");
	context.pMaterialManager->AddMaterial(diffMatBomb3, 11);

	DiffuseMaterial_Shadow* diffMatBomb4 = new DiffuseMaterial_Shadow();
	diffMatBomb4->SetLightDirection(context.pShadowMapper->GetLightDirection());
	diffMatBomb4->SetDiffuseTexture(L"./Resources/Textures/Bomb_diffuseP4.png");
	context.pMaterialManager->AddMaterial(diffMatBomb4, 12);

	DiffuseMaterial_Shadow* diffMatBombPU = new DiffuseMaterial_Shadow();
	diffMatBombPU->SetLightDirection(context.pShadowMapper->GetLightDirection());
	diffMatBombPU->SetDiffuseTexture(L"./Resources/Textures/BombPowerUp.png");
	context.pMaterialManager->AddMaterial(diffMatBombPU, 8);

	DiffuseMaterial_Shadow* diffMatRangePU = new DiffuseMaterial_Shadow();
	diffMatRangePU->SetLightDirection(context.pShadowMapper->GetLightDirection());
	diffMatRangePU->SetDiffuseTexture(L"./Resources/Textures/RangePowerUp.png");
	context.pMaterialManager->AddMaterial(diffMatRangePU, 9);


	//LEVEL
	//*****
	GameObject* pGroundObj = new GameObject();
	ModelComponent* pGroundModel = new ModelComponent(L"./Resources/Meshes/UnitPlane.ovm");
	pGroundModel->SetMaterial(4);

	pGroundObj->AddComponent(pGroundModel);
	pGroundObj->GetTransform()->Scale(14.46f, 14.46f, 14.46f);

	m_pGrid = new Grid(DirectX::XMFLOAT2(-58.5f, -58.5f), 135, 135, 14, 14, 9.f, 6, 5);
	m_pBorder = new LevelBorder(DirectX::XMFLOAT2(-67.5, -67.5), 135, 135, 9.f, 5);

	AddChild(pGroundObj);
	auto gridObjectsToAdd = m_pGrid->GetGameObjects();
	auto borderObjectsToAdd = m_pBorder->GetGameObjects();

	for (GameObject* pObject : *gridObjectsToAdd)
	{
		if (pObject)
		{
			AddChild(pObject);
		}
	}

	for (GameObject* pObject : borderObjectsToAdd)
	{
		AddChild(pObject);
	}

	//CHARACTERS
	//*********
	ExamCharacter* P1 = new ExamCharacter{ 4,11,100,pCameraComponent,0,7 };
	P1->GetTransform()->Translate(-58.5f, 0.f, -58.5f);
	AddChild(P1);
	m_VecCharacters.push_back(P1);

	ExamCharacter* P2 = new ExamCharacter{ 4,11,100,pCameraComponent,1,10 };
	P2->GetTransform()->Translate(58.5f, 0, 58.5f);
	AddChild(P2);
	m_VecCharacters.push_back(P2);

	//UI SETUP
	//********
	const GameSettings settings = OverlordGame::GetGameSettings();
	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Magneto.fnt");
	m_pResultFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Consolas_32.fnt");
	m_pP1Stats = new GameObject();
	m_pP2Stats = new GameObject();
	m_pPauseScreen = new GameObject(); 

	m_pP1Stats->AddComponent(new SpriteComponent(L"./Resources/Textures/CharacterStatsP1.png"));
	m_pP2Stats->AddComponent(new SpriteComponent(L"./Resources/Textures/CharacterStatsP2.png", DirectX::XMFLOAT2(1, 0)));
	m_pPauseScreen->AddComponent(new SpriteComponent(L"./Resources/Textures/TestMenu.png", DirectX::XMFLOAT2(0.5f, 0.5f)));

	m_pP1Stats->GetTransform()->Scale(0.2f, 0.2f, 0.2f);
	m_pP1Stats->GetTransform()->Translate(0, 0, 0);
	m_pP2Stats->GetTransform()->Translate(float(settings.Window.Width), 0, 0);
	m_pP2Stats->GetTransform()->Scale(0.2f, 0.2f, 0.2f);
	m_pPauseScreen->GetTransform()->Translate(float(settings.Window.Width) / 2.f, float(settings.Window.Height) / 2.f, 0);
	
	AddChild(m_pP1Stats);
	AddChild(m_pP2Stats);
	AddChild(m_pPauseScreen);

	m_pPauseScreen->SetRender(false);

}

void ExamScene::Update()
{
	if (m_VibrationTimer >= m_MaxVibrationTime)
	{
		for (ExamCharacter* pChar : m_VecCharacters)
		{
			GetGameContext().pInput->StopVibration(GamepadIndex(pChar->GetCharacterNr()));
		}
	}
	else
		m_VibrationTimer += GetGameContext().pGameTime->GetElapsed();

	if (!m_IsPaused && !m_EndGame)
	{
		GetGameContext().pInput->RefreshControllerConnections();
		CreateNewCharacter();
		CheckCharacterBombs();
		CheckCharacters();
		CheckPowerUps();
		CheckEmitters();
	}
	else if(m_IsPaused && !m_EndGame)
	{
		if (GetGameContext().pInput->IsActionTriggered(m_UnpauseActionID))
		{
			Unpause();
		}

		if (GetGameContext().pInput->IsActionTriggered(m_MenuUpActionID))
		{
			if (m_currSelectedMenuOption > 0)
				--m_currSelectedMenuOption;

			PMOptions currOption = m_arrPauseOptions[m_currSelectedMenuOption];

			switch (currOption)
			{
			case PMOptions::BackToMain:
				m_pPauseMenuCursor->GetTransform()->Translate(m_CursorPosBackToMain);
				break;
			case PMOptions::Restart:
				m_pPauseMenuCursor->GetTransform()->Translate(m_CursorPosRestart);
				break;
			case PMOptions::Quit:
				m_pPauseMenuCursor->GetTransform()->Translate(m_CursorPosQuit);
				break;
			default:
				break;
			}
			
		}

		if (GetGameContext().pInput->IsActionTriggered(m_MenuDownActionID))
		{
			if (m_currSelectedMenuOption < 2)
				++m_currSelectedMenuOption;

			PMOptions currOption = m_arrPauseOptions[m_currSelectedMenuOption];

			switch (currOption)
			{
			case PMOptions::BackToMain:
				m_pPauseMenuCursor->GetTransform()->Translate(m_CursorPosBackToMain);
				break;
			case PMOptions::Restart:
				m_pPauseMenuCursor->GetTransform()->Translate(m_CursorPosRestart);
				break;
			case PMOptions::Quit:
				m_pPauseMenuCursor->GetTransform()->Translate(m_CursorPosQuit);
				break;
			default:
				break;
			}
		}

		if (GetGameContext().pInput->IsActionTriggered(m_MenuSelectActionID))
		{
			PMOptions currOption = m_arrPauseOptions[m_currSelectedMenuOption];
			switch (currOption)
			{
			case PMOptions::BackToMain:
				SceneManager::GetInstance()->SetActiveGameScene(L"TitleScreen");
				break;
			case PMOptions::Restart:
				Restart();
				Unpause();
				break;
			case PMOptions::Quit:
				PostQuitMessage(0);
				break;
			default:
				break;
			}
		}
	}
	if (m_EndGame)
	{
		if (GetGameContext().pInput->IsActionTriggered(m_UnpauseActionID))
		{
			Unpause();
			Restart();
		}
	}
}

void ExamScene::Draw()
{
	const GameContext gameContext = GetGameContext();
	GameSettings gameSettings = OverlordGame::GetGameSettings();
	if (m_pFont->GetFontName() != L""
		&& m_pResultFont->GetFontName() != L"")
	{

			std::wstring FPSDisplay{ L"FPS: " };
			std::wstring convertedFPS{ std::to_wstring(gameContext.pGameTime->GetFPS()) };
			FPSDisplay.append(convertedFPS);
			std::wstring convertedP1Score{ std::to_wstring(m_P1Score) };
			std::wstring convertedP1Bombs{ std::to_wstring(m_P1Bombs) };
			std::wstring convertedP1Range{ std::to_wstring(m_P1Range) };
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP1Score, DirectX::XMFLOAT2(80, 95), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP1Bombs, DirectX::XMFLOAT2(80, 128), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP1Range, DirectX::XMFLOAT2(80, 167), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
			TextRenderer::GetInstance()->DrawText(m_pFont, FPSDisplay, DirectX::XMFLOAT2(587.5f, 30), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::White));
			std::wstring convertedP2Score{ std::to_wstring(m_P2Score) };
			std::wstring convertedP2Bombs{ std::to_wstring(m_P2Bombs) };
			std::wstring convertedP2Range{ std::to_wstring(m_P2Range) };
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP2Score, DirectX::XMFLOAT2(1175, 95), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP2Bombs, DirectX::XMFLOAT2(1175, 128), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP2Range, DirectX::XMFLOAT2(1175, 167), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
		
			
		

		if (createdCharacter3)
		{
			std::wstring convertedP3Score{ std::to_wstring(m_P3Score) };
			std::wstring convertedP3Bombs{ std::to_wstring(m_P3Bombs) };
			std::wstring convertedP3Range{ std::to_wstring(m_P3Range) };
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP3Score, DirectX::XMFLOAT2(80, 618), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP3Bombs, DirectX::XMFLOAT2(80, 648), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP3Range, DirectX::XMFLOAT2(80, 678), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
		}

		if (createdCharacter4)
		{
			std::wstring convertedP4Score{ std::to_wstring(m_P4Score) };
			std::wstring convertedP4Bombs{ std::to_wstring(m_P4Bombs) };
			std::wstring convertedP4Range{ std::to_wstring(m_P4Range) };
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP4Score, DirectX::XMFLOAT2(1175, 618), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP4Bombs, DirectX::XMFLOAT2(1175, 648), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
			TextRenderer::GetInstance()->DrawText(m_pFont, convertedP4Range, DirectX::XMFLOAT2(1175, 678), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Black));
		}

		if (m_EndGame)
		{
			std::wstring restartMessage{L"Press start/enter to restart" };
			TextRenderer::GetInstance()->DrawText(m_pResultFont, restartMessage, DirectX::XMFLOAT2(gameSettings.Window.Width / 2.f - 200.f, gameSettings.Window.Height / 2.f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::White));
			switch (m_pWinCharacter->GetCharacterNr())
			{
			case 0:
				TextRenderer::GetInstance()->DrawText(m_pResultFont, m_WinMessage, DirectX::XMFLOAT2(gameSettings.Window.Width / 2.f - 125.f, gameSettings.Window.Height / 2.f - 100.f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Yellow));
				break;

			case 1:
				TextRenderer::GetInstance()->DrawText(m_pResultFont, m_WinMessage, DirectX::XMFLOAT2(gameSettings.Window.Width / 2.f - 125.f, gameSettings.Window.Height / 2.f - 100.f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Orange));
				break;

			case 2:
				TextRenderer::GetInstance()->DrawText(m_pResultFont, m_WinMessage, DirectX::XMFLOAT2(gameSettings.Window.Width / 2.f - 125.f, gameSettings.Window.Height / 2.f - 100.f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Blue));
				break;

			case 3:
				TextRenderer::GetInstance()->DrawText(m_pResultFont, m_WinMessage, DirectX::XMFLOAT2(gameSettings.Window.Width / 2.f - 125.f, gameSettings.Window.Height / 2.f - 100.f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Green));
				break;
			default:
				break;
			}
			
		}

	}
}

void ExamScene::SceneActivated()
{
	//SOUND SETUP
	//***********
	if (!m_pBombSound)
		SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Audio/Explosion.wav", FMOD_LOOP_OFF, nullptr, &m_pBombSound);

	if (!m_pPlaceSound)
		SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Audio/Place.wav", FMOD_LOOP_OFF, nullptr, &m_pPlaceSound);

	if(!m_pDeathSound)
		SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Audio/PlayerDeath.wav", FMOD_LOOP_OFF, nullptr, &m_pDeathSound);

	if(!m_pBgm)
		SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Audio/bgm.mp3", FMOD_LOOP_NORMAL, nullptr, &m_pBgm);

	if(!m_pCollectSound)
		SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Audio/PickUp.wav", FMOD_LOOP_OFF, nullptr, &m_pCollectSound);

	if(!m_pPushSound)
		SoundManager::GetInstance()->GetSystem()->createSound("./Resources/Audio/Push.wav", FMOD_LOOP_OFF, nullptr, &m_pPushSound);

	if(!m_pSFXGroup)
		SoundManager::GetInstance()->GetSystem()->createChannelGroup("SFXgroup", &m_pSFXGroup);
	
	if (!m_pBgmGroup)
		SoundManager::GetInstance()->GetSystem()->createChannelGroup("BgmGroup", &m_pBgmGroup);

	FMOD::Channel* pBGMChannel;
	SoundManager::GetInstance()->GetSystem()->playSound(m_pBgm, nullptr, false, &pBGMChannel);
	pBGMChannel->setVolume(0.3f);
	pBGMChannel->setChannelGroup(m_pBgmGroup);
	
}

void ExamScene::SceneDeactivated()
{
	m_pBgmGroup->stop();
	m_pSFXGroup->stop();
	Restart();
	Unpause();
}

void ExamScene::CreateNewCharacter()
{
	
	if (!createdCharacter3)
	{
		if (GetGameContext().pInput->IsGamepadConnected(GamepadIndex::PlayerThree))
		{
			//create character
			GameSettings settings = OverlordGame::GetGameSettings();
			ExamCharacter* P3 = new ExamCharacter{ 4,11,100,m_pCamera->GetComponent<CameraComponent>(),2,11 };
			P3->GetTransform()->Translate(-58.5f, 0, 58.5f);
			AddChild(P3);
			m_VecCharacters.push_back(P3);

			//create UI for character
			m_pP3Stats = new GameObject();
			m_pP3Stats->AddComponent(new SpriteComponent(L"./Resources/Textures/CharacterStatsP3.png", DirectX::XMFLOAT2(0, 1)));
			m_pP3Stats->GetTransform()->Translate(0, float(settings.Window.Height), 0);
			m_pP3Stats->GetTransform()->Scale(0.2f, 0.2f, 0.2f);
			AddChild(m_pP3Stats);
			createdCharacter3 = true;
		}
	}

	if (!createdCharacter4)
	{
		if (GetGameContext().pInput->IsGamepadConnected(GamepadIndex::PlayerFour))
		{
			//create character
			GameSettings settings = OverlordGame::GetGameSettings();
			ExamCharacter* P4 = new ExamCharacter{ 4,11,100,m_pCamera->GetComponent<CameraComponent>(),3,12 };
			P4->GetTransform()->Translate(58.5f, 0, -58.5f);
			AddChild(P4);
			m_VecCharacters.push_back(P4);

			//create UI for character
			m_pP4Stats = new GameObject();
			m_pP4Stats->AddComponent(new SpriteComponent(L"./Resources/Textures/CharacterStatsP4.png", DirectX::XMFLOAT2(1, 1)));
			m_pP4Stats->GetTransform()->Translate(float(settings.Window.Width), float(settings.Window.Height), 0);
			m_pP4Stats->GetTransform()->Scale(0.2f, 0.2f, 0.2f);
			AddChild(m_pP4Stats);
			createdCharacter4 = true;
			
		}
	}
}

void ExamScene::CheckCharacterBombs()
{
	for (ExamCharacter* character : m_VecCharacters)
	{
		auto bombs = character->GetBombs();
		for (Bomb* bomb : *bombs)
		{
			std::vector<Bomb*> bombsToSet;
			if (bomb)
			{
				if (!bomb->IsInitialized())
				{
					FMOD::Channel* m_pSFXChannel;
					SoundManager::GetInstance()->GetSystem()->playSound(m_pPlaceSound, nullptr, false, &m_pSFXChannel);
					m_pSFXChannel->setChannelGroup(m_pSFXGroup);
					AddChild(bomb);
				}
				if (bomb->ShouldDestroy())
				{
					FMOD::Channel* m_pSFXChannel;
					SoundManager::GetInstance()->GetSystem()->playSound(m_pBombSound, nullptr, false, &m_pSFXChannel);
					m_pSFXChannel->setChannelGroup(m_pSFXGroup);
					m_pSFXChannel->setVolume(0.5f);
					auto objects = bomb->GetHitObjects();
					for (GameObject* hitObject : *objects)
					{
						if (hitObject)
						{
							if (hitObject->GetScene() == this)
							{
								ExamCharacter* characterHit = dynamic_cast<ExamCharacter*>(hitObject);
								Bomb* bombHit = dynamic_cast<Bomb*>(hitObject);
								
								if (characterHit)
								{
									FMOD::Channel* m_pDeathSFXChannel;
									SoundManager::GetInstance()->GetSystem()->playSound(m_pDeathSound, nullptr, false, &m_pDeathSFXChannel);
									m_pDeathSFXChannel->setVolume(1.f);
									m_pDeathSFXChannel->setChannelGroup(m_pSFXGroup);
									characterHit->SetDead();
								}
								else if (bombHit)
								{
									bombsToSet.push_back(bombHit);
								}
								else
								{
									HandlePowerUp(hitObject->GetTransform()->GetPosition());
									auto gridObjects = m_pGrid->GetGameObjects();
									gridObjects->erase(std::remove(gridObjects->begin(), gridObjects->end(), hitObject), gridObjects->end());
									RemoveChild(hitObject, true);
									hitObject = nullptr;
								}
							}
						}
						
					}
					for (ExamCharacter* pChar : m_VecCharacters)
					{
						if (bomb->GetScene() == this)
						{
							DirectX::XMFLOAT3 bombPos = bomb->GetTransform()->GetPosition();
							DirectX::XMFLOAT3 charPos = pChar->GetTransform()->GetPosition();
							float distance = hypot(hypot(bombPos.x - charPos.x, bombPos.y - charPos.y), bombPos.z - charPos.z);
							if (distance <= 50.f)
							{
								GetGameContext().pInput->SetVibration(1 - (distance / 50), 1 - (distance / 50), GamepadIndex(pChar->GetCharacterNr()));
							}
							if (distance <= 5.7f)
							{
								FMOD::Channel* m_pDeathSFXChannel;
								SoundManager::GetInstance()->GetSystem()->playSound(m_pDeathSound, nullptr, false, &m_pDeathSFXChannel);
								m_pDeathSFXChannel->setVolume(1.f);
								m_pDeathSFXChannel->setChannelGroup(m_pSFXGroup);
								pChar->SetDead();
							}
						}
						
						
					}
					m_VibrationTimer = 0;
					size_t originalSize = bombs->size();
					bombs->erase(std::remove(bombs->begin(), bombs->end(), bomb), bombs->end());
					size_t removedSize = bombs->size();
					if (bomb->GetScene() == this && originalSize > removedSize)
					{
						if (m_CanShake)
						{
							m_IsShaking = true;
							m_CanShake = false;
							m_currShakeTimer = 0;
							m_CurrTimeUntilShake = 0;
						}
						ExplosionEmitter* createdEmitter = new ExplosionEmitter{ bomb->GetTransform()->GetPosition(), bomb->GetRangeVector() };
						AddChild(createdEmitter);
						m_VecEmitters.push_back(createdEmitter);
						RemoveChild(bomb, true);
					}
					bomb = nullptr;
					for (Bomb* pBomb : bombsToSet)
					{
						if (pBomb)
						{
							if(pBomb->GetScene() == this)
								pBomb->ExecuteRaycast();
						}
						
					}
				}
			}
			
		}
	}
}

 void ExamScene::CheckCharacters()
{
	for (ExamCharacter* character : m_VecCharacters)
	{
		if (character->GetSetPaused())
		{
			InputAction unpause{};
			InputAction MenuUp{};
			InputAction MenuDown{};
			InputAction MenuSelect{};
			m_UnpauseActionID = int(character->GetNrOfInputElements() * m_VecCharacters.size());
			m_MenuUpActionID = m_UnpauseActionID + 1;
			m_MenuDownActionID = m_MenuUpActionID + 1;
			m_MenuSelectActionID = m_MenuDownActionID + 1;
			if (character->GetCharacterNr() == 0)
			{
				unpause = { m_UnpauseActionID,InputTriggerState::Pressed,VK_ESCAPE,-1,XINPUT_GAMEPAD_START,(GamepadIndex)character->GetCharacterNr() };
				MenuUp = { m_MenuUpActionID, InputTriggerState::Pressed, 'W',-1,XINPUT_GAMEPAD_DPAD_UP,(GamepadIndex)character->GetCharacterNr() };
				MenuDown = { m_MenuDownActionID, InputTriggerState::Pressed, 'S',-1,XINPUT_GAMEPAD_DPAD_DOWN,(GamepadIndex)character->GetCharacterNr() };
				MenuSelect = { m_MenuSelectActionID, InputTriggerState::Pressed,VK_SPACE,-1,XINPUT_GAMEPAD_A,(GamepadIndex)character->GetCharacterNr() };

			}
			else
			{
				unpause = { m_UnpauseActionID,InputTriggerState::Pressed,-1,-1,XINPUT_GAMEPAD_START,(GamepadIndex)character->GetCharacterNr() };
				MenuUp = { m_MenuUpActionID, InputTriggerState::Pressed, -1,-1,XINPUT_GAMEPAD_DPAD_UP,(GamepadIndex)character->GetCharacterNr() };
				MenuDown = { m_MenuDownActionID, InputTriggerState::Pressed,-1,-1,XINPUT_GAMEPAD_DPAD_DOWN,(GamepadIndex)character->GetCharacterNr() };
				MenuSelect = { m_MenuSelectActionID, InputTriggerState::Pressed,-1,-1,XINPUT_GAMEPAD_A,(GamepadIndex)character->GetCharacterNr() };
			}
				
			GetGameContext().pInput->AddInputAction(unpause);
			GetGameContext().pInput->AddInputAction(MenuUp,true);
			GetGameContext().pInput->AddInputAction(MenuDown,true);
			GetGameContext().pInput->AddInputAction(MenuSelect,true);
			m_pPauseScreen->SetRender(true);
			m_pPauseMenuCursor = new GameObject();
			switch (character->GetCharacterNr())
			{
			case 0:
				m_pPauseMenuCursor->AddComponent(new SpriteComponent(L"./Resources/Textures/BombCursorP1.png", DirectX::XMFLOAT2(0.5f, 0.5f)));
				break;

			case 1:
				m_pPauseMenuCursor->AddComponent(new SpriteComponent(L"./Resources/Textures/BombCursorP2.png", DirectX::XMFLOAT2(0.5f, 0.5f)));
				break;

			case 2: 
				m_pPauseMenuCursor->AddComponent(new SpriteComponent(L"./Resources/Textures/BombCursorP3.png", DirectX::XMFLOAT2(0.5f, 0.5f)));
				break;

			case 3:
				m_pPauseMenuCursor->AddComponent(new SpriteComponent(L"./Resources/Textures/BombCursorP4.png", DirectX::XMFLOAT2(0.5f, 0.5f)));
				break;

			default:
				break;
			}
			AddChild(m_pPauseMenuCursor);
			m_pPauseMenuCursor->GetTransform()->Scale(0.3f, 0.3f, 0.3f);
			m_pPauseMenuCursor->GetTransform()->Translate(m_CursorPosBackToMain);
			m_currSelectedMenuOption = 0;
			Pause();
			return;
		}


		if (character->GetShouldDestroy())
		{
			RemoveChild(character, true);
			m_VecCharacters.erase(std::remove(m_VecCharacters.begin(), m_VecCharacters.end(), character), m_VecCharacters.end());
			character = nullptr;
		}
	}
	if (m_VecCharacters.size() <= 1)
	{
		if (m_VecCharacters.size() == 1)
		{
			ExamCharacter* character = m_VecCharacters[0];
			int charNr = character->GetCharacterNr();
			InputAction unpause{};
			m_pWinCharacter = character;
			switch (charNr)
			{
			case 0:
				++m_P1Score;
				if (m_P1Score == m_RequiredPoints)
				{
					m_EndGame = true;
					m_UnpauseActionID = int(character->GetNrOfInputElements() * m_VecCharacters.size());

					unpause = { m_UnpauseActionID,InputTriggerState::Pressed,VK_RETURN,-1,XINPUT_GAMEPAD_START,(GamepadIndex)character->GetCharacterNr() };
					GetGameContext().pInput->AddInputAction(unpause, true);
					Pause();
					m_WinMessage = L"PLAYER 1 WINS";
				}
					
				
				break;

			case 1:
				++m_P2Score;
				if (m_P2Score == m_RequiredPoints)
				{
					m_EndGame = true;
					m_UnpauseActionID = int(character->GetNrOfInputElements() * m_VecCharacters.size());

					unpause = { m_UnpauseActionID,InputTriggerState::Pressed,VK_RETURN,-1,XINPUT_GAMEPAD_START,(GamepadIndex)character->GetCharacterNr() };
					GetGameContext().pInput->AddInputAction(unpause, true);
					Pause();
					m_WinMessage = L"PLAYER 2 WINS";
				}
					
				break;

			case 2:
				++m_P3Score;
				if (m_P3Score == m_RequiredPoints)
				{
					m_EndGame = true;
					m_UnpauseActionID = int(character->GetNrOfInputElements() * m_VecCharacters.size());

					unpause = { m_UnpauseActionID,InputTriggerState::Pressed,VK_RETURN,-1,XINPUT_GAMEPAD_START,(GamepadIndex)character->GetCharacterNr() };
					GetGameContext().pInput->AddInputAction(unpause, true);
					Pause();
					
					m_WinMessage = L"PLAYER 3 WINS";
				}
					
				break;

			case 3:
				++m_P4Score;
				if (m_P4Score == m_RequiredPoints)
				{
					m_EndGame = true;
					m_UnpauseActionID = int(character->GetNrOfInputElements() * m_VecCharacters.size());

					unpause = { m_UnpauseActionID,InputTriggerState::Pressed,VK_RETURN,-1,XINPUT_GAMEPAD_START,(GamepadIndex)character->GetCharacterNr() };
					GetGameContext().pInput->AddInputAction(unpause, true);
					Pause();
					m_WinMessage = L"PLAYER 4 WINS";
				}
					
				break;

			default:
				break;
			}
		}
		if (!m_EndGame)
		{
			Reset();
		}
	}
}

void ExamScene::CheckPowerUps()
{
	for (BombPowerUp* pBombPU : m_VecBombPU)
	{
		if (pBombPU)
		{
			if (pBombPU->GetScene() == this)
			{
				if (pBombPU->GetHit())
				{
					FMOD::Channel* m_pSFXChannel;
					SoundManager::GetInstance()->GetSystem()->playSound(m_pCollectSound, nullptr, false, &m_pSFXChannel);
					m_pSFXChannel->setChannelGroup(m_pSFXGroup);
					RemoveChild(pBombPU, true);
					m_VecBombPU.erase(std::remove(m_VecBombPU.begin(), m_VecBombPU.end(), pBombPU), m_VecBombPU.end());
				}
			}
		}
		
	}

	for (RangePowerUp* pRangePU : m_VecRangePU)
	{
		if (pRangePU)
		{
			if (pRangePU->GetScene() == this)
			{
				if (pRangePU->GetHit())
				{
					FMOD::Channel* m_pSFXChannel;
					SoundManager::GetInstance()->GetSystem()->playSound(m_pCollectSound, nullptr, false, &m_pSFXChannel);
					m_pSFXChannel->setChannelGroup(m_pSFXGroup);
					RemoveChild(pRangePU, true);
					m_VecRangePU.erase(std::remove(m_VecRangePU.begin(), m_VecRangePU.end(), pRangePU), m_VecRangePU.end());
				}
			}
		}

	}
}

void ExamScene::CheckEmitters()
{
	for (ExplosionEmitter* pEmitter : m_VecEmitters)
	{
		if (pEmitter->ShouldDestroy())
		{
			
			m_VecEmitters.erase(std::remove(m_VecEmitters.begin(), m_VecEmitters.end(), pEmitter), m_VecEmitters.end());
			RemoveChild(pEmitter, true);
		}
	}
}

void ExamScene::HandlePowerUp(DirectX::XMFLOAT3 pos)
{
	int rand = std::rand() % (100 + 1);
	if (rand > 90)
	{
		BombPowerUp* bombPU = new BombPowerUp(pos, 8);
		bombPU->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* otherActor, GameObject::TriggerAction action)
		{
			ExamCharacter* overlappingCharacter = dynamic_cast<ExamCharacter*>(otherActor);
			BombPowerUp* triggerObject = dynamic_cast<BombPowerUp*>(trigger);
			if (overlappingCharacter && action == GameObject::TriggerAction::ENTER)
			{
				triggerObject->SetHit();
				overlappingCharacter->IncrementNrOfBombs();
				switch (overlappingCharacter->GetCharacterNr())
				{
				case 0:
					m_P1Bombs = overlappingCharacter->GetNrOfBombs();
					break;
				case 1:
					m_P2Bombs = overlappingCharacter->GetNrOfBombs();
					break;
				case 2:
					m_P3Bombs = overlappingCharacter->GetNrOfBombs();
					break;
				case 3:
					m_P4Bombs = overlappingCharacter->GetNrOfBombs();
					break;
				default:
					break;
				}
			}

		});
		AddChild(bombPU);
		m_VecBombPU.push_back(bombPU);
	}
	else if (rand > 80)
	{
		RangePowerUp* rangePU = new RangePowerUp(pos, 9);
		rangePU->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* otherActor, GameObject::TriggerAction action)
		{
			ExamCharacter* overlappingCharacter = dynamic_cast<ExamCharacter*>(otherActor);
			RangePowerUp* triggerObject = dynamic_cast<RangePowerUp*>(trigger);
			if (overlappingCharacter && action == GameObject::TriggerAction::ENTER)
			{
				triggerObject->SetHit();
				overlappingCharacter->IncrementRangeMultiplier();
				switch (overlappingCharacter->GetCharacterNr())
				{
				case 0:
					m_P1Range = overlappingCharacter->GetRangeMultiplier();
					break;
				case 1:
					m_P2Range = overlappingCharacter->GetRangeMultiplier();
					break;
				case 2:
					m_P3Range = overlappingCharacter->GetRangeMultiplier();
					break;
				case 3:
					m_P4Range = overlappingCharacter->GetRangeMultiplier();
					break;
				default:
					break;
				}
			}

		});
		AddChild(rangePU);
		m_VecRangePU.push_back(rangePU);
	}
}

void ExamScene::Reset()
{
	m_IsShaking = false;
	m_pCamera->GetTransform()->Rotate(55.f, 0.f, 0.f);
	m_P1Bombs = 2;
	m_P2Bombs = 2;
	m_P3Bombs = 2;
	m_P4Bombs = 2;

	m_P1Range = 3;
	m_P2Range = 3;
	m_P3Range = 3;
	m_P4Range = 3;

	for (ExamCharacter* character : m_VecCharacters)
	{
		auto bombs = character->GetBombs();
		for (Bomb* pBomb : *bombs )
		{
			if (pBomb)
			{
				if (pBomb->GetScene() == this)
				{
					RemoveChild(pBomb, true);
					pBomb = nullptr;
				}
			}
		}
			RemoveChild(character, true);
	}
	m_VecCharacters.clear();

	auto gridObjectsToRemove = m_pGrid->GetGameObjects();
	for (GameObject* pObject : *gridObjectsToRemove)
	{
		if (pObject)
		{
			if (pObject->IsInitialized())
			{
				if (pObject->GetScene() == this)
				{
					RemoveChild(pObject, true);
				}
			}
		}
	}

	delete m_pGrid;
	m_pGrid = nullptr;

	for (BombPowerUp* pBombPU : m_VecBombPU)
	{
		if (pBombPU)
		{
			if (pBombPU->GetScene() == this)
			{
				m_VecBombPU.erase(std::remove(m_VecBombPU.begin(), m_VecBombPU.end(), pBombPU), m_VecBombPU.end());
				RemoveChild(pBombPU, true);
			}
			
		}
	}

	for (RangePowerUp* pRangePU : m_VecRangePU)
	{
		if (pRangePU)
		{
			if (pRangePU->GetScene() == this)
			{
				m_VecRangePU.erase(std::remove(m_VecRangePU.begin(), m_VecRangePU.end(), pRangePU), m_VecRangePU.end());
				RemoveChild(pRangePU, true);
			}

		}
	}

	m_pGrid = new Grid(DirectX::XMFLOAT2(-58.5f, -58.5f), 135, 135, 14, 14, 9.f, 6, 5);

	auto gridObjectsToAdd = m_pGrid->GetGameObjects();

	for (GameObject* pObject : *gridObjectsToAdd)
	{
		if (pObject)
		{
			AddChild(pObject);
		}
	}

	ExamCharacter::ResetCharacterNr();

	ExamCharacter* P1 = new ExamCharacter{ 4,11,100,m_pCamera->GetComponent<CameraComponent>(),0,7,false };
	P1->GetTransform()->Translate(-58.5f, 0.f, -58.5f);
	AddChild(P1);
	m_VecCharacters.push_back(P1);

	ExamCharacter* P2 = new ExamCharacter{ 4,11,100,m_pCamera->GetComponent<CameraComponent>(),1,10,false };
	P2->GetTransform()->Translate(58.5f, 0, 58.5f);
	AddChild(P2);
	m_VecCharacters.push_back(P2);

	if (createdCharacter3)
	{
		ExamCharacter* P3 = new ExamCharacter{ 4,11,100,m_pCamera->GetComponent<CameraComponent>(),2,11,false };
		P3->GetTransform()->Translate(-58.5f, 0, 58.5f);
		AddChild(P3);
		m_VecCharacters.push_back(P3);
	}

	if (createdCharacter4)
	{
		ExamCharacter* P4 = new ExamCharacter{ 4,11,100,m_pCamera->GetComponent<CameraComponent>(),3,12 ,false };
		P4->GetTransform()->Translate(58.5f, 0, -58.5f);
		AddChild(P4);
		m_VecCharacters.push_back(P4);
	}


	//FIRST CLEAR DOESNT ALWAYS DESTROY EVERYTHING
	for (BombPowerUp* pBombPU : m_VecBombPU)
	{
		if (pBombPU)
		{
			if (pBombPU->GetScene() == this)
			{
				RemoveChild(pBombPU, true);
				m_VecBombPU.erase(std::remove(m_VecBombPU.begin(), m_VecBombPU.end(), pBombPU), m_VecBombPU.end());
			}

		}
	}

	for (RangePowerUp* pRangePU : m_VecRangePU)
	{
		if (pRangePU)
		{
			if (pRangePU->GetScene() == this)
			{
				RemoveChild(pRangePU, true);
				m_VecRangePU.erase(std::remove(m_VecRangePU.begin(), m_VecRangePU.end(), pRangePU), m_VecRangePU.end());
			}

		}
	}
}

void ExamScene::Restart()
{
	m_P1Score = 0;
	m_P2Score = 0;
	m_P3Score = 0;
	m_P4Score = 0;
	m_EndGame = false;
	Reset();
}

void ExamScene::Unpause()
{
	GetGameContext().pInput->RemoveInputAction(m_UnpauseActionID);

	if (!m_EndGame)
	{
		GetGameContext().pInput->RemoveInputAction(m_MenuSelectActionID);
		GetGameContext().pInput->RemoveInputAction(m_MenuDownActionID);
		GetGameContext().pInput->RemoveInputAction(m_MenuUpActionID);
		RemoveChild(m_pPauseMenuCursor);
		m_pPauseMenuCursor = nullptr;
		m_pPauseScreen->SetRender(false);
	}
	for (ExamCharacter* Character : m_VecCharacters)
	{
		Character->UnpausePlayer();
	}
	m_IsPaused = false;

}

void ExamScene::Pause()
{

			for (ExamCharacter* characterPause : m_VecCharacters)
			{
				characterPause->PausePlayer();
			}
			m_IsPaused = true;
			return;
}

void ExamScene::CheckCameraShake()
{
	//CAMERA SHAKE
	if (m_IsShaking)
	{
		m_currShakeTimer += GetGameContext().pGameTime->GetElapsed();
		if (m_currShakeTimer < 0.75f)
		{

			float shakeValue = m_Amplitude * std::sin(2 * DirectX::XM_PI * m_Frequency * m_currShakeTimer);
			m_pCamera->GetTransform()->Rotate(55.f, 0, shakeValue / 2.f);
		}

		else
		{
			m_pCamera->GetTransform()->Rotate(55.f, 0.f, 0.f);
			m_IsShaking = false;
		}

	}

	if (!m_CanShake)
	{
		m_CurrTimeUntilShake += GetGameContext().pGameTime->GetElapsed();
		if (m_CurrTimeUntilShake >= m_MaxTimeUntilShake)
		{
			m_CanShake = true;
		}
	}
}
