#include "stdafx.h"
#include "Matrix3.h"

const Matrix3 Matrix3::ZERO(
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f);

const Matrix3 Matrix3::IDENTITY(
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f);

Matrix3 Matrix3::Inverse() const
{
	float det = m00 * m11 * m22 +
		m10 * m21 * m02 +
		m20 * m01 * m12 -
		m20 * m11 * m02 -
		m10 * m01 * m22 -
		m00 * m21 * m12;

	float invDet = 1.0f / det;

	return Matrix3(
		(m11 * m22 - m21 * m12) * invDet,
		-(m01 * m22 - m21 * m02) * invDet,
		(m01 * m12 - m11 * m02) * invDet,
		-(m10 * m22 - m20 * m12) * invDet,
		(m00 * m22 - m20 * m02) * invDet,
		-(m00 * m12 - m10 * m02) * invDet,
		(m10 * m21 - m20 * m11) * invDet,
		-(m00 * m21 - m20 * m01) * invDet,
		(m00 * m11 - m10 * m01) * invDet
	);
}