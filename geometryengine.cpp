// Copyright (C) 2016 The Qt Compaa
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
    : indexBuf(QOpenGLBuffer::IndexBuffer), indexBuf_plan(QOpenGLBuffer::IndexBuffer),is_loop(looping), is_animated(animating), frame(0)
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

    // Initialize Model
    std::string model_name = "ressources/skin.off";
    model = trimesh::TriMesh::read(model_name);
    root->reset(); 
    UpdateSkeletonGeometry();

	print_T_Mat(root->_T_Matrix);
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

//void GeometryEngine::UpdateModelGeometry()
//{
//	// il faut appeler la fonction aprés l'update du skeleton
//	// Compute the new position of each vertices
//	std::cout << "debut update"<< std::endl;
//	Joint* joint;
//	QMatrix4x4 T_Matrix;
//	QMatrix4x4 T_M0;
//	for (int i=0; i<3150/*vertices_model.size()*/; i++)
//	{
//		
//		//std::cout << "i : " << i << std::endl;
//		trimesh::Vec<3, float> vert = vertices_model_init.at(i);
//		QVector4D pos_init  = QVector4D(vert.x, vert.y, vert.z, 1.);
//		QVector4D pos       = QVector4D(0., 0., 0., 0.); 
//		//pos = pos_init + QVector4D(0, 50, 0, 0);
//		//std::cout << "initialize mat" << std::endl;
//		float zero[16] = { 0 };
//		T_Matrix = QMatrix4x4(zero);
//		T_Matrix.setColumn(3, QVector4D(0., 0., 0., 1.));
//		QVector4D Translation = QVector4D();
//		QVector4D Offset = QVector4D();
//		joint = root;
//		int k = 0;
//
//		//CompT_Matrixute the transform matrix
//		Compute_weight_position(Offset, Translation, T_Matrix, joint, i, k);
//		//T_Matrix.setColumn(3, -Translation); // - Translation initiale
//		//T_Matrix.setColumn(3, QVector4D(0., 0., 0., 1.));
//		//print_T_Mat(T_Matrix);
//		//pos = pos_init - Translation;
//		pos = pos + Translation;
//		pos = T_Matrix * (pos_init - Offset);
//
//		//// Link value
//		vertices_model[i][0] = pos.x(); 
//		vertices_model[i][1] = pos.y(); 
//		vertices_model[i][2] = pos.z(); 
//	}
//	
//}

//void GeometryEngine::Compute_weight_position(QVector4D& Offset, QVector4D& Translation, QMatrix4x4& T_Matrix, Joint* joint, int i, int& k)
//{
//    T_Matrix += weight_model[i][k] * joint->_T_Matrix;
//	Translation += weight_model[i][k] * joint->_T_Matrix.column(3);
//	Offset += weight_model[i][k] * joint->_Offset;
//	T_Matrix.setRow(3, QVector4D(0., 0., 0., 1.));
//	if ((weight_model[i][k] ==1)&(joint->_name == "l_hip_dup")) {
//		//std::cout << joint->_name << " weight : "<<weight_model[i][k] << std::endl;
//		//print_T_Mat(T_Matrix);
//		//std::cout << Translation.x() << " " <<  Translation.y() << " " <<  Translation.z() << " " << std::endl;
//	}
//	k++;
//
//
//	for (int ichild=0; ichild < joint->_children.size(); ichild++)
//	{
//		Compute_weight_position(Offset, Translation, T_Matrix, joint->_children[ichild], i, k);
//	}
//}
//
//std::vector<std::vector<float>> GeometryEngine::readWeights(std::string fileName) {
//	std::cout << "Reading "<< fileName <<" ..." << std::endl;
//    std::fstream file(fileName);
//    std::string line;
//	std::vector<std::vector<float>> matrix;
//	std::getline(file, line);
//    int i = 0;
//    while (std::getline(file, line)) {
//		std::cout << i << std::endl;
//        float weight;
//		float weight_max=0.;
//		int k_max=0;
//		int k=0;
//		std::vector<float> weight_line;
//        std::stringstream ss(line);
//        weight_model.push_back(std::vector<float>());
//		ss >> weight;
//        while (ss >> weight) {
//			std::cout << k <<" : "<<weight<<std::endl;
//		    if (weight > weight_max) { 
//				weight_max = weight;
//				k_max = k;
//			}
//			k++;
//            //weight_line.push_back(weight);
//        }
//		std::cout << k_max <<" : "<<weight_max<<std::endl;
//		weight_line.resize(32);
//		weight_line[k_max] = 1;
//		for (int k=0; k<weight_line.size() ; k++){
//			std::cout << "w : " << weight_line[k] << " // "<< weight_line.size() << std::endl;
//		}
//		matrix.push_back(weight_line);
//		i++;
//    }
//	std::cout << "fin read"<< std::endl;
//	return matrix;
//}

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
	float f_offset = 0;
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
    int i = 0;
    root->ComputeVertex(verticies, Transf_Matrix, _, i); 
    if (verbose) {
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
	//root->reset(); // A enlever pour voir l'animation
    root->animate(0);
    root->animate(frame);
    UpdateSkeletonGeometry();

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_LINES, 64, GL_UNSIGNED_SHORT, nullptr);

}
//! [2]

