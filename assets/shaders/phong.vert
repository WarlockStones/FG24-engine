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

// TODO: Decide on naming scheme...
// Differenciate on world and object space. pos and normal

void main() {
	// Order of matrix multiplication is important!
	gl_Position =  projection * view * model * vec4(pos_local, 1.0);
	pos_world = (model * vec4(pos_local, 1)).xyz;

	uv_world = uv_local;

	// Create a normal model matrix
	mat3 normal_model = mat3(transpose(inverse(model))); // TODO: Do this on the CPU once and send as uniform
	normal_world = normal_model * normal_local;
}
