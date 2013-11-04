#include "HiddenLineRemovalRenderer.h"

#include "Triangle.h"

HiddenLineRemovalRenderer::HiddenLineRemovalRenderer(Renderer *base)
: DecoratorRenderer(base)
{
}

HiddenLineRemovalRenderer::~HiddenLineRemovalRenderer()
{
}

void HiddenLineRemovalRenderer::drawTriangle(const Triangle &triangle)
{
	char old_r, old_g, old_b;
	base->color(&old_r, &old_g, &old_b);

	base->setColor(255, 255, 255);
	base->drawTriangle(triangle);

	base->setColor(old_r, old_g, old_b);
	base->drawLine(triangle.vA, triangle.vB);
	base->drawLine(triangle.vB, triangle.vC);
	base->drawLine(triangle.vC, triangle.vA);
}
