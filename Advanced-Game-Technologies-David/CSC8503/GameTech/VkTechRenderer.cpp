#include "VkTechRenderer.h"
using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

VkTechRenderer::VkTechRenderer(GameWorld& gameWorld) : VulkanRenderer(*Window::GetWindow()), gameWorld(gameWorld) {

}

VkTechRenderer::~VkTechRenderer() {

}

void VkTechRenderer::RenderFrame() {

}
