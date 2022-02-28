#pragma once
#include "../../Plugins/VulkanRendering/VulkanRenderer.h"
#include "../../Plugins/VulkanRendering/VulkanShader.h"
#include "../../Plugins/VulkanRendering/VulkanTexture.h"
#include "../../Plugins/VulkanRendering/VulkanMesh.h"
#include "../../Plugins/VulkanRendering/VulkanPipelineBuilder.h"
#include "../../Plugins/VulkanRendering/VulkanDescriptorSetLayoutBuilder.h"
#include "../../Plugins/VulkanRendering/VulkanRenderPassBuilder.h"

#include "../CSC8503Common/GameWorld.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8503 {
		class RenderObject;

		class VkTechRenderer : public VulkanRenderer {
		public:
			VkTechRenderer(GameWorld& gameWorld);
			~VkTechRenderer();

		protected:
			void	RenderFrame()	override;

			void	InitDefaultRenderPass() override { VulkanRenderer::InitDefaultRenderPass(); };
			void	InitDefaultDescriptorPool() override { VulkanRenderer::InitDefaultDescriptorPool(); };


			//Matrix4 SetupDebugLineMatrix()	const;
			//Matrix4 SetupDebugStringMatrix()const;

			//VulkanShader* defaultShader;

			GameWorld& gameWorld;

			//void BuildObjectList();
			//void SortObjectList();
			//void RenderShadowMap();
			//void RenderCamera();
			//void RenderSkybox();

			//void LoadSkybox();
			

			vector<const RenderObject*> activeObjects;

			//VulkanShader* skyboxShader;
			//VulkanMesh* skyboxMesh;
			//VulkanTexture* skyboxTex;

			////shadow mapping things
			//VulkanShader* shadowShader;
			//VulkanTexture* shadowTex;
			//VulkanTexture* shadowFBO;
			//Matrix4     shadowMatrix;

			//Vector4		lightColour;
			//float		lightRadius;
			//Vector3		lightPosition;
		};
	}
}