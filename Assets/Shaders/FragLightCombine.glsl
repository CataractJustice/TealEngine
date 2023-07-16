#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D AlbedoMap;
uniform sampler2D LightMap;
uniform sampler2D Dither;
#define DitherResolution (256.0f)
#define DitherStrength (1.0f / 240.0f)

void main()
{
	vec3 color = texture(AlbedoMap, TexCoord).xyz * (texture(LightMap, TexCoord).xyz + ((texture(Dither, gl_FragCoord.xy / DitherResolution) * 2.0f - 1.0f).xyz * DitherStrength));
	//color = color / (color + vec3(1.0, 0.949, 0.949));
	//color = pow(color, vec3(1.0/2.2)); 
	FragColor = vec4(color, 1.0f);
}