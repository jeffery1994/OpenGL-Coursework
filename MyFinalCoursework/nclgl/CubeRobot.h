#pragma once
#include "SceneNode.h"
#include "OBJMesh.h"

class CubeRobot : public SceneNode {
public:
	CubeRobot(void);
	~CubeRobot(void) {};
	virtual void Update(float msec);

	static void CreateCube() {
		OBJMesh *m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"cube.obj");
		cube = m;
	}
	static void DeleteCube() { delete cube; }
	void CheckCube()
	{
		if (cube == NULL) {
			cout << "A";
		}
	}

protected:
	static Mesh * cube;
	SceneNode * head;
	SceneNode * leftArm;
	SceneNode * rightArm;
};
