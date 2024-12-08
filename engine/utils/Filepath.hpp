#pragma once

// TODO: Constants. Symbolic paths to common directories

class Filepath {
public:
	Filepath(const char* path); // Translates paths into the OS required syntax
	// Filepath(const char* directory, const char* filename)
	const char* GetPath();
private:
	char path[254]{};
};
