// Copyright (C) 2016 The Qt Compaa
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "geometryengine.h"

#include <iostream>
#include <stack>
#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine(bool* looping, bool* animating)
    : frame(0), is_loop(looping), is_animated(animating), indexBuf(QOpenGLBuffer::IndexBuffer), indexBuf_plan(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

	arrayBuf_plan.create();
	indexBuf_plan.create();

    // Initialize Joint 
    std::string file = bvh_name; // à mettre en argument de l'exe
    root =  Joint::createFromFile(file);
    if (verbose){ 
        std::cout << "The Joint root is created : "<< std::endl;
        print_joint(root, 0);
    }

    // Initialize Model
    std::string model_name = "ressources/skin.off";
    model = trimesh::TriMesh::read(model_name);
    root->reset(); 
    UpdateSkeletonGeometry();

	model_skin = new Skin(model, root);
    
	root->animate(0);
    UpdateSkeletonGeometry();
	model_skin->Update_M(0);
	model_skin->Update_vertices(0);
    //UpdateModelGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();

	arrayBuf_plan.destroy();
	indexBuf_plan.destroy();

	arrayBuf_model.destroy();
	indexBuf_model.destroy();
}
//! [0]

void GeometryEngine::drawModelGeometry(QOpenGLShaderProgram *program)
{

	// Update geometru
	model_skin->Update_M(0);
	model_skin->Update_vertices(0);

	arrayBuf_model.bind();
	indexBuf_model.bind();

	// Read model vertice to VBO
    //arrayBuf_plan.allocate(&(model->vertices.front()), model->vertices.size() * sizeof(trimesh::point));
    //indexBuf_plan.allocate(&(model->faces.front()), model->faces.size() * 3 * sizeof(int));
    arrayBuf_plan.allocate(&model_skin->vertices.front(), model->vertices.size() * sizeof(trimesh::point));
    indexBuf_plan.allocate(&model_skin->indices->front(), model->faces.size() * 3 * sizeof(int));

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3);

    //glDrawElements(GL_TRIANGLES, 2*3*model->faces.size(), GL_UNSIGNED_INT, nullptr);
    glDrawElements(GL_POINTS, 2*3*model->faces.size(), GL_UNSIGNED_INT, nullptr);
	return;
}

void GeometryEngine::drawPlanGeometry(QOpenGLShaderProgram *program)
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
	float f_offset = -108;
	if (bvh_name != "ressources/xsens.bvh") f_offset = 0;
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

void GeometryEngine::UpdateSkeletonGeometry()
{
	
    int N = 31; // On sait qu'il en a 31 pour ce squelette
    QMatrix4x4 Transf_Matrix = QMatrix4x4(); // Initialize at Identity
    QMatrix4x4 _ = QMatrix4x4(); // Initialize at Identity
    QVector3D verticies[N]; 
	GLushort  indices  [2*(N-1)];
	std::stack<int> index;
    int i = 0;
    root->ComputeVertex(verticies, Transf_Matrix, _, i); 
    if (verbose) {
    	for (int i=0; i<N; i++)
    	{
    	    std::cout << i << " : " <<verticies[i][0] << ", "<< verticies[i][1] << ", "<<verticies[i][2] << ", "<< std::endl;
    	}
    }

    // Indices to create line frome verticies
	i=0;
    ComputeIndice(indices, root, i, index);
	if (verbose) {
		std::cout << "Indice of skeleton : " << std::endl;
		for (int i=0; i<2*(N-1); i+=2)
		{
			std::cout << i/2 << " : " << indices[i] <<", "<< indices[i+1] << std::endl;
		}
	}
//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(verticies, N * sizeof(QVector3D));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 2*(N-1) * sizeof(GLushort));
//! [1]
}

void GeometryEngine::ComputeIndice(GLushort (&indices)[], Joint* joint, int& i, std::stack<int>& index_stack)
{
	index_stack.push(i);
	for (int ichild=0; ichild<joint->_children.size(); ichild++)
	{
		//std::cout << index_stack.top() <<", " << i+1 << std::endl;
		indices[2*i] = index_stack.top() ; indices[2*i+1] = i+1;
		i +=1;
		//std::cout << joint->_name <<" a " << joint->_children.size() << " children :" << std::endl;
		ComputeIndice(indices, joint->_children[ichild], i, index_stack);
		//std::cout << "out !(" << joint->_name << ")" << std::endl;
		//std::cout << index_stack.top() << std::endl;
		index_stack.pop();
		
	}
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
	//root->reset(); // A enlever pour voir l'animation
    root->reset();
    root->animate(frame);
	std::cout << frame << std::endl;
    UpdateSkeletonGeometry();

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_LINES, 62, GL_UNSIGNED_SHORT, nullptr);

}
//! [2]

