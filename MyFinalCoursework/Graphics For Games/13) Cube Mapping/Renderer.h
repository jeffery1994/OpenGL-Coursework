#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Light.h"
#include "../nclgl/MD5Mesh.h"
#include "../nclgl/MD5FileData.h"
#include "../nclgl/MD5Node.h"
#include "../nclgl/MD5Anim.h"
#include "../nclgl/Particles.h"
#include "../nclgl/common.h"
#include "../nclgl/textMesh.h"
#include "../nclgl/sceneNode.h"
#include ".././nclgl/Frustum.h"
#include <sstream>
#include <iostream>
#include <algorithm>

#define SHADOWSIZE 2048 // New !
const float SKY_HEIGHT = 2000;
const int NUM_OF_PARTICLES = 2500;
const Vector3 velocity(0, -250, 0);
const Vector4 ParticleColour(1, 1, 1, 1);
const Vector3 ParticleScale(RAW_WIDTH * HEIGHTMAP_X / 500, RAW_HEIGHT * HEIGHTMAP_Z / 500, RAW_HEIGHT * HEIGHTMAP_Z / 500);
const int ParticleLifeLength = 10000;//milisecond
const int Distance = 100;

class Renderer : public OGLRenderer {
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);

	bool rotateCamera;
	float CamRotSpd;

	//scene 1
	virtual void RenderScene();
	virtual void UpdateScene(float msec);
	void UpdateScene1(float msec);
	void RenderScene1(float fps);
	int postEffect;
	int MoveLight;

	//scene 2
	void RenderScene2();
	void UpdateScene2(float msec);
	void RenderScene7();
	bool changeAnim;
	int MoveLight2;
	int animState;
	bool drawLaser;

	//scene3
	int showScene3;
	int lightNum;
protected:
	//Scene Switcher system
	void InitializeScene1();

	//Scene1**********************************************************************
	//Draw
	void DrawHeightMap();
	void DrawHeightMapUnderWater();
	void DrawWater();
	void DrawSkybox();
	void DrawParticles();
	void DrawGUI(float fps);
	void DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

	//Post Process Effect
	void DrawScene(float fps);
	void PresentScene();
	void GenerateTexture();
	void GenerateBuffer();


	//Shader
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* sceneShader;
	Shader* reverseColourShader;
	Shader* grayShader;
	Shader* kernelShader;
	Shader* blurShader;
	Shader* edgeShader;
	Shader* particleShader;
	Shader* textShader;

	//Mesh
	Mesh* heightMap;
	Mesh* quad;
	Mesh* texQuad;
	Mesh* testQuad;

	Mesh* normalHeightMap;

	Shader* normalHeightMapShader;
	Shader* bumpHeightMapShader;
	Shader* directionalShader;
	Shader* spotLightShader;

	void loadNormalHeightMapShader();
	void DrawNormalHeightMap();

	//Particles* test;
	Particles* PtlSys[NUM_OF_PARTICLES];

	Camera * camera;
	Light * light;

	//?
	GLuint cubeMap;
	GLuint snowTex;
	float waterRotate;

	void SetShaderLight2(Light& l);

	//Post Effect
	GLuint bufferFBO;
	GLuint bufferColourTex;
	GLuint bufferDepthTex;

	//GUI
	Font* basicFont;

	//Water Reflection
	unsigned int waterReflectionBuffer;
	GLuint waterRefractionBuffer;
	GLuint waterReflectionTex;
	GLuint waterRefractionTex;
	GLuint waterReflectionDepthTex;
	GLuint waterRefractionDepthTex;
	unsigned int rbo; //render buffer object

	void GenerateWaterBuffer();
	void GenerateWaterTex();
	void DrawTestQuadToFBO();
	void DrawTestQuad();
	void DrawReflectionHeightMap();
	void DrawRefractionHeightMap();

	Shader* waterReflectionShader;
	Shader* waterRefractionShader;
	Shader* FinalWaterShader;
	Shader* terribleShader;
	
	float waterHeight;

	//Scene2**********************************************************************
	void InitilizeScene2();
	void DrawMesh();
	void DrawFloor();
	void DrawShadowScene();
	void DrawCombinedScene();

	Shader * scene2Shader;
	Shader * shadowShader;

	Particles* laser;
	void DrawLaser();

	GLuint shadowTex;
	GLuint shadowFBO;

	MD5FileData * hellData;
	MD5Node * hellNode;
	Mesh * floor;

	Camera * camera2;
	Light * light2;

	void ChangeAnimState();

	//scene 7
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	void DrawScene7();


	SceneNode* root;
	Mesh* glass;
	Frustum frameFrustum;
	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;

	Shader* scene7Shader;

	Shader* laserShader;
};
