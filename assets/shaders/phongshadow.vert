#version 330 core

// Vertex attributes
layout (location = 0) in vec3 pos_local;
layout (location = 1) in vec2 uv_local;
layout (location = 2) in vec3 normal_local;

// Transformations
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv_world; 
out vec3 normal_world; 
out vec3 pos_world;

out vec4 shadowCoord;
uniform mat4 shadowMapMatrix;

void main() {
	// Order of matrix multiplication is important!
	gl_Position =  projection * view * model * vec4(pos_local, 1.0);
	pos_world = (model * vec4(pos_local, 1)).xyz;

	uv_world = uv_local;

	// Create a normal model matrix
	// TODO: Do this on the CPU once and send as uniform
	mat3 normal_model = mat3(transpose(inverse(model))); 
	normal_world = normal_model * normal_local;

	shadowCoord = shadowMapMatrix * vec4(pos_local, 1.0);
}
