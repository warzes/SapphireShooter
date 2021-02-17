#include "stdafx.h"
#include "TestAnims.h"

TestAnims::TestAnims() : ModelAnim("objects/stickman/stickman.fbx")
{
	RotateY(-90);
	Material material(glm::vec3(0.012f, 0.012f, 0.012f), glm::vec3(0.1f, 0.1f, 1.0f), glm::vec3(0.11f, 0.11f, 0.11f), 2);
	changeWholeMaterial(material);
	SetPosition(glm::vec3(0.0f, 0.48f, 0.0f));
	scale(glm::vec3(0.1f));

	//playStayAnim();
	//name = "stay";
	play(300, 320);
	setLoop(true);
	setSpeed(0.25);
}