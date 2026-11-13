/*#include <GL/glew.h>
#include <iostream>
#include "Renderer.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "FileSystem/loadStrFromFile.h"
#include "Graphics/Graphics.h"
#include <chrono>
#include <thread>
#include "Graphics/Texture.h"
#include "VertexArrayObject.h"
int main() 
{
	TealEngine::Graphics::init("Kirino Renderer");
	Kirino::Shader vertShader(loadStrFromFile("./Kirino/TestShaders/screen.vert").c_str(), GL_VERTEX_SHADER);
	Kirino::Shader fragShader(loadStrFromFile("./Kirino/TestShaders/red.frag").c_str(), GL_FRAGMENT_SHADER);

	Kirino::ShaderProgram shader(vertShader.getId(), fragShader.getId());
	Kirino::Material material(&shader);
	Kirino::Mesh mesh;
	Kirino::Mesh mesh2;
	mesh.indexCount = 3;
	mesh.indexOffset = 0;
	mesh.vertexOffset = 0;
	unsigned int ebo;
	unsigned int vbo;
	float verts[] = {0.05f,  0.05f, 0.0f, 0.05f, -0.05f, 0.0f, -0.05f, -0.05f, 0.0f, -0.05f,  0.05f, 0.0f};
	unsigned int inds[] = {0, 1, 3, 1, 2, 3};
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	mesh.vao->vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0, 0));
	mesh.vao->updateAttribs();
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	Kirino::Renderer renderer;

	TealEngine::Texture texture;
	texture.loadFromFile("./res/textures/Wall.png");

	glViewport(0, 0, 640, 480);
	material.useProgram();
	mesh2.vao = mesh.vao;
	mesh2.indexCount = 3;
	mesh2.indexOffset = 3;
	mesh2.vertexOffset = 0;
	Kirino::MeshInstance meshInstance;
	Kirino::MeshInstance meshInstance2;
	meshInstance.baseInstance = 0;
	meshInstance.material = &material;
	meshInstance.mesh = &mesh;
	meshInstance.mode = GL_TRIANGLES;
	meshInstance2.baseInstance = 0;
	meshInstance2.material = &material;
	meshInstance2.mesh = &mesh2;
	meshInstance2.mode = GL_TRIANGLES;
	auto error = glGetError();
	if(error != GL_NO_ERROR) 
	{
		std::cout << " code: " << error << "\n";
		throw;
	}
	float t = 0.0f;
	int tid = texture.id();
	material.setTexture("tex", &tid, 1, 0);
	while(!TealEngine::Graphics::window->shouldClose())
	{
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		material.setUniform("time", &t, sizeof(t), 0);
		t+=0.001f;
		renderer.render();
		renderer.emplace(meshInstance);
		renderer.emplace(meshInstance2);
		TealEngine::Graphics::display();
		auto error = glGetError();
		if(error != GL_NO_ERROR) 
		{
			std::cout << " code: " << error << "\n";
			throw;
		}
	}
}*/