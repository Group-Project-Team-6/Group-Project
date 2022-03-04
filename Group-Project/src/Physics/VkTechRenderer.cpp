#include "VkTechRenderer.h"
#include "../common/TextureLoader.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

VkTechRenderer::VkTechRenderer() : VulkanRenderer(*Window::GetWindow()){
	count = 0.0f;
	skyboxTex = (VulkanTexture*)TextureLoader::LoadAPITexture("checkerboard.png");

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
		.WithUniformBuffers(1, vk::ShaderStageFlagBits::eVertex)
		.WithSamplers(1, vk::ShaderStageFlagBits::eFragment);
	desSetLayout = desSetLayoutBuilder.Build(*this);

	set.push_back(BuildDescriptorSet(desSetLayout));

	vk::SamplerCreateInfo samplerInfo;
	samplerInfo
		.setMagFilter(vk::Filter::eLinear)
		.setMinFilter(vk::Filter::eLinear)
		.setAddressModeU(vk::SamplerAddressMode::eRepeat)
		.setAddressModeV(vk::SamplerAddressMode::eRepeat)
		.setAddressModeW(vk::SamplerAddressMode::eRepeat)
		.setAnisotropyEnable(true)
		.setMaxAnisotropy(gpu.getProperties().limits.maxSamplerAnisotropy)
		.setBorderColor(vk::BorderColor::eIntOpaqueBlack)
		.setUnnormalizedCoordinates(false)
		.setCompareEnable(false)
		.setCompareOp(vk::CompareOp::eAlways)
		.setMipmapMode(vk::SamplerMipmapMode::eLinear)
		.setMipLodBias(0.0f)
		.setMinLod(0.0f)
		.setMaxLod(0.0f);
	sampler = device.createSampler(samplerInfo);

	bufferInfo
		.setBuffer(matrixDataObject.buffer)
		.setOffset(0)
		.setRange(sizeof(matrix.array));

	imageInfo
		.setImageLayout(skyboxTex->GetLayout())
		.setImageView(skyboxTex->GetDefaultView())
		.setSampler(sampler);


	desWrite[0]
		.setDstSet(set[0])
		.setDstBinding(0)
		.setDstArrayElement(0)
		.setDescriptorType(vk::DescriptorType::eUniformBuffer)
		.setDescriptorCount(1)
		.setPBufferInfo(&bufferInfo)
		.setPImageInfo(nullptr)
		.setPTexelBufferView(nullptr);

	desWrite[1]
		.setDstSet(set[0])
		.setDstBinding(1)
		.setDstArrayElement(0)
		.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
		.setDescriptorCount(1)
		.setPBufferInfo(nullptr)
		.setPImageInfo(&imageInfo)
		.setPTexelBufferView(nullptr);

	device.updateDescriptorSets(2, desWrite, 0, nullptr);

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
	device.destroy(sampler);
	device.destroy(desSetLayout);
	device.destroy(pipeline.pipeline);
	device.destroy(pipeline.layout);
}

void VkTechRenderer::RenderFrame() {
	count += 0.001f;
	if (count > 0.8f) count = 0.0f;

	//std::cout << "Render" << std::endl;

	//Create pipeline
	matrix.SetPositionVector(Vector3(0.2f + count, 0.2f, 0.2f));
	UpdateUniformBuffer(matrixDataObject,matrix.array, sizeof(matrix.array));

	frameCmdBuffer.beginRenderPass(defaultBeginInfo, vk::SubpassContents::eInline);

	frameCmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.pipeline);
	frameCmdBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline.layout, 0, 1, set.data(), 0, nullptr);

	SubmitDrawCall(skyboxMesh,frameCmdBuffer);

	frameCmdBuffer.endRenderPass();	
}