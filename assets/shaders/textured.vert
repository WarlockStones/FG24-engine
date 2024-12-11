#version 330 core

layout (location = 0) in vec3 a_pos; // attribute_pos
layout (location = 1) in vec2 a_uv;  // attribute_uv

// Transformations
 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;

out vec2 uv; // Sends to fragment shader
out vec3 color;
out vec3 vertexPos;

void main() {
	// Order of matrix multiplication is important!
	gl_Position =  projection * view * model * vec4(a_pos, 1.0);

	uv = a_uv;
	color = vec3(1.0, 1.0, 1.0);
}
