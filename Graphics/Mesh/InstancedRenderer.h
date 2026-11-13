#include "GameNode/Component.h"
#include <map>
#include "Kirino/Mesh.h"
#include "Kirino/Material.h"
/*
namespace TealEngine
{
	class Mesh3DInstance;
	
	class Mesh3DInstancesRenderer 
	{
		public:
		struct InstanceData 
		{
			glm::mat4 model;
			glm::mat3 normalModel;
		};
		Mesh3DInstancesRenderer();
		std::vector<InstanceData> matrixData;
		std::vector<Mesh3DInstance*> instances;
		unsigned int addInstance(Mesh3DInstance* instance);
		void removeInstance(unsigned int id);
		void updateMatricesForInstance(unsigned int id);
		void render();
		unsigned int matrixGLBuffer;
	};
	class Mesh3DInstance : public Component
	{
		private:
		unsigned int instanceId;
		MeshInstancesRenderer* currentRenderer;
		static std::map<SceneNode* scene, std::map<unsigned long, MeshInstancesRenderer>> renderers;
		Kirino::Material* material;
		Kirino::Mesh* mesh;
		void enableRender();
		void disableRender();
		void restartRender();
		public:
		Mesh3DInstance();
		~Mesh3DInstance();
		static void renderAll(SceneNode* scene);
	};
} // namespace TealEngine
*/