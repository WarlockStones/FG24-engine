#include "ObjToMdl.hpp"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <cstring>
#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <cctype>
#include <vector> // C++
#include <array>
#include <map> // C++
#include <string_view> // C++
#include <string>
#include "utils/Filepath.hpp"
#include "utils/File.hpp"

namespace FG24 {
namespace ObjToMdl {
glm::vec3 ParseV(char* token) {
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

	return glm::vec3(vert[0], vert[1], vert[2]);
}

// TODO: Error handling
glm::vec2 ParseVT(char* token) {
	token = std::strtok(token, " ");
	token = std::strtok(nullptr, " "); // Tokenize to next past index
	// Read u v w values as vertex textureID coordinates
	float uvw[3]{};
	for (int i = 0; i < 3; ++i) {
		if (token) {
			if (std::sscanf(token, "%f", &uvw[i]) <= 0) {
				std::fprintf(stderr, "Error: ParseVT: invalid vertex textureID data!\n");
			}

			token = std::strtok(nullptr, " ");
		} else if (i == 0) {
			// v and w are optional but there must always be an u value
			std::fprintf(stderr, "Error: ParseVT: failed to tokenize vt 'u' value!\n");
		}
	}

	// std::printf("ParseVT: %f %f %f\n", uvw[0], uvw[1], uvw[2]);

	// There is no support for the optional w value.
	return glm::vec2(uvw[0], uvw[1]);
}

// TODO: Error handling
glm::vec3 ParseVN(char* token) {
	token = std::strtok(token, " ");
	token = std::strtok(nullptr, " "); // Tokenize to next past index
	// Read i j k values as vertex textureID coordinates
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

	return glm::vec3(data[0], data[1], data[2]);
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


void ParseF(const char* str, std::vector<Face>& faceVectorOut) {
	// elements,  e1, e2, e3 
	// v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
	// vertex 1,  vertex 2,  vertex 3

	// TODO: Handle optional vt syntax 1//1 2//2 3//3
	// TODO: Range check arrays
	// TODO: Check for inconsistent face element indices. i.e 1/1/1 2//2 3/3/3

	// A lambda can read values of constexpr variables without capturing it

	// Bit patterns used to document which types of index elements found
	constexpr std::uint8_t vBit = 0b001;
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

	// TODO: This would be fun to optimize. Atm I make lots of copies
	// Move scemantics? Construct in place? 
	std::vector<std::array<FaceIndexElement, 3>> triangulatedFaces;
	if (vertexCount > 3) {
		// Fan triangulation
		for (int i = 0; i < vertexCount - 2; ++i) {
			std::array<FaceIndexElement, 3> temp;
			temp[0] = f[0];
			temp[1] = f[i + 1];
			temp[2] = f[i + 2];
			triangulatedFaces.push_back(temp);
		}
	} else {
		// No need to triangulate, just add to vector
		std::array<FaceIndexElement, 3> temp;
		temp[0] = f[0];
		temp[1] = f[1];
		temp[2] = f[2];
		triangulatedFaces.push_back(temp);
	}

	for (auto& f3 : triangulatedFaces) {
		Face face;
		if (elementTypesFound & uvBit) {
			face.hasUV = true;
		}
		if (elementTypesFound & vnBit) {
			face.hasNormal = true;
		}
		for (int i = 0; i < 3; ++i) {
			face.v[i] = f3[i].v;
			face.uv[i] = f3[i].uv;
			face.vn[i] = f3[i].vn;
		}

		faceVectorOut.push_back(face);
	}
}

// TODO: Support .obj files that has other attribute configuration than v,vt,vn
ErrorCode LoadObjToVertexData(Filepath path, VertexData& vertexDataOut) {
	File::FileStream file(path, "rb");
	if (!file.ptr) {
		std::fprintf(
			stderr,
			"LoadObjToMeshData FileStream was null!\n%s\n",
			path.ToString());
		return ErrorCode::NoFile;
	}

	// Temporary attributes that has .obj indexing. Must be converted
	std::vector<glm::vec3> tempPositions; // vertex positions. TODO: Rename
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;
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
		} else if (std::strcmp(prefix, "f ") == 0) {
			static std::vector<Face> faces;
			ParseF(buf, faces);
			for (const Face& face : faces) {
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
			faces.clear();
		}
	}

	// We have added indices
	if (vInd.size() == 0) {
		std::fprintf(stderr, "Error: Parsed 0 indices!\n");
		return ErrorCode::LoadObjFailed;
	}

	// Indicies are of equal size
	if (vInd.size() != uvInd.size() || vInd.size() != vnInd.size()) {
		std::fprintf(stderr, "Error: Parsed .obj indicies are not of equal amount\n");
		return ErrorCode::LoadObjFailed;
	}

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	// Indicies will indicate how many vertices will be created.
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
	vertexDataOut.m_numVertices = positions.size();
	vertexDataOut.m_numVertexData = s;
	vertexDataOut.m_data = new float[s];
	std::size_t pos = 0;
	for (std::size_t i = 0; i < positions.size(); ++i) {
		vertexDataOut.m_data[pos++] = positions[i].x;
		vertexDataOut.m_data[pos++] = positions[i].y;
		vertexDataOut.m_data[pos++] = positions[i].z;
		vertexDataOut.m_data[pos++] = uvs[i].x;
		vertexDataOut.m_data[pos++] = uvs[i].y;
		vertexDataOut.m_data[pos++] = normals[i].x;
		vertexDataOut.m_data[pos++] = normals[i].y;
		vertexDataOut.m_data[pos++] = normals[i].z;
	}

	// TODO: Index

	return ErrorCode::Ok;
}

static constexpr std::uint32_t mdlVersion = 1;
static constexpr std::uint32_t magic = 0x6C646F6D; // "modl" ASCII to hex
ErrorCode LoadMdlToVertexData(Filepath filepath, VertexData& vertexDataOut) {
	File::FileStream fs(filepath, "rb");
	if (fs.IsValid()) {
		// Read file header
		std::uint32_t fileHeader[2]{};
		const std::size_t n = std::fread(&fileHeader[0], sizeof(fileHeader[0]), 2, fs.ptr);
		if (n == 2) {
			// Successfully read the header
			if (fileHeader[0] != magic) {
				std::fprintf(
					stderr,
					"Error: .mdl file has invalid magic word!\n\t%s\n",
					filepath.ToString());
				return ErrorCode::LoadMdlFailed;
			} 
			if (fileHeader[1] != mdlVersion) {
				std::fprintf(
					stderr,
					"Error: .mdl file has invalid version number!\n\t%s\n",
					filepath.ToString());
				return ErrorCode::LoadMdlFailed;
			    
			}

			// Read data header
			std::uint32_t numVertices{};
			if (std::fread(&numVertices, sizeof(std::uint32_t), 1, fs.ptr) != 1) {
				std::fprintf(
					stderr,
					"Error: Failed reading numVertices of mdl file:\n\t%s\n",
					filepath.ToString());
				return ErrorCode::LoadMdlFailed;
			}
			std::uint32_t numVertexData{};
			if (std::fread(&numVertexData, sizeof(std::uint32_t), 1, fs.ptr) != 1) {
				std::fprintf(
					stderr,
					"Error: Failed reading numVertexData of mdl file:\n\t%s\n",
					filepath.ToString());
				return ErrorCode::LoadMdlFailed;
			  
			}

			// Read data
			float* vertexData = new float[numVertexData];
			auto numRead = std::fread(&vertexData[0], sizeof(float), numVertexData, fs.ptr);
			if (numRead != numVertexData) {
				std::fprintf(
					stderr,
					"Error: Failed reading vertex data of mdl file:\n\t%s\n",
					filepath.ToString());
				delete[] vertexData;
				return ErrorCode::LoadMdlFailed;
			}

			vertexDataOut.m_data = vertexData;
			vertexDataOut.m_numVertexData = numVertexData;
			vertexDataOut.m_numVertices = numVertices;

		} else {
			std::fprintf(
				stderr,
				"Error: failed to read .mdl header!\n\t%s\n",
				filepath.ToString());
			return ErrorCode::LoadMdlFailed;
		}
	} else {
		std::printf("Could not find %s\n", filepath.ToString());
		return ErrorCode::NoFile;
	}

	return ErrorCode::Ok;
}

ErrorCode Serialize(std::string_view name, const VertexData& data) {
	// File header. 8 bytes:
    //   magic-word (uint32)
    //   version (uint32) 
    // Data header. 8 bytes:
    //   numVertices (uint32)
    //   numVertexData (uint32)
    // Data: 
    //   Vertex data (vertex position, uv, normals) stored as array of floats

    // magic-word MUST be equal to 0x6D6F646C "modl".
	// version number to detect old incompatible files.
  
	std::string path = "../../assets/mesh/";
	path += name;
	path += ".mdl";

	File::FileStream fs(Filepath(path.c_str()), "wb");
	if (fs.IsValid()) {
		std::size_t n = 0;
		n += std::fwrite(&magic, sizeof(std::uint32_t), 1, fs.ptr);               
		n += std::fwrite(&mdlVersion, sizeof(std::uint32_t), 1, fs.ptr);
		n += std::fwrite(&data.m_numVertices, sizeof(std::uint32_t), 1, fs.ptr);
		n += std::fwrite(&data.m_numVertexData, sizeof(std::uint32_t), 1, fs.ptr);
		n += std::fwrite(&data.m_data[0], sizeof(float), data.m_numVertexData, fs.ptr);
		std::size_t expected = 4; // 4 = magic + version + numVerts + numVertData
		expected += data.m_numVertexData;
		if (n != expected) {
			// TODO: Should probably delete the corrupt file
			std::fprintf(
				stderr,
				"Error: Failed to write .mdl file! Did not write expected amount.\n");
			std::fprintf(
				stderr,
				"Wrote '%zu' but expected to write '%zu'\n\t%s\n",
				n, expected, path.c_str());
			return ErrorCode::SaveObjFailed;
		}
	} else {
		std::fprintf(
			stderr,
			"Error: ObjToMdl, failed to open path!\n\t%s",
			path.c_str());
	}
	
	std::printf(
		"Successfully created new .mdl file from vertex data: %s\n",
		path.c_str());
	return ErrorCode::Ok;
}

} // namespace ObjToMdl
} // namespace FG24
