// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "framework/Session.hpp"
#include <cstdio>

int main(int argv, char** args) {

	FG24::Session session;

	if (session.Init() == 0) {
		session.Start();
		session.GameLoop();
	}

	std::printf("Main returned 0\n");
	return 0;
}
