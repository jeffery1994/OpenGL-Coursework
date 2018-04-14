#pragma once
# include <string>
# include <iostream>
# include <fstream>
# include "Vector2.h"
# include "Vector3.h"

# include "..\.\nclgl\mesh.h"

# define RAW_WIDTH1 257
# define RAW_HEIGHT1 257

# define HEIGHTMAP_X1 16.0f
# define HEIGHTMAP_Z1 16.0f
# define HEIGHTMAP_Y1 1.25f
# define HEIGHTMAP_TEX_X1 1.0f/16.0f
# define HEIGHTMAP_TEX_Z1 1.0f/16.0f

class HeightMap2 : public Mesh {
public:
	HeightMap2(std::string name);
	~HeightMap2(void) {};
};