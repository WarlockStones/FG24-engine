#include "InputManager.hpp"
#include <SDL2/SDL_events.h>
#include <stdio.h>

namespace FG24 {
	bool InputManager::ProcessInputs()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) {
				return false;
			}
			else if (event.type == SDL_KEYDOWN) {
				printf("Key down: %s\n", SDL_GetKeyName(event.key.keysym.sym));
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					return false;
				}
			} else if (event.type == SDL_KEYUP) {
				printf("Key up: %s\n", SDL_GetKeyName(event.key.keysym.sym));

			}
			else
			{
				// Unhandled event
			}
		}

		return true;
	}
};