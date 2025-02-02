#version 330 core

layout (location = 1) out vec3 screenSpacePos;

void main() {
  screenSpacePos = gl_FragCoord.xyz;
}
