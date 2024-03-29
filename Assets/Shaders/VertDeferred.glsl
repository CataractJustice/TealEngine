#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 uv;
layout (location = 4) in vec4 color;
layout (location = 5) in vec3 bitangent;

out vec2 TexCoord;
out vec4 Color;
out vec3 FragPos;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;
out mat3 TBN;
uniform mat4 pv_mat;
uniform mat4 model;
uniform mat4 n_model;
uniform bool use_color = false;
void main()
{
	TexCoord = uv;
	Color = use_color ? color : vec4(1.0f);
	FragPos = vec3(model * vec4(position, 1.0));
	Normal = mat3(n_model) * (normal);
	Tangent =  mat3(n_model) * (tangent);
	Bitangent = mat3(n_model) * (bitangent);
	TBN = mat3(Tangent, Bitangent, Normal);
	gl_Position = pv_mat * model * vec4(position, 1.0f);
}