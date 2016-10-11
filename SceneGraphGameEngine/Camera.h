#pragma once

#include "Model.h"

typedef std::vector<glm::vec4> Frustum;

class Camera
{
public:
	Camera();
	Camera(const Camera& other);
	~Camera();

	void render(glm::mat4& view, glm::mat4& projection);
	void setPosition(const glm::vec3& pos);
	//void setDirection(const glm::vec3& dir);
	void setFoV(float fov);
	void setHorizontalAngle(float angle);
	void setVerticalAngle(float angle);
	void update();
	void updateDirection();
	Frustum getFrustum() const;

	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;

	glm::vec3 mPosition;
	glm::vec3 mDirection;
	glm::vec3 mRight;
	glm::vec3 mUp;

	float FoV;
	float screenNear;
	float screenDepth;
	float horizontalAngle;
	float verticalAngle;
	float aspectRatio;
};

