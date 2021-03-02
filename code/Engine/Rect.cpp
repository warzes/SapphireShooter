#include "stdafx.h"
#include "Rect.h"

const Rect Rect::FULL(-1.0f, -1.0f, 1.0f, 1.0f);
const Rect Rect::POSITIVE(0.0f, 0.0f, 1.0f, 1.0f);
const Rect Rect::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

void Rect::Clip(const Rect& rect)
{
	if (rect.min.x > min.x)
		min.x = rect.min.x;
	if (rect.max.x < max.x)
		max.x = rect.max.x;
	if (rect.min.y > min.y)
		min.y = rect.min.y;
	if (rect.max.y < max.y)
		max.y = rect.max.y;

	if (min.x > max.x)
		std::swap(min.x, max.x);
	if (min.y > max.y)
		std::swap(min.y, max.y);
}