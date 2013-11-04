#include "WireframeRenderer.h"

#include "Triangle.h"

WireframeRenderer::WireframeRenderer(Renderer *base)
: DecoratorRenderer(base)
{
}

WireframeRenderer::~WireframeRenderer()
{
}

void WireframeRenderer::drawTriangle(const Triangle &triangle)
{
	base->drawLine(triangle.vA, triangle.vB);
	base->drawLine(triangle.vB, triangle.vC);
	base->drawLine(triangle.vC, triangle.vA);
}
