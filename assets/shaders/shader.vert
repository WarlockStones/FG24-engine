#version 330 core

layout (location = 0) in vec3 a_pos; // attribute_pos
layout (location = 1) in vec2 a_uv;  // attribute_uv

// Transformations
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv; // Sends to fragment shader

void main() {
	gl_Position = projection * view * model * vec4(a_pos, 1.0);
	uv = a_uv;
}
