#version 330 core

// Vertex attributes
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec3 in_normal;

// Transformations
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPosition = vec3(0.0, 5.0, 0.0);

out vec2 uv; // Sends to fragment shader
out vec3 v_normal; // Vertex normal
out vec3 vecToCamera; // Vector from vertex to camera position
out vec3 position; // World coordinates of the vertex

void main() {
	// Order of matrix multiplication is important!
	gl_Position =  projection * view * model * vec4(in_position, 1.0);
	position = (model * vec4(in_position, 1)).xyz;

	uv = in_uv;
	v_normal = mat3(transpose(inverse(view))) * in_normal;

	vecToCamera = normalize((cameraPosition.xyz - position.xyz));
}
