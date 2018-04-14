#pragma once
#include "OGLRenderer.h"
enum MeshBuffer
{
	//����/��ɫ/��ͼ/���ߣ����ڼ�����գ�/��������������ʹ��ĳһ��mesh��/bump map�е�tangent����/�ܻ�����
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, TANGENT_BUFFER ,MAX_BUFFER
};
class Mesh {
public:
	Mesh();
	~Mesh();

	virtual void Draw();
	//��meshָ��ָ��һ���»���������
	static Mesh* GenerateTriangle();

	static Mesh* Mesh::GenerateParticleQuad();

	static Mesh* Mesh::GenerateLaserQuad();

	static Mesh* GenerateTextQuad();
	//���ô�����ͼ��statemachine
	void SetTexture(GLuint tex) { texture = tex; }

	GLuint GetTexture() { return texture; }

	//�����ı���
	static Mesh* GenerateQuad();

	void SetBumpMap(unsigned int tex) { bumpTexture = tex; }

	unsigned int GetBumpMap() { return bumpTexture; }

	void SetSecondTexture(unsigned int tex) { secondTexture = tex; }

	unsigned int GetSecondTexture() { return secondTexture; }
	unsigned int GetDudvTexture() { return dudvTexture; }

	void SetdudvTexture(unsigned int tex) { dudvTexture = tex; }

protected:
	//�������ݺ���
	void BufferData();

	void GenerateNormals();

	void GenerateTangents();


	Vector3 Mesh::GenerateTangent(const Vector3 &a, const Vector3 &b,
		const Vector3 &c, const Vector2 &ta,
		const Vector2 &tb, const Vector2 &tc);
	//��
	GLuint arrayObject;

	//����������ʵ�statemachine
	GLuint texture;
	//��������
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	//�������ͣ����������Σ��ı��Σ�
	GLuint type;
	//������
	GLuint numIndices;
	
	unsigned int bumpTexture;
	unsigned int secondTexture;
	unsigned int dudvTexture;

	//��
	unsigned int* indices;

	//��ͼ����
	Vector2* textureCoords;

	//��������
	Vector3* vertices;
	//��������
	Vector3* normals;
	//��ɫ����
	Vector4* colours;

	Vector3* tangents;
};