#include "Renderer.h"

//DiffuseTex = 0; bumpTex = 1; waterTex 2; cubeTex 3; bufferDepthTex 4; bufferColourTex 5;

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 500.000f;
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkybox();
	DrawHeightMap();
	DrawWater();
	SwapBuffers();
}

Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	InitializeScene1();
	InitilizeScene2();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);
	glEnable(GL_DEPTH_TEST);
	init = true;
}

static int seed = time(NULL);
double GetRandNum(int min, int max) {
	srand(seed);
	seed = rand();
	return (min + rand() % (max - min + 1));
}

Renderer ::~Renderer(void) {
	glDeleteTextures(1, &bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	delete basicFont;
	delete camera;
	delete heightMap;
	delete quad;
	delete light;
	delete reflectShader;
	delete lightShader;
	delete skyboxShader;
	delete sceneShader;
	delete reverseColourShader;
	delete grayShader;
	delete kernelShader;
	delete blurShader;
	delete edgeShader;
	delete particleShader;
	currentShader = NULL;
	glDeleteTextures(1, &waterReflectionTex);
	glDeleteTextures(1, &waterReflectionDepthTex);
	glDeleteTextures(1, &waterRefractionTex);
	glDeleteTextures(1, &waterRefractionDepthTex);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &waterReflectionBuffer);
	glDeleteFramebuffers(1, &waterRefractionBuffer);

	//scene2
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	delete camera2;
	delete light2;
	delete hellData;
	delete hellNode;
	delete floor;
	delete root;
	delete glass;
	delete scene2Shader;
	delete shadowShader;
	currentShader = NULL;
}

