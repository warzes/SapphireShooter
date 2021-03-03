﻿#include "stdafx.h"
#if SE_OPENGL
#include "Profiler.h"
#include "Shader.h"
#include "GLGraphics.h"
#include "GLShaderProgram.h"
#include "GLShaderVariation.h"
#include "GLVertexBuffer.h"
#include "Engine/DebugNew.h"

const size_t MAX_NAME_LENGTH = 256;

int NumberPostfix(const String& str)
{
	for (size_t i = 0; i < str.Length(); ++i)
	{
		if (IsDigit(str[i]))
			return String::ToInt(str.CString() + i);
	}

	return -1;
}

ShaderProgram::ShaderProgram(ShaderVariation* vs_, ShaderVariation* ps_) :
	program(0),
	vs(vs_),
	ps(ps_)
{
}

ShaderProgram::~ShaderProgram()
{
	Release();
}

void ShaderProgram::Release()
{
	if (program)
	{
		glDeleteProgram(program);
		program = 0;
	}
}

bool ShaderProgram::Link()
{
	PROFILE(LinkShaderProgram);

	Release();

	if (!vs || !ps)
	{
		SE_LOG_ERROR("Shader(s) are null, can not link shader program");
		return false;
	}
	if (!vs->GLShader() || !ps->GLShader())
	{
		SE_LOG_ERROR("Shaders have not been compiled, can not link shader program");
		return false;
	}

	const String& vsSourceCode = vs->Parent() ? vs->Parent()->SourceCode() : String::EMPTY;
	const String& psSourceCode = ps->Parent() ? ps->Parent()->SourceCode() : String::EMPTY;

	program = glCreateProgram();
	if (!program)
	{
		SE_LOG_ERROR("Could not create shader program");
		return false;
	}

	glAttachShader(program, vs->GLShader());
	glAttachShader(program, ps->GLShader());
	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		int length, outLength;
		String errorString;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		errorString.Resize(length);
		glGetProgramInfoLog(program, length, &outLength, &errorString[0]);
		glDeleteProgram(program);
		program = 0;

		SE_LOG(("Could not link shaders " + FullName() + ": " + errorString).CString());
		return false;
	}

	SE_LOG(("Linked shaders " + FullName()).CString());

	glUseProgram(program);

	char nameBuffer[MAX_NAME_LENGTH];
	int numAttributes, numUniforms, numUniformBlocks, nameLength, numElements;
	GLenum type;

	attributes.Clear();

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
	for (int i = 0; i < numAttributes; ++i)
	{
		glGetActiveAttrib(program, i, (GLsizei)MAX_NAME_LENGTH, &nameLength, &numElements, &type, nameBuffer);

		VertexAttribute newAttribute;
		newAttribute.name = String(nameBuffer, nameLength);
		newAttribute.semantic = SEM_POSITION;
		newAttribute.index = 0;

		for (size_t j = 0; elementSemanticNames[j]; ++j)
		{
			if (newAttribute.name.StartsWith(elementSemanticNames[j], false))
			{
				int index = NumberPostfix(newAttribute.name);
				if (index >= 0)
					newAttribute.index = (unsigned char)index;
				break;
			}
			newAttribute.semantic = (ElementSemantic)(newAttribute.semantic + 1);
		}

		if (newAttribute.semantic == MAX_ELEMENT_SEMANTICS)
		{
			SE_LOG_WARNING(("Found vertex attribute " + newAttribute.name + " with no known semantic in shader program " + FullName()).CString());
			continue;
		}

		newAttribute.location = glGetAttribLocation(program, newAttribute.name.CString());
		attributes.Push(newAttribute);
	}

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
	int numTextures = 0;
	for (int i = 0; i < numUniforms; ++i)
	{
		glGetActiveUniform(program, i, MAX_NAME_LENGTH, &nameLength, &numElements, &type, nameBuffer);

		String name(nameBuffer, nameLength);
		if (type >= GL_SAMPLER_1D && type <= GL_SAMPLER_2D_SHADOW)
		{
			// Assign sampler uniforms to a texture unit according to the number appended to the sampler name
			int location = glGetUniformLocation(program, name.CString());
			int unit = NumberPostfix(name);
			// If no unit number specified, assign in appearance order starting from unit 0
			if (unit < 0)
				unit = numTextures;

			// Array samplers may have multiple elements, assign each sequentially
			if (numElements > 1)
			{
				Vector<int> units;
				for (int j = 0; j < numElements; ++j)
					units.Push(unit++);
				glUniform1iv(location, numElements, &units[0]);
			}
			else
				glUniform1iv(location, 1, &unit);

			numTextures += numElements;
		}
	}

	glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);
	for (int i = 0; i < numUniformBlocks; ++i)
	{
		glGetActiveUniformBlockName(program, i, (GLsizei)MAX_NAME_LENGTH, &nameLength, nameBuffer);

		// Determine whether uniform block belongs to vertex or pixel shader
		String name(nameBuffer, nameLength);
		bool foundVs = vsSourceCode.Contains(name);
		bool foundPs = psSourceCode.Contains(name);
		if (foundVs && foundPs)
		{
			SE_LOG("Found uniform block ??? in both vertex and pixel shader in shader program ???"); // TODO:
			continue;
		}

		// Vertex shader constant buffer bindings occupy slots starting from zero to maximum supported, pixel shader bindings
		// from that point onward
		unsigned blockIndex = glGetUniformBlockIndex(program, name.CString());

		int bindingIndex = NumberPostfix(name);
		// If no number postfix in the name, use the block index
		if (bindingIndex < 0)
			bindingIndex = blockIndex;
		if (foundPs)
			bindingIndex += (unsigned)graphics->NumVSConstantBuffers();

		glUniformBlockBinding(program, blockIndex, bindingIndex);
	}

	return true;
}

ShaderVariation* ShaderProgram::VertexShader() const
{
	return vs;
}

ShaderVariation* ShaderProgram::PixelShader() const
{
	return ps;
}

String ShaderProgram::FullName() const
{
	return (vs && ps) ? vs->FullName() + " " + ps->FullName() : String::EMPTY;
}

#endif // SE_OPENGL