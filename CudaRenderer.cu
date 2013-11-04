#include "CudaRenderer.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <GL/gl.h>
#include <cuda_runtime.h>

#include "Point.h"
#include "Triangle.h"

static __global__ void clearBuffersK(char *colorbuffer, float *depthbuffer,
	int npixels, char r, char g, char b)
{
	const int i = threadIdx.x + blockIdx.x * blockDim.x;

	if (i < npixels)
	{
		char *pixel = colorbuffer + i * 3;
		*pixel++ = r;
		*pixel++ = g;
		*pixel = b;

		depthbuffer[i] = INFINITY;
	}
}

static __global__ void drawLineK(char *colorbuffer, float *depthbuffer, float subdivisions,
	float dx, float dy, float dz, float x1, float y1, float z1,
	int w, int h, char r, char g, char b)
{
	const float pos = (threadIdx.x + blockIdx.x * blockDim.x) / (subdivisions - 1);

	if (pos <= 1)
	{
		const int x = roundf(x1 + dx * pos);
		const int y = roundf(y1 + dy * pos);

		if (x < 0 || x > w || y < 0 || y > h)
			return;

		const float z = z1 + dz * pos;

		const int i = x + y * w;

		if (depthbuffer[i] > z)
		{
			char *pixel = colorbuffer + i * 3;
			*pixel++ = r;
			*pixel++ = g;
			*pixel = b;

			depthbuffer[i] = z;
		}
	}
}

static __global__ void drawTriangleK(char *colorbuffer, float *depthbuffer,
	int x0, int y0, float z0, int x1, int y1, float z1, int x2, int y2, float z2,
	int min_x, int min_y, int w, int h, char r, char g, char b)
{
	const int x = min_x + threadIdx.x + blockIdx.x * blockDim.x;
	const int y = min_y + threadIdx.y + blockIdx.y * blockDim.y;

	if (x < 0 || x > w || y < 0 || y > h)
		return;

	// Controlla che il punto sia interno ai tre lati del triangolo
	const float prodscal01 = (x1-x0)*(y-y0) - (y1-y0)*(x-x0);
	const float prodscal12 = (x2-x1)*(y-y1) - (y2-y1)*(x-x1);
	const float prodscal20 = (x0-x2)*(y-y2) - (y0-y2)*(x-x2);
	if ( (prodscal01 <= 0 && prodscal12 <= 0 && prodscal20 <= 0) ||
		(prodscal01 >= 0 && prodscal12 >= 0 && prodscal20 >= 0) )
	{
		const int i = x + y * w;
		const float z = (((x0-x)*y1+(x-x1)*y0+(x1-x0)*y)*z2+((x-x0)*y2+(x2-x)*y0+(x0-x2)*y)*z1+((x1-x)*y2+(x-x2)*y1+(x2-x1)*y)*z0)/((x1-x0)*y2+(x0-x2)*y1+(x2-x1)*y0);

		if (depthbuffer[i] > z)
		{
			char *pixel = colorbuffer + i * 3;
			*pixel++ = r;
			*pixel++ = g;
			*pixel = b;

			depthbuffer[i] = z;
		}
	}
}

CudaRenderer::CudaRenderer()
: buffer_w(0), buffer_h(0), colorbuffer(0)
{
}