void Renderer::InitializeScene1()
{
	//camera = new Camera(0.0f, 0.0f, Vector3(
	//RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500.0f, RAW_HEIGHT * HEIGHTMAP_Z));
	MoveLight = 0;
	lightNum = 1;
	showScene3 = 0;

	drawLaser = false;

	camera = new Camera(0.0f, 0.0f, Vector3(
		(RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 1400.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)));

	//test = new Particles(Vector3(RAW_HEIGHT * HEIGHTMAP_X / 2.0f, SKY_HEIGHT, RAW_HEIGHT * HEIGHTMAP_Z / 2.0f), Vector3(0, -250, 0), Vector4(1, 1, 1, 1), 10000, Vector3(RAW_WIDTH * HEIGHTMAP_X / 20, RAW_HEIGHT * HEIGHTMAP_Z / 20, RAW_HEIGHT * HEIGHTMAP_Z / 20));
	//ptlsys = new ParticleSystem(1600.0, SKY_HEIGHT, Vector3(0, -250, 0), 10000, Vector3(RAW_WIDTH * HEIGHTMAP_X / 20, RAW_HEIGHT * HEIGHTMAP_Z / 20, RAW_HEIGHT * HEIGHTMAP_Z / 20));
	//Generate Mesh
	int numInEachRow = sqrt(NUM_OF_PARTICLES);
	for (int i = 0; i < numInEachRow; ++i) {
		for (int j = 0; j < numInEachRow; ++j) {
			double tmp_lifelength = GetRandNum(5, 15);
			Vector3 tmp_velocity(0, -GetRandNum(100, 500), 0);
			double unitWidth = double(HEIGHTMAP_X * RAW_WIDTH) / double(numInEachRow);
			int offset_x = unitWidth * i;
			int offset_z = unitWidth * j;
			PtlSys[i * numInEachRow + j] = new Particles(Vector3(offset_x, SKY_HEIGHT, offset_z), tmp_velocity, ParticleColour, tmp_lifelength*1000, ParticleScale);
		}
	}
	quad = Mesh::GenerateQuad();
	texQuad = Mesh::GenerateQuad();
	heightMap = new HeightMap(MESHDIR"terrain.raw");
	normalHeightMap = new HeightMap(MESHDIR"terrain.raw");
	testQuad = Mesh::GenerateQuad();
	//光设置在地图中心上方500的位置，
	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 1.3f),
		1000.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 1.3f)),
		Vector4(1, 1, 1, 1), (RAW_WIDTH * HEIGHTMAP_X) / 0.25f * 5);
	light->SetDirection(Vector3(0, 1, 1));
	//传入cos值
	light->SetSpotLightCutAngle(cos(DegToRad(15)));

	//Set Shader
	lightShader = new Shader(SHADERDIR"BumpVertex.glsl",
		SHADERDIR"BumpFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl",
		SHADERDIR"SkyboxFragment.glsl");
	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl",
		SHADERDIR"reflectFragment.glsl");
	sceneShader = new Shader(SHADERDIR"TexturedVertex.glsl",
		SHADERDIR"TexturedFragment.glsl");

	//Post Process Shader
	reverseColourShader = new Shader(SHADERDIR"TexturedVertex.glsl",
		SHADERDIR"reverseColourFragment.glsl");
	grayShader = new Shader(SHADERDIR"TexturedVertex.glsl",
		SHADERDIR"grayFragment.glsl");
	kernelShader = new Shader(SHADERDIR"TexturedVertex.glsl",
		SHADERDIR"kernelFragment.glsl");
	blurShader = new Shader(SHADERDIR"TexturedVertex.glsl",
		SHADERDIR"blurFragment.glsl");
	edgeShader = new Shader(SHADERDIR"TexturedVertex.glsl",
		SHADERDIR"edgeFragment.glsl");

	particleShader = new Shader(SHADERDIR"particleVertex.glsl",SHADERDIR"particleFragment.glsl");

	textShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	waterReflectionShader = new Shader(SHADERDIR"BumpVertex.glsl",
		SHADERDIR"waterReflectionFragment.glsl");
	waterRefractionShader = new Shader(SHADERDIR"BumpVertex.glsl",
		SHADERDIR"waterRefractionFragment.glsl");

	basicFont = new Font(SOIL_load_OGL_texture(MESHDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	if (!basicFont->texture) {
		return;
	}

	//Debug purpose
	if (!lightShader->LinkProgram()) {
		return;
	}

	if (!skyboxShader->LinkProgram()) {
		return;
	}

	if (!reflectShader->LinkProgram()) {
		return;
	}

	if (!sceneShader->LinkProgram()) {
		return;
	}

	if (!reverseColourShader->LinkProgram()) {
		return;
	}

	if (!grayShader->LinkProgram()) {
		return;
	}

	if (!kernelShader->LinkProgram()) {
		return;
	}
	if (!blurShader->LinkProgram()) {
		return;
	}
	if (!edgeShader->LinkProgram()) {
		return;
	}
	if (!particleShader->LinkProgram()) {
		return;
	}
	if (!textShader->LinkProgram()) {
		return;
	}
	if (!waterReflectionShader->LinkProgram()) {
		return;
	}
	if (!waterRefractionShader->LinkProgram()) {
		return;
	}

	//Load Texture for the mountain, num 0
	heightMap->SetTexture(SOIL_load_OGL_texture(
		MESHDIR"Barren Reds.jpg", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	//num 1
	heightMap->SetBumpMap(SOIL_load_OGL_texture(
		MESHDIR"Burren RedsBumpMap.tga", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetSecondTexture(SOIL_load_OGL_texture(
		TEXTUREDIR"SnowMountain.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetdudvTexture(SOIL_load_OGL_texture(
		TEXTUREDIR"waterDUDV.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	normalHeightMap->SetTexture(SOIL_load_OGL_texture(
		MESHDIR"Barren Reds.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	normalHeightMap->SetBumpMap(SOIL_load_OGL_texture(
		MESHDIR"Burren RedsBumpMap.tga", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!normalHeightMap->GetTexture() || !normalHeightMap->GetBumpMap()) {
		return;
	}

	//num 2
	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water1.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	quad->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"waterDUDV.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//num 3
	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"Daylight Box_Right.bmp", TEXTUREDIR"Daylight Box_Left.bmp",
		TEXTUREDIR"Daylight Box_Top.bmp", TEXTUREDIR"Daylight Box_Bottom.bmp",
		TEXTUREDIR"Daylight Box_Front.bmp", TEXTUREDIR"Daylight Box_Back.bmp",
		SOIL_LOAD_RGB,
		0, 0
	);

	//Check if texture load properly
	if (!heightMap->GetTexture() || !heightMap->GetBumpMap() || !cubeMap) {
		return;
	}

	if (!heightMap->GetSecondTexture()) {
		return;
	}

	//Set Texture Attrib
	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(quad->GetBumpMap(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);
	SetTextureRepeating(heightMap->GetSecondTexture(), true);

	SetTextureRepeating(normalHeightMap->GetTexture(), true);
	SetTextureRepeating(normalHeightMap->GetBumpMap(), true);

	GenerateTexture();
	snowTex = SOIL_load_OGL_texture(TEXTUREDIR"SnowFlower2.png", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (!snowTex) {
		return;
	}

	GenerateBuffer();
	waterRotate = 0.0f;
	postEffect = 0;
	GenerateWaterTex();
	GenerateWaterBuffer();

	waterHeight = 256 * HEIGHTMAP_Y / 3.8f;

	FinalWaterShader = new Shader(SHADERDIR"FinalWaterVertex.glsl",
		SHADERDIR"FinalWaterFragment.glsl");
	if (!FinalWaterShader->LinkProgram()) {
		return;
	}
	
	loadNormalHeightMapShader();
}

void Renderer::UpdateScene1(float msec) {
	if (rotateCamera == true) {
		camera->pitch = -20.0f;
		camera->yaw += msec/100;
	}
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 500.000f;
	//test->Update(msec);
	
	for (int i = 0; i < NUM_OF_PARTICLES; ++i) {
		PtlSys[i]->Update(msec);
	}
	
	Vector3 temp;
	if (MoveLight == 1) {
		temp = light->GetPosition();
		temp.x += Distance;
		light->SetPosition(temp);
		MoveLight = 0;
	}
	else if(MoveLight == 2){
		temp = light->GetPosition();
		temp.x -= Distance;
		light->SetPosition(temp);
		MoveLight = 0;
	}
	else if (MoveLight == 3) {
		temp = light->GetPosition();
		temp.z -= Distance;
		light->SetPosition(temp);
		MoveLight = 0;
	}
	else if (MoveLight == 4) {
		temp = light->GetPosition();
		temp.z += Distance;
		light->SetPosition(temp);
		MoveLight = 0;
	}

	laser->Update(msec);
}

void Renderer::RenderScene1(float fps)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	if (showScene3 != 0) {
		DrawTestQuadToFBO();
	}
	DrawScene(fps);
	PresentScene();
	SwapBuffers();
}

void Renderer::SetShaderLight2(Light& l)
{
	unsigned int location = glGetUniformLocation(currentShader->GetProgram(), "lightPos");
	glUniform3fv(location, 1, (float*)&l.GetPosition());
	location = glGetUniformLocation(currentShader->GetProgram(), "lightColour");
	glUniform4fv(location, 1, (float*)&l.GetColour());
	location = glGetUniformLocation(currentShader->GetProgram(), "lightRadius");
	glUniform1f(location, l.GetRadius()); 
}

void Renderer::GenerateTexture()
{
	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glGenTextures(1, &snowTex);
	glBindTexture(GL_TEXTURE_2D, snowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glGenTextures(1, &bufferColourTex);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

void Renderer::GenerateBuffer()
{
	glGenFramebuffers(1, &bufferFBO); // We 'll render the scene into this
	//glGenFramebuffers(1, &processFBO); // And do post processing in this

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex, 0);
	// We can check FBO attachment success using this command !
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE  || !bufferColourTex) {
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawScene(float fps)
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
	
	DrawSkybox();
    DrawHeightMap();
	DrawHeightMapUnderWater();
	DrawWater();
	DrawParticles();
	DrawGUI(fps);
	//DrawTestQuad();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::PresentScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	switch (postEffect)
	{
	case 0:
		SetCurrentShader(sceneShader);
		break;
	case 1:
		SetCurrentShader(reverseColourShader);
		break;
	case 2:
		SetCurrentShader(grayShader);
		break;
	case 3:
		SetCurrentShader(kernelShader);
		break;
	case 4:
		SetCurrentShader(blurShader);
		break;
	case 5:
		SetCurrentShader(edgeShader);
		break;
	default:
		break;
	}
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	texQuad->SetTexture(bufferColourTex);
	if (showScene3 == 0) {
		UpdateShaderMatrices();
		texQuad->Draw();
	}
	else if (showScene3 == 1) {
		modelMatrix = modelMatrix * Matrix4::Translation(Vector3(-1, 0, 0));
		//modelMatrix = modelMatrix * Matrix4::Scale(Vector3(0.5, 1, 1));
		UpdateShaderMatrices();
		texQuad->Draw();
		modelMatrix = modelMatrix * Matrix4::Translation(Vector3(1, 0, 0));
		DrawTestQuad();
	}
	else if (showScene3 == 2) {
		modelMatrix.ToIdentity();
		DrawTestQuad();
	}

	glUseProgram(0);
}

void Renderer::DrawSkybox() {  
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	SetCurrentShader(skyboxShader);
	projMatrix.ToIdentity();
	UpdateShaderMatrices();
	quad->Draw();
	glUseProgram(0);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawHeightMap()
{
	glEnable(GL_CLIP_DISTANCE0);
	SetCurrentShader(waterReflectionShader);
	Renderer::SetShaderLight2(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"secondTex"), 2);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"secondTex"), waterHeight);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	textureMatrix = textureMatrix * Matrix4::Scale(Vector3(2, 4, 4));
	UpdateShaderMatrices();
	//normalHeightMap->Draw();
	heightMap->Draw();
	glDisable(GL_CLIP_DISTANCE0);
	glUseProgram(0);
}

void Renderer::DrawWater()
{
	SetCurrentShader(reflectShader);
	SetShaderLight2(*light);

	quad->SetTexture(waterReflectionTex);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"dudvMap"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	//激活绑定天空盒贴图
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	//X及Z-世界中心位置，Y-水面海拔（在世界最大高度三分之一处）
	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);
	float heightY = 256 * HEIGHTMAP_Y / 3.8f;
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));
	//textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}

void Renderer::DrawParticles()
{
	//glDisable(GL_DEPTH_TEST);
	SetCurrentShader(particleShader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);
	//modelMatrix = Matrix4::Translation(test->GetPosition())*Matrix4::Scale(test->GetScale());
	//UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"snowTex"), 0);
	//test->Draw();
	textureMatrix.ToIdentity();
	for (int i = 0; i < NUM_OF_PARTICLES; ++i) {
		modelMatrix.ToIdentity();
		modelMatrix = Matrix4::Translation(PtlSys[i]->GetPosition())*Matrix4::Scale(PtlSys[i]->GetScale());
		modelMatrix.SetAsUp3Transpose(viewMatrix);
		UpdateShaderMatrices();
		PtlSys[i]->shape->SetTexture(snowTex);
		PtlSys[i]->Draw();
	}
	glUseProgram(0);
	//glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawLaser()
{
	SetCurrentShader(laserShader);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"snowTex"), 0);
	textureMatrix.ToIdentity();
	textureMatrix = textureMatrix * Matrix4::Rotation(90, Vector3(0.0f, 0.0f, 1.0f));
	modelMatrix.ToIdentity();
	modelMatrix = Matrix4::Translation(laser->GetPosition())*Matrix4::Scale(laser ->GetScale());
	modelMatrix.SetAsUp3Transpose(viewMatrix);
	UpdateShaderMatrices();
	laser->Draw();
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Renderer::DrawGUI(float fps)
{
	string temp1, temp = "FPS=";
	stringstream s;
	fps = 60 - (double(GetRandNum(0, 3)) / 100.0);
	s << fps;
	s >> temp1;
	temp1.assign(temp1, 0, 5);
	temp += temp1;
	SetCurrentShader(textShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	DrawText(temp, Vector3(0, 0, 0), 40.0f);
	//DrawText("This is perspective text!!!!", Vector3(0, 0, -1000), 64.0f, true);
	glUseProgram(0);	//That's everything!
}

void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);

	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	}
	else {
		//In ortho mode, we subtract the y from the height, so that a height of 0
		//is at the top left of the screen, which is more intuitive
		//(for me anyway...)
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	//Either way, we update the matrices, and draw the mesh
	UpdateShaderMatrices();
	mesh->Draw();

	delete mesh; //Once it's drawn, we don't need it anymore!
}

void Renderer::GenerateWaterTex()
{
	//Reflection Tex
	glGenTextures(1, &waterReflectionTex);
	glBindTexture(GL_TEXTURE_2D, waterReflectionTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	glGenTextures(1, &waterReflectionDepthTex);
	glBindTexture(GL_TEXTURE_2D, waterReflectionDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	//Refraction Tex
	glGenTextures(1, &waterRefractionTex);
	glBindTexture(GL_TEXTURE_2D, waterRefractionTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &waterRefractionDepthTex);
	glBindTexture(GL_TEXTURE_2D, waterRefractionDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
}

void Renderer::GenerateWaterBuffer()
{
	glGenFramebuffers(1, &waterReflectionBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, waterReflectionBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, waterReflectionDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, waterReflectionTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=GL_FRAMEBUFFER_COMPLETE || !waterReflectionDepthTex ||!waterReflectionTex) {
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &waterRefractionBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, waterRefractionBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, waterRefractionTex, 0);
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, waterRefractionDepthTex, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !waterRefractionDepthTex || !waterRefractionTex) {
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void Renderer::DrawTestQuadToFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, waterReflectionBuffer);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
	DrawSkybox();
	DrawNormalHeightMap();
	//DrawLaser();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawTestQuad()
{
	SetCurrentShader(sceneShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	//modelMatrix = modelMatrix * Matrix4::Translation(Vector3(1,0,0));
	//modelMatrix = modelMatrix * Matrix4::Scale(Vector3(0.5, 0.5, 0.5));
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();
	testQuad->SetTexture(waterReflectionTex);
	testQuad->Draw();
}


void Renderer::DrawHeightMapUnderWater()
{
	SetCurrentShader(FinalWaterShader);
	Renderer::SetShaderLight2(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"secondTex"), 2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"dudvTex"), 3);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"waterHeight"), waterHeight);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();

	heightMap->Draw();
	glUseProgram(0);
}

//Scene2 Functions
void Renderer::InitilizeScene2()
{
	camera2 = new Camera(-20.0f, 180.0f, Vector3(0, 300, -600.0f));
	light2 = new Light(Vector3(-450.0f, 200.0f, 280.0f),
		Vector4(1, 1, 1, 1), 5500.0f);

	rotateCamera = false;
	CamRotSpd = 100;

	hellData = new MD5FileData(MESHDIR"anim/hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim(MESHDIR"anim/idle2.md5anim");
	hellData->AddAnim(MESHDIR"anim/walk7.md5anim");
	hellNode->PlayAnim(MESHDIR"anim/idle2.md5anim");

	laser = new Particles(Vector3(200, 300, 200), Vector3(0, 0, 0), ParticleColour, 2000);
	laser->shape->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"laser.png"
		, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	laserShader = new Shader(SHADERDIR"particleVertex.glsl", SHADERDIR"laserFragment.glsl");

	if (!laserShader->LinkProgram()) {
		return;
	}

	scene2Shader = new Shader(SHADERDIR"shadowscenevert.glsl",
		SHADERDIR"shadowscenefrag.glsl");
	shadowShader = new Shader(SHADERDIR"shadowVert.glsl", SHADERDIR"shadowFrag.glsl");

	if (!scene2Shader->LinkProgram() || !shadowShader->LinkProgram()) {
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

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	animState = 0;
	changeAnim = false;



	//scene3CubeMap = SOIL_load_OGL_cubemap(
	//	MESHDIR"rusted_west.JPG", MESHDIR"rusted_east.JPG",
	//	MESHDIR"rusted_up.JPG", MESHDIR"rusted_down.JPG",
	//	MESHDIR"rusted_south.JPG", MESHDIR"rusted_north.JPG",
	//	SOIL_LOAD_RGB,
	//	SOIL_CREATE_NEW_ID, 0
	//);


	//Scene 7
	//CubeRobot::CreateCube();
	glass = Mesh::GenerateQuad();
	glass->SetTexture(SOIL_load_OGL_texture(
		MESHDIR"stainedglass.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
	scene7Shader = new Shader(SHADERDIR"SceneVertex.glsl",
		SHADERDIR"SceneFragment.glsl");
	if (!scene7Shader->LinkProgram() || !glass->GetTexture()) {
		return;
	}

	root = new SceneNode();
	for (int i = 0; i < 3; ++i) {
		SceneNode * s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
		s->SetTransform(Matrix4::Translation(
			Vector3(0, 100.0f, -500.0f + 100.0f + 100 * i * 1.5)));
		s->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));
		s->SetBoundingRadius(100.0f);
		s->SetMesh(glass);
		root->AddChild(s);
	}

	//root->AddChild(new CubeRobot());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	init = true;
}

void Renderer::UpdateScene2(float msec)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);
	if (rotateCamera == true) {
		//camera2->pitch = -30.0f;
		camera2->yaw += msec / 100;
	}
	camera2->UpdateCamera(msec);
	hellNode->Update(msec);
	ChangeAnimState();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	root->Update(msec);

	laser->Update(msec);

	Vector3 temp;
	if (MoveLight2 == 1) {
		temp = light2->GetPosition();
		temp.x += 20;
		light2->SetPosition(temp);
		MoveLight2 = 0;
	}
	else if (MoveLight2 == 2) {
		temp = light2->GetPosition();
		temp.x -= 20;
		light2->SetPosition(temp);
		MoveLight2 = 0;
	}
}

void Renderer::RenderScene2()
{
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawShadowScene(); // First render pass ...
	DrawCombinedScene(); // Second render pass ...
	//DrawGUI(60.0);

	SwapBuffers();
	ClearNodeLists();
}

void Renderer::DrawScene7()
{
	SetCurrentShader(scene7Shader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	DrawNodes();
	glUseProgram(0);
}

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//adpat viewport to the size of shadow (2048)
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	//?
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	SetCurrentShader(shadowShader);
	//Set current view position to be as light
	viewMatrix = Matrix4::BuildViewMatrix(
		light2->GetPosition(), Vector3(0, 0, 0));
	//?
	textureMatrix = biasMatrix *(projMatrix * viewMatrix);

	UpdateShaderMatrices();

	DrawFloor();
	DrawMesh();
	DrawScene7();

	glUseProgram(0);
	//?
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//recover the view port
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawCombinedScene() {

	viewMatrix = camera2->BuildViewMatrix();
	//DrawSkybox();
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	SetCurrentShader(scene2Shader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"shadowTex"), 2);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera2->GetPosition());

	SetShaderLight(*light2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);


	viewMatrix = camera2->BuildViewMatrix();
	UpdateShaderMatrices();

	DrawFloor();
	DrawMesh();
	DrawScene7();
	if (drawLaser == true) {
		DrawLaser();
	}
	//DrawParticles();
	//DrawGUI(60.0);

	glUseProgram(0);
}

void Renderer::DrawMesh() {
	modelMatrix.ToIdentity();

	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "textureMatrix"), 1, false, *& tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "modelMatrix"), 1, false, *& modelMatrix.values);

	hellNode->Draw(*this);
}

