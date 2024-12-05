#include "File.hpp"
#include <cstdio>
#include <cstdint>
#include <utility>
#include <cstring>
#include <cassert>
#include "renderer/Mesh.hpp"

namespace FG24 {
namespace File {
void FormatFilePath(char* s, std::size_t size) {
#ifdef _WIN32
	for (size_t i = 0; i < size; i++) {
		if (s[i] == '/') {
			s[i] = '\\';
		}
	}
#endif
}


const char* LoadTextFile(const char* path) {
	// TODO: Measure lambda vs static function
	auto Check = [path](bool failCondition, const char* functionName) {
		if (failCondition == true) {
			std::perror(functionName);
			std::fprintf(stderr, "Error: LoadTextFile %s failed for path '%s'\n",
				functionName, path);
			return true;
		} else {
			return false;
		}
	};

	std::size_t pathlen = std::strlen(path);
	assert(pathlen < 260);
	// Windows only allows for 260 character paths. This is probably faster than dynamic allocation
	// TODO: Measure stack array vs dynamic array
	char p[260];
	std::memcpy(p, path, sizeof(path));
	FormatFilePath(p, sizeof(path));

	// TODO: Consider using std::fopen_s instead - MSVC warning
	std::FILE* file = std::fopen(path, "r");
	if (Check(file == nullptr, "fopen()")) {
		return nullptr;
	}
		
	int result = std::fseek(file, 0, SEEK_END);
	if (Check(result != 0, "fseek()")) {
		return nullptr;
	}

	std::int32_t fileSize = std::ftell(file);
	if (Check(fileSize == -1L, "ftell()"))  {
		return nullptr;
	}

	result = std::fseek(file, 0, SEEK_SET);
	if (Check(result != 0, "fseek()")) {
		return nullptr;
	}

	// Allocate string of fileSize + 1 for '\0'
	std::size_t textBuffSize = static_cast<std::size_t>(fileSize + 1);
	char* text = new char[textBuffSize]; // TODO: Fix memory leak...

	// TODO: Read count is wrong when not using UNIX formatted file
	std::size_t readCount = std::fread(
		static_cast<char*>(text), sizeof(char), static_cast<std::size_t>(fileSize), file);
	if (Check(readCount != static_cast<std::size_t>(fileSize), "fread()")) {

		std::ferror(file);
		std::fprintf(stderr, "Read count: %zd is not file size %I32d\n", readCount, fileSize);
		return nullptr;
	}

	text[fileSize] = '\0';

	std::fclose(file);
	std::printf("Successfully read file of size %I32d\n", fileSize);

	return text;
}

MeshData LoadObjToMeshData(const char* path) {
	// Load v. Always 3 floats on line
	// Store first token as prefex
	// Check prefix if "v" else if "vt" etc etc
	// tokenization to load annoying annoying faces f.

	// TODO: Make this into a function that converts file paths
	std::size_t pathlen = std::strlen(path);
	assert(pathlen < 260);
	char p[260];
	std::memcpy(p, path, sizeof(path));
	FormatFilePath(p, sizeof(path));


	std::FILE* file = std::fopen(path, "rb");
	if (!file) {
		std::fprintf(stderr, "No file!\n%s\n", path);
	}
	assert(file); // TODO: Error handling. return default MeshData

	// There is no libc way of doing this, I guess I just have to read character by character
	constexpr int maxLineLength = 512;
	char lineBuffer[maxLineLength];
	std::size_t currentBufferLength = 0;
	int c; // int is required to handle EOF
	while ((c = std::fgetc(file)) != EOF) {

		// TODO: handle this error by reporting to the user that the file is bad
		assert(currentBufferLength < maxLineLength); 
		// std::putchar(c); // Debug print

		lineBuffer[currentBufferLength] = c;
		++currentBufferLength;
		if (c == '\n') { // Is this multiplatform?
			// Handle and clear the buffer
			// Tokenize the buffer
			std::putchar(c);

			// f 1413/1629/8163 1412/1630/8164 1408/1631/8165 1407/1632/8166
			// TODO: Check for n-gons

			char* tokens[4]; // 
			// At start of each word > > parse until ' 'white-space or '\n'
			// I could replace space with '\0' and basically have an optimised
			// multidimensional array!
			// I can just as I parse, if i find a ' ' I change it to a '\0'
			// Then hopefully I can securly translate the strings to a int/float
			// And if I can not then I safely terminate and do not seg fault :-)

			// TODO: Test if this crashes if you read bad data
			// char myarray[5] = {'-', '1', '2', '3', '\0'};
			// int i;
			// sscanf(myarray, "%d", &i);


			// v = xyz
			// vn = i j k
			// vt = uv w (both v and w are actually optional but I want 2 uv)

			// Face element reference grouping
			// FIRST: v1 indicies, then you can do the vt and vn groupings later
			// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 . . .
			// faceSpecifies a face element and its vertex reference number. 
			// vt is optional in f, so v1//vn1 is valid 1413//7210

			int b = lineBuffer[0];
			if (b == 'v') {
				// Handle vertex
				// Tokenize the buffer
				// AM I REALLY GOING TO WRITE A TOKENIZER IN C?!

			} else if (b == 'vt') {
				// Handle UV
			} else if (b == 'vn') {
				// Handle vertex normal. (Don't care about the optional w)
			} else if (b == 'f') {
				// Handle face index
			}

			for (int i = 0; i < currentBufferLength; ++i) {
				// Loop the line
			}

			currentBufferLength = 0; // Reset the buffer
		}
	}


	return MeshData();
}

} // namespace File
} // namespace FG24
