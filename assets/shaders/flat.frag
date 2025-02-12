#version 330 core

uniform sampler2D albedoMap;
uniform vec3 color = vec3(0,0,0);

in vec2 uv_world;

out vec4 fragColor;
void main() {
	vec4 albedoTexel = texture(albedoMap, uv_world);
	fragColor = albedoTexel + vec4(color, 1);
}
