#pragma once

#if SE_D3D11

#include "String.h"
#include "GPUObject.h"
#include "GraphicsDefs.h"

class Shader2;

// Compiled shader with specific defines.
class ShaderVariation : public RefCounted, public GPUObject
{
public:
    // Construct. Set parent shader and defines but do not compile yet.
<<<<<<< HEAD
    ShaderVariation(Shader* parent, const String& defines);
=======
    ShaderVariation(Shader2* parent, const String& defines);
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
    // Destruct.
    ~ShaderVariation();

    // Release the compiled shader.
    void Release() override;

    // Compile. Return true on success. No-op that returns previous result if compile already attempted.
    bool Compile();
    
    // Return the parent shader resource.
<<<<<<< HEAD
    Shader* Parent() const;
=======
    Shader2* Parent() const;
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
    // Return full name combined from parent resource name and compilation defines.
    String FullName() const;
    // Return shader stage.
    ShaderStage Stage() const { return stage; }
    // Return vertex element hash code for vertex shaders.
    unsigned ElementHash() const { return elementHash; }
    // Return whether compile attempted.
    bool IsCompiled() const { return compiled; }

    // Return the D3D11 shader byte blob. Null if not compiled yet or compile failed. Used internally and should not be called by portable application code.
    void* BlobObject() const { return blob; }
    // Return the D3D11 shader. Null if not compiled yet or compile failed. Used internally and should not be called by portable application code.
    void* ShaderObject() { return shader; }

private:
    // Parent shader resource.
<<<<<<< HEAD
    WeakPtr<Shader> parent;
=======
    WeakPtr<Shader2> parent;
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
    // Shader stage.
    ShaderStage stage;
    // Compilation defines.
    String defines;
    // D3D11 shader byte blob.
    void* blob;
    // D3D11 shader.
    void* shader;
    // Vertex shader element hash code.
    unsigned elementHash;
    // Compile attempted flag.
    bool compiled;
};

#endif // SE_D3D11