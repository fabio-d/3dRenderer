#ifndef WIREFRAMERENDERER_H
#define WIREFRAMERENDERER_H

#include "DecoratorRenderer.h"

/* Wireframe decorator class for Renderer */
class WireframeRenderer : public DecoratorRenderer
{
	public:
		WireframeRenderer(Renderer *base);
		virtual ~WireframeRenderer();

		virtual void drawTriangle(const Triangle &triangle);
};

#endif // WIREFRAMERENDERER_H