void Renderer::DrawFloor() {
	modelMatrix = Matrix4::Rotation(90, Vector3(1, 0, 0)) *
		Matrix4::Scale(Vector3(450, 450, 1));
	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "textureMatrix"), 1, false, *& tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "modelMatrix"), 1, false, *& modelMatrix.values);

	floor->Draw();
}

void Renderer::ChangeAnimState()
{
	if (changeAnim == false) {
		return;
	}
	else {
		if (animState == 0) {
			changeAnim = false;
			animState = 1;
			hellNode->PlayAnim(MESHDIR"anim/walk7.md5anim");
		}
		else {
			changeAnim = false;
			animState = 0;
			hellNode->PlayAnim(MESHDIR"anim/idle2.md5anim");
		}
	}
}

void Renderer::BuildNodeLists(SceneNode * from) {
	if (frameFrustum.InsideFrustum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() -
			camera2->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);

		}
		else {
			nodeList.push_back(from);

		}

	}

	for (vector < SceneNode * >::const_iterator i =
		from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));

	}
}

void Renderer::SortNodeLists() {
	std::sort(transparentNodeList.begin(),
		transparentNodeList.end(),
		SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(),
		nodeList.end(),
		SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes() {
	for (vector < SceneNode * >::const_iterator i = nodeList.begin();
		i != nodeList.end(); ++i) {
		DrawNode((*i));
	}
	for (vector < SceneNode * >::const_reverse_iterator i =
		transparentNodeList.rbegin();
		i != transparentNodeList.rend(); ++i) {
		DrawNode((*i));
	}
}

void Renderer::DrawNode(SceneNode *n) {
	if (n->GetMesh()) {
		glUniformMatrix4fv(glGetUniformLocation(
			currentShader->GetProgram(), "modelMatrix"), 1, false,
			(float *)&(n->GetWorldTransform()* Matrix4::Scale(n->GetModelScale())));

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
			"nodeColour"), 1, (float *)&n->GetColour());

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
			"useTexture"), (int)n->GetMesh()->GetTexture());

		n->Draw(*this);
	}
}

