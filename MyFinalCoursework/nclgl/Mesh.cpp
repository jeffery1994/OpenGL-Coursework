#include "Mesh.h"

Mesh::Mesh(void) {
	for (int i = 0; i < MAX_BUFFER; ++i) {
		bufferObject[i] = 0;
	}
	glGenVertexArrays(1, &arrayObject);

	texture = 0;
	bumpTexture = 0;
	numVertices = 0;
	type = GL_TRIANGLES;
	textureCoords = NULL;
	tangents = NULL;
	vertices = NULL;
	colours = NULL;
	normals = NULL;
	indices = NULL;
	numIndices = 0;
}

Mesh ::~Mesh(void) {
	glDeleteVertexArrays(1, &arrayObject);
	glDeleteBuffers(MAX_BUFFER, bufferObject);
	glDeleteTextures(1, &bumpTexture);
	glDeleteTextures(1, &texture);
	delete[] textureCoords;
	delete[] vertices;
	delete[]tangents;
	delete[] colours;
	delete[] normals;
	delete[] indices;
}

//画三角形
Mesh * Mesh::GenerateTriangle() {
	Mesh * m = new Mesh();
	//m -> numVertices = 6
	m->numVertices = 3;
	m->type = GL_TRIANGLES;

	m->vertices = new Vector3[m->numVertices];
	m->vertices[0] = Vector3(-0.0f, 0.5f, 0.0f);
	m->vertices[1] = Vector3(0.5f, -0.5f, 0.0f);
	m->vertices[2] = Vector3(-0.5f, -0.5f, 0.0f);

	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.5f, 0.0f);
	m->textureCoords[1] = Vector2(1.0f, 1.0f);
	m->textureCoords[2] = Vector2(0.0f, 1.0f);

	m->colours = new Vector4[m->numVertices];
	m->colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	m->colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	m->colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

	m->BufferData();
	return m;
}

//画四边形
Mesh* Mesh::GenerateQuad()
{
	Mesh * m = new Mesh();
	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->normals = new Vector3[m->numVertices];
	m->tangents = new Vector3[m->numVertices];

	m->vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = Vector3(-1.0f, 1.0f, 0.0f);
	m->vertices[2] = Vector3(1.0f, -1.0f, 0.0f);
	m->vertices[3] = Vector3(1.0f, 1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);

	for (unsigned int i = 0; i < m->numVertices; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		//由此可知平面是和x-y平面平行的
		m->normals[i] = Vector3(0.0f, 0.0f, -1.0f);
		//业界规定的x正方向单位向量为tangent
		m->tangents[i] = Vector3(1.0f, 0.0f, 0.0f);
	}

	m->BufferData();
	return m;
}

Mesh* Mesh::GenerateLaserQuad()
{
	Mesh * m = new Mesh();
	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->normals = new Vector3[m->numVertices];
	m->tangents = new Vector3[m->numVertices];

	m->vertices[0] = Vector3(-80.0f, -400.0f, 0.0f);
	m->vertices[1] = Vector3(-80.0f, 400.0f, 0.0f);
	m->vertices[2] = Vector3(80.0f, -400.0f, 0.0f);
	m->vertices[3] = Vector3(80.0f, 400.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);

	for (unsigned int i = 0; i < m->numVertices; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		//由此可知平面是和x-y平面平行的
		m->normals[i] = Vector3(0.0f, 0.0f, -1.0f);
		//业界规定的x正方向单位向量为tangent
		m->tangents[i] = Vector3(1.0f, 0.0f, 0.0f);
	}

	m->BufferData();
	return m;
}

Mesh* Mesh::GenerateParticleQuad() {
	Mesh * m = new Mesh();
	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->normals = new Vector3[m->numVertices];
	m->tangents = new Vector3[m->numVertices];

	m->vertices[0] = Vector3(-10.0f, -10.0f, 0.0f);
	m->vertices[1] = Vector3(-10.0f, 10.0f, 0.0f);
	m->vertices[2] = Vector3(10.0f, -10.0f, 0.0f);
	m->vertices[3] = Vector3(10.0f, 10.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);

	for (unsigned int i = 0; i < m->numVertices; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		//由此可知平面是和x-y平面平行的
		m->normals[i] = Vector3(0.0f, 0.0f, -1.0f);
		//业界规定的x正方向单位向量为tangent
		m->tangents[i] = Vector3(1.0f, 0.0f, 0.0f);
	}

	m->BufferData();
	return m;
}

Mesh* Mesh::GenerateTextQuad() {
	Mesh * m = new Mesh();
	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->normals = new Vector3[m->numVertices];
	m->tangents = new Vector3[m->numVertices];

	m->vertices[0] = Vector3(-1.0f, 1.0f, 0.0f);
	m->vertices[1] = Vector3(-1.0f, 0.5f, 0.0f);
	m->vertices[3] = Vector3(-0.5f, 0.5f, 0.0f);
	m->vertices[2] = Vector3(-0.5f, 1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);

	for (unsigned int i = 0; i < m->numVertices; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		//由此可知平面是和x-y平面平行的
		m->normals[i] = Vector3(0.0f, 0.0f, -1.0f);
		//业界规定的x正方向单位向量为tangent
		m->tangents[i] = Vector3(1.0f, 0.0f, 0.0f);
	}

	m->BufferData();
	return m;
}

