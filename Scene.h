#pragma once
#include <Windows.h>
#include <thread>
#include <atomic>
#include <future>
#include "raycast.h"
#include "ppm.h"
//#include "pngwriter.h"

class Camera
{
public:
	vmath::Vector3 P0, Vup;
	vmath::Vector3 N[3];
	float fov, angle, ratio;
	float s[2];
};


class Image
{
	PPM ppmFile;
public:
	int width, height;
	unsigned char * image;
	Color color, shadow, bg;

	void init() {
		image = new unsigned char[width * height * 3];
	}

	void setPixel(const int &x, const int &y) {
		int p = (y * width + x) * 3;
		image[p] = (char)color.r;
		image[p + 1] = (char)color.g;
		image[p + 2] = (char)color.b;
	}

	void writePPM() {
		ppmFile.writeFile((char*)image, width, height, "output");
	}

	~Image() { delete image; }
};


static class Scene
{
	// stuff in scene
	Camera camera;
	std::vector <Light> lights;
	std::vector <Quadric> shapes;
	Shader shader;
	Ray eyeRay, shadowRay;
	vmath::Vector3 Pc, Po, position;
	int samplesX, samplesY;
	float s, t, distance;
	//PPM textureFile;

public:
	Scene(Camera &cam, std::vector <Light> &l, std::vector <Quadric> &q) : camera(cam), lights(l), shapes(q) {};
	Scene() {}
	void init();
	void setCamera(Camera &cam) { camera = cam; }
	void loadCamera();
	void setShapes(const std::vector <Quadric> &q) { shapes = q; }
	void setLights(const std::vector <Light> &l) { lights = l; }
	void render_multithreaded(int);
	void render(int);
	Image image;
};