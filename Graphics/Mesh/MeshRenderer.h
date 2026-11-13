#pragma once
#include "GameNode/Component.h"
#include "Kirino/Mesh.h"
#include "Kirino/Material.h"
#include "Kirino/MeshInstance.h"
#include "Kirino/Renderer.h"
#include <vector>
#include "glm/glm.hpp"
#include "Graphics/Camera.h"
#include <set>
#include <map>

namespace TealEngine 
{
	struct TransformBlock 
	{
		glm::mat4 model;
		glm::mat4 normalModel;
	};
	class Mesh3DRenderer : public Component 
	{
		private:
		const Kirino::Material* material;
		const Kirino::Material* shadowsMaterial;
		const Kirino::Mesh* mesh;
		bool post;
		bool shadowCaster;
		public:
		float renderDistance;
		Mesh3DRenderer();
		void setMaterial(const Kirino::Material* material);
		void setShadowsMaterial(const Kirino::Material* material);
		void setMesh(const Kirino::Mesh* mesh);
		inline const Kirino::Mesh* getMesh() const { return mesh; };
		inline const Kirino::Material* getMaterial() const { return material; };
		inline void setShadowCasting(bool castShadow) { shadowCaster = castShadow; }
		inline const Kirino::Material* getShadowsMaterial() const { return shadowsMaterial; };
		inline bool isShadowCaster() { return shadowCaster; }
		inline void setPost(bool post) { this->post = post; }
		inline bool isPost() { return post; }
		void render(Kirino::StateColletion& stateCollection, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures) override;
		void shadowRender(Kirino::StateColletion& stateCollection, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures) override;
		void postRender(Kirino::StateColletion& stateCollection, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures) override;
	};
}