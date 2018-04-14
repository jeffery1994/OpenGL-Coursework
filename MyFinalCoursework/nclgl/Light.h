#pragma once
#include "Vector4.h"
#include "Vector3.h"

class Light {
public:
	//光的位置，颜色和有效范围（用于计算衰减）
	Light()
	{
		position = Vector3(0, 0, 0);
		colour = Vector4(0, 0, 0, 0);
		radius = 0;
	}

	Light(Vector3 position, Vector4 colour, float radius) {
		this->position = position;
		this->colour = colour;
		this->radius = radius;
	}
	~Light(){}

	//返回和设置位置
	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }
	
	//返回和设置光有效范围
	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }

	//范围和设置颜色
	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 val) { colour = val; }

	void SetDirection(Vector3 val) {
		direction = val;
	}
	Vector3 GetDirection() {
		return direction;
	}

	void SetSpotLightCutAngle(float val)
	{
		cutOffAngle = val;
	}
	float GetSpotLightCutAngle()
	{
		return cutOffAngle;
	}

protected:
	Vector3 position;
	Vector4 colour;
	float radius;
	Vector3 direction;
	float cutOffAngle;
};