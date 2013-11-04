#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <list>

#include "BaseRenderer.h"

class GLRenderer : public BaseRenderer
{
	public:
		GLRenderer();
		~GLRenderer();

		void setGeometry(int width, int height);
		void flushToFramebuffer();
		void clearBuffers();
		void drawLine(const Point3 &p1, const Point3 &p2);
		void drawTriangle(const Triangle &triangle);

	private:
		void drawLineNoTransform(const Point4 &p1, const Point4 &p2);
		void drawTriangleNoTransform(const Point4 &vA, const Point4 &vB,
			const Point4 &vC);
};

#endif // GLRENDERER_H
