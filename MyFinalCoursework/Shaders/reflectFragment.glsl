#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D dudvMap;
uniform samplerCube cubeTex;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float lightRadius;

in Vertex{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
	vec4 clipSpace;
} IN;

out vec4 gl_FragColor;

const float waveStrength = 0.02;

void main(void) {
	vec2 distortion1 = (texture(dudvMap, IN.texCoord).rg * 2.0 - 1.0) * waveStrength;
	vec2 coord = IN.texCoord;
	coord += distortion1;
	vec4 diffuse = texture (diffuseTex, coord);
	//diffuse.a = 0.3;
	vec3 incident = normalize(IN.worldPos - cameraPos);
	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.2, 1.0);
	vec3 temp = reflect(incident, normalize(IN.normal));

	temp.xy += distortion1;
	temp.z += distortion1.y;

	vec4 reflection = texture (cubeTex, temp);
	//vec4 reflection = texture (cubeTex, reflect(incident, normalize(IN.normal)));
	//gl_FragColor = ( lightColour * diffuse * atten )*( diffuse + reflection );
	//reflection.b += 0.1;
	//gl_FragColor =	reflection *0.7 + (lightColour*atten)*diffuse*0.6;
	gl_FragColor = reflection;
	//gl_FragColor.b += 0.1;
	gl_FragColor.a = 0.75;
	gl_FragColor.b += 0.1;
}

