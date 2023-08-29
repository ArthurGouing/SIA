
#include "skin.h"
#include <sstream>
#include <iostream>
using namespace std;

Skin::Skin(trimesh::TriMesh* model, Joint* root):
_root(root)
{
	vertices_init = model->vertices;
	vertices      = model->vertices;	
	indices       = &(model->faces);
	ReadWeights("ressources/weights.txt");
	Init_M0();
	_Mi.resize(_M0_1.size());
}

void Skin::ReadWeights(std::string fileName, bool hard_mode)
{
	std::cout << "Reading "<< fileName <<" ..." << std::endl;
    std::fstream file(fileName);
    std::string line;
	std::getline(file, line);
    int i = 0;
    while (std::getline(file, line)) {
        float weight;
		float weight_max=0.;
		int k_max=0;
		int k=0;
		std::vector<float> weight_line;
        std::stringstream ss(line);
        //weight_model.push_back(std::vector<float>());
		ss >> weight;
        while (ss >> weight) {
		    if ((hard_mode)&(weight > weight_max)) { 
				weight_max = weight;
				k_max = k;
			}
			else if (!hard_mode){
            	weight_line.push_back(weight);
			}
			k++;
        }
		//std::cout << k_max <<" : "<<weight_max<<std::endl;
		if (hard_mode) {
			weight_line.resize(32);
			weight_line[k_max] = 1;
		}
		weights.push_back(weight_line);
		i++;
    }
	std::cout << "fin read"<< std::endl;
}

void Skin::Update_vertices(int frame)
{
	QVector4D vert_init;
	QVector4D vert;

	for(int i=0; i<vertices.size(); i++)
	{
		vert = QVector4D(0., 0., 0., 1.0);
		vert_init = QVector4D(vertices_init[i][0], vertices_init[i][1], vertices_init[i][2], 1.);
		
		for(int k=0; k<_Mi.size(); k++)
		{
			vert += weights[i][k] * _Mi[k] * _M0_1[k] * vert_init;
		}
		vertices[i][0] = vert.x();
		vertices[i][1] = vert.y();
		vertices[i][2] = vert.z();
	}
}

void Skin::Init_M0()
{
	// Initialisation
	Joint* joint = _root;	
	_root->reset();

	// Fonction récursive
	set_rec_M0_1(joint);
}
void Skin::set_rec_M0_1(Joint* joint)
{
	QMatrix4x4 mat      = joint->_T_Matrix.transposed();
	QVector4D translate = -joint->_T_Matrix.column(3);

	mat.setColumn(3, translate); mat.setRow(3, QVector4D(0., 0., 0., 1.));
	_M0_1.push_back(mat);
	for(int ichild=0; ichild<joint->_children.size(); ichild++)
	{
		set_rec_M0_1(joint->_children[ichild]);
	}
}

void Skin::Update_M(int frame)
{
	// Initialisation
	Joint* joint = _root;	
	//_root->animate(0);
	_root->reset();
	int k=0;

	// Fonction récursive
	set_rec_Mi(joint, k);
}

void Skin::set_rec_Mi(Joint* joint, int& k)
{
	_Mi[k] = joint->_T_Matrix;
	k++;
	for(int ichild=0; ichild<joint->_children.size(); ichild++)
	{
		set_rec_Mi(joint->_children[ichild], k);
	}

}
