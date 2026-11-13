#version 330 core
layout (location = 0) out vec4 UnlitColor;
layout (location = 1) out vec4 FragColor;
layout (location = 2) out vec3 FragPosition;
layout (location = 3) out vec3 FragNormal;
layout (location = 4) out vec4 FragSpecular;
layout (location = 5) out vec4 FragLight;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    UnlitColor = vec4(0.15f, 0.1f, 0.1f, 1.0f);
    FragColor = vec4(0.1f, 0.5f, 0.78f, 1.0f);
	
	if((UnlitColor.a == 0.0f && FragColor.a < 0.5f)) 
	{
		discard;
	}
	FragPosition = FragPos;
	FragNormal = Normal;
	FragSpecular = glm::vec4(1.0f);
}