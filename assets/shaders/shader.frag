#version 330 core

out vec4 fragColor;

in vec3 color;
in vec3 vertexPos;
in vec2 uv;

uniform sampler2D tex;

void main() {
	// fragColor = texture(tex, uv) * vec4(color, 1.0);
	// fragColor = vec4(0.0, 1.0, 0.1, 1.0);
fragColor = vec4(0.0, 1.0, 0.0, 1.0);
}