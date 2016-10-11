#include "Camera.h"

Camera::Camera()
{
	FoV = 45;
	screenNear = 0.5;
	screenDepth = 1000;
	horizontalAngle = 0;
	verticalAngle = 0;
	aspectRatio = 4.0f / 3.0f;
	mPosition = glm::vec3(0, 0, 0);
	update();
}

Camera::Camera(const Camera& other)
{
	mViewMatrix = other.mViewMatrix;
	mProjectionMatrix = other.mProjectionMatrix;
}

Camera::~Camera()
{
}

void Camera::render(glm::mat4& view, glm::mat4& projection)
{
	update();
	projection = mProjectionMatrix;
	view = mViewMatrix;
}

void Camera::update()
{
	updateDirection();

	// Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	mProjectionMatrix = glm::perspective(FoV, aspectRatio, screenNear, screenDepth);

	// Camera matrix
	mViewMatrix = glm::lookAt(
		mPosition,           // Camera is here
		mPosition + mDirection, // and looks here : at the same position, plus "direction"
		mUp                  // Head is up (set to 0,-1,0 to look upside-down)
		);
}

void Camera::setPosition(const glm::vec3& pos)
{
	mPosition = pos;
}

//void Camera::setDirection(const glm::vec3& dir)
//{
//	direction = dir;
//}

void Camera::setFoV(float fov)
{
	FoV = fov;
}

void Camera::setHorizontalAngle(float angle)
{
	horizontalAngle = angle;
	//updateDirection();
}

void Camera::setVerticalAngle(float angle)
{
	verticalAngle = angle;
	//updateDirection();
}

void Camera::updateDirection()
{
	mDirection = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	mRight = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	mUp = glm::cross(mRight, mDirection);
}

Frustum Camera::getFrustum() const
{
	glm::mat4 mat = mProjectionMatrix*mViewMatrix;
	glm::vec4 frustum[6];
	enum {LEFT,RIGHT,TOP,BOTTOM,FRONT,BACK};
	enum {A,B,C,D};
	
	// Calculate the LEFT side
	frustum[LEFT][A] = mat[0][3] + mat[0][0];
	frustum[LEFT][B] = mat[1][3] + mat[1][0];
	frustum[LEFT][C] = mat[2][3] + mat[2][0];
	frustum[LEFT][D] = mat[3][3] + mat[3][0];
	
	// Calculate the RIGHT side
	frustum[RIGHT][A] = (mat[0][3]) - (mat[0][0]);
	frustum[RIGHT][B] = (mat[1][3]) - (mat[1][0]);
	frustum[RIGHT][C] = (mat[2][3]) - (mat[2][0]);
	frustum[RIGHT][D] = (mat[3][3]) - (mat[3][0]);
	
	// Calculate the TOP side
	frustum[TOP][A] = (mat[0][3]) - (mat[0][1]);
	frustum[TOP][B] = (mat[1][3]) - (mat[1][1]);
	frustum[TOP][C] = (mat[2][3]) - (mat[2][1]);
	frustum[TOP][D] = (mat[3][3]) - (mat[3][1]);

	// Calculate the BOTTOM side
	frustum[BOTTOM][A] = (mat[0][3]) + (mat[0][1]);
	frustum[BOTTOM][B] = (mat[1][3]) + (mat[1][1]);
	frustum[BOTTOM][C] = (mat[2][3]) + (mat[2][1]);
	frustum[BOTTOM][D] = (mat[3][3]) + (mat[3][1]);

	// Calculate the FRONT side
	frustum[FRONT][A] = (mat[0][3]) + (mat[0][2]);
	frustum[FRONT][B] = (mat[1][3]) + (mat[1][2]);
	frustum[FRONT][C] = (mat[2][3]) + (mat[2][2]);
	frustum[FRONT][D] = (mat[3][3]) + (mat[3][2]);

	// Calculate the BACK side
	frustum[BACK][A] = (mat[0][3]) - (mat[0][2]);
	frustum[BACK][B] = (mat[1][3]) - (mat[1][2]);
	frustum[BACK][C] = (mat[2][3]) - (mat[2][2]);
	frustum[BACK][D] = (mat[3][3]) - (mat[3][2]);
	
	Frustum res;
	for (int i = 0;i < 6;i++)
	{
		frustum[i] = glm::normalize(frustum[i]);
		res.push_back(frustum[i]);
	}
	return res;
}

//std::vector<glm::vec4> Camera::getFrustum2()
//{
//	float tanfov = tan(FoV);
//	float nearWidth = tanfov*screenNear;
//	float farWidth = tanfov*screenDepth;
//	float nearHeight = nearWidth;
//	float farHeight = farWidth;
//
//	glm::vec3 nearCenter = position + (direction * screenNear);
//	glm::vec3 farCenter = position + (direction * screenDepth);
//
//	glm::vec3 nearLeftTop = nearCenter - right * nearWidth + up * nearHeight;
//	glm::vec3 nearLeftBottom = nearCenter - right * nearWidth - up * nearHeight;
//	glm::vec3 nearRightTop = nearCenter + right * nearWidth + up * nearHeight;
//	glm::vec3 nearRightBottom = nearCenter + right * nearWidth - up * nearHeight;
//
//	glm::vec3 farLeftTop = farCenter - right * farWidth + up * farHeight;
//	glm::vec3 farLeftBottom = farCenter - right * farWidth - up * farHeight;
//	glm::vec3 farRightTop = farCenter + right * farWidth + up * farHeight;
//	glm::vec3 farRightBottom = farCenter + right * farWidth - up * farHeight;
//
//	glm::vec3 norm[6];
//
//	norm[0] = glm::cross(nearLeftBottom - nearLeftTop, nearRightTop - nearLeftTop);
//
//	norm[1] = glm::cross(farRightTop - farLeftTop, farLeftBottom - farLeftTop);
//
//	norm[2] = glm::cross(nearLeftBottom - nearLeftTop, farLeftTop - nearLeftTop);
//
//	norm[3] = glm::cross(farRightBottom - farRightTop, nearRightTop - farRightTop);
//
//	norm[4] = glm::cross(nearLeftTop - nearRightTop, farRightTop - nearRightTop);
//
//	norm[5] = glm::cross(nearLeftBottom - nearRightBottom, farRightBottom - nearRightBottom);
//
//	std::vector<glm::vec4> res;
//	float dist[6];
//	dist[0] = glm::length(nearCenter);
//	dist[1] = glm::length(farCenter);
//	dist[2] = 0;
//	dist[3] = 0;
//	dist[4] = 0;
//	dist[5] = 0;
//	for (int i = 0;i < 6;i++)
//	{
//		norm[i] = glm::normalize(norm[i]);
//		res.push_back(glm::vec4(norm[i], dist[i]));
//	}
//	return res;
//}
