#version 460 core

#define MAX_CASCADES 8
#define cascade_inner_bias 0.01f

in vec2 TexCoord;

layout (location = 5) out vec4 LightColor;

uniform sampler2D PositionMap;
uniform sampler2D NormalMap;
uniform sampler2D TransparentPositionMap;
uniform sampler2D TransparentNormalMap;
uniform sampler2D SpecularMap;

uniform sampler2D ShadowMap[MAX_CASCADES];
uniform mat4 lightModel[MAX_CASCADES];

uniform int resolution;
uniform int cascades;
uniform vec3 direction;
uniform vec3 viewPos;
uniform vec4 color;
uniform vec4 indirectColor;

float lightCalc(vec3 position)
{
	for(int k = 0; k < cascades; k++)
	{
		vec4 fPos = lightModel[k] * vec4(position, 1.0);
		vec3 proj = fPos.xyz / fPos.w;
	
		if(proj.x < -1.0f + cascade_inner_bias || proj.x > 1.0f - cascade_inner_bias || proj.y < -1.0f + cascade_inner_bias || proj.y > 1.0f - cascade_inner_bias || proj.z < -1.0f + cascade_inner_bias || proj.z > 1.0f - cascade_inner_bias)
			continue;
		else
		{
			proj = proj * 0.5 + 0.5;
			return texture(ShadowMap[k], proj.xy).r > (proj.z - 0.0001f) ? 1.0f : 0.0f;
		}
	}
	return 1.0f;
}

void main()
{
	vec3 normal = texture(NormalMap, TexCoord).xyz;
	vec3 position = vec3(texture(PositionMap, TexCoord));
	vec4 transparentPositionAlpha = texture(TransparentPositionMap, TexCoord);
	vec3 viewReflection = normalize(-reflect(position - viewPos, normal));
	vec4 spectex = texture(SpecularMap, TexCoord);
	float spec = pow(max(0.0f,dot(viewReflection, direction)) * spectex.z, spectex.y) * spectex.x;
	float alignment = dot(-normal, direction);
	float diffuse = max(0.0f,alignment);
	float light = lightCalc(position);
	float indirectIntensity = alignment * 0.5f + 0.5f;
	if(transparentPositionAlpha.w > 0.0f) 
	{
		transparentPositionAlpha.w = clamp(transparentPositionAlpha.w, 0.0f, 1.0f);
		vec3 transparentNormal = texture(TransparentNormalMap, TexCoord).xyz;
		float tAlignment = dot(-transparentNormal, direction);
		float tDiffuse = max(0.0f,tAlignment);
		float transparentLight = lightCalc(transparentPositionAlpha.xyz);
		spec = mix(spec, 0.0f, transparentPositionAlpha.w);
		diffuse = mix(diffuse, tDiffuse, transparentPositionAlpha.w);
		light = mix(light, transparentLight, transparentPositionAlpha.w);
		indirectIntensity = mix(indirectIntensity, tAlignment * 0.5f + 0.5f, transparentPositionAlpha.w);
	}
	LightColor = vec4(color.xyz*color.w, 1.0) * (spec/(1.0f+diffuse) + diffuse) * light + indirectColor * indirectIntensity;
}