#version 330 core

// The vertices that are to be rendered. Solid verticies that are to cast shadows
layout (location = 0) in vec3 in_Position;

uniform mat4 depthMvp;

void main() {
	gl_Position = depthMvp * vec4(in_Position, 1);
}
