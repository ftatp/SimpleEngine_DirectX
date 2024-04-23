#pragma once
#include "Mesh.h"

#include <memory>
#include <directxtk/SimpleMath.h>

namespace object
{
	shared_ptr<Mesh> CreateTriangle();
	shared_ptr<Mesh> CreateSquare();
	shared_ptr<Mesh> CreateBox();
	shared_ptr<Mesh> CreateGrid(const float width, const float height, const int numSlices, const int numStacks);
	shared_ptr<Mesh> CreateCylinder(const float bottomRadius, const float topRadius, float height, int numSlices);
	shared_ptr<Mesh> CreateSphere(const float radius, const int numSlices, const int numStacks);
}