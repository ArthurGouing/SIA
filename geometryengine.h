// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include "trimesh/include/TriMesh.h"
#include "joint.h"


#include <vector>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

extern bool verbose;

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
	trimesh::TriMesh*                  model;
	std::vector<trimesh::point>        vertices_model_init;
	std::vector<trimesh::point>        vertices_model;
	std::vector<trimesh::Vec<3, int>>*  indices_model  = nullptr;
	std::vector<std::vector<float>> 	weight_model;
    QOpenGLBuffer arrayBuf_model;
    QOpenGLBuffer indexBuf_model;
	void initModelGeometry();

	void Compute_weight_position(QVector4D& Offset, QVector4D& Translataion, QMatrix4x4& T_Matrix, Joint* joint, int i, int& k);
	
};
#endif // GEOMETRYENGINE_H
