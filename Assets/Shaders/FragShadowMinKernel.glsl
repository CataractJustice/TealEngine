#version 460 core

in vec2 TexCoord;

uniform sampler2D shadowmap;
uniform float pixelsize;

void main()
{
	gl_FragDepth = 
	min(
		texture(shadowmap, TexCoord).r,
		min(
			texture(shadowmap, TexCoord + vec2(-pixelsize, -pixelsize)).r,
			min(
				texture(shadowmap, TexCoord + vec2(0.0f, -pixelsize)).r,
				min(
					texture(shadowmap, TexCoord + vec2(pixelsize, -pixelsize)).r,
					min(
						texture(shadowmap, TexCoord + vec2(-pixelsize, 0.0f)).r,
						min(
							texture(shadowmap, TexCoord + vec2(pixelsize, 0.0f)).r,
							min(
								texture(shadowmap, TexCoord + vec2(-pixelsize, pixelsize)).r,
								min(
									texture(shadowmap, TexCoord + vec2(0.0f, pixelsize)).r,
									texture(shadowmap, TexCoord + vec2(pixelsize, pixelsize)).r
								)
							)
						)
					)
				)
			)
		)
	);
}