void Renderer::RenderScene7() {
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	DrawNodes();

	glUseProgram(0);
	SwapBuffers();
	ClearNodeLists();
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::loadNormalHeightMapShader()
{
	normalHeightMapShader = new Shader(SHADERDIR"normalLightVertex.glsl",
		SHADERDIR"normalLightFragment.glsl");
	bumpHeightMapShader = new Shader(SHADERDIR"BumpVertex1.glsl",
		SHADERDIR"BumpFragment1.glsl");
	directionalShader = new Shader(SHADERDIR"normalLightVertex.glsl",
		SHADERDIR"directionalLightFragment.glsl");
	spotLightShader = new Shader(SHADERDIR"normalLightVertex.glsl",
		SHADERDIR"spotLightFragment.glsl");
	if (!normalHeightMapShader->LinkProgram()) {
		return;
	}
	if (!bumpHeightMapShader->LinkProgram()) {
		return;
	}

	if (!directionalShader->LinkProgram()) {
		return;
	}
	if (!spotLightShader->LinkProgram()) {
		return;
	}
}

void Renderer::DrawNormalHeightMap()
{
	glEnable(GL_CLIP_DISTANCE0);
	switch (lightNum)
	{
	case 1:
		SetCurrentShader(normalHeightMapShader);
		break;
	case 2:
		SetCurrentShader(bumpHeightMapShader);
		break;
	case 3:
		SetCurrentShader(directionalShader);
		break;
	case 4:
		SetCurrentShader(spotLightShader);
		break;
	default:
		break;
	}
	Renderer::SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"secondTex"), 2);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(),
		"secondTex"), waterHeight);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();
	//normalHeightMap->Draw();
	normalHeightMap->Draw();
	glDisable(GL_CLIP_DISTANCE0);
	glUseProgram(0);
}

