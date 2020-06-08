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

#include <iostream>

#include "Camera.h"

#include "math/Math.h"

namespace GLSLPT
{
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
		float il = 1.f / (sqrtf(Dot(a, a)) + 0.0001f);
		r[0] = a[0] * il;
		r[1] = a[1] * il;
		r[2] = a[2] * il;
	}

	void GizmoFrustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16)
	{
		float temp, temp2, temp3, temp4;
		temp = 2.0f * znear;
		temp2 = right - left;
		temp3 = top - bottom;
		temp4 = zfar - znear;
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

	void GizmoPerspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16)
	{
		float ymax, xmax;
		ymax = znear * tanf(fovyInDegrees * 3.141592f / 180.0f);
		xmax = ymax * aspectRatio;
		GizmoFrustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
	}

	void GizmoLookAt(const float* eye, const float* at, const float* up, float* m16)
	{
		float X[3], Y[3], Z[3], tmp[3];

		tmp[0] = eye[0] - at[0];
		tmp[1] = eye[1] - at[1];
		tmp[2] = eye[2] - at[2];
		//Z.normalize(eye - at);
		Normalize(tmp, Z);
		Normalize(up, Y);
		//Y.normalize(up);

		Cross(Y, Z, tmp);
		//tmp.cross(Y, Z);
		Normalize(tmp, X);
		//X.normalize(tmp);

		Cross(Z, X, tmp);
		//tmp.cross(Z, X);
		Normalize(tmp, Y);
		//Y.normalize(tmp);

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

	Camera::Camera()
	{
		SetPosition(Vector3(0.0f, 0.0f, -10.0f));
	}

	Camera::Camera(const Vector3& eye, const Vector3& lookat, float inFov)
	{
		SetPosition(eye);
		LookAt(lookat);
		m_Fov = MMath::DegreesToRadians(inFov);
	}

	void Camera::GetGizmoViewProjection(float* view, float* projection)
	{
		Vector3 at = GetPosition() + GetForward();
		Vector3 p  = GetPosition();
		Vector3 up = GetUp();
		GizmoLookAt((float*)&p, (float*)&at, (float*)&up, view);

		float ratio = m_Width / m_Height;
		float fov_v = (1.f / ratio) * MMath::Tan(m_Fov / 2.f);
		GizmoPerspective(MMath::RadiansToDegrees(fov_v), ratio, m_Near, m_Far, projection);
	}

	void Camera::OnMousePos(const Vector2 mousePos)
	{
		m_CurrMouse  = mousePos;
	}

	void Camera::OnRMouse(bool down)
	{
		m_RMouseDown = down;
	}

	void Camera::OnMMouse(bool down)
	{
		m_MMouseDown = down;
	}

	void Camera::OnMouseWheel(float wheel)
	{
		m_MouseWheel = wheel;
	}

	void Camera::Update(float delta)
	{
		float mouseSpeedX = m_CurrMouse.x - m_LastMouse.x;
		float mouseSpeedY = m_CurrMouse.y - m_LastMouse.y;

		if (m_MMouseDown)
		{
			m_World.TranslateX(+mouseSpeedX * m_World.GetOrigin().Size() / 300);
			m_World.TranslateY(+mouseSpeedY * m_World.GetOrigin().Size() / 300);
		}
		else if (m_RMouseDown)
		{
			m_SpinX += -mouseSpeedX * smooth * speedFactor;
			m_SpinY += +mouseSpeedY * smooth * speedFactor;
		}

		if (m_MouseWheel != 0.0f) 
		{
			m_SpinZ = (m_World.GetOrigin().Size() + 0.1f) * speedFactor * m_MouseWheel / 20.0f;
		}

		m_World.TranslateZ(m_SpinZ);
		m_World.RotateY(m_SpinX, false, &Vector3::ZeroVector);
		m_World.RotateX(m_SpinY, true,  &Vector3::ZeroVector);

		if ((m_SpinX != 0.0f || m_SpinY != 0.0f || m_SpinZ != 0.0f) || m_MMouseDown) 
		{
			isMoving = true;
		}

		m_SpinX *= (1 - smooth);
		m_SpinY *= (1 - smooth);
		m_SpinZ *= (1 - smooth);

		m_LastMouse = m_CurrMouse;
	}

}
