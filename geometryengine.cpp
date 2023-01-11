// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "geometryengine.h"

#include <iostream>
#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    //QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    initCubeGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void GeometryEngine::initCubeGeometry()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    QVector3D vertices[] = {
	QVector3D(-0.0f, -0.0f,  0.0f), // root
	QVector3D(-1.0f, -0.0f,  0.0f), // left leg
	QVector3D(-1.0f, -1.0f,  0.0f),
	QVector3D(-1.0f, -2.0f,  0.0f),
	QVector3D(-1.0f, -3.0f,  0.0f),
	QVector3D(-1.0f, -4.0f,  0.0f),
	QVector3D(-1.0f, -5.0f,  0.0f),
	QVector3D(1.0f, -0.0f,  0.0f),  // right leg
	QVector3D(1.0f, -1.0f,  0.0f),
	QVector3D(1.0f, -2.0f,  0.0f),
	QVector3D(1.0f, -3.0f,  0.0f),
	QVector3D(1.0f, -4.0f,  0.0f),
	QVector3D(1.0f, -5.0f,  0.0f),
	QVector3D(-0.0f, 1.f,  0.0f),  // spine
	QVector3D(-0.0f, 2.f,  0.0f),
	QVector3D(-0.0f, 3.f,  0.0f),
	QVector3D(-0.0f, 4.f,  0.0f),  // mid
	QVector3D(-0.0f, 5.f,  0.0f),  // head
	QVector3D(-0.0f, 6.f,  0.0f),
	QVector3D(-0.0f, 7.f,  0.0f),
	QVector3D(-1.0f, 4.f,  0.0f),  // left hand
	QVector3D(-2.0f, 4.f,  0.0f),  
	QVector3D(-3.0f, 4.f,  0.0f),  
	QVector3D(-4.0f, 4.f,  0.0f),  
	QVector3D(1.0f, 4.f,  0.0f) , // right hand
	QVector3D(2.0f, 4.f,  0.0f) , 
	QVector3D(3.0f, 4.f,  0.0f) , 
	QVector3D(4.0f, 4.f,  0.0f)  

    };
    //VertexData vertices[] = {
    //    
    //    // Vertex data for face 0
    //    {, QVector2D(0.0f, 0.0f)},  // v0
    //    {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.0f)}, // v1
    //    {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)},  // v2
    //    {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v3

    //    // Vertex data for face 1
    //    {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D( 0.0f, 0.5f)}, // v4
    //    {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f)}, // v5
    //    {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v6
    //    {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v7

    //    // Vertex data for face 2
    //    {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v8
    //    {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)},  // v9
    //    {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}, // v10
    //    {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v11

    //    // Vertex data for face 3
    //    {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v12
    //    {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)},  // v13
    //    {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v14
    //    {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)},  // v15

    //    // Vertex data for face 4
    //    {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}, // v16
    //    {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v17
    //    {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v18
    //    {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v19


    //    {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v20
    //    {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v21
    //    {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v22
    //    {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}  // v23
    //};

    // Indices for drawing cube faces using triungle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
         0, 1, 
	 1, 2,
	 2, 3,
 	 3, 4,
	 4, 5,
	 0, 6,
	 6, 7,
	 7, 8,
	 8, 9,
	 9,10, 
	 0,11,
	 11, 12,
	 12, 13,
	 13, 14,
	 14, 15,
	 15, 16,
	 16, 17,
	 14, 18, 
	 18, 19, 
	 19, 20, 
	 20, 21, 
	 18, 22,
	 22, 23,
	 23, 24, 
	 24, 25, 
    };

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 25 * sizeof(QVector3D));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 50 * sizeof(GLushort));
//! [1]
}

//! [2]
void GeometryEngine::drawCubeGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

    // Offset for texture coordinate
    //offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    //int texcoordLocation = program->attributeLocation("a_texcoord");
    //program->enableAttributeArray(texcoordLocation);
    //program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(QVector3D));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_LINES, 25, GL_UNSIGNED_SHORT, nullptr);
}

