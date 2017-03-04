//Ryan Canales, 3D Vector Class header

#include <math.h>
#include <iostream>
#define PI 3.1415926536 

namespace vmath
{
	class Vector3
	{
		public:
			float x, y, z; 	// x,y,z coordinates for vector
			float w; 		// for homogeneous transformation

		// functions
		public:
			Vector3(float vx, float vy, float vz) : x(vx), y(vy), z(vz) {}
			Vector3(float vx, float vy, float vz, float vw) : x(vx), y(vy), z(vz), w(vw) {}
			Vector3() { x = 0; y = 0; z = 0; w = 1; }
			Vector3(const Vector3 &vec) { x = vec.x; y = vec.y; z = vec.z; w = vec.w; }		//copy constructor
			const inline float magnitude() const { return fabs(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2))); }
			const Vector3 unit() const;

			//operators 
			friend Vector3 operator+(const Vector3 &, const Vector3 &);     	//operator overload to add 2 vectors
			friend Vector3 operator-(const Vector3 &, const Vector3 &);     	//operator overload to subtract 2 vectors
			friend Vector3 operator*(const Vector3 &, const float &);        	//operator overload to multiply a vector by a scalar
			friend Vector3 operator*(const float &, const Vector3 &);        	//operator overload to multiply a vector by a scalar
			friend std::ostream& operator<< (std::ostream&, const Vector3&);  	//output stream
			Vector3& operator=(const Vector3&);
			bool operator==(const Vector3&);
			bool operator!=(const Vector3&);
	};

	Vector3 cross(const Vector3 &, const Vector3 &);	// defines the cross product of 2 3d vectors	
	inline float dot(const Vector3 &v1, const Vector3 &v2)	// defines the dot product
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	inline void normalize(const Vector3 *v)
	{
		*(Vector3 *)v = (Vector3)v->unit();
	}

	inline float degrees(float radians)
	{
		return radians * (180 / PI);
	}

	inline float radians(float degrees)
	{
		return degrees * (PI / 180);
	}
}
