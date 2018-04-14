#pragma once
# include <string>
# include <iostream>
# include <fstream>
# include "Vector2.h"
# include "Vector3.h"

# include "..\.\nclgl\mesh.h"

# define RAW_WIDTH 257
# define RAW_HEIGHT 257

# define HEIGHTMAP_X 30.0f
# define HEIGHTMAP_Z 30.0f
# define HEIGHTMAP_Y 2.5f
# define HEIGHTMAP_TEX_X 1.0f/30.0f
# define HEIGHTMAP_TEX_Z 1.0f/30.0f

class HeightMap : public Mesh {
public:
	HeightMap(std::string name);
	~HeightMap(void) {};
};