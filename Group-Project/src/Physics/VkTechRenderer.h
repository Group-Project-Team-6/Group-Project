#pragma once

#include "../VulkanRendering/VulkanRenderer.h"
#include "../VulkanRendering/VulkanShader.h"
#include "../VulkanRendering/VulkanTexture.h"
#include "../VulkanRendering/VulkanMesh.h"
#include "../VulkanRendering/VulkanPipelineBuilder.h"
#include "../VulkanRendering/VulkanDescriptorSetLayoutBuilder.h"
#include "../VulkanRendering/VulkanRenderPassBuilder.h"
#include "../VulkanRendering/VulkanShaderBuilder.h"

//#include "../CSC8503/GameWorld.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8503 {
		class RenderObject;

		class VkTechRenderer : public VulkanRenderer {
		public:
			VkTechRenderer();
			~VkTechRenderer();

		protected:
			void	RenderFrame()	override;

			void	InitDefaultRenderPass() override { VulkanRenderer::InitDefaultRenderPass(); };
			void	InitDefaultDescriptorPool() override { VulkanRenderer::InitDefaultDescriptorPool(); };


			//Matrix4 SetupDebugLineMatrix()	const;
			//Matrix4 SetupDebugStringMatrix()const;

			//VulkanShader* defaultShader;

			//GameWorld& gameWorld;

			//void BuildObjectList();
			//void SortObjectList();
			//void RenderShadowMap();
			//void RenderCamera();
			//void RenderSkybox();

			//void LoadSkybox();


			//vector<const RenderObject*> activeObjects;

			VulkanShader* skyboxShader;
			VulkanMesh* skyboxMesh;
			UniformData* matrixDataObject;
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