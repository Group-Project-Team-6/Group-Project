#include "VkTechRenderer.h"
using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

VkTechRenderer::VkTechRenderer() : VulkanRenderer(*Window::GetWindow()){
	skyboxMesh = new VulkanMesh("CharacterM.msh");
	skyboxMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	skyboxMesh->UploadToGPU(this);

	VulkanShaderBuilder builder = VulkanShaderBuilder()
		.WithVertexBinary("SimpleVert.spv")
		.WithFragmentBinary("SimpleFrag.spv");
	skyboxShader = builder.Build(*this);
	//InitUniformBuffer(*matrixDataObject, );
}

VkTechRenderer::~VkTechRenderer() {

}

void VkTechRenderer::RenderFrame() {
	
	std::cout << "Render" << std::endl;
	VulkanPipelineBuilder pipelineBuilder;
	/*VulkanRenderPassBuilder passBuilder;
	passBuilder
		.WithDebugName("pass builder");*/
	/*vk::RenderPass renderPass = passBuilder.Build(*this);*/

	/*VulkanDescriptorSetLayoutBuilder desSetLayoutBuilder;
	desSetLayoutBuilder
		.WithDebugName("desSetLayoutBuilder")
		.WithSamplers(1, vk::ShaderStageFlagBits::eAllGraphics);*/
	pipelineBuilder
		.WithDebugName("Pipeline")
		.WithDepthState(vk::CompareOp::eLess, true, true)
		.WithPass(defaultRenderPass)
		.WithShaderState(skyboxShader)
		.WithVertexSpecification(skyboxMesh->GetVertexSpecification(),vk::PrimitiveTopology::eTriangleList);
	VulkanPipeline pipeline = pipelineBuilder.Build(*this);

	frameCmdBuffer.beginRenderPass(defaultBeginInfo, vk::SubpassContents::eInline);
	frameCmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.pipeline);

	SubmitDrawCall(skyboxMesh,frameCmdBuffer);
	frameCmdBuffer.endRenderPass();
}