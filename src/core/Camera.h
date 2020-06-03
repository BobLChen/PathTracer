#pragma once

#include <glm/glm.hpp>

namespace GLSLPT
{
	class Camera
	{
	public:
		Camera(const glm::vec3& eye, const glm::vec3& lookat, float fov);

		Camera(const Camera& other);

		Camera& operator = (const Camera& other);

		void OffsetOrientation(float dx, float dy);
		void Strafe(float dx, float dy);
		void ChangeRadius(float dr);
		void UpdateCamera();
		void ComputeViewProjectionMatrix(float* view, float* projection, float ratio);
		void SetFov(float val);

		glm::vec3 position;
		glm::vec3 pivot;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 forward;
		glm::vec3 worldUp;

		float pitch;
		float yaw;
		float fov;
		float focalDist;
		float aperture;
		float radius;
		bool isMoving;
	};
}