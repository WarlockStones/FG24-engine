// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#pragma once
#include "camera/Camera.hpp"

class SDL_Window;

namespace FG24 {
class Mesh;
class Shader;
class Actor;

class Renderer {
public:
	~Renderer();
	bool Init(int windowWidth, int windowHeight); // Init SDL and OpenGL
	void Update(Actor** actors, int actorsCount);

	Camera camera; // Temporary just putting camera here
private:
	SDL_Window* window{};
	void* context{}; // SDL_GLContext is a void* typedef

};
};
