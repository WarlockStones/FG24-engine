// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#pragma once
class SDL_Window;

namespace FG24 {
class Renderer {
public:
	Renderer() = default;
	~Renderer();
	bool Init(int windowWidth, int windowHeight); // Init SDL and OpenGL
	void Update();

private:
	SDL_Window* window{};
	void* context{}; // SDL_GLContext is a void* typedef
};
};