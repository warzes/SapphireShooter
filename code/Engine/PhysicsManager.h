#pragma once

#include "Camera3D.h"
#include "Game/Enemy.h" // TODO:

struct Ray2
{
	glm::vec3 pos;
	glm::vec3 dir;
};

class PhysicsManager
{
public:
	static PhysicsManager& Get()
	{
		static PhysicsManager instance;
		return instance;
	}

	void Update(Camera3D& cam, float dt, std::vector<Enemy*>& enemies);

	void CastRay() { m_castRay = true; }
	Ray2& GetRay() { return m_ray; }
	bool GetDebugRayCastDraw() { return m_debugRayCastDraw; }
	float GetGravity() { return m_gravity; }
	void OnEnemyHit(Enemy* enemy);
	void OnPlayerHit(float damage);
	bool PointInSphere(Camera3D& cam, glm::vec3&, float radius);

private:
	PhysicsManager() = default;
	PhysicsManager(const PhysicsManager&) = delete;
	void operator=(const PhysicsManager&) = delete;

	Ray2 castRayFromMouse(Camera3D& cam);
	Ray2 castRayFromWeapon(Camera3D& cam);
	void checkRaySphereCollision(Camera3D& cam, std::vector<Enemy*> enemies);
	bool raySphere(Camera3D& cam, glm::vec3 RayDirWorld, double SphereRadius, double x, double y, double z);

	Ray2 m_ray;
	float m_mouseX = 0.0f;
	float m_mouseY = 0.0f;
	bool m_debugRayCastDraw = false;
	bool m_collision = false;
	bool m_castRay = false;
	float m_gravity = -90.0f;
};