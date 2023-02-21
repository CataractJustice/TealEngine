#version 330 core

layout (location = 0) out vec4 UnlitColor;
layout (location = 1) out vec4 FragColor;
layout (location = 2) out vec3 FragPosition;
layout (location = 3) out vec3 FragNormal;
layout (location = 4) out vec4 FragSpecular;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;
in vec4 Color;

uniform sampler2D unlit;
uniform sampler2D texture;
uniform sampler2D specular;
uniform sampler2D normals;

uniform vec4 textureColor = vec4(1.0f);
uniform vec4 unlitColor = vec4(1.0f);
uniform vec3 specularColor = vec3(1.0f);

void main()
{
    UnlitColor = texture(unlit, TexCoord) * Color * unlitColor;
    FragColor = texture(texture, TexCoord) * Color * textureColor;
	FragPosition = FragPos;
	vec3 ln = texture(normals, TexCoord).rgb * 2.0 - 1.0;
	FragNormal = normalize(TBN * ln);
	vec4 specvec = texture(specular, TexCoord);
	specvec.x *= specularColor.x;
	specvec.y *= specularColor.y;
	specvec.z *= specularColor.z;
	FragSpecular = specvec;
}