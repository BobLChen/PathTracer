/**********************************************************************
Copyright (c) 2020 BobLChen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************/

#pragma once

#include "math/Matrix4x4.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

namespace GLSLPT
{
	class Camera
	{
	public:
		Camera(const Vector3& eye, const Vector3& lookat, float fov);

		Camera();

		inline void TranslateX(float distance)
		{
			m_World.TranslateX(distance);
		}

		inline void TranslateY(float distance)
		{
			m_World.TranslateY(distance);
		}

		inline void TranslateZ(float distance)
		{
			m_World.TranslateZ(distance);
		}

		inline void RotateX(float angle)
		{
			m_World.RotateX(angle);
		}

		inline void RotateY(float angle)
		{
			m_World.RotateY(angle);
		}

		inline void RotateZ(float angle)
		{
			m_World.RotateZ(angle);
		}

		inline void LookAt(float x, float y, float z, float smooth = 1.0f)
		{
			LookAt(Vector3(x, y, z), smooth);
		}

		inline void LookAt(const Vector3& target, float smooth = 1.0f)
		{
			m_World.LookAt(target, nullptr, smooth);
		}

		inline void LookAt(const Vector3& target, const Vector3& up, float smooth = 1.0f)
		{
			m_World.LookAt(target, &up, smooth);
		}

		inline Vector3 GetPosition()
		{
			return m_World.GetOrigin();
		}

		inline void SetPosition(const Vector3& pos)
		{
			m_World.SetPosition(pos);
		}

		inline void SetPosition(float x, float y, float z)
		{
			m_World.SetPosition(Vector3(x, y, z));
		}

		inline void SetOrientation(const Vector3& dir)
		{
			m_World.SetOrientation(dir, &Vector3::UpVector, 1.0f);
		}

		inline void SetRotation(const Vector3& rotation)
		{
			m_World.SetRotation(rotation);
		}

		inline void SetRotation(float eulerX, float eulerY, float eulerZ)
		{
			m_World.SetRotation(Vector3(eulerX, eulerY, eulerZ));
		}

		inline Vector3 GetRight() const
		{
			return m_World.GetRight();
		}

		inline Vector3 GetUp() const
		{
			return m_World.GetUp();
		}

		inline Vector3 GetForward() const
		{
			return m_World.GetForward();
		}

		inline Vector3 GetLeft() const
		{
			return m_World.GetLeft();
		}

		inline Vector3 GetBackward() const
		{
			return m_World.GetBackward();
		}

		inline Vector3 GetDown() const
		{
			return m_World.GetDown();
		}

		inline const Matrix4x4& GetView()
		{
			m_View = m_World.Inverse();
			return m_View;
		}

		inline const Matrix4x4& GetProjection()
		{
			return m_Projection;
		}

		inline const Matrix4x4& GetViewProjection()
		{
			m_View = m_World.Inverse();
			m_ViewProjection = m_View * m_Projection;
			return m_ViewProjection;
		}

		inline void SetTransform(const Matrix4x4& world)
		{
			m_World = world;
		}

		inline const Matrix4x4& GetTransform()
		{
			return m_World;
		}

		inline void Perspective(float fovy, float width, float height, float zNear, float zFar)
		{
			m_Fov    = fovy;
			m_Near   = zNear;
			m_Far    = zFar;
			m_Aspect = width / height;
			m_Width  = width;
			m_Height = height;

			m_Projection.Perspective(fovy, width, height, zNear, zFar);
		}

		inline void SetFov(float fov)
		{
			m_Fov = fov;
			m_Projection.Perspective(m_Fov, m_Width, m_Height, m_Near, m_Far);
		}

		inline float GetNear() const
		{
			return m_Near;
		}

		inline float GetFar() const
		{
			return m_Far;
		}

		inline float GetFov() const
		{
			return m_Fov;
		}

		inline float GetAspect() const
		{
			return m_Aspect;
		}

		void GetGizmoViewProjection(float* view, float* projection);

		void Update(float delta);

		void OnMousePos(const Vector2 mousePos);

		void OnRMouse(bool down);

		void OnMMouse(bool down);

		void OnMouseWheel(float wheel);

	public:

		float		smooth = 1.0f;
		float		speed = 1.0f;
		float		speedFactor = 0.5f;

		float		focalDist = 1.0f;
		float		aperture = 0.0f;

		bool        isMoving = false;

	protected:

		bool        m_RMouseDown = false;
		bool        m_MMouseDown = false;

		Vector2		m_LastMouse;
		Vector2     m_CurrMouse;
		float		m_MouseWheel = 0.0f;

		float		m_SpinX = 0.0f;
		float		m_SpinY = 0.0f;
		float		m_SpinZ = 0.0f;
		
		Matrix4x4	m_World;
		Matrix4x4	m_View;
		Matrix4x4	m_Projection;
		Matrix4x4	m_ViewProjection;

		float		m_Near = 1.0f;
		float		m_Far = 3000.0f;
		float       m_Width = 1400.0f;
		float       m_Height = 900.0f;

		// Perspective
		float		m_Fov = PI / 4.0f;
		float		m_Aspect = 1.0f;
	};

}
