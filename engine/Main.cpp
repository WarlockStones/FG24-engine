// Copyright 2024 Changemaker Educations AB. All Rights Reserved
#include "framework/Session.hpp"
#include <cstdio>

#include "utils/File.hpp"
#include "renderer/Mesh.hpp"


int main(int argv, char** args) {

	FG24::Session session;

	/*
	FILE* fp = std::fopen("../../assets/mesh/flag.obj", "r");
	if (fp == nullptr) {
		fp = std::fopen("../../assets/mesh/flag.obj", "r");
	}
	char buf[512]{};
	if (fp != nullptr) {
	  std::fgets(buf, sizeof(buf), fp);
	} else {
	  std::printf("no fopen...\n");
	}
	std::printf("%s", buf);

	if (fp) {
	  std::fclose(fp);
	}
	*/

	std::printf("%ld\n", sizeof(FG24::Face) + sizeof(FG24::Normal) + sizeof(FG24::UV) + sizeof(FG24::Vertex));
	std::printf("%ld\n", sizeof(FG24::MeshData));
	FG24::File::LoadObjToMeshData("../../assets/mesh/flag.obj");

	/*
	if (session.Init() == 0) {
		session.Start();
		session.GameLoop();
	}
	*/
	std::printf("Main returned 0\n");

	return 0;
}
