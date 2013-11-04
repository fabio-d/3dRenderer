#include "BaseRenderer.h"

BaseRenderer::BaseRenderer()
{
}

BaseRenderer::~BaseRenderer()
{
}

void BaseRenderer::resetMatrixStack()
{
	mtxStack.clear();
	currMatrix = Matrix::identity();
}

void BaseRenderer::pushMatrix()
{
	mtxStack.push_back(currMatrix);
}

void BaseRenderer::popMatrix()
{
	currMatrix = mtxStack.back();
	mtxStack.pop_back();
}

void BaseRenderer::multMatrix(const Matrix &mtx)
{
	currMatrix = mtx * currMatrix;
}

const Matrix& BaseRenderer::matrix() const
{
	return currMatrix;
}

void BaseRenderer::setColor(char r, char g, char b)
{
	color_r = r;
	color_g = g;
	color_b = b;
}

void BaseRenderer::color(char *r, char *g, char *b) const
{
	*r = color_r;
	*g = color_g;
	*b = color_b;
}
