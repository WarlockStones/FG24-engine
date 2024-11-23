#pragma once
#include <cstdint>

namespace FG24 {

class Shader {
public:
	Shader() = default;
	~Shader() = default;
	bool CompileShader(const char* vertPath, const char* fragPath);
	std::uint32_t GetShaderProgram() const;

	std::uint32_t program{};
private:
};

}
