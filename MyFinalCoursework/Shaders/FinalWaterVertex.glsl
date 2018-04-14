#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform float waterHeight;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec2 texCoord;
in vec3 tangent;

out Vertex{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
	vec3 tempPos;
} OUT;

void main(void) {
	OUT.colour = colour;
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.tangent = normalize(normalMatrix * normalize(tangent));
	OUT.binormal = normalize(normalMatrix * normalize(cross(normal, tangent)));
	OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;
	OUT.tempPos = OUT.worldPos;
	float distanceToWater = OUT.worldPos.y - waterHeight;
	OUT.tempPos.y -= 2.0 * distanceToWater;
	//vec4 temp_position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
	vec4 temp_position = (projMatrix * viewMatrix) * vec4(OUT.tempPos, 1.0);
	gl_Position = temp_position;
}