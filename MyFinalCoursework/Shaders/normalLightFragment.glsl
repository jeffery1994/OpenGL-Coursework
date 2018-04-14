# version 150 core

 uniform sampler2D diffuseTex ;

 uniform vec3 cameraPos ;
 uniform vec4 lightColour ;
 uniform vec3 lightPos ;
 uniform float lightRadius ;

 in Vertex {
 vec3 colour ;
 vec2 texCoord ;
 vec3 normal ;
 vec3 worldPos ;
 } IN;

 out vec4 gl_FragColor ;

 float specularStrength = 0.5;

 float ambientStrength = 0.1;

void main ( void ) {
 vec4 diffuse = texture ( diffuseTex , IN. texCoord );

 //光到片段的方向向量
 vec3 incident = normalize ( lightPos - IN. worldPos );

//光和向量的点乘积，获得漫反射系数lambert
 float lambert = max (0.0 , dot ( incident , IN. normal ));

 //光到片段的距离
float dist = length ( lightPos - IN. worldPos );

//光率减根据光的半径和光到片段的距离计算出衰减系数
 float atten = 1.0 - clamp ( dist / lightRadius , 0.0 , 1.0);

 //镜头到片段的方向向量
vec3 viewDir = normalize ( cameraPos - IN. worldPos );

//？
 vec3 halfDir = normalize ( incident + viewDir );

 //反射光线方向向量
 vec3 reflectDir = reflect(-incident, IN.normal);
 
 //镜面反射光系数
 float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

 //float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
 //float sFactor = pow ( rFactor , 50.0 );

 //镜面反射光RGB
//vec3 colour = ( diffuse .rgb * lightColour .rgb );
 //colour += ( lightColour .rgb * sFactor ) * 0.33;

 //镜面反射光RGB
 vec3 specular = specularStrength * spec * lightColour.rgb;

 //漫反射
 vec3 diffuse1 = diffuse.rgb * lambert * lightColour.rgb;
 //gl_FragColor = vec4 ( colour * atten * lambert , diffuse .a);

 //环境光
 vec3 ambient = (diffuse.rgb * lightColour.rgb) * ambientStrength;
 //gl_FragColor .rgb += ( diffuse .rgb * lightColour .rgb ) * ambientStrength;

 vec3 final = (specular + diffuse1)*atten + ambient;
 gl_FragColor = vec4(final, 1.0);
}