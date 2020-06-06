#pragma once

#include "math/Vector3.h"

namespace GLSLPT
{
	class Camera
	{
	public:
		Camera(const Vector3& eye, const Vector3& lookat, float fov);

		Camera(const Camera& other);

		Camera& operator = (const Camera& other);

		void OffsetOrientation(float dx, float dy);
		void Strafe(float dx, float dy);
		void ChangeRadius(float dr);
		void UpdateCamera();
		void ComputeViewProjectionMatrix(float* view, float* projection, float ratio);
		void SetFov(float val);

		Vector3 position;
		Vector3 pivot;
		Vector3 up;
		Vector3 right;
		Vector3 forward;
		Vector3 worldUp;

		float pitch;
		float yaw;
		float fov;
		float focalDist;
		float aperture;
		float radius;
		bool isMoving;
	};
}
