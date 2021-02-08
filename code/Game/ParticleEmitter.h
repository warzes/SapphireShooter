#pragma once

#include "Particle.h"
#include "Transformation.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

class Camera;

class ParticleEmitter
{
public:
	void Init(unsigned numberOfParticles);
	void Render(const Camera& camera, float dt, const glm::vec3 &origin = glm::vec3(0.0f, 0.0f, 0.0f));

	std::vector<Particle>& GetParticle() { return m_particles; }
	std::vector<glm::vec3>& GetPos() { return m_position; }

private:
	//enum { VERTEX_VB, COLOR_VB, TOTAL_BUFFERS };

	Transform m_transform;
	GLuint m_vao, m_vbo, m_textureID;
	std::shared_ptr<Texture2D> m_texture;
	std::shared_ptr<ShaderProgram> m_shader;
	unsigned m_numberOfParticles = 500;

	std::vector<Particle> m_particles;
	std::vector<glm::vec3> m_position;
};