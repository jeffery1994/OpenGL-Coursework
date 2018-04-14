# pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
# include <vector>

/*实际上，每一个Scene Node中都是一个Mesh类对象
举例，如果要创造一个SceneNode中包含三角形的话：
Mesh* triangle = new Mesh();
triangle->generateTriangle();
SceneNode Origin(triangle);
*/
class SceneNode {
public:
	SceneNode(Mesh *m = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
	~SceneNode(void);

	void SetTransform(const Matrix4 & matrix) { transform = matrix; }

	const Matrix4 & GetTransform() const { return transform; }

	Matrix4 GetWorldTransform() const { return worldTransform; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 c) { colour = c; }

	Vector3 GetModelScale() const { return modelScale; }
	void SetModelScale(Vector3 s) { modelScale = s; }

	Mesh * GetMesh() const { return mesh; }
	void SetMesh(Mesh *m) { mesh = m; }

	void AddChild(SceneNode * s);

	virtual void Update(float msec);
	virtual void Draw(OGLRenderer& r);

	std::vector <SceneNode*>::const_iterator GetChildIteratorStart() {
		return children.begin();
	}

	std::vector <SceneNode*>::const_iterator GetChildIteratorEnd() {
		return children.end();
	}

	float GetBoundingRadius() const {
		return boundingRadius;
	}

	void SetBoundingRadius(float f) {
		boundingRadius = f;
	}

	float GetCameraDistance() const {
		return distanceFromCamera;
	}

	void SetCameraDistance(float f) {
		distanceFromCamera = f;
	}

	//在深度测试前比较两个点的深度
	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera) ?
			true : false;
	}

protected:
	//指向自己的从属
	SceneNode * parent;
	//指向自己的具体图形
	Mesh * mesh;
	//转换为世界坐标
	Matrix4 worldTransform;
	//转换为和自己的父节点的相应位置关系
	//即是说将一个子节点转换到世界坐标，就是先将其转换到和父节点存在相应关系的
	//对应位置，然后再用与父节点相同的世界转换坐标，将其转换到世界坐标系
	Matrix4 transform;
	//缩放
	Vector3 modelScale;
	//颜色
	Vector4 colour;
	//将child封装在容器vector中
	std::vector <SceneNode*> children;
	float distanceFromCamera;
	float boundingRadius;
};
