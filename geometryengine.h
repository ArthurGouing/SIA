// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include "trimesh/include/TriMesh.h"
#include "joint.h"
#include "skin.h"


#include <stack>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

extern bool verbose;
extern std::string bvh_name;

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine(bool* looping, bool* animating);
    virtual ~GeometryEngine();

    float get_delta_time(){return root->frame_time;}

    void drawSkeletonGeometry(QOpenGLShaderProgram *program);
	void drawPlanGeometry(QOpenGLShaderProgram *program);
	void drawModelGeometry(QOpenGLShaderProgram *program);

private:
	// Skeleton geometry
    Joint* root;
    int frame;
    bool* is_loop;
    bool* is_animated;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    void UpdateSkeletonGeometry();
	void UpdateModelGeometry();
	std::vector<std::vector<float>> readWeights(std::string fileName);

	// Ground geometry
    QOpenGLBuffer arrayBuf_plan;
    QOpenGLBuffer indexBuf_plan;

	// Model geometry
	trimesh::TriMesh* model;
	Skin*             model_skin;
    QOpenGLBuffer     arrayBuf_model;
    QOpenGLBuffer     indexBuf_model;

	void ComputeIndice(GLushort (&indices)[], Joint* joint, int& i, std::stack<int>& index_stack);
	void Compute_weight_position(QVector4D& Offset, QVector4D& Translataion, QMatrix4x4& T_Matrix, Joint* joint, int i, int& k);
	
};
#endif // GEOMETRYENGINE_H

