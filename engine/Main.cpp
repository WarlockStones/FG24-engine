// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "framework/Session.hpp"
#include <cstdio>

#include "utils/Memory.hpp"

int main(int argv, char** args) {

    auto kb = FG24::Memory::GetAvailableKilobytes();
	std::printf("Avialable KBs: %lu\n", kb);
	std::printf("Avialable MBs: %lu\n", kb/1000);

	FG24::Session session;

	if (session.Init() == 0) {
		session.Start();
		session.GameLoop();
	}

	std::printf("Main returned 0\n");
	return 0;
}
