#pragma once

class Vertex
{
public:
	Vertex(
		const glm::vec3& pos, 
		const glm::vec3& col, 
		const glm::vec2& tex, 
		const glm::vec3& normals, 
		const glm::vec3& tangent = glm::vec3(1.0f))
		: m_pos(pos)
		, m_col(col)
		, m_tex(tex)
		, m_normals(normals)
		, m_tangent(tangent)
	{
	}

	glm::vec3& GetPos() { return m_pos; }
	glm::vec3& GetCol() { return m_col; }
	glm::vec2& GetTex() { return m_tex; }
	glm::vec3& GetNormals() { return m_normals; }
	glm::vec3& GetTangent() { return m_tangent; }

private:
	glm::vec3 m_pos;
	glm::vec3 m_col;
	glm::vec2 m_tex;
	glm::vec3 m_normals;
	glm::vec3 m_tangent;
};