#ifndef DECORATORRENDERER_H
#define DECORATORRENDERER_H

#include "Renderer.h"

/* Renderer decorator class */
class DecoratorRenderer : public Renderer
{
	public:
		DecoratorRenderer(Renderer *base); // takes ownership
		virtual ~DecoratorRenderer();

		virtual void setGeometry(int width, int height);
		virtual void flushToFramebuffer();
		virtual void resetMatrixStack();
		virtual void pushMatrix();
		virtual void popMatrix();
		virtual void multMatrix(const Matrix &mtx);
		virtual const Matrix& matrix() const;
		virtual void setColor(char r, char g, char b);
		virtual void color(char *r, char *g, char *b) const;
		virtual void clearBuffers();
		virtual void drawLine(const Point3 &p1, const Point3 &p2);
		virtual void drawTriangle(const Triangle &triangle);

	protected:
		Renderer *const base;
};

#endif // DECORATORRENDERER_H
