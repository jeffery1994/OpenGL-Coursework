# version 150 core

uniform sampler2D diffuseTex;

in Vertex{
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void) {
	//gl_FragColor = texture (diffuseTex, IN.texCoord);
	gl_FragColor = vec4(vec3(1.0 - texture(diffuseTex, IN.texCoord)), 1.0);
}