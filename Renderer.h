#ifndef RENDERER_H
#define RENDERER_H

class Matrix;
class Point3;
class Triangle;

/* Interface for rendering engines */
class Renderer
{
	public:
		virtual ~Renderer();

		/* Resize the underlying buffers */
		virtual void setGeometry(int width, int height) = 0;

		/* Flush local colorbuffer to GL framebuffer, if applicable */
		virtual void flushToFramebuffer() = 0;

		/* Empty the matrix stack and load identity matrix */
		virtual void resetMatrixStack() = 0;

		/* Push the current matrix in the matrix stack */
		virtual void pushMatrix() = 0;

		/* Replace the current matrix with a matrix popped out of the
		 * matrix stack */
		virtual void popMatrix() = 0;

		/* Replace the current matrix: currMatrix = mtx * currMatrix */
		virtual void multMatrix(const Matrix &mtx) = 0;

		/* Read current matrix */
		virtual const Matrix& matrix() const = 0;

		/* Set color for subsequent operations */
		virtual void setColor(char r, char g, char b) = 0;

		/* Read current color */
		virtual void color(char *r, char *g, char *b) const = 0;

		/* Clear colorbuffer with current color and clear depthbuffer */
		virtual void clearBuffers() = 0;

		/* Draw a line in 3D space */
		virtual void drawLine(const Point3 &p1, const Point3 &p2) = 0;

		/* Draw a triangle in 3D space */
		virtual void drawTriangle(const Triangle &triangle) = 0;
};

#endif // RENDERER_H
