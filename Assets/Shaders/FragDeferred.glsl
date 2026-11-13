#version 330 core
layout (location = 0) out vec4 UnlitColor;
layout (location = 1) out vec4 FragColor;
layout (location = 2) out vec3 FragPosition;
layout (location = 3) out vec3 FragNormal;
layout (location = 4) out vec4 FragSpecular;
layout (location = 5) out vec4 FragLight;

in vec2 TexCoord;
in vec3 FragPos;
in mat3 TBN;

/*layout (std140) uniform Deferred
{
};*/

uniform vec4 textureColor;
uniform vec4 unlitColor;
uniform vec3 specularColor;
uniform sampler2D unlit;
uniform sampler2D textureImage;
uniform sampler2D specular;
uniform sampler2D normals;

void main()
{
    UnlitColor = texture(unlit, TexCoord) * unlitColor;
    FragColor = texture(textureImage, TexCoord) * textureColor;
	if((UnlitColor.a == 0.0f && FragColor.a < 0.5f)) 
	{
		discard;
	}
	FragPosition = FragPos;
	UnlitColor.a = 1.0f;
	FragColor.a = 1.0f;
	vec3 ln = texture(normals, TexCoord).rgb * 2.0 - 1.0;
	ln.g = -ln.g;
	FragNormal = normalize(TBN * ln);
	vec4 specvec = texture(specular, TexCoord);
	specvec.x *= specularColor.x;
	specvec.y *= specularColor.y;
	specvec.z *= specularColor.z;
	FragSpecular = specvec;
}