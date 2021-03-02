#pragma once

class Point2
{
public:
	static const Point2 Zero;

	Point2() = default;
	Point2(const Point2& p) : x(p.x), y(p.y) {}
	Point2(int nx, int ny) : x(nx), y(ny) {}

	Point2& operator+=(const Point2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Point2& operator-=(const Point2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Point2 operator+(const Point2& rhs) const { return Point2(x + rhs.x, y + rhs.y); }
	Point2 operator-() const { return Point2(-x, -y); }
	Point2 operator-(const Point2& rhs) const { return Point2(x - rhs.x, y - rhs.y); }

	bool operator==(const Point2& rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Point2& rhs) const { return !(*this == rhs); }

	int x = 0;
	int y = 0;
};