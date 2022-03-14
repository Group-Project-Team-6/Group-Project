#pragma once
#include "../OpenGLRendering/OGLRenderer.h"
#include "../OpenGLRendering/OGLShader.h"
#include "../OpenGLRendering/OGLTexture.h"
#include "../OpenGLRendering/OGLMesh.h"

#include "GameWorld.h"
#include "..//common/Matrix4.h"

namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8503 {
		class RenderObject;
		class GameTechRenderer : public OGLRenderer	{
		public:
			GameTechRenderer(GameWorld& world);
			~GameTechRenderer();  

		protected:
			void RenderFrame()	override;

			Matrix4 SetupDebugLineMatrix()	const override;
			Matrix4 SetupDebugStringMatrix()const override;

			OGLShader*	defaultShader;

			GameWorld&	gameWorld;

			void initTextures();
			void UpdatePaints();
			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();
			void RenderCamera(); 
			void RenderSkybox();

			void LoadSkybox();

			vector<GameEntity*> activeObjects;
			vector<GameEntity*> activeTransparentObjects;

			OGLMesh* painterMesh;
			OGLShader* painterShader;
			GLuint		PainterFBO;

			OGLShader*  skyboxShader;
			OGLMesh*	skyboxMesh;
			GLuint		skyboxTex;

			//shadow mapping things
			OGLShader*	shadowShader;
			GLuint		shadowTex;
			GLuint		shadowColourTex;
			GLuint		shadowFBO;
			Matrix4     shadowMatrix;

			Vector4		lightColour;
			float		lightRadius;
			Vector3		lightPosition;

			bool painted;
			bool initTexture;
		};
	}
}

