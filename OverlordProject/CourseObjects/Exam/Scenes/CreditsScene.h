#pragma once
#include <GameScene.h>
class CreditsScene final : public GameScene
{
public:
	CreditsScene();
	virtual ~CreditsScene() = default;

	CreditsScene(const CreditsScene& other) = delete;
	CreditsScene(CreditsScene&& other) noexcept = delete;
	CreditsScene& operator=(const CreditsScene& other) = delete;
	CreditsScene& operator=(CreditsScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	GameObject* m_pCreditsScreenSprite;

};

