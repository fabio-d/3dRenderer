#ifndef CUDARENDERER_H
#define CUDARENDERER_H

#include <list>

#include "BaseRenderer.h"

class CudaRenderer : public BaseRenderer
{
	public:
		CudaRenderer();
		~CudaRenderer();

		void setGeometry(int width, int height);
		void flushToFramebuffer();
		void clearBuffers();
		void drawLine(const Point3 &p1, const Point3 &p2);
		void drawTriangle(const Triangle &triangle);

	private:
		void drawLineNoTransform(const Point4 &p1, const Point4 &p2);
		void drawTriangleNoTransform(const Point4 &vA, const Point4 &vB,
			const Point4 &vC);

		int buffer_w, buffer_h;
		char *colorbuffer;  // buffer_w * buffer_h * 3 canali (RGB)
		float *depthbuffer; // buffer_w * buffer_h

		Matrix normMatrix;
};

#endif // CUDARENDERER_H
