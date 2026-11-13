#include "MeshRenderer.h"
#include "Core.h"
namespace TealEngine 
{
	Mesh3DRenderer::Mesh3DRenderer() 
	{
		this->addProp(new StringKeyMapValuePointerProp<Kirino::Material>(&material, &Core::materialsManager.getLoadedMap()), "Material");
		this->addProp(new StringKeyMapValuePointerProp<Kirino::Mesh>(&mesh, &Core::modelsManager.getLoadedMap()), "Mesh");
		mesh = nullptr;
		material = nullptr;
		shadowsMaterial = nullptr;
		post = false;
		shadowCaster = true;
		this->addProp(new BoolProp(&post), "Render in post");
		this->addProp(new BoolProp(&shadowCaster), "Shadow caster");
		this->addProp(new FloatProp(&renderDistance), "Render distance");
		renderDistance = 0.0f;
	}

	void Mesh3DRenderer::setMaterial(const Kirino::Material* material) { this->material = material; }
	void Mesh3DRenderer::setShadowsMaterial(const Kirino::Material* material) { this->shadowsMaterial = material; }
	void Mesh3DRenderer::setMesh(const Kirino::Mesh* mesh) { this->mesh = mesh; };

	void Mesh3DRenderer::render(Kirino::StateColletion& stateCollection, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures) 
	{
		if(isPost()) return;
		if(this->getMesh() == nullptr) return;
		if(this->getMesh()->indexCount <= 0) return;
		if(this->getMesh()->vao == nullptr) return;
		if(this->getMaterial() == nullptr && material == nullptr) return;
		const Transform& t = ((GameNode3D*)this->getParent())->getWorldTransform();
		glm::vec3 cameraPosition = Core::camera->getParentOfTypeConst<GameNode3D>()->getWorldTransform().getPosition();
		//distance culling
		if(this->renderDistance > 0.0f) 
		{
			const float r = this->renderDistance * this->renderDistance;
			const glm::vec3 delta = t.getPosition() - cameraPosition;
			if((delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) > r) 
			{
				return;
			}
		}
		//setup transform
		TransformBlock tb = {t.getMatrix(), t.getNormalsModel()};
		Core::transformUBO.upload(tb);
		//Frustum culling
		const auto& bb = this->getMesh()->boundingBox;
		//Bounding box bounding sphere
		float R = glm::length(glm::vec3(bb.minX, bb.minY, bb.minZ) * t.getScale() - glm::vec3(bb.maxX, bb.maxY, bb.maxZ) * t.getScale());
		glm::vec3 center = (glm::vec3(bb.minX, bb.minY, bb.minZ) + glm::vec3(bb.maxX, bb.maxY, bb.maxZ)) * 0.5f;
		center = t.pointFromLocalToGlobal(center);
		//cull
		const float front = glm::dot(frustum.farNormal, center) - R - frustum.farD;
		const float back = glm::dot(frustum.nearNormal, center) - R - frustum.nearD;
		const float left = glm::dot(frustum.leftNormal, center) - R - frustum.leftD;
		const float right = glm::dot(frustum.rightNormal, center) - R - frustum.rightD;
		const float up = glm::dot(frustum.upNormal, center) - R - frustum.upD;
		const float bottom = glm::dot(frustum.bottomNormal, center) - R - frustum.bottomD;
		if(front > 0.0f || back > 0.0f || left > 0.0f || right > 0.0f || up > 0.0f || bottom > 0.0f) 
		{
			return;
		}
		Kirino::Material* mat = stateCollection.forcedMaterial ? stateCollection.forcedMaterial : const_cast<Kirino::Material*>(this->getMaterial());

		mat->useProgram();
		stateCollection.shader = mat->getShaderProgram();
		mat->setupTextures(stateCollection.textureBindings);
		mat->setupUniformBlocks(stateCollection.uboBindings);
		mat->setupUniforms();
		//Draw mesh
		glBindVertexArray(*this->getMesh()->vao);
		glDrawElementsBaseVertex(GL_TRIANGLES, this->getMesh()->indexCount, GL_UNSIGNED_INT, 0, this->getMesh()->vertexOffset);
	}

	void Mesh3DRenderer::shadowRender(Kirino::StateColletion& stateCollection, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures) 
	{
		if(!this->isShadowCaster()) return;
		Kirino::Material* m = stateCollection.forcedMaterial;
		if(shadowsMaterial) stateCollection.forcedMaterial = const_cast<Kirino::Material*>(shadowsMaterial);
		render(stateCollection, frustum, renderTextures);
		stateCollection.forcedMaterial = m;
	}
	void Mesh3DRenderer::postRender(Kirino::StateColletion& stateCollection, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures) 
	{
		if(!isPost()) return;
		this->setPost(false);
		render(stateCollection, frustum, renderTextures);
		this->setPost(true);
	}
	
}