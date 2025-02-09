#version 330 core

layout (location = 0) in vec3 pos_local;
layout (location = 1) in vec2 uv_local;
layout (location = 2) in vec3 normal_local;
layout (location = 3) in vec3 pos_local2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float blendAmount = 0;

out vec2 uv_world; 

void main() {
	// Order of matrix multiplication is important!
    vec3 pos = pos_local + blendAmount * pos_local2;
	gl_Position =  projection * view * model * vec4(pos_local, 1.0);
	uv_world = uv_local;
}
