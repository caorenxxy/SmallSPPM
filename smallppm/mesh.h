#pragma once
#include "def.h"
#include "linagl.h"
#include "transform.h"
#include "triangle.h"

NAMESPACE_BEGIN

struct Face {
	Face() {}
	Face(int v0, int v1, int v2) {
		vertIdx[0] = v0;
		vertIdx[1] = v1;
		vertIdx[2] = v2;
	}
	int vertIdx[3];
};

class Mesh {
public:
	Mesh(){}
	void LoadFromFile(std::string finename, bool reverseVertices = false);
	void SetMaterial(const std::shared_ptr<Material> material);
	void SetEmission(const Vec3 emission);
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	std::vector<Face> faces;
	Transform transform;
	Vec3 emission;
	bool isEmission = false;
	std::shared_ptr<Material> material;
	std::vector<Triangle> untransformedTriangles;
};

NAMESPACE_END