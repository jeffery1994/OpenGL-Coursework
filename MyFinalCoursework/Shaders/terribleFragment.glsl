# version 150 core

uniform sampler2D diffuseTex;

in vec4 clipSpace;

out vec4 gl_FragColor;

void main(void) {
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
	vec2 reflectionTexCoord = vec2(ndc.x, -ndc.y);
	vec4 reflectionColour = texture(diffuseTex, reflectionTexCoord);
	gl_FragColor = reflectionColour;
}

