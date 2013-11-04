#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>

#include "Point.h"
#include "Renderer.h"
#include "Triangle.h"

#include "CudaRenderer.h"
#include "GLRenderer.h"
#include "HiddenLineRemovalRenderer.h"
#include "WireframeRenderer.h"

#define FPS_FRAMES_RESOLUTION 100

static Renderer *renderer;

static float faseA = 0;
static float faseB = 0;

void drawCube(int color_shift)
{
	// back
	{
		static Triangle tr1 = { Point3(-1, -1, 1), Point3(1, -1, 1), Point3(1, 1, 1) };
		static Triangle tr2 = { Point3(-1, -1, 1), Point3(1, 1, 1), Point3(-1, 1, 1) };
		renderer->setColor(255 >> color_shift, 0, 255 >> color_shift);
		renderer->drawTriangle(tr1);
		renderer->drawTriangle(tr2);
	}

	// front
	{
		static Triangle tr1 = { Point3(-1, -1, -1), Point3(1, -1, -1), Point3(1, 1, -1) };
		static Triangle tr2 = { Point3(-1, -1, -1), Point3(1, 1, -1), Point3(-1, 1, -1) };
		renderer->setColor(128 >> color_shift, 128 >> color_shift, 128 >> color_shift);
		renderer->drawTriangle(tr1);
		renderer->drawTriangle(tr2);
	}

	// left
	{
		static Triangle tr1 = { Point3(-1, -1, -1), Point3(-1, 1, -1), Point3(-1, 1, 1) };
		static Triangle tr2 = { Point3(-1, -1, -1), Point3(-1, 1, 1), Point3(-1, -1, 1) };
		renderer->setColor(0, 0, 255 >> color_shift);
		renderer->drawTriangle(tr1);
		renderer->drawTriangle(tr2);
	}

	// right
	{
		static Triangle tr1 = { Point3(1, -1, -1), Point3(1, 1, -1), Point3(1, 1, 1) };
		static Triangle tr2 = { Point3(1, -1, -1), Point3(1, 1, 1), Point3(1, -1, 1) };
		renderer->setColor(0, 255 >> color_shift, 255 >> color_shift);
		renderer->drawTriangle(tr1);
		renderer->drawTriangle(tr2);
	}

	// bottom
	{
		static Triangle tr1 = { Point3(-1, -1, -1), Point3(-1, -1, 1), Point3(1, -1, 1) };
		static Triangle tr2 = { Point3(-1, -1, -1), Point3(1, -1, 1), Point3(1, -1, -1) };
		renderer->setColor(255 >> color_shift, 128 >> color_shift, 0);
		renderer->drawTriangle(tr1);
		renderer->drawTriangle(tr2);
	}

	// top
	{
		static Triangle tr1 = { Point3(-1, 1, -1), Point3(-1, 1, 1), Point3(1, 1, 1) };
		static Triangle tr2 = { Point3(-1, 1, -1), Point3(1, 1, 1), Point3(1, 1, -1) };
		renderer->setColor(255 >> color_shift, 0, 0);
		renderer->drawTriangle(tr1);
		renderer->drawTriangle(tr2);
	}
}

void showGlutString(const char *c)
{
	while (*c)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c++);
}

void Display()
{
	renderer->setColor(255, 255, 255);
	renderer->clearBuffers();

	renderer->resetMatrixStack();
	renderer->multMatrix(Matrix::scale(1, 1, .01)); // HACK

	renderer->multMatrix(Matrix::translate(0, 0, 20) * Matrix::perspective(5));

	renderer->multMatrix(Matrix::rotate(faseA, 0, 1, 0));
	renderer->multMatrix(Matrix::rotate(faseB, 1, 0, 0));
	drawCube(0);

	renderer->multMatrix(Matrix::translate(3, 0, 0));
	renderer->multMatrix(Matrix::scale(.5, .5, .5));
	renderer->multMatrix(Matrix::rotate(faseA, 0, 1, 0));
	renderer->multMatrix(Matrix::rotate(faseB, 1, 0, 0));
	drawCube(1);

	renderer->flushToFramebuffer();

	glRasterPos2f(-1, -.95);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	int currTick = glutGet(GLUT_ELAPSED_TIME);
	static int lastTick = 0;
	static int remainingFrames = 0;
	static float elapsed_seconds = 0;

	if (remainingFrames-- == 0)
	{
		elapsed_seconds = ( (lastTick == 0) ? 0.f : ( (currTick - lastTick) / 1000.f ) );
		lastTick = currTick;
		remainingFrames = FPS_FRAMES_RESOLUTION;
	}

	char buffer[100];
	sprintf(buffer, "FPS: %.5g", FPS_FRAMES_RESOLUTION / elapsed_seconds);
	showGlutString(buffer);

	glutSwapBuffers();
}

void Reshape(GLint width, GLint height)
{
	renderer->setGeometry(width, height);
}

void MouseButton(int button, int state, int x, int y)
{
}

void MouseMotion(int x, int y)
{
}

void AnimateScene(int)
{
  faseA += .02;
  faseB += .004;

  // Force redraw
  glutPostRedisplay();
  glutTimerFunc(1000/60, AnimateScene, 0);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'W':
		case 'w':
			//renderer->setDrawMode(Renderer::Wireframe);
			break;
		case 'S':
		case 's':
			//renderer->setDrawMode(Renderer::Solid);
			break;
		case 'H':
		case 'h':
			//renderer->setDrawMode(Renderer::WireframeHiddenLineRemoval);
			break;
	}
}

int main(int argc, char** argv)
{
	// GLUT Window Initialization
	glutInit(&argc, argv);

	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("GLUT example");

	bool error = false;
	if (argc != 2)
		error = true;
	else if (!strcmp("solidcuda", argv[1]))
		renderer = new CudaRenderer();
	else if (!strcmp("wirecuda", argv[1]))
		renderer = new WireframeRenderer(new CudaRenderer());
	else if (!strcmp("hlrcuda", argv[1]))
		renderer = new HiddenLineRemovalRenderer(new CudaRenderer());
	else if (!strcmp("solidgl", argv[1]))
		renderer = new GLRenderer();
	else if (!strcmp("wiregl", argv[1]))
		renderer = new WireframeRenderer(new GLRenderer());
	else if (!strcmp("hlrgl", argv[1]))
		renderer = new HiddenLineRemovalRenderer(new GLRenderer());
	else
		error = true;

	if (error)
	{
		fprintf(stderr, "Uso: %s <solidcuda | wirecuda | hlrcuda | solidgl | wiregl | hlrgl>\n", argv[0]);
		return EXIT_FAILURE;
	}

	Reshape(800, 600);

	// Register callbacks
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	//glutIdleFunc(AnimateScene);
	glutTimerFunc(100, AnimateScene, 0);

	// Turn the flow of control over to GLUT
	glutMainLoop();

	delete renderer;

	return EXIT_SUCCESS;
}
