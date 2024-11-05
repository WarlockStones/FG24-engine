#pragma once
class SDL_Window;

namespace FG24 {
	class Renderer {
	public:
		Renderer() = default;
		~Renderer();
		bool Init(int WindowWidth, int WindowHeight); // Init SDL and OpenGL
		void Update();

	private:
		SDL_Window* pWindow;
		void* Context; // SDL_GLContext is a void* typedef
	};
};