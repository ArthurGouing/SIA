// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "geometryengine.h"

#include <iostream>
#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine(bool* looping, bool* animating)
    : indexBuf(QOpenGLBuffer::IndexBuffer), indexBuf_plan(QOpenGLBuffer::IndexBuffer),is_loop(looping), is_animated(animating)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

	arrayBuf_plan.create();
	indexBuf_plan.create();

    // Initialize Joint
    std::string file = "ressources/walk1.bvh"; // à mettre en argument de l'exe

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

	arrayBuf_plan.destroy();
	indexBuf_plan.destroy();
}
//! [0]

void GeometryEngine::DrawPlanGeometry(QOpenGLShaderProgram *program)
{
    arrayBuf_plan.bind();
    indexBuf_plan.bind();
    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

	program->setUniformValue("is_plan", true);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    float size     = 500;
    float tex_size = 5;
	float f_offset = 111;
	VertexData verticies_plan[] = {
		{QVector3D(-size, f_offset, -size), QVector2D(0., 0.)},
		{QVector3D( size, f_offset, -size), QVector2D(tex_size, 0.)},
		{QVector3D( size, f_offset,  size), QVector2D(tex_size, tex_size)},
		{QVector3D(-size, f_offset,  size), QVector2D(0., tex_size)}, 
	};

	GLushort index_plan[] = {0, 3, 2, 1 };
    // Transfer vertex data to VBO 0
    arrayBuf_plan.bind();
    arrayBuf_plan.allocate(verticies_plan, 4 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf_plan.bind();
    indexBuf_plan.allocate(index_plan, 4 * sizeof(GLushort));

    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, nullptr);
}

void GeometryEngine::UpdateSkeletonGeometry(int frame)
{
	
    int N = 31; // On sait qu'il en a 31 pour ce squelette
    QMatrix4x4 Transf_Matrix = QMatrix4x4(); // Initialize at Identity
    QVector3D verticies[N]; 
    int i = 0;
    root->ComputeVertex(verticies, Transf_Matrix, i); 
    if ((verbose)&(frame==1)) {
    	std::cout<< "vertices array at frame "<< frame << std::endl;
    	for (int i=0; i<N; i++)
    	{
    	    std::cout << i << " : " <<verticies[i][0] << ", "<< verticies[i][1] << ", "<<verticies[i][2] << ", "<< std::endl;
    	}
    }

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
    };

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(verticies, 31 * sizeof(QVector3D));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 64 * sizeof(GLushort));
//! [1]
}

//! [2]
void GeometryEngine::drawSkeletonGeometry(QOpenGLShaderProgram *program)
{

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

    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

	program->setUniformValue("is_plan", false);
	
    // Offset for texture coordinate
    //offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    //int texcoordLocation = program->attributeLocation("a_texcoord");
    //program->enableAttributeArray(texcoordLocation);
    //program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(QVector3D));

    // Udate geometry
    root->animate(frame);
    UpdateSkeletonGeometry(frame);

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_LINES, 64, GL_UNSIGNED_SHORT, nullptr);

}
//! [2]

