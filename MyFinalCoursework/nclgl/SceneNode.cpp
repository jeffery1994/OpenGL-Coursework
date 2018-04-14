#include "SceneNode.h"

SceneNode::SceneNode(Mesh* mesh, Vector4 colour)
{
	this->mesh = mesh;
	this->colour = colour;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
}

SceneNode::~SceneNode()
{
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}


//增加子节点，实际上就是传入一个sceneNode类，推入子节点vector容器，并让他的父节点
//指针指向自己
void SceneNode::AddChild(SceneNode* s)
{
	children.push_back(s);
	s->parent = this;
}

//利用递归画出所有的子节点和自己
void SceneNode::Draw(OGLRenderer& r) {
	if (mesh != NULL) {
		mesh->Draw();
	}
}

//Update函数实际上是将物体转换到世界坐标
void SceneNode::Update(float msec) {
	//如果他有父节点，就通过父节点转换到世界坐标系
	if (parent) {
		worldTransform = parent->worldTransform * transform;
	}
	//如果没有，就通过自身的世界转换矩阵转换到世界坐标
	else {
		worldTransform = transform;
	}
	for (vector<SceneNode*>::iterator i = children.begin();i != children.end();++i) {
		(*i)->Update(msec);
	}
}
