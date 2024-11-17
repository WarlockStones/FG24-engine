// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "InputManager.hpp"
#include <SDL2/SDL_events.h>
#include <cstdio>

namespace FG24 {


bool InputManager::ProcessInputs() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return false;
		}
		else if (event.type == SDL_KEYDOWN) {
			std::printf("Key down: %s\n", SDL_GetKeyName(event.key.keysym.sym));
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				return false;
				break;

				case SDLK_w:
				std::printf("w\n");
				input_w = true;
			    break;
				
				case SDLK_d:
				input_d = true;
			    break;

				case SDLK_s:
				input_s = true;
			    break;

				case SDLK_a:
				input_a = true;
			    break;

				default:
				break;
			}

		} else if (event.type == SDL_KEYUP) {
			std::printf("Key up: %s\n", SDL_GetKeyName(event.key.keysym.sym));
			switch (event.key.keysym.sym) {
				case SDLK_w:
				input_w = false;
			    break;
				
				case SDLK_d:
				input_d = false;
			    break;

				case SDLK_s:
				input_s = false;
			    break;

				case SDLK_a:
				input_a = false;
			    break;

				default:
				break;
			}

		} else {
			// Unhandled event
		}
	}

	return true;
}
};
