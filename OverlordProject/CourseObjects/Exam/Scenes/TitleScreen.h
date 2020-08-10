#pragma once
#include <GameScene.h>
class GameObject;

enum class TSOptions
{
	START,
	EXIT,
	CREDITS
};

class TitleScreen final : public GameScene
{
public:
	TitleScreen();
	virtual ~TitleScreen() = default;

	TitleScreen(const TitleScreen& other) = delete;
	TitleScreen(TitleScreen&& other) noexcept = delete;
	TitleScreen& operator=(const TitleScreen& other) = delete;
	TitleScreen& operator=(TitleScreen&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	GameObject* m_pTitleScreenSprite;
	const DirectX::XMFLOAT3 m_CursorPosStart{ 505, 392, 0 };
	const DirectX::XMFLOAT3 m_CursorPosExit{ 505, 487, 0 };
	const DirectX::XMFLOAT3 m_CursorPosCredits{ 505,572,0 };
	GameObject* m_pCursorSprite;
	TSOptions m_SelectedOption{TSOptions::START};
};

