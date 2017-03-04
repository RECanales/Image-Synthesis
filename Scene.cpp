#include "Scene.h"

using namespace std;
using namespace vmath;

void Scene::init()
{
	printf("Preparing scene...\n");

	// shapes
	vmath::Vector3 Vup(0, 1, 0);
	vmath::Vector3 v2(1, 0, 0);
	float shininess = 40;
	Color specColor(241, 107, 96);
	vector <Quadric> shapest =
	{
		Quadric(Color(128, 60, 60), Color(255, 255, 255), 100, vmath::Vector3(-0.3, 0, 0), Vup, v2, vmath::Vector3(1.1, 1.1, 1.1), 1.0f, 1.0f, 1.0f, 0.0f, -1.0f),	// red sphere
		Quadric(Color(0, 60, 128), Color(255, 255, 255), 100, vmath::Vector3(-1.8, 0, -2), Vup, v2, vmath::Vector3(1, 1, 1), 1.0f, 1.0f, 1.0f, 0.0f, -1.0f),	// blue sphere
		/*
		Quadric(Color(224, 36, 0), specColor, shininess, vmath::Vector3(-0.5, 0.7, 0), Vup, v2, vmath::Vector3(0.99, 0.99, 0.99), 1.0f, 1.0f, 1.0f, 0.0f, -1.0f),
		Quadric(Color(207, 40, 0), specColor, shininess, vmath::Vector3(0.7, 0, -2), Vup, v2, vmath::Vector3(1, 1, 1), 1.0f, 1.0f, 1.0f, 0.0f, -1.0f),
		Quadric(Color(225, 31, 0), specColor, shininess, vmath::Vector3(0, -1.8, -2.0), Vup, v2, vmath::Vector3(1, 1, 1), 1.0f, 1.0f, 1.0f, 0.0f, -1.0f),
		Quadric(Color(67, 78, 204), specColor, shininess, vmath::Vector3(0, 2.6, -3), vmath::Vector3(0.5, 0.87, 0), vmath::Vector3(-0.87, 0.5, 0), vmath::Vector3(2, 1, 2), 1.0f, 1.0f, 1.0f, 0.0f, -1.0f),	// bird body
		Quadric(Color(68, 82, 194), specColor, shininess, vmath::Vector3(1.8, 2, -4.2), Vup, v2, vmath::Vector3(0.8, 0.7, 0.8), 1.0f, 1.0f, 1.0f, 0.0f, -1.0f),	// bird head
		*/
		Quadric(Color(92, 92, 92), specColor, shininess, vmath::Vector3(0, 0, -200), Vup, v2, vmath::Vector3(10, 50, 10), 1, 1, 0, 0, -1),	// cylinder
	};

	Light light4;
	light4.intensity = Color(1, 1, 1);
	light4.position = vmath::Vector3(-8, -8, 17, 0);
	light4.direction = vmath::Vector3(-1 * light4.position).unit();
	light4.ambientVal = 0.25;
	light4.attenuationFactor = 0.0f;
	light4.coneAngle = 30;
	lights.emplace_back(light4);

	Light light5;
	light5.intensity = Color(0.5, 0.5, 0.5);
	light5.position = vmath::Vector3(4, 2, -10);
	light5.direction = vmath::Vector3(-1 * light5.position.unit());
	light5.ambientVal = 0.25;
	light5.attenuationFactor = 0.0f;
	light5.coneAngle = 30.0;
	lights.emplace_back(light5);

	Light light6;
	light6.intensity = Color(0.25, 0.25, 0.25);
	light6.position = vmath::Vector3(5, 0, 10);
	light6.direction = vmath::Vector3(-1 * light6.position.unit());
	light6.ambientVal = 0.25;
	light6.attenuationFactor = 0.0f;
	light6.coneAngle = 30.0;
	lights.emplace_back(light6);

	int t_width = 200;
	int t_height = 200;
	//shader.texture = textureFile.readFile("tex.ppm", t_width, t_height);

	// Scene params
	setShapes(shapest);
	loadCamera();
}

void Scene::loadCamera()
{
	// load camera parameters from file
	ifstream params("params.txt", std::ifstream::in);
	string line;
	int row = 0;
	vector <float> numvec;
	if (params.is_open())
	{
		while (getline(params, line))
		{
			string num;
			for (int i = 0; i < line.size(); ++i) //converts string to double and adds to vector
			{
				char c = line[i];

				if (!isspace(c) && c != '\\') num.push_back(c);
				else
				{
					float value = atof(num.c_str());
					numvec.emplace_back(value);
					num.clear();
				}
			}

			switch (row)
			{
			case 0:
				image.width = numvec[0]; image.height = numvec[1];
				break;
			case 1:
				camera.P0 = vmath::Vector3(numvec[0], numvec[1], numvec[2]);
				break;
			case 2:
				camera.N[2] = vmath::Vector3(numvec[0], numvec[1], numvec[2]).unit();
				break;
			case 3:
				camera.Vup = vmath::Vector3(numvec[0], numvec[1], numvec[2]).unit();
				break;
			case 4:
				camera.fov = numvec[0];
				break;
			}

			numvec.clear();
			row++;
		}

		params.close();
		std::cout << "Camera parameters loaded successfully\n\n";
	}

	camera.s[0] = image.width;
	camera.s[1] = image.height;
	camera.angle = tan(vmath::radians(camera.fov * 0.5));
	camera.ratio = (float)image.width / (float)image.height;
	camera.N[0] = cross(camera.N[2], camera.Vup).unit();
	camera.N[1] = cross(camera.N[0], camera.N[2]).unit();


	Pc = camera.P0 + (camera.N[2] * max(camera.s[0], camera.s[1]));
	Po = Pc - (camera.s[0] / 2) * camera.N[0] - (camera.s[1] / 2) * camera.N[1];

	s = camera.ratio * camera.angle;
	
	image.init();
	image.shadow = Color(61, 12, 4);	// shadow color
	image.bg = Color(30, 75, 36);
}

