#include "stdafx.h"
#include "Mesh2.h"

Mesh2::Mesh2(const std::vector<MeshVertex>& verts, const std::vector<unsigned>& inds, const std::vector<MeshTexture>& texs, const Material& mat) : vertices(verts), indices(inds), textures(texs), material(mat)
{
	initMesh();
}

Mesh2::Mesh2(const Mesh2& mesh) : Shape2(mesh)
{
	swap(mesh);
}

Mesh2& Mesh2::operator=(const Mesh2& mesh)
{
	Shape2::operator=((const Shape2&)mesh);
	clear();
	swap(mesh);
	return *this;
}

void Mesh2::swap(const Mesh2& mesh)
{
	EBO = mesh.EBO;
	vertices = mesh.vertices;
	indices = mesh.indices;
	textures = mesh.textures;
	material = mesh.material;
}

void Mesh2::clear()
{
	if (willBeClear())
		glDeleteBuffers(1, &EBO);
}

Mesh2::~Mesh2()
{
	clear();
}

void Mesh2::initMesh()
{
	unsigned size = sizeof(MeshVertex);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * size, &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, size, (void*)offsetof(MeshVertex, texCoords));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, size, (void*)offsetof(MeshVertex, normal));

	glBindVertexArray(0);
}

void Mesh2::Render(std::shared_ptr<ShaderProgram> program)
{
	unsigned diffuseNum = 0, specularNum = 0;
	for (unsigned i = 0; i < textures.size(); ++i)
	{
		Texture::active(i);

		std::string number;
		std::string type = textures[i].type;
		if (type == "texture_diffuse")
			number = std::to_string(diffuseNum++);
		else if (type == "texture_specular")
			number = std::to_string(specularNum++);

		program->SetInteger(std::string(type + "[" + number + "]").c_str(), i);
		textures[i].texture.bind(GL_TEXTURE_2D);
	}

	program->SetInteger("diffuseTexturesAmount", diffuseNum);
	program->SetInteger("specularTexturesAmount", specularNum);
	material.Render(program);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	Texture::unbind(GL_TEXTURE_2D);
}