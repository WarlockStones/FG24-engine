// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "engine/Engine.hpp"
#include <stdio.h>
int main(int argv, char** args)
{
	FG24::Engine engine;
	if (engine.Init() == 0)	{
		 engine.GameLoop();
	}

	printf("Program exited...\n");
}