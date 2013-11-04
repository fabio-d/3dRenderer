#include "DecoratorRenderer.h"

DecoratorRenderer::DecoratorRenderer(Renderer *base)
: base(base)
{
}

DecoratorRenderer::~DecoratorRenderer()
{
	delete base;
}

void DecoratorRenderer::setGeometry(int width, int height)
{
	base->setGeometry(width, height);
}

void DecoratorRenderer::flushToFramebuffer()
{
	base->flushToFramebuffer();
}

void DecoratorRenderer::resetMatrixStack()
{
	base->resetMatrixStack();
}

void DecoratorRenderer::pushMatrix()
{
	base->pushMatrix();
}

void DecoratorRenderer::popMatrix()
{
	base->popMatrix();
}

void DecoratorRenderer::multMatrix(const Matrix &mtx)
{
	base->multMatrix(mtx);
}

const Matrix& DecoratorRenderer::matrix() const
{
	return base->matrix();
}

void DecoratorRenderer::setColor(char r, char g, char b)
{
	base->setColor(r, g, b);
}

void DecoratorRenderer::color(char *r, char *g, char *b) const
{
	base->color(r, g, b);
}

void DecoratorRenderer::clearBuffers()
{
	base->clearBuffers();
}

void DecoratorRenderer::drawLine(const Point3 &p1, const Point3 &p2)
{
	base->drawLine(p1, p2);
}

void DecoratorRenderer::drawTriangle(const Triangle &triangle)
{
	base->drawTriangle(triangle);
}
