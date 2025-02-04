#version 330 core
#define MAX_LIGHTS 8 // Make sure this matches in C++
// LightType
#define POINT 0
#define SPOT 1
#define DIRECTIONAL 2

uniform sampler2D albedoMap;
uniform sampler2D specularMap;
uniform sampler2D shadowMap;

uniform vec2 shadowMapTexelSize = vec2(1024, 1024);
uniform float shadowCalcBias = 0.0005;

uniform vec3 cameraPosition = vec3(0.0, 5.0, 0.0);

uniform vec4 PrimaryColorVec4 = vec4(1.0, 1.0, 1.0, 1.0); // Used for debugging
uniform vec4 lightAmbient = vec4(1, 1, 1, 1); // Base light regardless of pos
uniform vec4 lightDiffuse[MAX_LIGHTS]; // Diffuse color
uniform vec4 lightSpecular[MAX_LIGHTS]; // Specular highlight color
uniform vec3 lightPosition[MAX_LIGHTS]; 
// Constant, linear, quadratic - light fading by distance
uniform vec3 lightAttenuation[MAX_LIGHTS];
uniform vec3 lightRotation[MAX_LIGHTS];
uniform float lightCutoff[MAX_LIGHTS]; // used for spotlights
uniform int lightType[MAX_LIGHTS]; // 0 = point, 1 = spot, 2 = directional
uniform int activeLights = 0;


uniform vec4 materialAmbient = vec4(1,1,1,1);
uniform vec4 materialDiffuse = vec4(1,1,1,1);
uniform vec4 materialSpecular = vec4(1,1,1,1);
uniform int materialShininess = 8;

in vec2 uv_world;
in vec3 normal_world;
in vec3 pos_world; // World coordinates of the vertex

in vec4 shadowCoord;
out vec4 fragColor;

float CalculateShadowFactor() {
	// float shad = texture(shadowMap, shadowCoord.xy).x; // hmm worked ok
/*
	vec4 shadowSpace = shadowMapMatrix * vec4(pos_world, 1); // World space? Try local
	float shadowSample = texture(shadowMap, shadowSpace.xy).r;
	if (shadowSample < 0) {
		shadowSample = 0;
	}

	return shadowSample;
*/
  return 0;
}

vec3 CalculateSpecular(vec3 lightDirection, vec3 normalizedNormal, int index) {
	// Calculate brightness
	vec3 vertPositionToCamera = normalize((cameraPosition.xyz - pos_world.xyz));
	vec3 vectorToCamera = normalize(vertPositionToCamera);
	vec3 halfVector = normalize((lightDirection + vectorToCamera) / 2);
	float initialBrightness = max(dot(halfVector, normalizedNormal), 0.0);
	
	float totalBrightness = initialBrightness;
	for (int i = 0; i < materialShininess; ++i) {
		totalBrightness *= initialBrightness;
	}

	return totalBrightness * lightSpecular[index].xyz * materialSpecular.xyz;
}

float CalculateAttenuation(int index) {
		float distanceToLight = length(lightPosition[index].xyz - pos_world.xyz);
		return 
			1 / (
				lightAttenuation[index].x +
				lightAttenuation[index].y *
				distanceToLight +
				lightAttenuation[index].z * 
				pow(distanceToLight, 2));
}


void main() {
	// Cumulative luminosity calculated from every light combined
	vec3 diffuseLuminosity = vec3(0);
	vec3 specularLuminosity = vec3(0);

	vec4 albedoTexel = texture(albedoMap, uv_world);
	// Set initial color to get correct alpha/transparency
	fragColor = vec4(0,0,0, albedoTexel.w); 

	vec4 specularTexel = texture(specularMap, uv_world); // Should be vec3

	// Add from each light
	for (int i = 0; i < activeLights; ++i) {
		vec3 diffuseToAdd = vec3(0,0,0);
		vec3 specularToAdd = vec3(0,0,0);

		vec3 lightDirection = normalize(lightPosition[i].xyz - pos_world.xyz);
		vec3 normalizedNormal = normalize(vec3(normal_world.xyz));

		if (lightType[i] == POINT) { 
			// Intensify light by how aligned the normal is to the direction to the light
			float difIntensity = max(dot(normalizedNormal, lightDirection), 0.0);

			// Do diffuse and specular only if on correct side to be lit
			if (difIntensity > 0) {
				vec3 diffusePart = difIntensity * lightDiffuse[i].xyz * materialDiffuse.xyz;

				float attenuation = CalculateAttenuation(i);
				diffuseToAdd += diffusePart * attenuation; 

				specularToAdd +=
					CalculateSpecular(lightDirection, normalizedNormal, i) *
					attenuation * specularTexel.xyz;
			}
			diffuseLuminosity += diffuseToAdd;
			specularLuminosity += specularToAdd;

		} else if (lightType[i] == SPOT) {
			float spotDotCutoff = dot(normalize(-lightRotation[i]), lightDirection);

			// Only light if within the cutoff
			if (spotDotCutoff > lightCutoff[i]) {

				// Do light calculations just like point light
				float difIntensity = max(dot(normalizedNormal, lightDirection), 0.0);

				if (difIntensity > 0) {
					vec3 diffusePart = difIntensity * lightDiffuse[i].xyz * materialDiffuse.xyz;

					float attenuation = CalculateAttenuation(i);
					diffuseToAdd += diffusePart * attenuation; 

					specularToAdd.xyz += 
						CalculateSpecular(lightDirection, normalizedNormal, i) *
						attenuation;

				}
				diffuseLuminosity += diffuseToAdd;
				specularLuminosity += specularToAdd;
			}

		} else if (lightType[i] == DIRECTIONAL) {
			// TODO: use light rotation instead of position for directional light
			float difIntensity = max(dot(normalizedNormal, lightDirection), 0.0);

			if (difIntensity > 0) {
				// Directional light omits attenuation
				diffuseToAdd = difIntensity * lightDiffuse[i].xyz * materialDiffuse.xyz;
				specularToAdd.xyz += CalculateSpecular(lightDirection, normalizedNormal, i);
			}

			float shadowFactor = CalculateShadowFactor();
			if (shadowFactor > 0) {
				diffuseToAdd.xyz *= 1 - shadowFactor;
				specularToAdd.xyz *= 1 - shadowFactor;
			}

			diffuseLuminosity += diffuseToAdd;
			specularLuminosity += specularToAdd;
		}
	} // End of for each light loop

	// Add ambient
	fragColor.xyz += albedoTexel.xyz * materialAmbient.xyz * lightAmbient.xyz;
	// Add diffuse
	fragColor.xyz += albedoTexel.xyz * diffuseLuminosity.xyz;
	// Add specular
	fragColor.xyz += specularLuminosity.xyz;

	// Debugging shadow
	// WORKS!
	float shad = texture(shadowMap, shadowCoord.xy).x; // hmm worked ok
	fragColor *= shad;


	// fragColor.xyz = vec3(vis, vis, vis);

	// fragColor.xyz = diffuseLuminosity + specularLuminosity;

	// Debug color override
	// fragColor *= primaryColorVec4;
	// fragColor = vec4(normal_world, 0);
}
