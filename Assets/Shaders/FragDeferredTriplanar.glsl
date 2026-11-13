#version 330 core

layout (location = 0) out vec4 UnlitColor;
layout (location = 1) out vec4 FragColor;
layout (location = 2) out vec3 FragPosition;
layout (location = 3) out vec3 FragNormal;
layout (location = 4) out vec4 FragSpecular;
layout (location = 5) out vec4 FragLight;

in vec3 TexOffset;
in vec3 TexWeight;
in vec3 FragPos;
in vec4 ClipPos;
in vec3 Normal;

uniform sampler2D textureImage;

uniform float texScale;
uniform vec2 atlasScale;
uniform vec4 spec = vec4(1.0f);

vec4 getTextureValue(vec3 w, float materialOffset, vec2 textureOffset, vec2 topTextureOffset) 
{
	vec2 UVx = (fract(FragPos.zy / texScale) + materialOffset) / atlasScale;
	vec2 UVy = (fract(FragPos.xz / texScale) + materialOffset) / atlasScale;
	vec2 UVz = (fract(FragPos.xy / texScale) + materialOffset) / atlasScale;
	
	vec4 t0 = texture(textureImage, UVx + textureOffset);
	vec4 t1 = texture(textureImage, UVy + textureOffset);
	vec4 t1top = texture(textureImage, UVy + topTextureOffset);
	vec4 t2 = texture(textureImage, UVz + textureOffset);

	return t0 * w.x + ((Normal.y > 0.0f) ? (t1top * w.y) : (t1 * w.y)) + t2 * w.z; 
}

vec3 getNormalValue(vec3 w, float materialOffset, vec2 textureOffset, vec2 topTextureOffset) 
{
	vec2 UVx = (fract(FragPos.zy / texScale) + materialOffset) / atlasScale;
	vec2 UVy = (fract(FragPos.xz / texScale) + materialOffset) / atlasScale;
	vec2 UVz = (fract(FragPos.xy / texScale) + materialOffset) / atlasScale;
	
	vec3 n0 = texture(textureImage, UVx + textureOffset).xyz;
	vec3 n1 = texture(textureImage, UVy + textureOffset).xyz;
	vec3 n1top = texture(textureImage, UVy + topTextureOffset).xyz;
	vec3 n2 = texture(textureImage, UVz + textureOffset).xyz;

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

	return normalize(
		n0.zyx * w.x +
		(Normal.y < 0.0f ? (n1.xzy) : (n1top.xzy)) * w.y +
		n2.xyz * w.z
	);
}

void main()
{
	vec2 texOffset = vec2(0.0f, 2.0f / atlasScale.y);
	float w0 = abs(Normal.x * Normal.x * Normal.x);
	float w1 = abs(Normal.y * Normal.y * Normal.y);
	float w2 = abs(Normal.z * Normal.z * Normal.z);
	vec3 w = vec3(w0, w1, w2);

	FragColor = 
	getTextureValue(w, TexOffset.x, texOffset * 0.0f, texOffset) * TexWeight.x + 
	getTextureValue(w, TexOffset.y, texOffset * 0.0f, texOffset) * TexWeight.y + 
	getTextureValue(w, TexOffset.z, texOffset * 0.0f, texOffset) * TexWeight.z;
	UnlitColor = 
	getTextureValue(w, TexOffset.x, texOffset * 4.0f, texOffset * 4.0f) * TexWeight.x + 
	getTextureValue(w, TexOffset.y, texOffset * 4.0f, texOffset * 4.0f) * TexWeight.y + 
	getTextureValue(w, TexOffset.z, texOffset * 4.0f, texOffset * 4.0f) * TexWeight.z;
	FragNormal = 
	getNormalValue(w, TexOffset.x, texOffset * 2.0f, texOffset * 3.0f) * TexWeight.x + 
	getNormalValue(w, TexOffset.y, texOffset * 2.0f, texOffset * 3.0f) * TexWeight.y + 
	getNormalValue(w, TexOffset.z, texOffset * 2.0f, texOffset * 3.0f) * TexWeight.z;
	FragSpecular = 
	(getTextureValue(w, TexOffset.x, texOffset * 5.0f, texOffset * 5.0f) * TexWeight.x + 
	getTextureValue(w, TexOffset.y,  texOffset * 5.0f, texOffset * 5.0f) * TexWeight.y + 
	getTextureValue(w, TexOffset.z,  texOffset * 5.0f, texOffset * 5.0f) * TexWeight.z) * spec;
	FragPosition = FragPos;
}