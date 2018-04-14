# version 150 core

uniform sampler2D diffuseTex ;

in Vertex {
	vec2 texCoord ;
} IN;

out vec4 gl_FragColor ;

void main ( void ) {
	gl_FragColor = texture ( diffuseTex , IN. texCoord );
	float average = (gl_FragColor.r + gl_FragColor.g + gl_FragColor.b)/3.0;
	gl_FragColor = vec4(average, average, average, 1.0);
}