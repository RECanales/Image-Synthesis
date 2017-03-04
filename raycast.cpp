#include "raycast.h"

using namespace std;
using namespace vmath;

bool Quadric::intersect(const Ray &ray, float &t)
{
	float A, B, C;

	A = a[0][2] * pow((dot(N[0], ray.direction) / s[0]), 2) + 
		a[1][2] * pow((dot(N[1], ray.direction) / s[1]), 2) +
		a[2][2] * pow((dot(N[2], ray.direction) / s[2]), 2);

	B = a[0][2] * (2.0f * dot(N[0], ray.direction) * dot(N[0], ray.position - center)) / (s[0] * s[0]) +
		a[1][2] * (2.0f * dot(N[1], ray.direction) * dot(N[1], ray.position - center)) / (s[1] * s[1]) +
		a[2][2] * (2.0f * dot(N[2], ray.direction) * dot(N[2], ray.position - center)) / (s[2] * s[2]) +
		a[2][1] * dot(N[2], ray.direction) / s[2];

	C = a[0][2] * pow((dot(N[0], ray.position - center) / s[0]), 2) +
		a[1][2] * pow((dot(N[1], ray.position - center) / s[1]), 2) +
		a[2][2] * pow((dot(N[2], ray.position - center) / s[2]), 2) +
		a[2][1] * (dot(N[2], ray.position - center) / s[2]) + a[0][0];

	float discriminant = (B * B) - 4 * A * C;
	if (discriminant < 0) return false;

	float t1 = (-B - std::sqrt(discriminant)) / (2 * A);
	float t2 = (-B + std::sqrt(discriminant)) / (2 * A);

	t = std::min(t1, t2);
	return true;
}

void Quadric::get_normal()
{
	vmath::Vector3 endTerm = (a[2][1] / s[2]) * N[2];
	vmath::Vector3 d = hitPoint - center;
	normal = vmath::Vector3(0, 0, 0);

	normal = vmath::Vector3((2.0f * a[0][2] * (dot(N[0], d) / (s[0] * s[0]))) * N[0] +
		(2.0f * a[1][2] * (dot(N[1], d) / (s[1] * s[1]))) * N[1] + 
		(2.0f * a[2][2] * (dot(N[2], d) / (s[2] * s[2]))) * N[2] + endTerm).unit();
}

Color Shader::Shade(const bool &shiny, const Light &light, const Quadric &Q, const vmath::Vector3 &Npe, const bool &sharp)
{
	const vmath::Vector3 &P = Q.hitPoint;
	const vmath::Vector3 &N = Q.normal;
	float d;
	vmath::Vector3 surfaceToLight;
	float attenuation;

	// directional light
	if (light.position.w == 0.0)
	{
		surfaceToLight = light.position.unit();
		attenuation = 1.0; // no attenuation
	}

	else
	{
		// point light
		surfaceToLight = vmath::Vector3(P - light.position).unit();
		d = vmath::Vector3(P - light.position).magnitude();
		attenuation = 1.0 / (1.0 + light.attenuationFactor * pow(d, 2));

		// spot light
		float lightToSurfaceAngle = acos(dot(surfaceToLight, light.direction.unit()));
		lightToSurfaceAngle = vmath::degrees(lightToSurfaceAngle);
		if (lightToSurfaceAngle > light.coneAngle) attenuation = 0.0; // directional
	}

	float diffuseVal = std::max(0.0f, dot(N, surfaceToLight));
	Color diffuse = diffuseVal * Q.color * light.intensity;
	Color ambient = light.ambientVal * Q.color * light.intensity;

	//------- specular -------//
	float specularVal = 0.0;
	if (shiny)
	{
		if (diffuseVal > 0)
		{
			vmath::Vector3 reflection;
			float dDotN = dot(surfaceToLight, N);
			if (dDotN == 0) reflection = surfaceToLight;
			else
			{
				reflection = (-2 * dDotN) * N + surfaceToLight;
				specularVal = pow(std::max(0.0f, dot(Npe, reflection)), Q.shininess);
				if (specularVal > 0.2 && sharp) specularVal = 1.0;
			}
		}
	}

	Color specular = specularVal * Q.specColor * light.intensity;
	return ambient + attenuation * (diffuse + specular);
}

Color Shader::Outlines(const Light &light, const Quadric &Q, const vmath::Vector3 &Npe, const bool &sharp)
{
	const vmath::Vector3 &P = Q.hitPoint;
	const vmath::Vector3 &N = Q.normal;
	vmath::normalize(&Npe);
	float angle = vmath::degrees(acos(dot(Npe, N)));	// check angle between ray and normal at intersection
	if (angle > 105) return Shade(true, light, Q, Npe, sharp);
	else return Color(0, 0, 0);
}