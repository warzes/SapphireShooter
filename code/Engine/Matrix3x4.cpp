#include "stdafx.h"
#include "Matrix3x4.h"

const Matrix3x4 Matrix3x4::ZERO(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f);

const Matrix3x4 Matrix3x4::IDENTITY(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f);

Matrix3x4::Matrix3x4(const Vector3& translation, const Quaternion& rotation, float scale)
{
	SetRotation(rotation.RotationMatrix() * scale);
	SetTranslation(translation);
}

Matrix3x4::Matrix3x4(const Vector3& translation, const Quaternion& rotation, const Vector3& scale)
{
	SetRotation(rotation.RotationMatrix().Scaled(scale));
	SetTranslation(translation);
}

void Matrix3x4::Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const
{
	translation.x = m03;
	translation.y = m13;
	translation.z = m23;

	scale.x = sqrtf(m00 * m00 + m10 * m10 + m20 * m20);
	scale.y = sqrtf(m01 * m01 + m11 * m11 + m21 * m21);
	scale.z = sqrtf(m02 * m02 + m12 * m12 + m22 * m22);

	Vector3 invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
	rotation = Quaternion(ToMatrix3().Scaled(invScale));
}

Matrix3x4 Matrix3x4::Inverse() const
{
	float det = m00 * m11 * m22 +
		m10 * m21 * m02 +
		m20 * m01 * m12 -
		m20 * m11 * m02 -
		m10 * m01 * m22 -
		m00 * m21 * m12;

	float invDet = 1.0f / det;
	Matrix3x4 ret;

	ret.m00 = (m11 * m22 - m21 * m12) * invDet;
	ret.m01 = -(m01 * m22 - m21 * m02) * invDet;
	ret.m02 = (m01 * m12 - m11 * m02) * invDet;
	ret.m03 = -(m03 * ret.m00 + m13 * ret.m01 + m23 * ret.m02);
	ret.m10 = -(m10 * m22 - m20 * m12) * invDet;
	ret.m11 = (m00 * m22 - m20 * m02) * invDet;
	ret.m12 = -(m00 * m12 - m10 * m02) * invDet;
	ret.m13 = -(m03 * ret.m10 + m13 * ret.m11 + m23 * ret.m12);
	ret.m20 = (m10 * m21 - m20 * m11) * invDet;
	ret.m21 = -(m00 * m21 - m20 * m01) * invDet;
	ret.m22 = (m00 * m11 - m10 * m01) * invDet;
	ret.m23 = -(m03 * ret.m20 + m13 * ret.m21 + m23 * ret.m22);

	return ret;
}