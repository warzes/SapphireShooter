#include "stdafx.h"
#include "PhysicsManager.h"
#include "Player.h"
#include "Engine.h"

void PhysicsManager::Update(Camera3D& cam, float dt, std::vector<Enemy*>& enemies)
{
	// Check if ray casting is true
	if (m_castRay)
	{
		// Cast ray and check for collision
		m_castRay = false;
		m_debugRayCastDraw = true;
		m_ray = castRayFromWeapon(cam);
		checkRaySphereCollision(cam, enemies);
	}
}

Ray PhysicsManager::castRayFromMouse(Camera3D& cam)
{
	// screen space (viewport coordinates)
	float x = (2.0f * m_mouseX) / GetEngineDescription().window.width - 1.0f;
	float y = 1.0f - (2.0f * m_mouseY) / GetEngineDescription().window.height;
	float z = 1.0f;

	// normalised device space
	glm::vec3 mouseNdcCoords = glm::vec3(x, y, z);
	glm::vec4 mouseClipCoords = glm::vec4(mouseNdcCoords.x, mouseNdcCoords.y, -1.0f, 1.0f);
	glm::mat4 invProjMat = glm::inverse(cam.GetProjectionMatrix());
	glm::vec4 eyeCoords = invProjMat * mouseClipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
	glm::mat4 invViewMat = glm::inverse(cam.GetViewMatrix());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));

	Ray ray;
	ray.pos = cam.GetCameraPos();
	ray.dir = rayDirection;

	return ray;
}

Ray PhysicsManager::castRayFromWeapon(Camera3D& cam)
{
	Ray ray;
	ray.pos = cam.GetCameraPos();
	ray.dir = cam.GetCameraForward();
	return ray;
}

bool PhysicsManager::raySphere(Camera3D& cam, glm::vec3 RayDirWorld, double SphereRadius, double x, double y, double z)
{
	// work out components of quadratic
	glm::vec3 v = glm::vec3(x, y, z) - cam.GetCameraPos();
	long double a = glm::dot(RayDirWorld, RayDirWorld);
	long double b = 2.0 * glm::dot(v, RayDirWorld);
	long double c = glm::dot(v, v) - SphereRadius * SphereRadius;
	long double b_squared_minus_4ac = b * b + (-4.0) * a * c;

	if (b_squared_minus_4ac == 0)
	{
		// One real root 
		return true;
	}
	else if (b_squared_minus_4ac > 0)
	{
		// Two real roots 
		long double x1 = (-b - sqrt(b_squared_minus_4ac)) / (2.0 * a);
		long double x2 = (-b + sqrt(b_squared_minus_4ac)) / (2.0 * a);

		if (x1 >= 0.0 || x2 >= 0.0)
			return true;
		if (x1 < 0.0 || x2 >= 0.0)
			return true;
	}

	// No real roots
	return false;
}

void PhysicsManager::checkRaySphereCollision(Camera3D& cam, std::vector<Enemy*> enemies)
{
	for (auto i = enemies.begin(); i != enemies.end(); ++i)
	{
		m_collision = raySphere(cam, m_ray.dir, 3.0f, (*i)->GetPos().x, (*i)->GetPos().y, (*i)->GetPos().z);

		// Check if the ray is colliding with the sphere
		if (m_collision)
		{
			// Pass in the value the iterator is pointing to to the OnEnemyHit() function
			OnEnemyHit((*i));
		}
	}
}

inline void PhysicsManager::OnEnemyHit(Enemy* enemy)
{
	enemy->ReduceHealth(/*Player::Get().GetCurrWeapon().GetDamage()*/1); // TODO: damage
}

void PhysicsManager::OnPlayerHit(float damage)
{
	//Player::Get().ReduceHealth(damage);  // TODO: damage
}

bool PhysicsManager::PointInSphere(Camera3D& cam, glm::vec3& other, float radius)
{
	// Calculate distance between player and center of circle
	float distanceSq = std::pow(cam.GetCameraPos().x - other.x, 2) + std::pow(cam.GetCameraPos().y - other.y, 2) + std::pow(cam.GetCameraPos().z - other.z, 2);

	// Check if the player is within the radius (if radius is bigger than point is inside circle) 
	if (distanceSq < (radius * radius))
	{
		return true;
	}

	return false;
}