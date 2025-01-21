#include "File.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring> // memcmp
#include <cassert>
#include <cctype>
#include <utility> // c++
#include <vector>  // c++
#include <map>     // c++
#include "renderer/Mesh.hpp"
#include "Filepath.hpp"
#include "utils/Vec.hpp"

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

// TODO: Do not use fseek() and ftell() to compute the size of a regular file
// https://wiki.sei.cmu.edu/confluence/display/c/FIO19-C.+Do+not+use+fseek%28%29+and+ftell%28%29+to+compute+the+size+of+a+regular+file
// TODO: Create one method for Windows and another for Linux
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

	FileStream file(path, "rb"); // t, text-mode converts windows CRLF to LF.
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

	std::size_t readCount = std::fread(
		static_cast<char*>(text), sizeof(char), static_cast<std::size_t>(fileSize), file.ptr);
	if (Check(readCount != static_cast<std::size_t>(fileSize), "fread()")) {
		std::ferror(file.ptr);
		std::fprintf(stderr, "Read count: %zd is not file size %I32d\n", readCount, fileSize);
		return nullptr;
	}

	text[fileSize] = '\0';

	// TODO: Construct the object in return value or move? Or just use unique_ptr?
	return text;
}

Vec3 ParseV(char* token) {
	token = std::strtok(token, " "); 
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

	return Vec3(vert[0], vert[1], vert[2]);
}

// TODO: Error handling
Vec2 ParseVT(char* token) {
	token = std::strtok(token, " ");
	token = std::strtok(nullptr, " "); // Tokenize to next past index
	// Read u v w values as vertex texture coordinates
	float uvw[3]{};
	for (int i = 0; i < 3; ++i) {
		if (token) {
			if (std::sscanf(token, "%f", &uvw[i]) <= 0) {
				std::fprintf(stderr, "Error: ParseVT: invalid vertex texture data!\n");
			}

			token = std::strtok(nullptr, " ");
		} else if (i == 0) {
			// v and w are optional but there must always be an u value
			std::fprintf(stderr, "Error: ParseVT: failed to tokenize vt 'u' value!\n");
		}
	}
	
	// std::printf("ParseVT: %f %f %f\n", uvw[0], uvw[1], uvw[2]);

	// There is no support for the optional w value.
	return Vec2(uvw[0], uvw[1]);
}

// TODO: Error handling
Vec3 ParseVN(char* token) {
	token = std::strtok(token, " ");
	token = std::strtok(nullptr, " "); // Tokenize to next past index
	// Read i j k values as vertex texture coordinates
	float data[3]{};
	for (int i = 0; i < 3; ++i) {
		if (token) {
			if (std::sscanf(token, "%f", &data[i]) <= 0) {
				std::fprintf(stderr, "Error: ParseVN: invalid vertex normal data!\n");
			}

			token = std::strtok(nullptr, " ");
		} else {
			// i j k are not optional. There must be 3 values
			std::fprintf(stderr, "Error: ParseVN: invalid vertex normal data!\n");
		}
	}

	return Vec3(data[0], data[1], data[2]);
}

struct FaceIndexElement {
	int v, uv, vn;
};

struct Face { // TODO: Remove need for this Face struct
	static constexpr std::size_t max = 3;
	bool hasUV = false;
	bool hasNormal = false;
	std::size_t numIndices = 0;
	std::int32_t v[max]{}, uv[max]{}, vn[max]{}; // Indicies
};


