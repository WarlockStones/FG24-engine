#version 330 core

layout (location = 0) in vec3 a_pos; // attribute_pos
layout (location = 1) in vec2 a_uv;  // attribute_uv
layout (location = 2) in vec3 a_normal;  // attribute_normal

// Transformations
 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;

out vec2 uv; // Sends to fragment shader
out vec3 color;
out vec3 vertexPos;
out vec3 normal;

void main() {
	// Order of matrix multiplication is important!
	gl_Position =  projection * view * model * vec4(a_pos, 1.0);

	uv = a_uv;
	color = vec3(1.0, 1.0, 1.0);
	normal = (vec4(a_normal, 1) * model).xyz;
	normal = mat3(transpose(inverse(view))) * a_normal;
	
	// v_normal = normalize(mat3(currentNormalMatrix) * in_Normal).xyz;
}
