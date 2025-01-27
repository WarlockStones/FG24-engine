#include "KeyInput.hpp"
#include <cstdio>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>
#include "Globals.hpp"
#include "imgui_impl_sdl2.h"

namespace FG24 {

void KeyInput::ProcessInput() {
	SDL_Event e{};
	while (SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);
		if (e.type == SDL_QUIT) {
			std::printf("KeyInput: SDL_QUIT received. Exiting game loop...\n");
			g_runGameLoop = false;
		} else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
			std::printf("KeyInput: Escape key received. Exiting game loop...\n");
			g_runGameLoop = false;
			break;

			case SDLK_F1:
			static SDL_bool relativeMouse = SDL_TRUE;
			SDL_SetRelativeMouseMode(relativeMouse); 
			relativeMouse = static_cast<SDL_bool>(!relativeMouse);
			break;
			

			// Movement keys
			case SDLK_UP:
			case SDLK_w:
			g_yvel = 1;
			break;

			case SDLK_RIGHT:
			case SDLK_d:
			g_xvel = 1;
			break;

			case SDLK_DOWN:
			case SDLK_s:
			g_yvel = -1;
			break;
			
			case SDLK_LEFT:
			case SDLK_a:
			g_xvel = -1;
			break;

			case SDLK_1:
			g_action1 = true;
			break;

			case SDLK_2:
			g_action2 = true;
			break;

			case SDLK_3:
			g_action3 = true;
			break;

			case SDLK_4:
			g_action4 = true;
			break;

			default:
			break;
			}
		} else if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym) {
			// Movement keys
			case SDLK_UP:
			case SDLK_w:
			if (g_yvel > 0) {
				g_yvel = 0;
			}
			break;

			case SDLK_RIGHT:
			case SDLK_d:
			if (g_xvel > 0) {
				g_xvel = 0;
			}
			break;

			case SDLK_DOWN:
			case SDLK_s:
			if (g_yvel < 0) {
				g_yvel = 0;
			}
			break;

			case SDLK_LEFT:
			case SDLK_a:
			if (g_xvel < 0) {
				g_xvel = 0;
			}
			break;

			default:
			break;
			}
		} else if (e.type == SDL_MOUSEMOTION) {
			g_xRelativeMouseMotion = e.motion.xrel;
			g_yRelativeMouseMotion = e.motion.yrel;
		}
	}
}
} // namespace FG24
