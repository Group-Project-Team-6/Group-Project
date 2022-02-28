#include "VkTechRenderer.h"
using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

VkTechRenderer::VkTechRenderer() : VulkanRenderer(*Window::GetWindow()){
	skyboxMesh = new VulkanMesh("Cube.msh");
	skyboxMesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	skyboxMesh->UploadToGPU(this);
}

VkTechRenderer::~VkTechRenderer() {

}

void VkTechRenderer::RenderFrame() {
	std::cout << "Render" << std::endl;
	SubmitDrawCall(skyboxMesh,frameCmdBuffer);
}