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
GeometryEngine::GeometryEngine(bool* looping, bool* animating)
    : indexBuf(QOpenGLBuffer::IndexBuffer), is_loop(looping), is_animated(animating)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initialize Joint
    std::string file = "walk1.bvh";
    root =  Joint::createFromFile(file);
    if (verbose){ 
        std::cout << "The Joint root is created : "<< std::endl;
        print_joint(root, 0);
    }
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
    // Create the vertices from the Joint
    //int N = root->joint_size(); // A coder dans l'idéle
    int N = 32; // On sait qu'il en a 31 pour ce squelette
    QMatrix4x4 Transf_Matrix = QMatrix4x4(); // Initialize at Identity
    QVector3D vertices[N]; 
    int i = 0;
    root->ComputeVertex(vertices, Transf_Matrix, i); 
    std::cout<< "vertices array at frame "<< frame << std::endl;
    for (int i=0; i<N; i++)
    {
        std::cout << i << " : " <<vertices[i][0] << ", "<< vertices[i][1] << ", "<<vertices[i][2] << ", "<< std::endl;
    }
    //std::cout << *root;
    //print_joint(root, 0);

    // Indices to create line frome verticies
    GLushort indices[] = {
     0, 1, 
	 1, 2,
	 2, 3,
 	 3, 4,
	 4, 5,
	 5, 6,
	 0, 7, // r_hip
	 7, 8,
	 8, 9,
	 9, 10,
	 10,11, 
	 11,12, 
	 0,13, // spline_1
	 13, 14,
	 14, 15,
	 15, 16, // head
	 14, 15,
	 15, 16,
	 16, 17,
	 17, 18,
	 18, 19,
	 19, 20,
	 16, 21,  //l_arm
	 21, 22, 
	 22, 23, 
	 23, 24, 
	 24, 25, 
	 16, 26, //r_arm
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
    if ((frame <= root->nb_frames)&(*is_animated)) // faire cette attrbcut
    {
    	frame++;
    }
    if ((frame>root->nb_frames)&(*is_loop)) { 
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
    root->animate(0);
    UpdateSkeletonGeometry(frame);

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_LINES, 64, GL_UNSIGNED_SHORT, nullptr);

}
//! [2]
