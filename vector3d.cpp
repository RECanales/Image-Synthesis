//Ryan Canales, 3D Vector Class implementation

#include "vector3d.h"

namespace vmath
{
	const Vector3 Vector3::unit() const
	{
		float mag = this->magnitude();
		if (mag == 1) return *this;

		if (mag >= 0) return Vector3(x / mag, y / mag, z / mag);
		else throw "Exception: Vector length not greater than zero\n";
	}

	Vector3 operator+(const Vector3 &a, const Vector3 &b)
	{
		Vector3 tmp(a.x + b.x, a.y + b.y, a.z + b.z);
		return tmp;
	}

	Vector3 operator-(const Vector3 &a, const Vector3 &b)
	{
		Vector3 tmp(a.x - b.x, a.y - b.y, a.z - b.z);
		return tmp;
	}


	Vector3 operator*(const Vector3 &a, const float &c)
	{
		Vector3 tmp(c * a.x, c * a.y, c * a.z);
		return tmp;
	}

	Vector3 operator*(const float &c, const Vector3 &a)
	{
		Vector3 tmp(c * a.x, c * a.y, c * a.z);
		return tmp;
	}

	Vector3& Vector3::operator=(const Vector3& a)
	{
		if (this == &a)
			return *this;
		this->x = a.x;
		this->y = a.y;
		this->z = a.z;
		return *this;
	}

	bool Vector3::operator==(const Vector3& a)
	{
		if (this->x == a.x && this->y == a.y && this->z == a.z) return true;
		return false;
	}

	bool Vector3::operator!=(const Vector3& a)
	{
		if (this->x != a.x && this->y != a.y && this->z != a.z) return true;
		return false;
	}

	std::ostream& operator<<(std::ostream& os, const Vector3& v)
	{
		//write vector to output stream
		os << "[" << v.x << " " << v.y << " " << v.z << "]";

		return os;
	}

	// helper function definitions
	Vector3 cross(const Vector3 &v1, const Vector3 &v2)
	{
		Vector3 result
		(
			v1.y * v2.z - v2.y * v1.z,
			v2.x * v1.z - v1.x * v2.z,
			v1.x * v2.y - v2.x * v1.y
		);

		return result;
	}
}
