#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "vector3d.h"
#include "Color.h"
#include <time.h>

class Ray	// Convinience class; it simply holds a rays position and direction
{
public:
	vmath::Vector3 position, direction;
	Ray(vmath::Vector3 i_pos, vmath::Vector3 i_dir) : position(i_pos), direction(i_dir) {}
	Ray() {};
};


class Quadric	// General quadrics
{
	float a[3][3], s[3];
	vmath::Vector3 N[3];
	vmath::Vector3 center;
public:
	Quadric(Color col, Color sc, float sh, vmath::Vector3 c, vmath::Vector3 vup, vmath::Vector3 v2, vmath::Vector3 scale, float A02, float A12, float A22, float A21, float A00) :
		color(col), specColor(sc), shininess(sh), center(c) {
		a[0][2] = A02; a[1][2] = A12; a[2][2] = A22; a[2][1] = A21; a[0][0] = A00; 
		s[0] = scale.x; s[1] = scale.y; s[2] = scale.z;

		N[0] = vup.unit();
		N[2] = v2.unit();
		N[0] = cross(N[2], vup).unit();
		N[1] = cross(N[0], N[2]).unit();
	};

	Color color, specColor;
	vmath::Vector3 hitPoint, normal;
	float shininess;
	bool intersect(const Ray &ray, float &t);
	void get_normal();
};


class Light
{
public:
	vmath::Vector3 position, direction;
	Color intensity;
	float ambientVal, attenuationFactor, coneAngle;
};


class Shader
{
public:
	Color Shade(const bool &shiny, const Light &light, const Quadric &Q, const vmath::Vector3 &Npe, const bool &sharp);
	Color Outlines(const Light &light, const Quadric &Q, const vmath::Vector3 &Npe, const bool &sharp);
	//unsigned char * texture;
};

//-------- helper functions--------//
//---------------------------------//

inline float sampleRay(const Ray &ray, float &t, Quadric &Q)
{
	if (Q.intersect(ray, t))
	{
		vmath::Vector3 Ph = vmath::Vector3(ray.position + t * ray.direction);
		return (ray.position - Ph).magnitude();
	}

	return INFINITY;
}

// this function checks for an intersection between light and object in scene to determine which objects should be in shadow
inline bool visible(const Ray &shadowRay, const float &dist, float &t, std::size_t &c, std::vector <Quadric> &shapes)
{
	unsigned count = 0;
	for (int i = 0; i < shapes.size(); ++i)
	{
		const float surfaceDist = sampleRay(shadowRay, t, shapes[i]);
		if (dist < surfaceDist)  count++;
		if (count >= c && i <= c) return true;
	}

	return false;
}