// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#pragma once
namespace FG24 {
	class Renderer;
	class InputManager;
	class Engine {
	public:
		Engine() = default;
		bool Init();	// Initializes core engine modules like renderer
		void Start();	// Starts the game. Loads game objects
		void GameLoop();
		~Engine();

	private:
		InputManager* inputManager{};
		Renderer* renderer{};
};
}