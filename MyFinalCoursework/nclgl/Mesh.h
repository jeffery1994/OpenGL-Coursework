#pragma once
#include "OGLRenderer.h"
enum MeshBuffer
{
	//顶点/颜色/贴图/法线（用于计算光照）/索引（用来批量使用某一个mesh）/bump map中的tangent缓存/总缓存数
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, TANGENT_BUFFER ,MAX_BUFFER
};
class Mesh {
public:
	Mesh();
	~Mesh();

	virtual void Draw();
	//让mesh指针指向一个新画的三角形
	static Mesh* GenerateTriangle();

	static Mesh* Mesh::GenerateParticleQuad();

	static Mesh* Mesh::GenerateLaserQuad();

	static Mesh* GenerateTextQuad();
	//设置处理贴图的statemachine
	void SetTexture(GLuint tex) { texture = tex; }

	GLuint GetTexture() { return texture; }

	//创建四边形
	static Mesh* GenerateQuad();

	void SetBumpMap(unsigned int tex) { bumpTexture = tex; }

	unsigned int GetBumpMap() { return bumpTexture; }

	void SetSecondTexture(unsigned int tex) { secondTexture = tex; }

	unsigned int GetSecondTexture() { return secondTexture; }
	unsigned int GetDudvTexture() { return dudvTexture; }

	void SetdudvTexture(unsigned int tex) { dudvTexture = tex; }

protected:
	//缓存数据函数
	void BufferData();

	void GenerateNormals();

	void GenerateTangents();


	Vector3 Mesh::GenerateTangent(const Vector3 &a, const Vector3 &b,
		const Vector3 &c, const Vector2 &ta,
		const Vector2 &tb, const Vector2 &tc);
	//？
	GLuint arrayObject;

	//用来处理材质的statemachine
	GLuint texture;
	//缓存数组
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	//网格类型（例如三角形，四边形）
	GLuint type;
	//顶点数
	GLuint numIndices;
	
	unsigned int bumpTexture;
	unsigned int secondTexture;
	unsigned int dudvTexture;

	//？
	unsigned int* indices;

	//贴图坐标
	Vector2* textureCoords;

	//顶点坐标
	Vector3* vertices;
	//法线数据
	Vector3* normals;
	//颜色坐标
	Vector4* colours;

	Vector3* tangents;
};