#version 330 core

layout (location = 0) in vec3 pos_local;
layout (location = 1) in vec2 uv_local;
layout (location = 2) in vec3 normal_local;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv_world; 

void main() {
	// Order of matrix multiplication is important!
	gl_Position =  projection * view * model * vec4(pos_local, 1.0);
	uv_world = uv_local;
}
