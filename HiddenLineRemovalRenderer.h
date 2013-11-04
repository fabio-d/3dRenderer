#ifndef HIDDENLINEREMOVALRENDERER_H
#define HIDDENLINEREMOVALRENDERER_H

#include "DecoratorRenderer.h"

/* Hidden line removal decorator class for Renderer */
class HiddenLineRemovalRenderer : public DecoratorRenderer
{
	public:
		HiddenLineRemovalRenderer(Renderer *base);
		virtual ~HiddenLineRemovalRenderer();

		virtual void drawTriangle(const Triangle &triangle);
};

#endif // HIDDENLINEREMOVALRENDERER_H
