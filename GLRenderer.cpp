#include "GLRenderer.h"

#include <GL/gl.h>

#include "Point.h"
#include "Triangle.h"

GLRenderer::GLRenderer()
{
	glEnable(GL_DEPTH_TEST);
}

GLRenderer::~GLRenderer()
{
	glDisable(GL_DEPTH_TEST);
}

void GLRenderer::setGeometry(int w, int h)
{
	glViewport(0, 0, w, h);
}

void GLRenderer::flushToFramebuffer()
{
}

void GLRenderer::clearBuffers()
{
	unsigned char r, g, b;
	color((char*)&r, (char*)&g, (char*)&b);

	glClearColor(r / 255.f, g / 255.f, b / 255.f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::drawLine(const Point3 &p1, const Point3 &p2)
{
	const Matrix &mtx = matrix();

	drawLineNoTransform(Point4(p1) * mtx, Point4(p2) * mtx);
}

void GLRenderer::drawTriangle(const Triangle &t)
{
	const Matrix &mtx = matrix();

	const Point4 tA = Point4(t.vA) * mtx;
	const Point4 tB = Point4(t.vB) * mtx;
	const Point4 tC = Point4(t.vC) * mtx;

	drawTriangleNoTransform(tA, tB, tC);
}

void GLRenderer::drawLineNoTransform(const Point4 &p1, const Point4 &p2)
{
	unsigned char r, g, b;
	color((char*)&r, (char*)&g, (char*)&b);

	glColor4f(r / 255.f, g / 255.f, b / 255.f, 1);
	glBegin(GL_LINES);
		glVertex3f(p1.x / p1.t, p1.y / p1.t, p1.z);
		glVertex3f(p2.x / p2.t, p2.y / p2.t, p2.z);
	glEnd();
}

void GLRenderer::drawTriangleNoTransform(const Point4 &vA, const Point4 &vB,
	const Point4 &vC)
{
	unsigned char r, g, b;
	color((char*)&r, (char*)&g, (char*)&b);

	glColor4f(r / 255.f, g / 255.f, b / 255.f, 1);
	glBegin(GL_TRIANGLES);
		glVertex3f(vA.x / vA.t, vA.y / vA.t, vA.z);
		glVertex3f(vB.x / vB.t, vB.y / vB.t, vB.z);
		glVertex3f(vC.x / vC.t, vC.y / vC.t, vC.z);
	glEnd();
}
