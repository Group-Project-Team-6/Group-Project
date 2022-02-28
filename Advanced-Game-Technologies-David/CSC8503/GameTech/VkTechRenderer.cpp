#include "VkTechRenderer.h"
using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

VkTechRenderer::VkTechRenderer(GameWorld& gameWorld) : VulkanRenderer(*Window::GetWindow()), gameWorld(gameWorld) {
	//InitUniformBuffer();
}

VkTechRenderer::~VkTechRenderer() {

}

void VkTechRenderer::RenderFrame() {
	/*float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);*/
	Camera* cam = gameWorld.GetMainCamera();
	gameWorld.OperateOnContents(
		[&](GameObject* o) {
			if (o->IsActive()) {
				const RenderObject* g = o->GetRenderObject();
				if (g) {
					activeObjects.emplace_back(g);
				}
			}
		}
	);
	for (const auto& i : activeObjects) {
		VulkanPipelineBuilder pipelineBuilder;
		VulkanRenderPassBuilder passBuilder;
		passBuilder
			.WithDebugName("pass builder");
		vk::RenderPass renderPass = passBuilder.Build(*this);
		/*VulkanDescriptorSetLayoutBuilder desSetLayoutBuilder;
		desSetLayoutBuilder
			.WithDebugName("desSetLayoutBuilder")
			.WithSamplers(1,vk::ShaderStageFlagBits::eAllGraphics)
			.*/
		pipelineBuilder
			.WithDebugName("Pipeline")
			.WithDepthState(vk::CompareOp::eLess, true, true)
			.WithPass(renderPass)
			.WithShaderState(dynamic_cast<VulkanShader*>((*i).GetShader()));
			VulkanPipeline pipeline = pipelineBuilder.Build(*this);

		frameCmdBuffer.beginRenderPass(defaultBeginInfo, vk::SubpassContents::eInline);
		frameCmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.pipeline);
		VulkanMesh* m = dynamic_cast<VulkanMesh*>(i->GetMesh());
		SubmitDrawCall(m, frameCmdBuffer);
		frameCmdBuffer.endRenderPass();
	}
}