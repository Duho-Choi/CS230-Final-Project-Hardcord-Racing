
#include "../Engine/Engine.h"
#include "../Engine/Collision.h"
#include "../Engine/ShowCollision.h"
#include "Runner.h"
#include "GameObjectTypes.h"
#include "Runner_Anims.h"		// Runner_Anim
#include "GameParticles.h"

Runner::Runner(math::vec2 startPos)
	: GameObject(startPos, 0, { 0.5, 0.5 }), leftKey(CS230::InputKey::Keyboard::A),
	rightKey(CS230::InputKey::Keyboard::D), frontKey(CS230::InputKey::Keyboard::W),
	backKey(CS230::InputKey::Keyboard::S), healthPoint(0), isDead(false), rotation(0)
{
	AddGOComponent(new CS230::Sprite("Assets/Mode3/Runner.spt", this));
}
void Runner::Update(double dt) 
{
	GetGOComponent<CS230::Sprite>()->Update(dt);

	if (isDead == false)
	{
		// Go Left & Right
		if (leftKey.IsKeyDown() == true)
		{
			if (rotation < maxRotation)
				rotation += 1.0 * dt;

			UpdateVelocity(math::vec2{ -left_right_accel * dt, 0 });
		}
		if (rightKey.IsKeyDown() == true)
		{
			if (rotation > -maxRotation)
				rotation -= 1.0 * dt;

			UpdateVelocity(math::vec2{ left_right_accel * dt, 0 });
		}
		if (leftKey.IsKeyDown() == false && rightKey.IsKeyDown() == false)
		{
			if (rotation > 0.02 || rotation < -0.02)
			{
				if (rotation > 0)
					rotation -= 0.7 * dt;
				else if (rotation < 0)
					rotation += 0.7 * dt;
			}
		}

		// Go Front
		if (frontKey.IsKeyDown() == true)
		{
			if(GetVelocity().y < maxVelocity)
				UpdateVelocity({ 0, accel * dt });
		}
		if (backKey.IsKeyDown() == true)
		{
			if (GetVelocity().y > -maxVelocity)
				UpdateVelocity({ 0, -accel * dt });
		}
		if (frontKey.IsKeyReleased() == true && frontKey.IsKeyDown() == false)
		{

		}

		// Go Back

		// Laser
		//CS230::GameObjectManager* gameObjectManager = Engine::GetGSComponent<CS230::GameObjectManager>();
		//if (laserKey.IsKeyDown() == true)
		//{
		//	if (wasLaserKeyPressed == false)
		//	{

		//	}
		//}
	}

	UpdateVelocity(-GetVelocity() * Runner::drag * dt);
	UpdatePosition(GetVelocity() * dt);
}
void Runner::Draw(math::TransformMatrix cameraMatrix)
{
	CS230::Sprite* spritePtr = GetGOComponent<CS230::Sprite>();
	math::TransformMatrix displayMatrix = GetMatrix() * cameraMatrix * math::RotateMatrix{ rotation };

	if (spritePtr != nullptr)
		spritePtr->Draw(displayMatrix);

#ifdef _DEBUG
	// Show Collision
	if (Engine::GetGSComponent<ShowCollision>()->IsEnabled()
		&& GetGOComponent<CS230::Collision>() != nullptr)
		GetGOComponent<CS230::Collision>()->Draw(cameraMatrix);
#endif
}

// Object Type & CanCollideWith
GameObjectType Runner::GetObjectType()
{
	return GameObjectType::Runner;
}
std::string Runner::GetObjectTypeName()
{
	return "Runner";
}
bool Runner::CanCollideWith(GameObjectType objectBType)
{
	if (objectBType == GameObjectType::Police || objectBType == GameObjectType::Spike)
		return true;
	else
		return false;
}

// Resolve Collision
void Runner::ResolveCollision(CS230::GameObject* objectB)
{
	if (CanCollideWith(objectB->GetObjectType()) == true)
	{
		GetGOComponent<CS230::Sprite>()->PlayAnimation(static_cast<int>(Runner_Anim::Explode_Anim));
		RemoveGOComponent<CS230::Collision>();
		isDead = true;
	}
}