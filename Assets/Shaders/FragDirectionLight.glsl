#version 330 core

#define MAX_CASCADES 8
#define cascade_inner_bias 0.01f

in vec2 TexCoord;

layout (location = 5) out vec4 LightColor;

//deferred render textures
//uniform sampler2D RenderTexture;
uniform sampler2D AlbedoMap;
uniform sampler2D PositionMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;

uniform sampler2D ShadowMap[MAX_CASCADES];
uniform mat4 lightModel[MAX_CASCADES];

uniform int resolution;
uniform int cascades;
uniform vec3 direction;
uniform vec3 viewPos;
uniform vec4 color;

#define SSSfactor 0.60

float lightCalc(vec3 position, vec3 normal)
{
	for(int k = 0; k < cascades; k++)
	{
		vec4 fPos = lightModel[k] * vec4(position - direction * 0.01f, 1.0);
		vec3 proj = fPos.xyz / fPos.w;
	
		if(proj.x < -1.0f + cascade_inner_bias || proj.x > 1.0f - cascade_inner_bias || proj.y < -1.0f + cascade_inner_bias || proj.y > 1.0f - cascade_inner_bias || proj.z < -1.0f + cascade_inner_bias || proj.z > 1.0f - cascade_inner_bias)
			continue;
		else
		{
			float near = (1.0f + lightModel[k][2][3]) / (lightModel[k][2][2]);
			float far = -(1.0f - lightModel[k][2][3]) / (lightModel[k][2][2]);
			proj = proj * 0.5 + 0.5;
			return step(0.0, (proj.z - texture(ShadowMap[k], proj.xy).r) * (far - near));
		}
	}
	return 0.0f;
}

void main()
{
	vec3 normal = texture(NormalMap, TexCoord).xyz;
	vec3 position = vec3(texture(PositionMap, TexCoord));
	vec3 viewReflection = normalize(-reflect(position - viewPos, normal));
	vec4 spectex = texture(SpecularMap, TexCoord);
	float spec = pow(max(0.0f,dot(viewReflection, direction)) * spectex.z, spectex.y) * spectex.x;
	float diffuse = max(0.0f,dot(-normal, direction));
	float light = lightCalc(position, normal);
	LightColor = vec4(color.xyz*color.w, 1.0) * mix((spec/(1.0f+diffuse) + diffuse) * ((light > 0.0f) ? (1.0f) : (0.0f)), max(0.0f, min(0.0f, light)+1.0f), SSSfactor);
}