# include "ShadowRenderer.h"

ShadowRenderer::ShadowRenderer(Window & parent) : OGLRenderer(parent) {
	camera = new Camera(-8.0f, 40.0f, Vector3(-200.0f, 50.0f, 250.0f));
	light = new Light(Vector3(-450.0f, 200.0f, 280.0f),
		Vector4(1, 1, 1, 1), 5500.0f);

	hellData = new MD5FileData(MESHDIR"anim/hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim(MESHDIR"anim/idle2.md5anim");
	hellData->AddAnim(MESHDIR"anim/walk7.md5anim");
	hellNode->PlayAnim(MESHDIR"anim/walk7.md5anim");
    //hellNode->PlayAnim(MESHDIR"anim/idle2.md5anim");

	sceneShader = new Shader(SHADERDIR"shadowscenevert.glsl",
		SHADERDIR"shadowscenefrag.glsl");
	shadowShader = new Shader(SHADERDIR"shadowVert.glsl", SHADERDIR"shadowFrag.glsl");

	if (!sceneShader->LinkProgram() || !shadowShader->LinkProgram()) {
		return;
	}
	//Generate Texture
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Generate Buffer
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowTex, 0);
	//tell opengl to stop renderering colour
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Generate Floor
	floor = Mesh::GenerateQuad();
	floor->SetTexture(SOIL_load_OGL_texture(MESHDIR"brick.tga"
		, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	floor->SetBumpMap(SOIL_load_OGL_texture(MESHDIR"brickDOT3.tga"
		, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	SetTextureRepeating(floor->GetTexture(), true);

	glEnable(GL_DEPTH_TEST);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);
	init = true;

}

ShadowRenderer ::~ShadowRenderer(void) {
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	delete camera;
	delete light;
	delete hellData;
	delete hellNode;
	delete floor;

	delete sceneShader;
	delete shadowShader;
	currentShader = NULL;

}

void ShadowRenderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	hellNode->Update(msec);
}

void ShadowRenderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawShadowScene(); // First render pass ...
	DrawCombinedScene(); // Second render pass ...

	SwapBuffers();

}

//Draw Shadow
void ShadowRenderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//adpat viewport to the size of shadow (2048)
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	//?
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	SetCurrentShader(shadowShader);
	//Set current view position to be as light
	viewMatrix = Matrix4::BuildViewMatrix(
		light->GetPosition(), Vector3(0, 0, 0));
	//?
	textureMatrix = biasMatrix *(projMatrix * viewMatrix);

	UpdateShaderMatrices();

	DrawFloor();
	DrawMesh();

	glUseProgram(0);
	//?
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//recover the view port
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

//Draw Scene
void ShadowRenderer::DrawCombinedScene() {
	SetCurrentShader(sceneShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"shadowTex"), 2);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera->GetPosition());

	SetShaderLight(*light);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();

	DrawFloor();
	DrawMesh();

	glUseProgram(0);

}

void ShadowRenderer::DrawMesh() {
	modelMatrix.ToIdentity();

	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "textureMatrix"), 1, false, *& tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "modelMatrix"), 1, false, *& modelMatrix.values);

	hellNode->Draw(*this);

}

void ShadowRenderer::DrawFloor() {
	modelMatrix = Matrix4::Rotation(90, Vector3(1, 0, 0)) *
		Matrix4::Scale(Vector3(450, 450, 1));
	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "textureMatrix"), 1, false, *& tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "modelMatrix"), 1, false, *& modelMatrix.values);

	floor->Draw();
}