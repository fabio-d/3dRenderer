#ifndef BASERENDERER_H
#define BASERENDERER_H

#include <list>

#include "Matrix.h"
#include "Renderer.h"

/* Base class for rendering engines */
class BaseRenderer : public Renderer
{
	public:
		BaseRenderer();
		virtual ~BaseRenderer();

		virtual void setGeometry(int width, int height) = 0;
		virtual void flushToFramebuffer() = 0;

		virtual void resetMatrixStack();
		virtual void pushMatrix();
		virtual void popMatrix();
		virtual void multMatrix(const Matrix &mtx);
		virtual const Matrix& matrix() const;

		virtual void setColor(char r, char g, char b);
		virtual void color(char *r, char *g, char *b) const;

		virtual void clearBuffers() = 0;
		virtual void drawLine(const Point3 &p1, const Point3 &p2) = 0;
		virtual void drawTriangle(const Triangle &triangle) = 0;

	private:
		char color_r, color_g, color_b;

		std::list<Matrix> mtxStack;
		Matrix currMatrix;
};

#endif // BASERENDERER_H
