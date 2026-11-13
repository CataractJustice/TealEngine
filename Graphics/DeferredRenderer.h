#pragma once
#include "GameNode/Component.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Graphics/Mesh/MeshRendererInstanced.h"
#include "FrameBuffer/FrameBuffer.h"
#include "Graphics/Light/Light.h"
#include "Kirino/TextureBindings.h"
#include "Kirino/UBOBindings.h"
#include "Kirino/StateColletion.h"
namespace TealEngine 
{
	class DeferredRenderer : public Component 
	{
		private:
		Mesh3DRendererInstancedList renderList;
		Texture depth, unlit, color, position, normal, specular, light, transparentPositionAlpha, transparentNormal;
		FrameBuffer fb;
		Kirino::Material lightCombineMaterial;
		std::unordered_set<Component*> renderers; 
		std::unordered_set<Component*> postRenderers;
		std::unordered_set<Component*> shadowRenderers;
		std::unordered_set<Component*> transparentRenderers;
		std::unordered_set<ILight*> lightsSet;
		Kirino::StateColletion stateCollection;
		bool lightRenderingState;
		public:
		DeferredRenderer();
		void onSceneComponentAdd(Component* component, GameNode* parent) override;
		void onSceneComponentRemove(Component* component, GameNode* parent) override;
		void render(Camera* camera);
		void resize(unsigned int width, unsigned int height);
		inline Mesh3DRendererInstancedList& getRendererList() { return renderList; }
		void onWindowResize(int width, int height) override;
		void renderModels(Kirino::Material* material = nullptr); 
		void renderPostModels();
		void renderTransparentModels();
	};
}