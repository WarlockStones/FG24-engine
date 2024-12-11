#include "File.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cctype>
#include <utility> // c++
#include <vector>  // c++
#include "renderer/Mesh.hpp"
#include "Filepath.hpp"

namespace FG24 {
namespace File {

// RAII c FILE*. This class is local to this file
class FileStream {
public:
	FILE* ptr = nullptr;
	FileStream(const char* path, const char* mode)
		// TODO: Consider using std::fopen_s instead - MSVC warning
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

	// std::printf("Successfully read file of size %I32d\n", fileSize);

	// TODO: Construct the object in return value or move? Or just use unique_ptr?
	return text;
}

Vertex ParseV(char* token) {
	token = std::strtok(nullptr, " "); // Tokenize to next past index
	// Read x y z values for vertex data
	float vert[3]{};
	for (int i = 0; i < 3; ++i) {
		if (token) {
			if (std::sscanf(token, "%f", &vert[i]) > 0) {
			} else {
				std::fprintf(stderr, "Error: ParseV: invalid vertex data!\n");
			}

			token = std::strtok(nullptr, " ");
		} else {
			// TODO: Error handling
			std::fprintf(stderr, "Error: ParseV: Found bad vertex data!\n");
		}
	}

	Vertex v;
	v.x = vert[0];
	v.y = vert[1];
	v.z = vert[2];

	return v;
}

// TODO: Error checking tokenizer
Face ParseF(char* token) {

	// Tokenize
	token = std::strtok(nullptr, " "); // Get prefix

	// Each face contains at least 3 values
	// f 111/111/111 222/222/222 333/333/333 ...

	char* fTokens[512] {nullptr}; // Realistically no face will have 512
	std::size_t faceCount = 0; // TODO: rename to indicesCount or numIndices; This is all 1 face
	while (token != nullptr) {
		fTokens[faceCount] = token;
		faceCount++;
		token = std::strtok(nullptr, " ");
	}
	if (faceCount == 4) {
		// TODO: Triangulate quads and inform the user of this action
	}
	else if (faceCount > 4) {
		std::printf ("Mesh has n-gons. Please fix...\n");
	}

	// Tokenize the /
	Face face;
	face.numIndices = 3; // Hard coded to only be 3
	// Loop through all values and tokenize 3 more times for v, vt, vn
	// TODO: Handle more than 3 entries
	for (std::size_t i = 0; i < 3 && fTokens[i] != nullptr; ++i) {

		static auto getInt = [](char* token) {
			std::uint32_t result = 0;
			if (token) {
				if (std::sscanf(token, "%ul", &result) > 0) {
				} else {
					std::fprintf(stderr, "Error: could not turn face string into int!\n");
				}
			} else {
				// TODO: handle .obj files with // like '111//111 222/222/222 333//333'
				// if '//' ignore. Vertex texture coordinate indicies are optional
				// TODO: Handle .obj files with no / like '1 2 3'. no token '/'
				std::fprintf(stderr, "Error: attempting to tokenize a bad face token string!\n");
				std::fprintf(stderr, "Maybe the .obj file has a face value with '//'?\n");
			}
			return result;
		};

		char* t = std::strtok(fTokens[i], "/");
		face.v[i] = getInt(t);
		t = std::strtok(nullptr, "/");
		face.vt[i] = getInt(t);
		t = std::strtok(nullptr, "/");
		face.vn[i] = getInt(t);
	}

	return face; // move instead of copy?
}

// TODO: Better error handling
MeshData LoadObjToMeshData(Filepath filepath) {
	const char* path = filepath.GetPath();

	FileStream file(path, "rb");
	if (!file.ptr) {
		std::fprintf(stderr, "No file!\n%s\n", path);
		return MeshData();
	} 

	// If the file has meta data, read and allocate necessary size instead of std::vector
	// I did not want to loop twice to do that.
	std::vector<Vertex> vertices;
	vertices.reserve(1024);
	// std::vector<Face> faces;
	std::vector<std::uint32_t> ind; // Face indices
	ind.reserve(1024);

	constexpr std::size_t bufMax = 256;
	for (char buf[bufMax]; std::fgets(buf, bufMax, file.ptr) != nullptr;) {
		// Tokenize
		char* token = std::strtok(buf, " "); // Get prefix
		if (token) {
			if (std::strcmp(token, "v") == 0) {
				vertices.push_back(ParseV(token));
			} else if (std::strcmp(token, "vt") == 0) {
				// Handle UV
			} else if (std::strcmp(token, "vn") == 0) {
				// Handle vertex normal
			} else if (std::strcmp(token, "f")  == 0) {
				Face face = ParseF(token);
				ind.push_back(face.v[0]);
				ind.push_back(face.v[1]);
				ind.push_back(face.v[2]);
			}
		}
	}

	Vertex* v = new Vertex[vertices.size()];
	std::copy(vertices.begin(), vertices.end(), v);

	std::uint32_t* indices = new std::uint32_t[ind.size()*3];
	std::copy(ind.begin(), ind.end(), indices);

	MeshData data;
	data.vertices = v;
	data.numVertices = vertices.size();

	data.indices = indices;
	data.numIndices = ind.size();

	data.normals = nullptr;

	data.UVs = nullptr;

#ifdef false
	std::printf("Printing the stuff in File.cpp: \n");
	for (std::size_t i = 0; i < data.numVertices; ++i) {
		std::printf("%f %f %f\n", data.vertices[i].x, data.vertices[i].y, data.vertices[i].z);
	}
	std::printf("Printing indices in File.cpp\n");
	for (int i = 0; i < ind.size(); ++i) {
		std::printf("%u ", indices[i]);
	}
	std::printf("\n");
#endif
	
	return data;
}

MeshData OldParseObj(const char* path) {
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
	char lineBuffer[maxLineLength+1]; // +1 to accommodate for '\0'
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
			// lineBuffer should be tokenized with null-terminated strings
			// f'\0'111/222/333'\0'111/222/333'\0'111/222/333'\n''\0'
			//                                                    ^^^ - the +1

			// TODO: Check for n-gons and triangulate quads

			// v = xy
			// vt = uv w (both v and w are actually optional but I want 2 uv)
			// vn = i j k

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
			for (std::size_t i = 0; i < tokenCount; ++i) {
				tokens[i] = nullptr;
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
