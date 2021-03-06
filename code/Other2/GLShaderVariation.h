#pragma once

#if SE_OPENGL

#include "String.h"
#include "GPUObject.h"
#include "GraphicsDefs.h"

class Shader2;

// Compiled shader with specific defines.
class ShaderVariation : public RefCounted, public GPUObject
{
public:
	// Construct. Set parent shader and defines but do not compile yet.
	ShaderVariation(Shader2* parent, const String& defines);
	// Destruct.
	~ShaderVariation();

	// Release the compiled shader.
	void Release() override;

	// Compile. Return true on success. No-op that return previous result if compile already attempted.
	bool Compile();

	// Return the parent shader resource.
	Shader2* Parent() const;
	// Return full name combined from parent resource name and compilation defines.
	String FullName() const;
	// Return shader stage.
	ShaderStage Stage() const { return stage; }
	// Return whether compile attempted.
	bool IsCompiled() const { return compiled; }

	// Return the OpenGL shader identifier. Used internally and should not be called by portable application code.
	unsigned GLShader() const { return shader; }

private:
	// OpenGL shader object identifier.
	unsigned shader;
	// Parent shader resource.
	WeakPtr<Shader2> parent;
	// Shader stage.
	ShaderStage stage;
	// Compilation defines.
	String defines;
	// Compile attempted flag.
	bool compiled;
};

#endif // SE_OPENGL