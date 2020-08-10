#pragma once
#include <GameScene.h>
class KeyboardControlScreen : public GameScene
{
public:
	KeyboardControlScreen();
	virtual ~KeyboardControlScreen() = default;

	KeyboardControlScreen(const KeyboardControlScreen& other) = delete;
	KeyboardControlScreen(KeyboardControlScreen&& other) noexcept = delete;
	KeyboardControlScreen& operator=(const KeyboardControlScreen& other) = delete;
	KeyboardControlScreen& operator=(KeyboardControlScreen&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	GameObject* m_pControlScreenSprite;
};

