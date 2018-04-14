# version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D secondTex;
uniform sampler2D bumpTex;
uniform sampler2D dudvTex;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex{
	vec3 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent; // New !
	vec3 binormal; // New !
	vec3 worldPos;
	vec3 tempPos;
} IN;

out vec4 gl_FragColor;

const float waveStrength = 10;

void main(void) {
	vec2 distortion1 = (texture(dudvTex, IN.texCoord).rg * 2.0 - 1.0) * waveStrength;
	vec2 newTexCoord = IN.texCoord;
	newTexCoord += distortion1;
	vec4 diffuse = texture (diffuseTex, newTexCoord);
	vec4 second = texture(secondTex, newTexCoord);

	mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
	vec3 normal = normalize(TBN * (texture (bumpTex, IN.texCoord).rgb * 2.0 - 1.0));
	vec3 incident = normalize(lightPos - IN.worldPos);
	float lambert = max(0.0, dot(incident, normal));
	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);

	float rFactor = max(0.0, dot(halfDir, normal));
	float sFactor = pow(rFactor, 50.0);
	if (IN.worldPos.y >= 168.421) {
		if (IN.worldPos.y > 500) {
			//vec3 colour = (second.rgb * lightColour.rgb);
			//colour += (lightColour.rgb * sFactor) * 0.33;
			//gl_FragColor = vec4(colour * atten * lambert, second.a);
			//gl_FragColor.rgb += (second.rgb * lightColour.rgb) * 0.1;
			gl_FragColor = second;
		}
		else {
			float factor = (IN.worldPos.y - 100) / 500;
			vec4 mixcolor = mix(diffuse, second, factor);
			//vec3 colour = (mixcolor.rgb * lightColour.rgb);
			//colour += (lightColour.rgb * sFactor) * 0.33;
			//gl_FragColor = vec4(colour * atten * lambert, diffuse.a);
			//gl_FragColor.rgb += (mixcolor.rgb * lightColour.rgb) * 0.1;
			gl_FragColor = mixcolor;
		}
	}
	else {
		discard;
	}
}