void Scene::render_multithreaded(int samples)
{
	samplesX = samplesY = samples;
	const float offset = 0.5;
	size_t max = image.width * image.height;
	size_t cores = thread::hardware_concurrency();
	volatile atomic <size_t> count(0);
	vector <future <void> > future_vector;

	// start timer
	printf("Rendering on %i threads...\n", (int)cores);
	
	// render and antialias, multithreaded
	while (cores--)
	{
		future_vector.emplace_back(async([=, &count]()
		{
			while (true)
			{
				size_t index = count++;
				if (index >= max)
					break;

				const int x = index % image.width;
				const int y = index / image.width;
				
				srand((unsigned)time(NULL));
				double rx = (double)rand() / (double)RAND_MAX;
				srand((unsigned)time(NULL));
				double ry = (double)rand() / (double)RAND_MAX;

				image.color = Color(0, 0, 0);

				for (int m = 0; m < samplesX; ++m)
				{
					for (int n = 0; n < samplesY; ++n)
					{
						distance = INFINITY;
						const float X = x + (m + rx) / (float)samplesX;
						const float Y = y + (n + ry) / (float)samplesY;

						position = Po + (X / image.width) * (camera.s[0] * camera.N[0]) + ((image.height - Y) / image.height) * (camera.s[1] * camera.N[1]);
						position.x = position.x * s;
						position.y = position.y * s;

						// camera ray direction
						eyeRay.position = camera.P0;
						eyeRay.direction = Vector3(position - camera.P0).unit();

						for (size_t i = 0; i < shapes.size(); ++i)
						{
							Quadric &Q = shapes[i];
							float dist = sampleRay(eyeRay, t, Q);
							if (dist < distance - offset)
							{
								distance = dist;
								Q.hitPoint = Vector3(eyeRay.position + t * eyeRay.direction);
								Q.get_normal();

								for (size_t j = 0; j < lights.size(); ++j)
								{
									Light &light = lights[j];
									const Vector3 &l_pos = light.position;

									// get shadow ray direction and origin
									const Vector3 &tmp = l_pos - Q.hitPoint;
									shadowRay.direction = 2 * dot(tmp, eyeRay.direction) * eyeRay.direction - tmp;
									shadowRay.position = Q.hitPoint;
									const float toLight = shadowRay.direction.magnitude();
									normalize(&shadowRay.direction);

									if (visible(shadowRay, toLight, t, i, shapes))
										image.color = image.color + shader.Shade(true, light, Q, eyeRay.direction, false);
								}
							}
						}
					}
				}

				image.color = image.color / ((float)samplesX * (float)samplesY);
				if (image.color.r > 255) image.color.r = 255;
				if (image.color.g > 255) image.color.g = 255;
				if (image.color.b > 255) image.color.b = 255;

				image.setPixel(x, y);
			}
		}));
	}
}

void Scene::render(int samples)
{
	samplesX = samplesY = samples;
	const float offset = 0.5;
	Color &color = image.color;

	printf("Rendering on 1 thread...\n");

	// render and antialias
	for (int y = image.height - 1; y >= 0; --y)
	{
		for (int x = 0; x < image.width; ++x)
		{
			srand((unsigned)time(NULL));
			double rx = (double)rand() / (double)RAND_MAX;
			srand((unsigned)time(NULL));
			double ry = (double)rand() / (double)RAND_MAX;

			color = Color(0, 0, 0);
			for (int m = 0; m < samplesX; ++m)
			{
				for (int n = 0; n < samplesY; ++n)
				{
					float distance = INFINITY;
					float X = x + (m + rx) / (float)samplesX;
					float Y = y + (n + ry) / (float)samplesY;

					position = Po + (X / image.width) * (camera.s[0] * camera.N[0]) + ((image.height - Y) / image.height) * (camera.s[1] * camera.N[1]);
					position.x = position.x * s;
					position.y = position.y * s;

					// camera ray direction
					eyeRay.position = camera.P0;
					eyeRay.direction = Vector3(position - camera.P0).unit();

					for (size_t i = 0; i < shapes.size(); ++i)
					{
						Quadric &Q = shapes[i];
						float dist = sampleRay(eyeRay, t, Q);
						if (dist < distance - offset)
						{
							distance = dist;
							Q.hitPoint = Vector3(eyeRay.position + t * eyeRay.direction);
							Q.get_normal();

							for (size_t j = 0; j < lights.size(); ++j)
							{
								Light &light = lights[j];
								const Vector3 &l_pos = light.position;

								// get shadow ray direction and origin
								const Vector3 &tmp = l_pos - Q.hitPoint;
								shadowRay.direction = 2 * dot(tmp, eyeRay.direction) * eyeRay.direction - tmp;
								shadowRay.position = Q.hitPoint;
								const float toLight = shadowRay.direction.magnitude();
								normalize(&shadowRay.direction);

								if (visible(shadowRay, toLight, t, i, shapes))
									//color = color + shader.Shade(true, light, Q, eyeRay.direction, false);
									color = color + shader.Outlines(light, Q, eyeRay.direction, true);
							}
						}
					}
				}
			}

			color = color / ((float)samplesX * (float)samplesY);
			if (color.r > 255) color.r = 255;
			if (color.g > 255) color.g = 255;
			if (color.b > 255) color.b = 255;

			image.setPixel(x, y);
		}
	}
}