Face ParseF(const char* str) {
	// elements,  e1, e2, e3 
	// v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
	// vertex 1,  vertex 2,  vertex 3

	// TODO: Handle optional vt syntax 1//1 2//2 3//3
	// TODO: Range check arrays
    // TODO: Check for inconsistent face element indices. i.e 1/1/1 2//2 3/3/3

	// A lambda can read values of constexpr variables without capturing it

	// Bit patterns used to document which types of index elements found
	constexpr std::uint8_t vBit  = 0b001;
	constexpr std::uint8_t uvBit = 0b010;
	constexpr std::uint8_t vnBit = 0b100;

	static auto AddToCorrectBuffer = [vBit, uvBit, vnBit](
		const char* buf,
		int elementCount,
		int vIndex,
		FaceIndexElement* out) {
			switch (elementCount) {
				case 0:
					std::sscanf(buf, "%d", &out[vIndex].v); 
					--out[vIndex].v; // .obj indexing starts at 1, not 0
					return vBit; 
					break;
				case 1:
					std::sscanf(buf, "%d", &out[vIndex].uv);
					--out[vIndex].uv;
					return uvBit; 
					break;
				case 2:
					std::sscanf(buf, "%d", &out[vIndex].vn);
					--out[vIndex].vn;
					return vnBit;
					break;
				default:
					// Error
					break;
			}
			return std::uint8_t(0b000);
	};

	constexpr int vMax = 48; // Arbitrarily sets max to 48
	FaceIndexElement f[vMax]{};
	std::uint8_t elementTypesFound = 0b000;
	int vertexCount = 0;
	int numCount = 0;
	int elementCount = 0;
	constexpr int bufSize = 124;
	char buf[bufSize];
	for (std::size_t i = 0; str[i] != '\0'; ++i) {
		// Parser can not yet handle "//"
	    // TODO: This should not be an assert! It is an user error
		assert((str[i] == '/' && str[i + 1] == '/') == false); 

		char c = str[i];
		// std::printf("|%c|", str[i]);

		if (std::isdigit(c)) { // Found a valid symbol
			if (numCount < bufSize - 1) {
				buf[numCount] = c;
				buf[numCount + 1] = '\0';
				++numCount;
			}
		} else if (c == '/') {
			elementTypesFound |= AddToCorrectBuffer(buf, elementCount, vertexCount, f);
			++elementCount;
			numCount = 0;
		} else if (c == ' ' || c == '\n') {
			if (numCount > 0) {
				elementTypesFound |= AddToCorrectBuffer(buf, elementCount, vertexCount, f);
				++vertexCount;
			}
			numCount = 0;
			elementCount = 0;
		}
	}

	// TODO: Triangulate
	// if vertexCount == 4; triangulate
	// if vertexCount > 4; triangulate? complain

#if false
	std::printf("VertexCount: %d\n", vertexCount);
	for (int i = 0; i < vertexCount; ++i) {
		std::printf("%d %d %d\n", f[i].v, f[i].uv, f[i].vn);
	}
#endif

	Face face; 

	if (elementTypesFound & uvBit) {
	  face.hasUV = true;
	}
	if (elementTypesFound & vnBit) {
	  face.hasNormal = true;
	}

	// TODO: Remove need of face and this awkward operation
	for (int i = 0; i < 3; ++i) {
		face.v[i] = f[i].v;
		face.uv[i] = f[i].uv;
		face.vn[i] = f[i].vn;
	}

	return face;
}

// Packed contigious memory for fast memcmp
/* May be used later for indicies
struct Vertex {
	Vec3 position;
	Vec2 uv;
	Vec3 normal;
	bool operator<(const Vertex& other) const {
		return std::memcmp(
			reinterpret_cast<const void*>(this),
			reinterpret_cast<const void*>(&other),
			sizeof(Vertex))>0;
	};
};
*/


