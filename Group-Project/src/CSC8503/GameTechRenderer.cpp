#include "GameTechRenderer.h"
#include "../game/GameEntity.h"
#include "../Common/Camera.h"
#include "../Common/Vector2.h"
#include "../Common/Vector3.h"
#include "../Common/TextureLoader.h"
#include "Transform.h"
#include "../game/Painter.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8503;

#define SHADOWSIZE 2048

Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5, 0.5, 0.5)) * Matrix4::Scale(Vector3(0.5, 0.5, 0.5));

GameTechRenderer::GameTechRenderer(GameWorld& world) : OGLRenderer(*Window::GetWindow()), gameWorld(world)	{
	initTexture = false;
	painted = false;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	shadowShader = new OGLShader("GameTechShadowVert.glsl", "GameTechShadowFrag.glsl");

	// Depth Attachment
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			     SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	//FBO
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT ,GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		return;
	}

	glClearColor(1, 1, 1, 1);

	//Set up the light properties
	lightColour = Vector4(0.8f, 0.8f, 0.5f, 1.0f);
	lightRadius = 1000.0f;
	lightPosition = Vector3(-200.0f, 60.0f, 200.0f);

	//painter
	painterShader = dynamic_cast<OGLShader*>(LoadShader("PaintEffectShader.set"));
	painterMesh = new OGLMesh();
	painterMesh->SetVertexPositions({ Vector3(-1, 1,-1), Vector3(-1,-1,-1) , Vector3(1,-1,-1) , Vector3(1,1,-1) });
	painterMesh->SetVertexIndices({ 0,1,2,2,3,0 });
	painterMesh->SetVertexTextureCoords({ Vector2(0,1), Vector2(0,0),Vector2(1,0), Vector2(1,1) });
	painterMesh->UploadToGPU();

	//Skybox!
	skyboxShader = new OGLShader("skyboxVertex.glsl", "skyboxFragment.glsl");
	skyboxMesh = new OGLMesh();
	skyboxMesh->SetVertexPositions({Vector3(-1, 1,-1), Vector3(-1,-1,-1) , Vector3(1,-1,-1) , Vector3(1,1,-1) });
	skyboxMesh->SetVertexIndices({ 0,1,2,2,3,0 });
	skyboxMesh->UploadToGPU();

	LoadSkybox();
}

GameTechRenderer::~GameTechRenderer()	{
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
}

