#ifndef _SKIN_H_
#define _SKIN_H_

#include "joint.h"
#include "trimesh/include/TriMesh.h"

#include <vector>
#include <QVector4D>
#include <QtGui/QMatrix4x4>

class Skin {
public:
	// Constructor
	Skin(trimesh::TriMesh* model, Joint* root);
	// Joint to skin to
	Joint* _root;

	// Property of the model
	std::vector<trimesh::point>       vertices_init;	
	std::vector<trimesh::point>       vertices;	
	std::vector<trimesh::Vec<3, int>>* indices = nullptr;	
	std::vector<std::vector<float> >  weights;	

	// Matrice de transformation
	std::vector<QMatrix4x4> _M0_1;
	std::vector<QMatrix4x4> _Mi;

	// Fonction Update
	void Update_vertices(int frame);
	void Update_M(int frame);

private:
	// Fonction d'initialisation
	void ReadWeights(std::string fileName, bool hard_mode=false);
	void Init_M0();


	// Fonction récursive appelé dans les fonction précédantes
	void set_rec_Mi(Joint* joint, int& k);
	void set_rec_M0_1(Joint* joint);

};
#endif
