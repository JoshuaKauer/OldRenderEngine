#pragma once
#include <stdio.h>
#include <fstream>
#include <direct.h>
#include <iostream>
#include "Scene.h"

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm\glm.hpp>
#pragma warning(pop)

class Reader
{
	Vertex* verts;
public:


	Scene* readSceneFile(const char* filename);

	void displayVec3(glm::vec3* vec);

	void displayScene(Scene* scene);

	//void showCurrentDir();
};

