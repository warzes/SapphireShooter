#include "stdafx.h"
#include "Profiler.h"
#include "ResourceCache.h"
#include "File.h"
#include "FileSystem.h"
#include "Shader.h"
#include "ShaderVariation.h"

Shader2::Shader2() :
	stage(SHADER_VS)
{
}

Shader2::~Shader2()
{
}

void Shader2::RegisterObject()
{
	RegisterFactory<Shader2>();
}

bool Shader2::BeginLoad(Stream& source)
{
	String extension = Extension(source.Name());
	stage = (extension == ".vs" || extension == ".vert") ? SHADER_VS : SHADER_PS;
	sourceCode.Clear();
	return ProcessIncludes(sourceCode, source);
}

bool Shader2::EndLoad()
{
	// Release existing variations (if any) to allow them to be recompiled with changed code
	for (auto it = variations.Begin(); it != variations.End(); ++it)
		it->second->Release();
	return true;
}

void Shader2::Define(ShaderStage stage_, const String& code)
{
	stage = stage_;
	sourceCode = code;
	EndLoad();
}

ShaderVariation* Shader2::CreateVariation(const String& definesIn)
{
	StringHash definesHash(definesIn);
	auto it = variations.Find(definesHash);
	if (it != variations.End())
		return it->second.Get();

	// If initially not found, normalize the defines and try again
	String defines = NormalizeDefines(definesIn);
	definesHash = defines;
	it = variations.Find(definesHash);
	if (it != variations.End())
		return it->second.Get();

	ShaderVariation* newVariation = new ShaderVariation(this, defines);
	variations[definesHash] = newVariation;
	return newVariation;
}

bool Shader2::ProcessIncludes(String& code, Stream& source)
{
	ResourceCache* cache = Subsystem<ResourceCache>();

	while (!source.IsEof())
	{
		String line = source.ReadLine();

		if (line.StartsWith("#include"))
		{
			String includeFileName = Path(source.Name()) + line.Substring(9).Replaced("\"", "").Trimmed();
			AutoPtr<Stream> includeStream = cache->OpenResource(includeFileName);
			if (!includeStream)
				return false;

			// Add the include file into the current code recursively
			if (!ProcessIncludes(code, *includeStream))
				return false;
		}
		else
		{
			code += line;
			code += "\n";
		}
	}

	// Finally insert an empty line to mark the space between files
	code += "\n";

	return true;
}

String Shader2::NormalizeDefines(const String& defines)
{
	String ret;
	Vector<String> definesVec = defines.ToUpper().Split(' ');
	Sort(definesVec.Begin(), definesVec.End());

	for (auto it = definesVec.Begin(); it != definesVec.End(); ++it)
	{
		if (it != definesVec.Begin())
			ret += " ";
		ret += *it;
	}

	return ret;
}