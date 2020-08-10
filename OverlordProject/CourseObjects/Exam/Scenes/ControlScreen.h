#pragma once
#include <GameScene.h>
class ControlScreen : public GameScene
{
public:
	ControlScreen();
	virtual ~ControlScreen() = default;

	ControlScreen(const ControlScreen& other) = delete;
	ControlScreen(ControlScreen&& other) noexcept = delete;
	ControlScreen& operator=(const ControlScreen& other) = delete;
	ControlScreen& operator=(ControlScreen&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	GameObject* m_pControlScreenSprite;
};

