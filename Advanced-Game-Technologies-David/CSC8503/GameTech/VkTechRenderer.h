#pragma once
#include "../../Plugins/VulkanRendering/VulkanRenderer.h"
#include "../../Plugins/VulkanRendering/VulkanShader.h"
#include "../../Plugins/VulkanRendering/VulkanTexture.h"
#include "../../Plugins/VulkanRendering/VulkanMesh.h"

#include "../CSC8503Common/GameWorld.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8503 {
		class RenderObject;

		class VkTechRenderer : public VulkanRenderer {
		public:
			VkTechRenderer(GameWorld& world);
			~VkTechRenderer();

		protected:
			void RenderFrame()	override;

			Matrix4 SetupDebugLineMatrix()	const override;
			Matrix4 SetupDebugStringMatrix()const override;

			VulkanShader* defaultShader;

			GameWorld& gameWorld;

			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();
			void RenderCamera();
			void RenderSkybox();

			void LoadSkybox();

			vector<const RenderObject*> activeObjects;

			VulkanShader* skyboxShader;
			VulkanMesh* skyboxMesh;
			GLuint		skyboxTex;

			//shadow mapping things
			OGLShader* shadowShader;
			GLuint		shadowTex;
			GLuint		shadowFBO;
			Matrix4     shadowMatrix;

			Vector4		lightColour;
			float		lightRadius;
			Vector3		lightPosition;
		};
	}
}