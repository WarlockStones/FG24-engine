// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "engine/Engine.hpp"
#include <cstdio>

int main(int argv, char** args) {

	FG24::Engine engine;

	if (engine.Init() == 0)	{
		engine.Start();
		engine.GameLoop();
	}

	std::printf("Program exited...\n");
}
