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
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				return false;
			}
		} else if (event.type == SDL_KEYUP) {
			std::printf("Key up: %s\n", SDL_GetKeyName(event.key.keysym.sym));

		} else {
			// Unhandled event
		}
	}

	return true;
}
};