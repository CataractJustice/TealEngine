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
uniform sampler2D sideTextureImage;
uniform sampler2D topTextureImage;
uniform sampler2D specular;
uniform sampler2D sideNormals;
uniform sampler2D topNormals;

uniform float texScale;
uniform vec2 atlasScale;
uniform vec4 textureColor = vec4(1.0f);
uniform vec4 unlitColor = vec4(1.0f);

void main()
{
	float w0 = pow(abs(dot(Normal, vec3(1.0f, 0.0f, 0.0f))),2.0f);
	float w1 = pow(abs(dot(Normal, vec3(0.0f, 1.0f, 0.0f))),2.0f);
	float w2 = pow(abs(dot(Normal, vec3(0.0f, 0.0f, 1.0f))),2.0f);

	vec2 UVx = (fract(FragPos.zy / texScale) + TexCoord) / atlasScale;
	vec2 UVy = (fract(FragPos.xz / texScale) + TexCoord) / atlasScale;
	vec2 UVz = (fract(FragPos.xy / texScale) + TexCoord) / atlasScale;
	
	vec3 n0 = texture(sideNormals, UVx).xyz * 2.0f - 1.0f;
	vec3 n1 = texture(sideNormals, UVy).xyz * 2.0f - 1.0f;
	vec3 n1top = texture(topNormals, UVy).xyz * 2.0f - 1.0f;
	vec3 n2 = texture(sideNormals, UVz).xyz * 2.0f - 1.0f;

	n0 = vec3(
    n0.xy + Normal.zy,
    abs(n0.z) * Normal.x
    );

	n1 = vec3(
    n1.xy + Normal.xz,
    abs(n1.z) * Normal.y
    );

	n1top = vec3(
    n1top.xy + Normal.xz,
    abs(n1top.z) * Normal.y
    );

	n2 = vec3(
    n2.xy + Normal.xy,
    abs(n2.z) * Normal.z
    );

	
    FragColor = (
	texture(sideTextureImage, UVx) * w0 + 
	(Normal.y > 0.0f ? texture(topTextureImage, UVy) * w1 : texture(sideTextureImage, UVy) * w1) + 
	texture(sideTextureImage, UVz) * w2
	) * textureColor * Color;

    UnlitColor = (
	texture(unlit, UVx) * w0 + 
	texture(unlit, UVy) * w1 + 
	texture(unlit, UVz) * w2
	) * textureColor * UnlitColor;

	if((UnlitColor.a + FragColor.a) == 0.0) discard;
	UnlitColor.a = 1.0f;
	
	FragPosition = FragPos;

	FragNormal = normalize(
    n0.zyx * w0 +
    (Normal.y < 0.0f ? (n1.xzy) : (n1top.xzy)) * w1 +
    n2.xyz * w2
    );
	
	FragSpecular = texture(specular, vec2(0.0f));
}