void CudaRenderer::setGeometry(int w, int h)
{
	fprintf(stderr, "Buffer ridimensionato a %dx%d\n", w, h);

	if (colorbuffer != 0)
	{
		cudaFree(colorbuffer);
		cudaFree(depthbuffer);
	}

	cudaError_t err = cudaMalloc(&colorbuffer, w * h * 3);
	if (err != cudaSuccess)
	{
		fprintf(stderr, "cudaMalloc (colorbuffer) fallito: %d - %s\n", err, cudaGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	err = cudaMalloc(&depthbuffer, w * h * sizeof(float));
	if (err != cudaSuccess)
	{
		fprintf(stderr, "cudaMalloc (depthbuffer) fallito: %d - %s\n", err, cudaGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	buffer_w = w;
	buffer_h = h;

	normMatrix = Matrix::translate(1, 1, 0) * Matrix::scale(buffer_w / 2.f, buffer_h / 2.f, 1);
}

void CudaRenderer::flushToFramebuffer()
{
	const int buffsize = buffer_w * buffer_h * 3;
	char * temp = (char*)malloc(buffsize);
	if (temp == 0)
	{
		fprintf(stderr, "memoria esaurita in flushToFramebuffer\n");
		exit(EXIT_FAILURE);
	}

	cudaError_t err = cudaMemcpy(temp, colorbuffer, buffsize, cudaMemcpyDeviceToHost);
	if (err != cudaSuccess)
	{
		fprintf(stderr, "cudaMemcpy fallito: %d - %s\n", err, cudaGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	glRasterPos2i(-1, -1);
	glDrawPixels(buffer_w, buffer_h, GL_RGB, GL_UNSIGNED_BYTE, temp);

	free(temp);
}

void CudaRenderer::clearBuffers()
{
	char r, g, b;
	color(&r, &g, &b);

	const int npixels = buffer_w * buffer_h;

	const int blockSize = 32;
	const int numBlocks = (npixels + blockSize - 1) / blockSize;

	clearBuffersK<<<numBlocks, blockSize>>>(colorbuffer, depthbuffer,
		npixels, r, g, b);

	cudaError_t err = cudaThreadSynchronize();
	if (err != cudaSuccess)
	{
		fprintf(stderr, "clearBuffersK fallito: %d - %s\n", err, cudaGetErrorString(err));
		exit(EXIT_FAILURE);
	}
}

void CudaRenderer::drawLine(const Point3 &p1, const Point3 &p2)
{
	const Matrix mtx = matrix() * normMatrix;

	drawLineNoTransform(Point4(p1) * mtx, Point4(p2) * mtx);
}

void CudaRenderer::drawTriangle(const Triangle &t)
{
	const Matrix mtx = matrix() * normMatrix;

	const Point4 tA = Point4(t.vA) * mtx;
	const Point4 tB = Point4(t.vB) * mtx;
	const Point4 tC = Point4(t.vC) * mtx;

	drawTriangleNoTransform(tA, tB, tC);
}

CudaRenderer::~CudaRenderer()
{
	if (colorbuffer != 0)
	{
		cudaFree(colorbuffer);
		cudaFree(depthbuffer);
	}
}

void CudaRenderer::drawLineNoTransform(const Point4 &p1, const Point4 &p2)
{
	char r, g, b;
	color(&r, &g, &b);

	const int x1 = roundf(p1.x / p1.t);
	const int y1 = roundf(p1.y / p1.t);
	const int x2 = roundf(p2.x / p2.t);
	const int y2 = roundf(p2.y / p2.t);

	const int subdivisions = 2 * ceil(sqrt(pow(x2-x1, 2) + pow(y2-y1, 2)));
	const float dx = (x2 - x1);
	const float dy = (y2 - y1);
	const float dz = (p2.z - p1.z);

	const int blockSize = 32;
	const int numBlocks = (subdivisions + blockSize - 1) / blockSize;

	drawLineK<<<numBlocks, blockSize>>>(colorbuffer, depthbuffer, subdivisions,
		dx, dy, dz, x1, y1, p1.z, buffer_w, buffer_h,
		r, g, b);

	cudaError_t err = cudaThreadSynchronize();
	if (err != cudaSuccess)
	{
		fprintf(stderr, "drawLineK fallito: %d - %s\n", err, cudaGetErrorString(err));
		exit(EXIT_FAILURE);
	}
}

void CudaRenderer::drawTriangleNoTransform(const Point4 &p0, const Point4 &p1,
	const Point4 &p2)
{
	char r, g, b;
	color(&r, &g, &b);

	const int x0 = roundf(p0.x / p0.t);
	const int y0 = roundf(p0.y / p0.t);
	const float z0 = p0.z;
	const int x1 = roundf(p1.x / p1.t);
	const int y1 = roundf(p1.y / p1.t);
	const float z1 = p1.z;
	const int x2 = roundf(p2.x / p2.t);
	const int y2 = roundf(p2.y / p2.t);
	const float z2 = p2.z;

	// Calcola bounding box
	const float min_x = min(min(x0, x1), x2);
	const float max_x = max(max(x0, x1), x2);
	const float min_y = min(min(y0, y1), y2);
	const float max_y = max(max(y0, y1), y2);

	dim3 blockSize;
	blockSize.x = 16;
	blockSize.y = 16;

	dim3 numBlocks;
	numBlocks.x = ( (max_x - min_x + 1) + blockSize.x - 1 ) / blockSize.x;
	numBlocks.y = ( (max_y - min_y + 1) + blockSize.y - 1 ) / blockSize.y;

	drawTriangleK<<<numBlocks, blockSize>>>(colorbuffer, depthbuffer,
		x0, y0, z0, x1, y1, z1, x2, y2, z2,
		min_x, min_y, buffer_w, buffer_h, r, g, b);
}
