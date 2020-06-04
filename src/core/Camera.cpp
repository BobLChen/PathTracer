#include <iostream>

#include "Camera.h"

#include "math/Math.h"

namespace GLSLPT
{
	void Frustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16)
	{
		float temp  = 2.0f * znear;
		float temp2 = right - left;
		float temp3 = top - bottom;
		float temp4 = zfar - znear;

		m16[0] = temp / temp2;
		m16[1] = 0.0;
		m16[2] = 0.0;
		m16[3] = 0.0;
		m16[4] = 0.0;
		m16[5] = temp / temp3;
		m16[6] = 0.0;
		m16[7] = 0.0;
		m16[8] = (right + left) / temp2;
		m16[9] = (top + bottom) / temp3;
		m16[10] = (-zfar - znear) / temp4;
		m16[11] = -1.0f;
		m16[12] = 0.0;
		m16[13] = 0.0;
		m16[14] = (-temp * zfar) / temp4;
		m16[15] = 0.0;
	}

	void Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16)
	{
		float ymax = znear * tanf(fovyInDegrees * PI / 180.0f);
		float xmax = ymax * aspectRatio;
		Frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
	}

	void Cross(const float* a, const float* b, float* r)
	{
		r[0] = a[1] * b[2] - a[2] * b[1];
		r[1] = a[2] * b[0] - a[0] * b[2];
		r[2] = a[0] * b[1] - a[1] * b[0];
	}

	float Dot(const float* a, const float* b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}

	void Normalize(const float* a, float* r)
	{
		float il = 1.f / (sqrtf(Dot(a, a)) + FLT_EPSILON);
		r[0] = a[0] * il;
		r[1] = a[1] * il;
		r[2] = a[2] * il;
	}

	void LookAt(const float* eye, const float* at, const float* up, float* m16)
	{
		float X[3];
		float Y[3];
		float Z[3];
		float tmp[3];

		tmp[0] = eye[0] - at[0];
		tmp[1] = eye[1] - at[1];
		tmp[2] = eye[2] - at[2];

		Normalize(tmp, Z);
		Normalize(up, Y);

		Cross(Y, Z, tmp);
		Normalize(tmp, X);

		Cross(Z, X, tmp);
		Normalize(tmp, Y);

		m16[0] = X[0];
		m16[1] = Y[0];
		m16[2] = Z[0];
		m16[3] = 0.0f;
		m16[4] = X[1];
		m16[5] = Y[1];
		m16[6] = Z[1];
		m16[7] = 0.0f;
		m16[8] = X[2];
		m16[9] = Y[2];
		m16[10] = Z[2];
		m16[11] = 0.0f;
		m16[12] = -Dot(X, eye);
		m16[13] = -Dot(Y, eye);
		m16[14] = -Dot(Z, eye);
		m16[15] = 1.0f;
	}

	Camera::Camera(const glm::vec3& eye, const glm::vec3& lookat, float inFov)
	{
		position = eye;
		pivot    = lookat;
		worldUp  = glm::vec3(0, 1, 0);

		glm::vec3 dir = glm::normalize(pivot - position);

		pitch = glm::degrees(asin(dir.y));
		yaw = glm::degrees(atan2(dir.z, dir.x));
		radius = glm::distance(eye, lookat);
		fov = glm::radians(inFov);
		focalDist = 0.1f;
		aperture = 0.0;
		UpdateCamera();
	}

	Camera::Camera(const Camera& other)
	{
		*this = other;
	}

	Camera& Camera::operator = (const Camera& other)
	{
		position = other.position;
		pivot = other.pivot;
		up = other.up;
		right = other.right;
		forward = other.forward;
		worldUp = other.worldUp;

		pitch = other.pitch;
		yaw = other.yaw;
		fov = other.fov;
		focalDist = other.focalDist;
		aperture = other.aperture;
		radius = other.radius;
		isMoving = other.isMoving;

		return *this;
	}

	void Camera::OffsetOrientation(float dx, float dy)
	{
		pitch -= dy;
		yaw += dx;
		UpdateCamera();
	}

	void Camera::Strafe(float dx, float dy)
	{
		glm::vec3 translation = -dx * right + dy * up;
		pivot = pivot + translation;
		UpdateCamera();
	}

	void Camera::ChangeRadius(float dr)
	{
		radius += dr;
		UpdateCamera();
	}

	void Camera::SetFov(float val)
	{
		fov = glm::radians(val);
	}

	void Camera::UpdateCamera()
	{
		glm::vec3 forward_temp;
		forward_temp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		forward_temp.y = sin(glm::radians(pitch));
		forward_temp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		forward = glm::normalize(forward_temp);
		position = pivot + -forward * radius;

		right = glm::normalize(glm::cross(forward, worldUp));
		up = glm::normalize(glm::cross(right, forward));
	}

	void Camera::ComputeViewProjectionMatrix(float* view, float* projection, float ratio)
	{
		auto at = position + forward;
		LookAt(&position.x, &at.x, &up.x, view);
		float rfov = (1.f / ratio) * tanf(fov / 2.f);
		Perspective(glm::degrees(rfov), ratio, 0.1f, 1000.f, projection);
	}

}