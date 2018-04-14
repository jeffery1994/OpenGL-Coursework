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

 //�⵽Ƭ�εķ�������
 vec3 incident = normalize ( lightPos - IN. worldPos );

//��������ĵ�˻������������ϵ��lambert
 float lambert = max (0.0 , dot ( incident , IN. normal ));

 //�⵽Ƭ�εľ���
float dist = length ( lightPos - IN. worldPos );

//���ʼ����ݹ�İ뾶�͹⵽Ƭ�εľ�������˥��ϵ��
 float atten = 1.0 - clamp ( dist / lightRadius , 0.0 , 1.0);

 //��ͷ��Ƭ�εķ�������
vec3 viewDir = normalize ( cameraPos - IN. worldPos );

//��
 vec3 halfDir = normalize ( incident + viewDir );

 //������߷�������
 vec3 reflectDir = reflect(-incident, IN.normal);
 
 //���淴���ϵ��
 float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

 //float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
 //float sFactor = pow ( rFactor , 50.0 );

 //���淴���RGB
//vec3 colour = ( diffuse .rgb * lightColour .rgb );
 //colour += ( lightColour .rgb * sFactor ) * 0.33;

 //���淴���RGB
 vec3 specular = specularStrength * spec * lightColour.rgb;

 //������
 vec3 diffuse1 = diffuse.rgb * lambert * lightColour.rgb;
 //gl_FragColor = vec4 ( colour * atten * lambert , diffuse .a);

 //������
 vec3 ambient = (diffuse.rgb * lightColour.rgb) * ambientStrength;
 //gl_FragColor .rgb += ( diffuse .rgb * lightColour .rgb ) * ambientStrength;

 vec3 final = (specular + diffuse1)*atten + ambient;
 gl_FragColor = vec4(final, 1.0);
}