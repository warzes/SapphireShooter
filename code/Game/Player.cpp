#include "stdafx.h"
#include "Player.h"
#include "Keyboard.h"
#include "PhysicsManager.h"

void Player::Init(Camera3D& cam, const glm::vec3& initialPosition)
{
	cam.SetCameraPos(initialPosition);

	m_spotLight.Configure(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.027f, 0.0028f, 22.5f, 25.0f);
}

void Player::Update(Camera3D& cam, Terrain& terrain, float dt)
{
	if (!IsPlayerWalking())
	{
		m_walking = false;
		m_sprinting = false;
	}

	processInput(cam, dt);

#pragma region // CAMERA_MOVEMENT
	if (m_camMovements[CAM_FORWARD])
		cam.MoveForward(dt);
	if (m_camMovements[CAM_BACKWARD])
		cam.MoveBackward(dt);
	if (m_camMovements[CAM_LEFT])
		cam.StrafeLeft(dt);
	if (m_camMovements[CAM_RIGHT])
		cam.StrafeRight(dt);
	if (m_camMovements[CAM_RISE])
		cam.Rise(dt);
	if (m_camMovements[CAM_FALL])
		cam.Fall(dt);
#pragma endregion

	// Check if player is jumping
	if (m_jumping)
	{
		m_upwardSpeed += PhysicsManager::Get().GetGravity() * dt;
		cam.GetCameraPos().y += m_upwardSpeed * dt;

		if (cam.GetCameraPos().y < terrain.GetHeightOfTerrain(cam.GetCameraPos().x, cam.GetCameraPos().z) + 10.0f)
		{
			m_upwardSpeed = 0.0f;
			cam.GetCameraPos().y = terrain.GetHeightOfTerrain(cam.GetCameraPos().x, cam.GetCameraPos().z) + 10.0f;
			m_jumping = false;
		}
	}
	else
	{
		cam.GetCameraPos().y = terrain.GetHeightOfTerrain(cam.GetCameraPos().x, cam.GetCameraPos().z) + 10.0f;
		m_isInAir = false;
	}
}

// Function that checks if the camera (FPS character) is moving 
bool Player::IsPlayerWalking()
{
	return m_camMovements[CAM_FORWARD] || m_camMovements[CAM_BACKWARD] || m_camMovements[CAM_LEFT] || m_camMovements[CAM_RIGHT];
}

void Player::processInput(Camera3D& cam, float dt)
{
	static Keyboard& keyboard = Keyboard::Get();

	if (keyboard.KeyDown(Keyboard::KEY_W))
	{
		m_camMovements[CAM_FORWARD] = true;
		m_walking = true;
	}
	if (keyboard.KeyDown(Keyboard::KEY_S))
	{
		m_camMovements[CAM_BACKWARD] = true;
		m_walking = true;
	}
	if (keyboard.KeyDown(Keyboard::KEY_A))
	{
		m_camMovements[CAM_LEFT] = true;
		m_walking = true;
	}
	if (keyboard.KeyDown(Keyboard::KEY_D))
	{
		m_camMovements[CAM_RIGHT] = true;
		m_walking = true;
	}
	if (keyboard.KeyDown(Keyboard::KEY_R))
	{
		//m_camMovements[CAM_RISE] = true;
	}
	if (keyboard.KeyDown(Keyboard::KEY_F))
	{
		//m_camMovements[CAM_FALL] = true;
	}
	if (keyboard.KeyDown(Keyboard::KEY_SPACE))
	{
		if (!m_isInAir)
		{
			m_jumping = true;
			m_isInAir = true;
			m_upwardSpeed = m_jumpHeight;
		}
	}
	if (keyboard.KeyDown(Keyboard::KEY_SHIFT))
	{
		// Only sprint if the player is already walking 
		if (IsPlayerWalking())
		{
			cam.SetCameraSpeed(30.0f);
			m_sprinting = true;
		}

		m_walking = false;
	}

	if (keyboard.KeyUp(Keyboard::KEY_W))
	{
		m_camMovements[CAM_FORWARD] = false;
	}
	if (keyboard.KeyUp(Keyboard::KEY_S))
	{
		m_camMovements[CAM_BACKWARD] = false;
	}
	if (keyboard.KeyUp(Keyboard::KEY_A))
	{
		m_camMovements[CAM_LEFT] = false;
	}
	if (keyboard.KeyUp(Keyboard::KEY_D))
	{
		m_camMovements[CAM_RIGHT] = false;
	}
	if (keyboard.KeyPressed(Keyboard::KEY_F))
	{
		//m_toggleFlashlight = !m_toggleFlashlight;
		/// Check if the flash light is toggled on
		//if (m_toggleFlashlight)
		//{
		//	// Turn on the flash light
		//	m_spotLight->GetDiffuse() = glm::vec3(5.0f, 5.0f, 5.0f);
		//	m_spotLight->GetSpecular() = glm::vec3(1.0f, 1.0f, 1.0f);
		//	//Audio::Get().PlaySound(Audio::Get().GetSoundsMap().find("FlashOn")->second);
		//}
		//else
		//{
		//	// Turn off the flash light
		//	m_spotLight->GetDiffuse() = glm::vec3(0.0f, 0.0f, 0.0f);
		//	m_spotLight->GetSpecular() = glm::vec3(0.0f, 0.0f, 0.0f);
		//	//Audio::Get().PlaySound(Audio::Get().GetSoundsMap().find("FlashOff")->second);
		//}
	}
	if (keyboard.KeyPressed(Keyboard::KEY_R))
	{
		/// Check if the weapon is not full 
		//if (m_currWeapon->GetAmmoCount() != m_currWeapon->GetMaxAmmoCount())
		//{
		//	// Reload
		//	m_reloading = true;
		//	//Audio::Get().PlaySoundOnCustomChannel(Audio::Get().GetSoundsMap().find("Reload")->second, 4);
		//}
	}
	if (keyboard.KeyUp(Keyboard::KEY_SHIFT))
	{
		m_sprinting = false;

		if (IsPlayerWalking())
		{
			m_walking = true;
		}

		cam.SetCameraSpeed(20.0f);
	}

	//if (mouse.ButtonDown(Mouse::BUTTON_LEFT))
	//{
	//	//m_firing = true;
	//}
	//if (mouse.ButtonDown(Mouse::BUTTON_RIGHT))
	//{
	//	/// Check if the player is not using assault rifle
	//	//if (!m_usingAR)
	//	//{
	//	//	// Set sniper aiming to true
	//	//	m_sniperAiming = true;
	//	//}
	//}
	//if (mouse.ButtonUp(Mouse::BUTTON_LEFT))
	//{
	//	//m_firing = false;
	//}
	//if (mouse.ButtonUp(Mouse::BUTTON_RIGHT))
	//{
	//	//m_sniperAiming = false;
	//}

	cam.MouseMoveUpdate(dt);
}