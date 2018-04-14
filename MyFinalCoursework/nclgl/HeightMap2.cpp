#include "HeightMap2.h"

HeightMap2::HeightMap2(std::string name) {
	ifstream file(name.c_str(), ios::binary);
	if (!file) {
		return;
	}
	numVertices = RAW_WIDTH1*RAW_HEIGHT1;
	numIndices = (RAW_WIDTH1 - 1)*(RAW_HEIGHT1 - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];
	unsigned char*data = new unsigned char[numVertices];
	file.read((char*)data, numVertices * sizeof(unsigned char));
	file.close();

	for (int x = 0; x < RAW_WIDTH1; ++x) {
		for (int z = 0; z < RAW_HEIGHT1; ++z) {
			int offset = (x*RAW_WIDTH1) + z;

			vertices[offset] = Vector3(x*HEIGHTMAP_X1, data[offset] * HEIGHTMAP_Y1, z*HEIGHTMAP_Z1);
			textureCoords[offset] = Vector2(x * HEIGHTMAP_TEX_X1, z * HEIGHTMAP_TEX_Z1);
		}
	}

	delete data;
	numIndices = 0;

	for (int x = 0; x < RAW_WIDTH1 - 1; ++x) {
		for (int z = 0; z < RAW_HEIGHT1 - 1; ++z) {
			int a = (x * (RAW_WIDTH1)) + z;
			int b = ((x + 1) * (RAW_WIDTH1)) + z;
			int c = ((x + 1) * (RAW_WIDTH1)) + (z + 1);
			int d = (x * (RAW_WIDTH1)) + (z + 1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
		}
	}

	//生成法向量和tangent向量，用于光照计算
	GenerateNormals();
	GenerateTangents();
	BufferData();
}