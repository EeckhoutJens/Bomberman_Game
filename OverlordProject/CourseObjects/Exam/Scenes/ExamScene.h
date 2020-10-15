#pragma once
#include <GameScene.h>
#include "SoundManager.h"
class ExamCharacter;
class FixedCamera;
class Grid;
class LevelBorder;
class TextureData;
class SpriteFont;
class BombPowerUp;
class RangePowerUp;
class ExplosionEmitter;


enum class PMOptions
{
	BackToMain,
	Restart,
	Quit
};

class ExamScene final : public GameScene
{
public:
	ExamScene();
	virtual ~ExamScene();

	ExamScene(const ExamScene& other) = delete;
	ExamScene(ExamScene&& other) noexcept = delete;
	ExamScene& operator=(const ExamScene& other) = delete;
	ExamScene& operator=(ExamScene&& other) noexcept = delete;
	FMOD::ChannelGroup* GetSFXChannel() { return m_pSFXGroup; }
	FMOD::Sound* GetBombSound() { return m_pPushSound; }

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SceneActivated() override;
	void SceneDeactivated() override;

private:
	//FUNCTIONS
	void CreateNewCharacter();
	void CheckCharacterBombs();
	void CheckCharacters();
	void CheckPowerUps();
	void CheckEmitters();
	void HandlePowerUp(DirectX::XMFLOAT3 pos);
	void Reset();
	void Restart();
	void Unpause();
	void Pause();
	void CheckCameraShake();

	//VARIABLES
	std::vector<ExamCharacter*> m_VecCharacters;
	std::vector<BombPowerUp*> m_VecBombPU;
	std::vector<RangePowerUp*> m_VecRangePU;
	std::vector<ExplosionEmitter*> m_VecEmitters;
	bool createdCharacter3{ false };
	bool createdCharacter4{ false };
	bool m_IsPaused{ false };
	bool m_EndGame{ false };
	bool m_IsShaking{ false };
	bool m_CanShake{ true };
	const float m_MaxVibrationTime{ 0.5f };
	const float m_Amplitude{3.25f};
	const float m_Frequency{12.5f};
	float m_VibrationTimer{};
	int m_P1Score{}, m_P2Score{}, m_P3Score{}, m_P4Score{};
	int m_P1Bombs{2}, m_P2Bombs{2}, m_P3Bombs{2}, m_P4Bombs{2};
	int m_P1Range{ 3 }, m_P2Range{ 3 }, m_P3Range{ 3 }, m_P4Range{ 3 };
	int m_UnpauseActionID{};
	int m_MenuUpActionID{};
	int m_MenuDownActionID{};
	int m_MenuSelectActionID{};
	int m_currSelectedMenuOption{ 0 };
	float m_currShakeTimer{0};
	const float  m_MaxShakeTimer{ 0.15f };
	const float m_MaxTimeUntilShake{ 4.f };
	float m_CurrTimeUntilShake{};
	const int m_RequiredPoints{ 3 };
	std::wstring m_WinMessage;
	FixedCamera* m_pCamera{};
	Grid* m_pGrid{};
	LevelBorder* m_pBorder{};
	FMOD::Sound* m_pBombSound{};
	FMOD::Sound* m_pPlaceSound{};
	FMOD::Sound* m_pDeathSound{};
	FMOD::Sound* m_pBgm{};
	FMOD::Sound* m_pPushSound{};
	FMOD::Sound* m_pCollectSound{};
	FMOD::ChannelGroup* m_pBgmGroup{};
	FMOD::ChannelGroup* m_pSFXGroup{};

	ExamCharacter* m_pWinCharacter = nullptr;

	SpriteFont* m_pFont = nullptr;

	SpriteFont* m_pResultFont = nullptr;

	GameObject* m_pP1Stats = nullptr;

	GameObject* m_pP2Stats = nullptr;

	GameObject* m_pP3Stats = nullptr;

	GameObject* m_pP4Stats = nullptr;

	GameObject* m_pPauseScreen = nullptr;

	GameObject* m_pPauseMenuCursor = nullptr;

	DirectX::XMFLOAT3 m_CursorPosBackToMain{450, 80,0};
	DirectX::XMFLOAT3 m_CursorPosRestart{450,320,0};
	DirectX::XMFLOAT3 m_CursorPosQuit{450,560,0};

	const PMOptions m_arrPauseOptions[3]{ PMOptions::BackToMain,PMOptions::Restart,PMOptions::Quit };
};