void GameTechRenderer::LoadSkybox() {
	string filenames[6] = {
		"/Cubemap/skyrender0004.png",
		"/Cubemap/skyrender0001.png",
		"/Cubemap/skyrender0003.png",
		"/Cubemap/skyrender0006.png",
		"/Cubemap/skyrender0002.png",
		"/Cubemap/skyrender0005.png"
	};

	int width[6]	= { 0 };
	int height[6]	= { 0 };
	int channels[6] = { 0 };
	int flags[6]	= { 0 };

	vector<char*> texData(6, nullptr);

	for (int i = 0; i < 6; ++i) {
		TextureLoader::LoadTexture(filenames[i], texData[i], width[i], height[i], channels[i], flags[i]);
		if (i > 0 && (width[i] != width[0] || height[0] != height[0])) {
			std::cout << __FUNCTION__ << " cubemap input textures don't match in size?\n";
			return;
		}
	}
	glGenTextures(1, &skyboxTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	GLenum type = channels[0] == 4 ? GL_RGBA : GL_RGB;

	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width[i], height[i], 0, type, GL_UNSIGNED_BYTE, texData[i]);
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void GameTechRenderer::RenderFrame() {
	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	BuildObjectList();
	if (!initTexture) {
		initTextures();
		initTexture = true;
	}
	UpdatePaints();
	SortObjectList();
	RenderShadowMap();
	RenderSkybox();
	RenderCamera();
	glDisable(GL_CULL_FACE); //Todo - text indices are going the wrong way...

}

void GameTechRenderer::initTextures() {
	glDisable(GL_CULL_FACE);
	PainterMap map = Painter::GetPaintInfos();
	for (int j = 0; j < 2; j++) {
		vector<GameEntity*>* list;

		if (j == 1) {
			list = &activeTransparentObjects;
		}
		else {
			list = &activeObjects;
		}
		for (int i = 0; i < (*list).size(); i++) {
			GameTimer t;
			if ((*list)[i]->GetName() != "Wall") continue;

			(*list)[i]->GetRenderObject()->SetColour(Vector4(Vector3((*list)[i]->GetRenderObject()->GetColour()), 0.0f));
			//OGLTexture* objTex = dynamic_cast<OGLTexture*>(activeObjects[i]->GetRenderObject()->GetDefaultTexture());
			OGLTexture* renderTex = dynamic_cast<OGLTexture*>((*list)[i]->GetRenderObject()->GetDefaultTexture());

			GLuint tex = renderTex->GetObjectID();
			if(renderTex->GetFBO()) glBindFramebuffer(GL_FRAMEBUFFER, renderTex->GetFBO());
			glViewport(0, 0, renderTex->GetWidth(), renderTex->GetHeight());

			BindShader(painterShader);

			BindTextureToShader(renderTex, "hitTex", 0);

			int hitUV = glGetUniformLocation(painterShader->GetProgramID(), "hitPos");
			glUniform3fv(hitUV, 1, Vector3(0, 0, 0).array);

			int initID = glGetUniformLocation(painterShader->GetProgramID(), "isInit");
			glUniform1i(initID, 1);

			int modelLocation = glGetUniformLocation(painterShader->GetProgramID(), "modelMatrix");
			Matrix4 modelMatrix = (*list)[i]->GetTransform().GetMatrix();

			glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

			BindMesh(painterMesh);
			//int layerCount = it->first->GetRenderObject()->GetMesh()->GetSubMeshCount();
			//for (int i = 0; i < layerCount; ++i) {
			DrawBoundMesh();
			//}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			t.Tick();
			std::cout << "InitTextures: " << t.GetTimeDeltaMSec() << std::endl;
		}
	}
	glViewport(0, 0, currentWidth, currentHeight);
	glEnable(GL_CULL_FACE);
}

void GameTechRenderer::UpdatePaints() {
	glDisable(GL_CULL_FACE);
	PainterMap map = Painter::GetPaintInfos();
	for (auto& it = map.begin(); it != map.end(); it++) {
		for (int j = 0; j < 2; j++) {
			vector<GameEntity*>* list;

			if (j == 1) {
				list = &activeTransparentObjects;
			}
			else {
				list = &activeObjects;
			}
			for (int i = 0; i < (*list).size(); i++) {

				if ((*list)[i]->GetName() == "Bullet") continue;

				if ((it->second - (*list)[i]->GetTransform().GetPosition()).Length() > (1.0f + (*list)[i]->GetTransform().GetScale().Length() * 0.5f)) continue;
				(*list)[i]->GetRenderObject()->SetColour(Vector4(Vector3((*list)[i]->GetRenderObject()->GetColour()), 1.0f));
				//OGLTexture* objTex = dynamic_cast<OGLTexture*>(activeObjects[i]->GetRenderObject()->GetDefaultTexture());
				OGLTexture* renderTex = dynamic_cast<OGLTexture*>((*list)[i]->GetRenderObject()->GetDefaultTexture());

				glBindFramebuffer(GL_FRAMEBUFFER, renderTex->GetFBO());
				GLuint tex = renderTex->GetObjectID();
				glViewport(0, 0, renderTex->GetWidth(), renderTex->GetHeight());

				BindShader(painterShader);

				BindTextureToShader(renderTex, "hitTex", 0);

				int hitUV = glGetUniformLocation(painterShader->GetProgramID(), "hitPos");
				glUniform3fv(hitUV, 1, it->second.array);

				int initID = glGetUniformLocation(painterShader->GetProgramID(), "isInit");
				glUniform1i(initID, 0);

				int modelLocation = glGetUniformLocation(painterShader->GetProgramID(), "modelMatrix");
				Matrix4 modelMatrix = (*list)[i]->GetTransform().GetMatrix();

				glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

				BindMesh((*list)[i]->GetRenderObject()->GetMesh());
				//int layerCount = it->first->GetRenderObject()->GetMesh()->GetSubMeshCount();
				//for (int i = 0; i < layerCount; ++i) {
				DrawBoundMesh();
				//}
				glViewport(0, 0, currentWidth, currentHeight);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

			}
		}
	}
	Painter::ClearPaint();
	glEnable(GL_CULL_FACE);
}

void GameTechRenderer::BuildObjectList() {
	activeObjects.clear();
	activeTransparentObjects.clear();

	gameWorld.OperateOnContents(
		[&](GameEntity* o) {
			if (o->IsActive()) {
				if (o->GetName() == "Wall" && (o->GetTransform().GetPosition() - gameWorld.GetMainCamera()->GetPosition()).Length() < 11.0f) {
					activeTransparentObjects.emplace_back(o);
				}
				else {
					activeObjects.emplace_back(o);
				}
			}
		}
	);


}

void GameTechRenderer::SortObjectList() {
	//Who cares!

}

void GameTechRenderer::RenderShadowMap() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glCullFace(GL_FRONT);
	glDepthMask(GL_TRUE);
	BindShader(shadowShader);
	int mvpLocation = glGetUniformLocation(shadowShader->GetProgramID(), "mvpMatrix");

	Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(lightPosition, Vector3(0, 0, 0), Vector3(0,1,0));
	Matrix4 shadowProjMatrix = Matrix4::Perspective(100.0f, 500.0f, 1, 45.0f);

	Matrix4 mvMatrix = shadowProjMatrix * shadowViewMatrix;

	shadowMatrix = biasMatrix * mvMatrix; //we'll use this one later on

	for (int j = 0; j < 2; j++) {
		vector<GameEntity*>* list;

		if (j == 1) {
			list = &activeTransparentObjects;
		}
		else {
			list = &activeObjects;
		}
		for (const auto& i : *list) {
			if (i->GetRenderObject()->GetColour().w == 0.0f) continue;
			Matrix4 modelMatrix = (*i).GetTransform().GetMatrix();
			Matrix4 mvpMatrix = mvMatrix * modelMatrix;
			glUniformMatrix4fv(mvpLocation, 1, false, (float*)&mvpMatrix);
			OGLTexture* renderTex = dynamic_cast<OGLTexture*>(i->GetRenderObject()->GetDefaultTexture());
			BindTextureToShader(renderTex, "mainTex", 0);

			BindMesh((*i).GetRenderObject()->GetMesh());
			int layerCount = (*i).GetRenderObject()->GetMesh()->GetSubMeshCount();
			for (int i = 0; i < layerCount; ++i) {
				DrawBoundMesh(i);
			}
		}
	}

	glViewport(0, 0, currentWidth, currentHeight);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
}

