#include "File.hpp"
#include <cstdio>
#include <cstdint>
#include <utility>
#include <cstring>
#include <cassert>
#include <cctype>
#include "renderer/Mesh.hpp"

namespace FG24 {
namespace File {

// RAII c FILE*. This class is local to this file
static class FileStream {
public:
	FILE* ptr = nullptr;
	FileStream(const char* path, const char* mode)
		: ptr(std::fopen(path, mode)) {
	}

	~FileStream() {
		if (ptr != nullptr) {
			fclose(ptr);
		}
	}
};

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
	FileStream file(path, "r");
	if (Check(file.ptr == nullptr, "fopen()")) {
		return nullptr;
	}
		
	int result = std::fseek(file.ptr, 0, SEEK_END);
	if (Check(result != 0, "fseek()")) {
		return nullptr;
	}

	std::int32_t fileSize = std::ftell(file.ptr);
	if (Check(fileSize == -1L, "ftell()"))  {
		return nullptr;
	}

	result = std::fseek(file.ptr, 0, SEEK_SET);
	if (Check(result != 0, "fseek()")) {
		return nullptr;
	}

	// Allocate string of fileSize + 1 for '\0'
	std::size_t textBuffSize = static_cast<std::size_t>(fileSize + 1);
	char* text = new char[textBuffSize]; // TODO: Fix memory leak...

	// TODO: Read count is wrong when not using UNIX formatted file
	std::size_t readCount = std::fread(
		static_cast<char*>(text), sizeof(char), static_cast<std::size_t>(fileSize), file.ptr);
	if (Check(readCount != static_cast<std::size_t>(fileSize), "fread()")) {

		std::ferror(file.ptr);
		std::fprintf(stderr, "Read count: %zd is not file size %I32d\n", readCount, fileSize);
		return nullptr;
	}

	text[fileSize] = '\0';

	std::printf("Successfully read file of size %I32d\n", fileSize);

	return text;
}

// TODO: Should return something
static void ParseUVline(const char** tokens) {

}

// TODO: Utilise the safe versions of c-functions e.g. sscanf instead of scanf
// TODO: Do proper error handling
MeshData LoadObjToMeshData(const char* path) {
	// TODO: Make this into a function that converts file paths
	//       Or I should probably have a FilePath class, and handle common file-paths as literals '../../assets'.
	std::size_t pathlen = std::strlen(path);
	if (pathlen > 260) {
		// TODO: Do proper error handling
		std::fprintf(stderr, "ERROR: Path to .obj file is too long!\n%s\n", path);
		return MeshData();
	}

	char p[260];
	std::memcpy(p, path, sizeof(path));
	FormatFilePath(p, sizeof(path));

	FileStream file(path, "rb");
	if (!file.ptr) {
		std::fprintf(stderr, "No file!\n%s\n", path);
		return MeshData();
	}

	// There is no libc way of doing this, I guess I just have to read character by character
	// TODO: Explain what is going on here. Why I am doing this to prepare reader for the following code
	constexpr std::size_t maxLineLength = 512;
	char lineBuffer[maxLineLength+1]; // +1 to accomondate for '\0'
	std::size_t currentBufferLength = 0;
	int c; // int is required to handle EOF
	char previousChar = '0'; // not an empty space
	constexpr std::size_t maxTokens = 4;
	char* tokens[maxTokens] {nullptr};
	std::size_t tokenCount = 0;
	while ((c = std::fgetc(file.ptr)) != EOF) {

		// TODO: handle this error by reporting to the user that the file is bad
		if (currentBufferLength > maxLineLength) {
			std::fprintf(stderr, "Error: LoadObjToMeshData: A line in .obj is too long!\n");
			return MeshData();
		}
		assert(currentBufferLength < maxLineLength); 

		// TODO: Change to switch case for readability?
		if (c == '\n' && currentBufferLength > 0) { 
			lineBuffer[currentBufferLength + 1] = '\0'; // To make sure last token is a null-terminated string
			// At this point lineBuffer should be filled with null-terminated strings
			// f'\0'111/222/333'\0'111/222/333'\0'111/222/333'\n''\0'
			//                                                    ^^^ - the +1

			// TODO: Check for n-gons

			// v = xyz
			// vn = i j k
			// vt = uv w (both v and w are actually optional but I want 2 uv)

			// Face element reference grouping
			// FIRST: v1 indicies, then you can do the vt and vn groupings later
			// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 . . .
			// faceSpecifies a face element and its vertex reference number. 
			// vt is optional in f, so v1//vn1 is valid 1413//7210

			if (std::strcmp(lineBuffer, "v") == 0) {
				// Read 3 tokens. The x y z floats
				for (int i = 0; i < 3; ++i) {
					if (tokens[i] == nullptr) {
						std::fprintf(stderr, "Error: LoadObjToMeshData: Found invalid vertex data!\n");
						return MeshData();
					}
					// TODO:: Allocate the data for MeshData
					float value{};
					if (std::sscanf(tokens[i], "%f", &value) > 0)	{
						std::printf("vertex token '%d' = '%f'\n", i, value);
					} else {
						std::fprintf(stderr, "Error: LoadObjMeshData: Failed to read vertex float data!\n");
						return MeshData();
					}
				}
			}
			else if (std::strcmp(lineBuffer, "vt") == 0) {
				// Handle UV
			} else if (std::strcmp(lineBuffer, "vn") == 0) {
				// Handle vertex normal. (Don't care about the optional w)
			} else if (std::strcmp(lineBuffer, "f") == 0) {
				// Handle face index
			}

			// Reset buffer length and tokens
			currentBufferLength = 0; 
			for (int i = 0; i < tokenCount; ++i) {
				tokens[i] == nullptr;
			}
			tokenCount = 0; 


		} else if (c == ' ') {
			// Tokenize the lineBuffer and use it as an array of null-terimanted c-strings
			lineBuffer[currentBufferLength] = '\0';
			++currentBufferLength;
		}  else if (std::isalnum(c) || c == '-' || c == '.') { // Found a valid symbol
			if (previousChar == ' ' && tokenCount < maxTokens) {
				// Found a new word after some spaces
				// Add pointer to new token. Using tokens and lineBuffer as a multidimensional array
				tokens[tokenCount] = &lineBuffer[currentBufferLength];
				++tokenCount;
			}
			lineBuffer[currentBufferLength] = c;
			++currentBufferLength;
		}
		previousChar = c;
	}

	return MeshData();
}

} // namespace File
} // namespace FG24
