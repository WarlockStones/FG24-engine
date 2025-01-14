#version 330 core

uniform sampler2D tex;

uniform vec4 PrimaryColorVec4 = vec4(1.0, 1.0, 1.0, 1.0); // Used for debugging
uniform vec4 lightAmbient = vec4(0.2, 0.2, 0.2, 1); // Base light regardless of pos
uniform vec4 lightDiffuse = vec4(1, 0.9, 0.8, 1); // Diffuse color
// uniform vec4 lightSpecular = vec4(0.9, 0.8, 0.7, 1); // Specular highlight color
uniform vec4 lightSpecular = vec4(1.0, 0.0, 1.0, 1); // Odd specular color
uniform vec3 lightPosition = vec3(10, 25, 15); // World position of the light
// Constant, linear, quadratic - light fading by distance
uniform vec3 lightAttenuation = vec3(1, 0.1, 0.001);

uniform vec4 materialAmbient = vec4(1,1,1,1);
uniform vec4 materialDiffuse = vec4(1,1,1,1);
uniform vec4 materialSpecular = vec4(1,1,1,1);
uniform int materialShininess = 8;

in vec2 uv;
in vec3 v_normal; // Vertex normal
in vec3 vecToCamera; // Vector from vertex to eye
in vec3 position; // World coordinates of the vertex

out vec4 fragColor;

void main() {
	// fragColor = texture(tex, uv) * vec4(color, 1.0);
	// fragColor.xyz = normal; // Debug draw normal
    vec4 texel = vec4(0.0, 1.0, 0.1, 1.0); // Placeholder color for textures
	
	fragColor = vec4(0,0,0,texel.w); // Initialize color at 0 lighting
	fragColor.xyz += vec3(materialAmbient * lightAmbient); // Ambient light first
	vec3 lightDirection = normalize(position.xyz - lightPosition.xyz);

	// Interpolated mesh normal may not be normalized, fix it
	vec3 normalizedNormal = normalize(vec3(v_normal.xyz));
	
	float difIntensity = max(dot(lightDirection, normalizedNormal), 0.0);
	
	// Do diffuse and specular only if on the right side
	if (difIntensity > 0) {
		vec3 diffusePart = difIntensity * lightDiffuse.xyz * materialDiffuse.xyz;
		float distance = length(lightPosition.xyz - position.xyz);

		float attenuation = 
			1 / (
				lightAttenuation.x +
				lightAttenuation.y *
				distance +
				lightAttenuation.z * 
				pow(distance, 2));

		// Add diffuse
		fragColor.xyz += diffusePart * attenuation; 
		// Calculate specular
		vec3 vectorToCamera = normalize(vecToCamera);
		vec3 halfVector = normalize((-lightDirection + vectorToCamera) / 2);
		float initialBrightness = max(dot(halfVector, normalizedNormal), 0.0);
		
		float totalBrightness = initialBrightness;
		for (int i = 0; i < materialShininess; ++i) {
			totalBrightness *= initialBrightness;
		}

		// Add specular
		fragColor.xyz += 
		  totalBrightness *
		  lightSpecular.xyz *
		  attenuation *
		  materialSpecular.xyz;
	}

	// Override color if you want to debug specific entity or property
	// fragColor *= primaryColorVec4;
}
