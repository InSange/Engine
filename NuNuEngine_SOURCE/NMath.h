#pragma once

namespace NuNu::math
{
	struct Vector2
	{
		static Vector2 One;
		static Vector2 Zero;

		float x;
		float y;

		Vector2()
			: x(0.0f)
			, y(0.0f)
		{

		}

		Vector2(float _x, float _y)
			: x(_x)
			, y(_y)
		{

		}
		Vector2 operator+ (Vector2 other)
		{
			return Vector2(x + other.x, y + other.y);
		}
		Vector2 operator- (Vector2 other)
		{
			return Vector2(x - other.x, y - other.y);
		}
		Vector2 operator/ (float v)
		{
			return Vector2(x / v, y / v);
		}
	};
}

