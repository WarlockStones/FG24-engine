#include "KeyInput.hpp"
#include <cstdio>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>
#include "Globals.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"

namespace FG24 {

void KeyInput::ProcessInput() {
	SDL_Event e{};
	while (SDL_PollEvent(&e)) {
		ImGui:ImGui_ImplSDL2_ProcessEvent(&e); // Send event to imgui
		if (e.type == SDL_QUIT) {
			std::printf("KeyInput: SDL_QUIT received. Exiting game loop...\n");
			g_runGameLoop = false;
		} else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
			std::printf("KeyInput: Escape key received. Exiting game loop...\n");
			g_runGameLoop = false;
			break;

			// Movement keys
			case SDLK_w:
			g_yvel = 1;
			break;

			case SDLK_d:
			g_xvel = 1;
			break;

			case SDLK_s:
			g_yvel = -1;
			break;

			case SDLK_a:
			g_xvel = -1;
			break;

			default:
			break;
			}
		} else if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym) {
			// Movement keys
			case SDLK_w:
			if (g_yvel > 0) {
				g_yvel = 0;
			}
			break;

			case SDLK_d:
			if (g_xvel > 0) {
				g_xvel = 0;
			}
			break;

			case SDLK_s:
			if (g_yvel < 0) {
				g_yvel = 0;
			}
			break;

			case SDLK_a:
			if (g_xvel < 0) {
				g_xvel = 0;
			}
			break;

			default:
			break;
			}
		}
	}
}
} // namespace FG24