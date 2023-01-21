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
    : indexBuf(QOpenGLBuffer::IndexBuffer), is_loop(true)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initialize Joint
    std::string file = "walk1.bvh";
    root =  Joint::createFromFile(file);
    frame = 0;

    UpdateSkeletonGeometry(frame);
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void GeometryEngine::UpdateSkeletonGeometry(int frame)
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    QVector3D vertices[] = {
	QVector3D(-0.0f, -0.0f,  frame*0.01), //0 // root // Hips
	QVector3D(-1.0f, -0.0f,  0.0f), //1 /       // l_hips
	QVector3D(-1.0f, -1.0f,  0.0f), //2         // l_thig
	QVector3D(-1.0f, -2.0f,  0.0f), //3         // l_knee
	QVector3D(-1.0f, -3.0f,  0.0f), //4         // l_foot
	QVector3D(-1.0f, -4.0f,  0.0f), //5         // l_toes
	QVector3D(-1.0f, -5.0f,  0.0f), //6         // l_toes_end
	QVector3D(1.0f, -0.0f,  0.0f),  //7 right leg
	QVector3D(1.0f, -1.0f,  0.0f), //8
	QVector3D(1.0f, -2.0f,  0.0f), //9
	QVector3D(1.0f, -3.0f,  0.0f), //10
	QVector3D(1.0f, -4.0f,  0.0f), //11
	QVector3D(1.0f, -5.0f,  0.0f), //12
	QVector3D(-0.0f, 1.f,  0.0f),  //13 //spine 1
	QVector3D(-0.0f, 2.f,  0.0f),  //14 //spine 2
	QVector3D(-0.0f, 3.f,  0.0f),  //15 //spine 3
	QVector3D(-0.0f, 4.f,  0.0f),  //16 //spine 4 (mid)
	QVector3D(-0.0f, 5.f,  0.0f),  //17 //neck1   (head)
	QVector3D(-0.0f, 6.f,  0.0f),  //18 //neck2
	QVector3D(-0.0f, 7.f,  0.0f),  //18 //head
	QVector3D(-0.0f, 8.f,  0.0f),  //20 //head_end
	QVector3D(-1.0f, 4.f,  0.0f),  //21 //l_clavicule (left hand)
	QVector3D(-2.0f, 4.f,  0.0f),  //22 //l_shoulder  
	QVector3D(-3.0f, 4.f,  0.0f),  //23 //l_elbow
	QVector3D(-4.0f, 4.f,  0.0f),  //24 //l_hand
	QVector3D(-5.0f, 5.f,  0.0f),  //25 //l_hand_end
	QVector3D(1.0f, 4.f,  0.0f) ,  //26 right hand
	QVector3D(2.0f, 4.f,  0.0f) ,  //27
	QVector3D(3.0f, 4.f,  0.0f) ,  //28
	QVector3D(3.0f, 5.f,  0.0f) ,  //29
	QVector3D(4.0f, 4.f,  0.0f),   //30
	QVector3D(5.0f, 5.f,  0.0f),   //31

    };
    GLushort indices[] = {
         0, 1, 
	 1, 2,
	 2, 3,
 	 3, 4,
	 4, 5,
	 5, 6,
	 0, 7,
	 7, 8,
	 8, 9,
	 9, 10,
	 10,11, 
	 11,12, 
	 0,13,
	 13, 14,
	 14, 15,
	 15, 16,
	 14, 15,
	 15, 16,
	 16, 17,
	 17, 18,
	 18, 19,
	 19, 20,
	 16, 21, 
	 21, 22, 
	 22, 23, 
	 23, 24, 
	 24, 25, 
	 16, 26,
	 26, 27,
	 27, 28, 
	 28, 29, 
	 29, 30, 
	 30, 31, 
    };

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 32 * sizeof(QVector3D));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 64 * sizeof(GLushort));
//! [1]
}

//! [2]
void GeometryEngine::drawSkeletonGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Update frame
    if (frame <= root->nb_frames) // faire cette attrbcut
    {
    	frame++;
    }
    else if (is_loop) { 
	frame = 0;
    }

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
    // Udate geometry
    UpdateSkeletonGeometry(frame);

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_LINES, 64, GL_UNSIGNED_SHORT, nullptr);

}
//! [2]
