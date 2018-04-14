#pragma once
#include "Vector4.h"
#include "Vector3.h"

class Light {
public:
	//���λ�ã���ɫ����Ч��Χ�����ڼ���˥����
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

	//���غ�����λ��
	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }
	
	//���غ����ù���Ч��Χ
	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }

	//��Χ��������ɫ
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