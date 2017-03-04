// Ryan Canales Project 5: Texture Mapping

#ifdef _WIN32
#include <Windows.h>
#include <gl/freeglut.h> 

#elif __linux__
#include <GL/glut.h>
#else
#   error "Unknown compiler"
#endif

#include <ctime>
#include "Scene.h"

static Scene scene;

using namespace std;
using namespace vmath;

static void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		glutDestroyWindow(glutGetWindow());
		break;

	case 27:
		glutDestroyWindow(glutGetWindow());
		break;

	case 's' :
		scene.image.writePPM();
		break;
	}
}

static void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(-1, -1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(scene.image.width, scene.image.height, GL_RGB, GL_UNSIGNED_BYTE, scene.image.image);
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	scene.init();

	int choice, samples;
	cout << "Options:\n\t1) Single threaded render\n\t2) Multi-threaded render (not fully working)\n";
	cin >> choice;

	cout << "How many samples per pixel for antialiasing?\n";
	cin >> samples;
	if (samples > 8) cout << "That would take too long to render. Setting to 8 samples.\n";
	else if (samples < 1) {
		cout << "Setting samples to 2\n";
		samples = 2;
	}

	clock_t start;
	double duration;
	start = clock();

	switch (choice)
	{
	case 1:
		scene.render(samples);
		break;
	case 2:
		scene.render_multithreaded(samples);
		break;
	default:
		scene.render(samples);
		break;
	}

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	printf("Render Time: %f sec\n", duration);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(scene.image.width, scene.image.height);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Ryan Canales - Texture Mapping");

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyboardFunc);
	glutMainLoop();
	return 0;
}