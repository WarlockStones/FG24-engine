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

	Vertex v;
	v.x = vert[0];
	v.y = vert[1];
	v.z = vert[2];

	return v;
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
	constexpr std::uint8_t vBit  = 0b001;
	constexpr std::uint8_t uvBit = 0b010;
	constexpr std::uint8_t vnBit = 0b100;

	static auto AddToCorrectBuffer = [](
		const char* buf,
		int elementCount,
		int vIndex,
		FaceIndexElement* out) {
			switch (elementCount) {
				case 0: 
					std::sscanf(buf, "%d", &out[vIndex].v);
					return vBit; 
					break;
				case 1:
					std::sscanf(buf, "%d", &out[vIndex].uv);
					return uvBit; 
					break;
				case 2:
					std::sscanf(buf, "%d", &out[vIndex].vn);
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
	std::uint8_t elements = 0b000;
	int vertexCount = 0;
	int numCount = 0;
	int elementCount = 0;
	constexpr int bufSize = 124;
	char buf[bufSize];
	for (std::size_t i = 0; str[i] != '\0'; ++i) {
		// Parser can not yet handle "//"
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
			elements |= AddToCorrectBuffer(buf, elementCount, vertexCount, f);
			++elementCount;
			numCount = 0;
		} else if (c == ' ' || c == '\n') {
			if (numCount > 0) {
				elements |= AddToCorrectBuffer(buf, elementCount, vertexCount, f);
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

	if (elements & uvBit) {
	  face.hasUV = true;
	}
	if (elements & vnBit) {
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

// TODO: Error handling
UV ParseVT(char* token) {
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
	UV uv;
	uv.u = uvw[0];
	uv.v = uvw[1];
	return uv;
}

// TODO: Error handling
Normal ParseVN(char* token) {
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

	Normal n;
	n.i = data[0];
	n.j = data[1];
	n.k = data[2];
	return n;
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
	vertices.reserve(512);
	
	std::vector<UV> UVs;
	UVs.reserve(512);

	std::vector<Normal> normals;
	normals.reserve(512);

	std::vector<std::int32_t> vInd;
	vInd.reserve(512);
	std::vector<std::int32_t> uvInd;
	uvInd.reserve(512);
	std::vector<std::int32_t> vnInd;
	vnInd.reserve(512);

	constexpr std::size_t bufMax = 256;
	for (char buf[bufMax]{}; std::fgets(buf, bufMax, file.ptr) != nullptr;) {
		char prefix[3] = { buf[0], buf[1] , '\0' };
		if (std::strcmp(prefix, "v ") == 0) {
			vertices.push_back(ParseV(buf));
		} else if (std::strcmp(prefix, "vt") == 0) {
			UVs.push_back(ParseVT(buf));
		} else if (std::strcmp(prefix, "vn") == 0) {
			normals.push_back(ParseVN(buf));
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

	// TODO: Error check that the Face Index Elmenets vectors are of equal size
	// i.e Check that the face optional elements are consistent.
	// i.e: f 1//1 2/2/ BAD!
	// i.e: f 1 2 3/3/3 BAD!
	// Face element indices error checks
	if (vInd.size() == 0) {
		std::fprintf(stderr, "Error: Parsed 0 face vertex indices!\n");
		return MeshData();
	}

	// TODO: Is this a valid error? Check .obj specification
	if (uvInd.size() != vnInd.size()) {
		std::fprintf(stderr, "Error: The amount of UV idices != amount normal indices!\n");
		std::fprintf(stderr, "There may have been an issue with parsing face index elements.\n");

		return MeshData();
	}

	Vertex* v = new Vertex[vertices.size()];
	std::copy(vertices.begin(), vertices.end(), v);

	UV* uv = new UV[UVs.size()];
	std::copy(UVs.begin(), UVs.end(), uv);

	Normal* n = new Normal[normals.size()];
	std::copy(normals.begin(), normals.end(), n);

	std::int32_t* vertexIndices = new std::int32_t[vInd.size()];
	std::copy(vInd.begin(), vInd.end(), vertexIndices);

	std::int32_t* uvIndices = nullptr;
	if (uvInd.size() > 0) {
	  uvIndices = new std::int32_t[uvInd.size()];
	  std::copy(uvInd.begin(), uvInd.end(), uvIndices);
	}
	std::int32_t* normalIndices = nullptr;
	if (vnInd.size() > 0) {
	  normalIndices = new std::int32_t[vnInd.size()];
	  std::copy(vnInd.begin(), vnInd.end(), normalIndices);
	}

	MeshData data;
	data.vertices = v;
	data.numVertices = vertices.size();

	data.UVs = uv;
	data.numUVs = UVs.size();

	data.normals = n;
	data.numNormals = normals.size();

	data.vertexIndices = vertexIndices;
	data.numVertexIndices = vInd.size();

	data.numUVIndices = uvInd.size();
	data.UVIndices = uvIndices;
  
	data.numNormalIndices = vnInd.size();
	data.normalIndices = normalIndices; 
	

#if true
	std::printf("--- Printing the stuff in File.cpp: ---\n");
	for (std::size_t i = 0; i < data.numVertices; ++i) {
		std::printf("%f %f %f\n", data.vertices[i].x, data.vertices[i].y, data.vertices[i].z);
	}
	std::printf("uv:\n");
	for (std::size_t i = 0; i < data.numUVs; ++i) {
		std::printf("%f %f\n", data.UVs[i].u, data.UVs[i].v);
	}
	std::printf("n:\n");
	for (std::size_t i = 0; i < data.numNormals; ++i) {
	  const auto& x = data.normals[i];
	  std::printf("%f %f %f\n", x.i, x.j, x.k);
	}
	std::printf("vertex indices:\n");
	for (std::size_t i = 0; i < data.numVertexIndices; ++i) {
		std::printf("%d ", data.vertexIndices[i]);
	}
	std::printf("\nuv indices:\n");
	for (std::size_t i = 0; i < data.numUVIndices; ++i) {
		std::printf("%d ", data.UVIndices[i]);
	}
	std::printf("\nnormal indices:\n");
	for (std::size_t i = 0; i < data.numNormalIndices; ++i) {
		std::printf("%d ", data.normalIndices[i]);
	}
	std::printf("\n--- End of File.cpp ---\n");
#endif

	// TODO: Order indices. Do the "Indexing" operation and just feed OpenGL ordered vertices
	return data;
}

} // namespace File
} // namespace FG24