void GameTechRenderer::RenderSkybox() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	BindShader(skyboxShader);

	int projLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "projMatrix");
	int viewLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "viewMatrix");
	int texLocation  = glGetUniformLocation(skyboxShader->GetProgramID(), "cubeTex");

	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

	glUniform1i(texLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	BindMesh(skyboxMesh);
	DrawBoundMesh();

	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void GameTechRenderer::RenderCamera() {
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	OGLShader* activeShader = nullptr;
	int projLocation	= 0;
	int viewLocation	= 0;
	int modelLocation	= 0;
	int colourLocation  = 0;
	int hasVColLocation = 0;
	int hasTexLocation  = 0;
	int isTransparent	= 0;
	int shadowLocation  = 0;

	int lightPosLocation	= 0;
	int lightColourLocation = 0;
	int lightRadiusLocation = 0;

	int cameraLocation = 0;

	int count = 0;

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glDepthFunc(GL_LESS);

	for (int j = 0; j < 2; j++) {
		vector<GameEntity*>* list;

		if (j == 1) {
			list = &activeTransparentObjects;
			glDepthMask(GL_FALSE);
		}
		else {
			list = &activeObjects;
			glDepthMask(GL_TRUE);
		}

		for (const auto& i : *list) {
			//if (i->GetRenderObject()->GetColour().w == 0.0f) continue;
			//Draw to Texture
			OGLShader* shader = (OGLShader*)(*i).GetRenderObject()->GetShader();
			BindShader(shader);
			BindTextureToShader((OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture(), "mainTex", 0);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			if (activeShader != shader) {
				projLocation = glGetUniformLocation(shader->GetProgramID(), "projMatrix");
				viewLocation = glGetUniformLocation(shader->GetProgramID(), "viewMatrix");
				modelLocation = glGetUniformLocation(shader->GetProgramID(), "modelMatrix");
				shadowLocation = glGetUniformLocation(shader->GetProgramID(), "shadowMatrix");
				colourLocation = glGetUniformLocation(shader->GetProgramID(), "objectColour");
				hasVColLocation = glGetUniformLocation(shader->GetProgramID(), "hasVertexColours");
				hasTexLocation = glGetUniformLocation(shader->GetProgramID(), "hasTexture");
				isTransparent = glGetUniformLocation(shader->GetProgramID(), "isTransparent");

				lightPosLocation = glGetUniformLocation(shader->GetProgramID(), "lightPos");
				lightColourLocation = glGetUniformLocation(shader->GetProgramID(), "lightColour");
				lightRadiusLocation = glGetUniformLocation(shader->GetProgramID(), "lightRadius");
				GLuint timeLocation = glGetUniformLocation(shader->GetProgramID(), "dt");
				cameraLocation = glGetUniformLocation(shader->GetProgramID(), "cameraPos");
				glUniform3fv(cameraLocation, 1, (float*)&gameWorld.GetMainCamera()->GetPosition());

				glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
				glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

				glUniform3fv(lightPosLocation, 1, (float*)&lightPosition);
				glUniform4fv(lightColourLocation, 1, (float*)&lightColour);
				glUniform1f(lightRadiusLocation, lightRadius);
				glUniform1f(timeLocation, Window::GetTimer()->GetTotalTimeSeconds());

				int shadowTexLocation = glGetUniformLocation(shader->GetProgramID(), "shadowTex");
				glUniform1i(shadowTexLocation, 1);

				activeShader = shader;
			}

			Matrix4 modelMatrix = (*i).GetTransform().GetMatrix();
			glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

			Matrix4 fullShadowMat = shadowMatrix * modelMatrix;
			glUniformMatrix4fv(shadowLocation, 1, false, (float*)&fullShadowMat);

			glUniform4fv(colourLocation, 1, (float*)&i->GetRenderObject()->GetColour());

			glUniform1i(hasVColLocation, !(*i).GetRenderObject()->GetMesh()->GetColourData().empty());

			glUniform1i(hasTexLocation, (OGLTexture*)(*i).GetRenderObject()->GetDefaultTexture() ? 1 : 0);
			glUniform1i(isTransparent, j==1 ? 1 : 0);

			BindMesh((*i).GetRenderObject()->GetMesh());
			int layerCount = (*i).GetRenderObject()->GetMesh()->GetSubMeshCount();
			for (int i = 0; i < layerCount; ++i) {
				DrawBoundMesh(i);
				count++;
			}
		}
	}
	std::cout << count << std::endl;
	glDepthMask(GL_TRUE);
}

Matrix4 GameTechRenderer::SetupDebugLineMatrix()	const  {
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	return projMatrix * viewMatrix;
}

Matrix4 GameTechRenderer::SetupDebugStringMatrix()	const {
	return Matrix4::Orthographic(-1, 1.0f, 100, 0, 0, 100);
}
