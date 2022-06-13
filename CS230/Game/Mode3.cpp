
#include "../Engine/Engine.h"	//GetGameStateManager
#include "../Engine/ShowCollision.h"
#include "Mode3.h"
#include "Runner.h"
#include "Police.h"
#include "Fonts.h"
#include "Score.h"
#include "Screens.h"
#include "GameParticles.h"
#include "Hp.h"
#include "Mode3_background.h"

Mode3::Mode3() : modeReload(CS230::InputKey::Keyboard::R), mainMenu(CS230::InputKey::Keyboard::Escape) {}

void Mode3::Load()
{
	// Add GameObjectManager
	AddGSComponent(new CS230::GameObjectManager);

	// Add GameObjects to GameObjectManager
	CS230::GameObjectManager* gameObjectManagerPtr = GetGSComponent<CS230::GameObjectManager>(); 
	runnerPtr = new Runner(math::vec2{ Engine::GetWindow().GetSize() / 2 });
	gameObjectManagerPtr->Add(runnerPtr);
	gameObjectManagerPtr->Add(new Police(math::vec2{ 200, 200 }));
	gameObjectManagerPtr->Add(new Police(math::vec2{ 1200, 200 }));

	// backgroundPtr
	AddGSComponent(new Mode3_background("Assets/Mode3/background_track.jpg"));

	CS230::Camera* cameraPtr = new CS230::Camera({});
	AddGSComponent(cameraPtr);
	cameraPtr->SetExtent({ { 0, 0 }, { 0, 100000000 }});

	// Add Fonts
	// Score
	AddGSComponent(new Score{ 0, Fonts::Font1 });
	// Hp
	AddGSComponent(new Hp{ 100, Fonts::Font1 });
	// Game Over
	GameOverTexture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture("Game Over", 0x00FFFFFF, true);
	ScoreTexture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture("Score : ", 0xFFFFFFFF, true);
	RestartTexture = Engine::GetSpriteFont(static_cast<int>(Fonts::Font1)).DrawTextToTexture("Press r to restart", 0xFFFFFFFF, true);

	// Add Particle
	AddGSComponent(new DirtEmitter());
	
	// if Debug, Add ShowCollision
#ifdef _DEBUG
	AddGSComponent(new ShowCollision(CS230::InputKey::Keyboard::Tilde));
#endif
}

void Mode3::Update(double dt)
{
	GetGSComponent<CS230::GameObjectManager>()->Update(dt);
	GetGSComponent<CS230::Camera>()->Mode3_Update(dt, Mode3::speed);
	GetGSComponent<Mode3_background>()->Update(GetGSComponent<CS230::Camera>()->GetPosition());

	if (mainMenu.IsKeyReleased() == true)
	{
	// @ Modify update to player Ptr
	//GetGSComponent<CS230::Camera>()->Update(heroPtr->GetPosition());
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenu));
	}
#if _DEBUG
	if (modeReload.IsKeyReleased() == true)
	{
		Engine::GetGameStateManager().ReloadState();
	}
#endif
	// Update ShowCollision
#if _DEBUG
	GetGSComponent<ShowCollision>()->Update(dt);
#endif
}
void Mode3::Draw()
{
	Engine::GetWindow().Clear(0x50FF50FF);
	
	// Background / Camera
	CS230::Camera* cameraPtr = GetGSComponent<CS230::Camera>();

	math::ivec2 background_vec = { (Engine::GetWindow().GetSize().x - GetGSComponent<Mode3_background>()->GetSize().x) / 2, 0};
	GetGSComponent<Mode3_background>()->Draw((static_cast<math::vec2>(background_vec) - cameraPtr->GetPosition()));

	math::ivec2 winSize = Engine::GetWindow().GetSize();
	
	if (runnerPtr->IsDead() == false)
	{
		GetGSComponent<Score>()->Draw(math::ivec2{ 10, winSize.y - 5 });
	}
	else if (runnerPtr->IsDead() == true)
	{
		//GetGSComponent<Score>()->Draw(math::ivec2{ winSize.x / 2 -  });
		GameOverTexture.Draw(math::TranslateMatrix(math::ivec2{ winSize.x / 2 - GameOverTexture.GetSize().x / 2, winSize.y / 2 + GameOverTexture.GetSize().y / 2 }));
		RestartTexture.Draw(math::TranslateMatrix(math::ivec2{ winSize.x / 2 - RestartTexture.GetSize().x / 2, winSize.y / 2 - RestartTexture.GetSize().y / 2 }));
	}

	// GameObjectManager
	math::TransformMatrix cameraMatrix = cameraPtr->GetMatrix();
	GetGSComponent<CS230::GameObjectManager>()->DrawAll(cameraMatrix);

	// Draw Fonts
	// Score & Hp
	GetGSComponent<Score>()->Draw(math::ivec2{ 10, winSize.y - 5 });
	GetGSComponent<Hp>()->Draw(math::ivec2{ winSize.x - 10, winSize.y - 5 });
}
void Mode3::Unload()
{
	runnerPtr = nullptr;
	ClearGSComponent();
}