void Mesh::GenerateNormals()
{
	if (!normals) {
		normals = new Vector3[numVertices];
	}
	for (unsigned int i = 0; i < numVertices; ++i) {
		//初始化全部法向量为0
		normals[i] = Vector3();
	}
	if (indices) {
		for (unsigned int i = 0; i < numIndices; i+=3) {
			unsigned int a = indices[i];
			unsigned int b = indices[i+1];
			unsigned int c = indices[i+2];

			//Experiment
			//向量ba 和向量 ca, 计算出三角形平面abc的法向量normal, “逆时针顺序” 
			Vector3 normal = Vector3::Cross( (vertices[b] - vertices[a]), (vertices[c] - vertices[a]));

			//将组成这个三角形平面的三个顶点法向量都设置为normal，因为法向量初始化为0了，所以直接+=即可
			normals[a] += normal;
			normals[b] += normal;
			normals[c] += normal;
		}
	}
	else {
		for (unsigned int i = 0; i < numVertices; i += 3) {
			Vector3 &a = vertices[i];
			Vector3 &b = vertices[i];
			Vector3 &c = vertices[i];

			Vector3 normal = Vector3::Cross(b - a, c - a);

			normals[i] = normal;
			normals[i + 1] = normal;
			normals[i + 2] = normal;
		}
	}
}

void Mesh::GenerateTangents()
{
	if (!tangents) {
		 tangents = new Vector3[numVertices];
		
	}
	 for (GLuint i = 0; i < numVertices; ++i) {
		 tangents[i] = Vector3();
	}
	
		 if (indices) {
		 for (GLuint i = 0; i < numIndices; i += 3) {
			 int a = indices[i];
			 int b = indices[i + 1];
			 int c = indices[i + 2];
			
			Vector3 tangent = GenerateTangent(vertices[a], vertices[b],
					 vertices[c], textureCoords[a],
					 textureCoords[b], textureCoords[c]);
			
			 tangents[a] += tangent;
			 tangents[b] += tangent;
			 tangents[c] += tangent;
			
		}
		
	}
	 else {
		 for (GLuint i = 0; i < numVertices; i += 3) {
			 Vector3 tangent = GenerateTangent(vertices[i], vertices[i + 1],
				 vertices[i + 2], textureCoords[i],
				 textureCoords[i + 1], textureCoords[i + 2]);
			
				 tangents[i] += tangent;
			 tangents[i + 1] += tangent;
			 tangents[i + 2] += tangent;
		}
		
	}
	 for (GLuint i = 0; i < numVertices; ++i) {
		 tangents[i].Normalise();
	}
}

Vector3 Mesh::GenerateTangent(const Vector3 &a, const Vector3 &b,
	 const Vector3 &c, const Vector2 &ta,
	 const Vector2 &tb, const Vector2 &tc) {
	 Vector2 coord1 = tb - ta;
	 Vector2 coord2 = tc - ta;
	
		 Vector3 vertex1 = b - a;
	 Vector3 vertex2 = c - a;
	
		 Vector3 axis = Vector3(vertex1 * coord2.y - vertex2 * coord1.y);
	
		 float factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);
	
		 return axis * factor;
	
}

void Mesh::BufferData() {
	//缓存顶点数据到GPU RAM
	glBindVertexArray(arrayObject);
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3),
		vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_BUFFER);

	//缓存纹理数据
	if (textureCoords) {
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2),
			textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}

	//缓存颜色数据
	if (colours) { // Just in case the data has no colour attribute ...
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector4),
			colours, GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}
	if (indices) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
	}
	if (normals) {
		glGenBuffers(1, &bufferObject[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMAL_BUFFER);
	}
	if (tangents) {
		glGenBuffers(1, &bufferObject[TANGENT_BUFFER]);
		 glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		 glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3),
			 tangents, GL_STATIC_DRAW);
		 glVertexAttribPointer(TANGENT_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);
		 glEnableVertexAttribArray(TANGENT_BUFFER);
	}
	glBindVertexArray(0);
}

void Mesh::Draw() {
	glActiveTexture(GL_TEXTURE0);
	//绑定一级材质
	glBindTexture(GL_TEXTURE_2D, texture);

	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bumpTexture);

	//绑定二级材质
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, secondTexture);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, dudvTexture);
	//绑定顶点
	glBindVertexArray(arrayObject);
	if (bufferObject[INDEX_BUFFER]) {
		//如果存在indexBuffer
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	}
	else {
		//根据传入的类型画N组顶点
		glDrawArrays(type, 0, numVertices);
	}
	//解绑顶点
	glBindVertexArray(0);
	//解绑定材质
	glBindTexture(GL_TEXTURE_2D, 0);
}
