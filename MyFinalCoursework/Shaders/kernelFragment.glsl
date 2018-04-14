# version 150 core

uniform sampler2D diffuseTex ;

in Vertex {
	vec2 texCoord ;
} IN;

out vec4 gl_FragColor ;

const float offset = 1.0 / 300.0;  

void main ( void ) {
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );
	
	 float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

	 vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(diffuseTex, IN.texCoord.st + offsets[i]));
    }
	
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
	
	gl_FragColor = vec4(col,1.0);
}