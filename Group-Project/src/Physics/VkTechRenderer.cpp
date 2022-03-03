#include "VkTechRenderer.h"
using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

VkTechRenderer::VkTechRenderer() : VulkanRenderer(*Window::GetWindow()){
	count = 0.0f;

	basicTex = (VulkanTexture*)TextureLoader::LoadAPITexture("checkerboard.png");

	skyboxMesh = new VulkanMesh("CharacterM.msh");
	skyboxMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	skyboxMesh->UploadToGPU(this);

	builder = VulkanShaderBuilder()
		.WithVertexBinary("SimpleVert.spv")
		.WithFragmentBinary("SimpleFrag.spv");
	skyboxShader = builder.Build(*this);

	matrix = Matrix4();
	matrix.SetPositionVector(Vector3(0.2f, 0.2f, 0.2f));
	InitUniformBuffer(matrixDataObject,matrix.array,sizeof(matrix.array));

	desSetLayoutBuilder
		.WithDebugName("desSetLayoutBuilder")
		.WithUniformBuffers(1, vk::ShaderStageFlagBits::eVertex);
		//.WithSamplers(1, vk::ShaderStageFlagBits::eFragment);
	desSetLayout = desSetLayoutBuilder.Build(*this);

	set.push_back(BuildDescriptorSet(desSetLayout));

	bufferInfo
		.setBuffer(matrixDataObject.buffer)
		.setOffset(0)
		.setRange(sizeof(matrix.array));

	desWrite
		.setDstSet(set[0])
		.setDstBinding(0)
		.setDstArrayElement(0)
		.setDescriptorType(vk::DescriptorType::eUniformBuffer)
		.setDescriptorCount(1)
		.setPBufferInfo(&bufferInfo)
		.setPImageInfo(nullptr)
		.setPTexelBufferView(nullptr);

	device.updateDescriptorSets(1, &desWrite, 0, nullptr);

	pipelineBuilder
		.WithDebugName("Pipeline")
		.WithDepthState(vk::CompareOp::eLess, true, true)
		.WithPass(defaultRenderPass)
		.WithShaderState(skyboxShader)
		.WithVertexSpecification(skyboxMesh->GetVertexSpecification(), vk::PrimitiveTopology::eTriangleList)
		.WithDescriptorSetLayout(desSetLayout);
	pipeline = pipelineBuilder.Build(*this);
}

VkTechRenderer::~VkTechRenderer() {
	device.destroy(desSetLayout);
	device.destroy(pipeline.pipeline);
	device.destroy(pipeline.layout);
}

void VkTechRenderer::RenderFrame() {
	count += 0.001f;
	if (count > 0.8f) count = 0.0f;

	std::cout << "Render" << std::endl;

	//Create pipeline
	matrix.SetPositionVector(Vector3(0.2f + count, 0.2f, 0.2f));
	UpdateUniformBuffer(matrixDataObject,matrix.array, sizeof(matrix.array));

	frameCmdBuffer.beginRenderPass(defaultBeginInfo, vk::SubpassContents::eInline);

	frameCmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.pipeline);
	frameCmdBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline.layout, 0, 1, set.data(), 0, nullptr);

	SubmitDrawCall(skyboxMesh,frameCmdBuffer);

	frameCmdBuffer.endRenderPass();	
}