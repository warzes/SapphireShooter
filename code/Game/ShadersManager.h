#pragma once

#include "ShaderProgram.h"

class ShadersManager
{
	std::shared_ptr<ShaderProgram> object = nullptr;
	std::shared_ptr<ShaderProgram> anim = nullptr;
	std::shared_ptr<ShaderProgram> depth = nullptr;
	std::shared_ptr<ShaderProgram> depthAnim = nullptr;
	std::shared_ptr<ShaderProgram> depthTerrain = nullptr;
	std::shared_ptr<ShaderProgram> hdr = nullptr;
	std::shared_ptr<ShaderProgram> blur = nullptr;
	std::shared_ptr<ShaderProgram> particle = nullptr;
	std::shared_ptr<ShaderProgram> skybox = nullptr;
	std::shared_ptr<ShaderProgram> terrain = nullptr;
	std::shared_ptr<ShaderProgram> font = nullptr;
	std::shared_ptr<ShaderProgram> postprocess = nullptr;
	std::shared_ptr<ShaderProgram> water = nullptr;

public:
	void setObjectProgram(const std::shared_ptr<ShaderProgram> prog) { object = prog; }
	void setAnimProgram(const std::shared_ptr<ShaderProgram> prog) { anim = prog; }
	void setDepthProgram(const std::shared_ptr<ShaderProgram> prog) { depth = prog; }
	void setAnimDepthProgram(const std::shared_ptr<ShaderProgram> prog) { depthAnim = prog; }
	void setTerrainDepthProgram(const std::shared_ptr<ShaderProgram> prog) { depthTerrain = prog; }
	void setHDRProgram(const std::shared_ptr<ShaderProgram> prog) { hdr = prog; }
	void setPostProcessProgram(const std::shared_ptr<ShaderProgram> prog) { postprocess = prog; }
	void setBlurProgram(const std::shared_ptr<ShaderProgram> prog) { blur = prog; }
	void setParticleProgram(const std::shared_ptr<ShaderProgram> prog) { particle = prog; }
	void setSkyboxProgram(const std::shared_ptr<ShaderProgram> prog) { skybox = prog; }
	void setTerrainProgram(const std::shared_ptr<ShaderProgram> prog) { terrain = prog; }
	void setFontProgram(const std::shared_ptr<ShaderProgram> prog) { font = prog; }
	void setWaterProgram(const std::shared_ptr<ShaderProgram> prog) { water = prog; }

	std::shared_ptr<ShaderProgram> getObjectProgram() { return object; }
	std::shared_ptr<ShaderProgram> getAnimProgram() { return anim; }
	std::shared_ptr<ShaderProgram> getDepthProgram() { return depth; }
	std::shared_ptr<ShaderProgram> getAnimDepthProgram() { return depthAnim; }
	std::shared_ptr<ShaderProgram> getTerrainDepthProgram() { return depthTerrain; }
	std::shared_ptr<ShaderProgram> getHDRProgram() { return hdr; }
	std::shared_ptr<ShaderProgram> getPostProcessProgram() { return postprocess; }
	std::shared_ptr<ShaderProgram> getBlurProgram() { return blur; }
	std::shared_ptr<ShaderProgram> getParticleProgram() { return particle; }
	std::shared_ptr<ShaderProgram> getSkyboxProgram() { return skybox; }
	std::shared_ptr<ShaderProgram> getTerrainProgram() { return terrain; }
	std::shared_ptr<ShaderProgram> getFontProgram() { return font; }
	std::shared_ptr<ShaderProgram> getWaterProgram() { return water; }
};