// TODO: Support .obj files that has other attribute configuration than v,vt,vn
ErrorCode LoadObjToMeshData(
	Filepath filepath,
	float*& vertexDataOut,
	std::size_t& numVertexDataOut,
	std::size_t& numVerticiesOut)
{
	const char* path = filepath.GetPath();

	FileStream file(path, "rb");
	if (!file.ptr) {
		std::fprintf(stderr, "LoadObjToMeshData FileStream was null!\n%s\n", path);
		return ErrorCode::NoFile;
	} 

	// Temporary attributes that has .obj indexing. Must be converted
	std::vector<Vec3> tempPositions; // vertex positions. TODO: Rename
	std::vector<Vec2> tempUVs;
	std::vector<Vec3> tempNormals;
	tempPositions.reserve(512);
	tempUVs.reserve(512);
	tempNormals.reserve(512);

	std::vector<std::int32_t> vInd;
	std::vector<std::int32_t> uvInd;
	std::vector<std::int32_t> vnInd;
	vInd.reserve(512);
	uvInd.reserve(512);
	vnInd.reserve(512);

	// TODO:
	// i.e Check that the face optional elements are consistent.
	// i.e: f 1//1 2/2/ BAD!
	// i.e: f 1 2 3/3/3 BAD!
	// Face element indices error checks

	constexpr std::size_t bufMax = 256;
	for (char buf[bufMax]{}; std::fgets(buf, bufMax, file.ptr) != nullptr;) {
		char prefix[3] = { buf[0], buf[1] , '\0' };
		if (std::strcmp(prefix, "v ") == 0) {
			tempPositions.push_back(ParseV(buf));
		} else if (std::strcmp(prefix, "vt") == 0) {
			tempUVs.push_back(ParseVT(buf));
		} else if (std::strcmp(prefix, "vn") == 0) {
			tempNormals.push_back(ParseVN(buf));
		} else if (std::strcmp(prefix, "f ")  == 0) {
			Face face = ParseF(buf);
			vInd.push_back(face.v[0]);
			vInd.push_back(face.v[1]);
			vInd.push_back(face.v[2]);

			if (face.hasUV) {
				uvInd.push_back(face.uv[0]);
				uvInd.push_back(face.uv[1]);
				uvInd.push_back(face.uv[2]);
			}
			if (face.hasNormal) {
				vnInd.push_back(face.vn[0]);
				vnInd.push_back(face.vn[1]);
				vnInd.push_back(face.vn[2]);
			}
		}
	}

	// We have added indicies
	if (vInd.size() == 0) {
		std::fprintf(stderr, "Error: Parsed 0 indices!\n");
		return ErrorCode::LoadObjFailed;
	}

	// Indicies are of equal size
	if (vInd.size() != uvInd.size() || vInd.size() != vnInd.size()) {
		std::fprintf(stderr, "Error: Parsed .obj indicies are not of equal amount\n");
		return ErrorCode::LoadObjFailed;
	}

	std::vector<Vec3> positions;
	std::vector<Vec2> uvs;
	std::vector<Vec3> normals;
	// Indicies will indicate how many verticies will be created.
	// TODO: Make them arrays? (No reason not to use std::vector though ...)
	positions.reserve(vInd.size()); // indicies should be of equal size
uvs.reserve(uvInd.size()); 
	normals.reserve(vnInd.size()); 

	// Convert .obj indexed attributes to raw, non-indexed attributes
	for (std::size_t i = 0; i < vInd.size(); ++i) {
		positions.push_back(tempPositions[vInd[i]]);
	    uvs.push_back(tempUVs[uvInd[i]]);
	    normals.push_back(tempNormals[vnInd[i]]);
	}

	std::size_t s = positions.size() * 3;
	s += uvs.size() * 2;
	s += normals.size() * 3;
	numVerticiesOut = positions.size();
	numVertexDataOut = s;
	vertexDataOut = new float[s];
	std::size_t pos = 0;
	for (std::size_t i = 0; i < positions.size(); ++i) {
		vertexDataOut[pos++] = positions[i].x;
		vertexDataOut[pos++] = positions[i].y;
		vertexDataOut[pos++] = positions[i].z;
		vertexDataOut[pos++] = uvs[i].x;
		vertexDataOut[pos++] = uvs[i].y;
		vertexDataOut[pos++] = normals[i].x;
		vertexDataOut[pos++] = normals[i].y;
		vertexDataOut[pos++] = normals[i].z;
	}
	
	// TODO: Index

	return ErrorCode::Ok;
}

} // namespace File
} // namespace FG24
