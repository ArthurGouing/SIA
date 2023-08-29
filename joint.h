#ifndef _JOINT_H_
#define _JOINT_H_

#include <iostream>

#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <QVector4D>
#include <QtGui/QMatrix4x4>


extern bool verbose;

class AnimCurve {
public :
	AnimCurve() {};
	~AnimCurve() {
		_values.clear();
	}
public :
	std::string name;					// name of dof
	std::vector<double> _values;		 		// different keyframes = animation curve
};


enum RotateOrder {roXYZ=0, roYZX, roZXY, roXZY, roYXZ, roZYX};

class Joint {
public :
	std::string _name;					// name of joint
	double _offX;						// initial offset in X
	double _offY;						// initial offset in Y
	double _offZ;						// initial offset in Z
	std::vector<AnimCurve> _dofs;		// keyframes : _animCurves[i][f] = i-th dof at frame f;
	double _curTx;						// current value of translation on X
	double _curTy;						// current value of translation on Y
	double _curTz;						// current value of translation on Z
	double _curRx;						// current value of rotation about X (deg)
	double _curRy;						// current value of rotation about Y (deg)
	double _curRz;						// current value of rotation about Z (deg)
	int _rorder;						// order of euler angles to reconstruct rotation
	std::vector<Joint*> _children;	// children of the current joint
        int nb_frames;
        float frame_time;
	QMatrix4x4 _T_Matrix;  // transformation matrix of the joint
	QMatrix4x4 _R_Matrix;  // transformation matrix of the joint without offset of the joint
	QVector4D _Translation;
	QVector4D _Offset;


public :
	// Constructor :
	Joint() : _T_Matrix(){};
	// Destructor :
	~Joint() {
		_dofs.clear();
		_children.clear();
	}

	// Create from data :
	static Joint* create(std::string name, double offX, double offY, double offZ, Joint* parent) {
		Joint* child = new Joint();
		child->_name = name;
		child->_offX = offX;
		child->_offY = offY;
		child->_offZ = offZ;
		child->_curTx = 0;
		child->_curTy = 0;
		child->_curTz = 0;
		child->_curRx = 0;
		child->_curRy = 0;
		child->_curRz = 0;
		if(parent != NULL) {
			parent->_children.push_back(child);
		}
		return child;
	}
	// Read dofs for each frame
	void read_dof(std::ifstream& file, int iframe); 

	// Load from file (.bvh) :	
	static Joint* createFromFile(std::string fileName);
	
 	// Compute global position
    	QVector3D getGlobalPosition(QMatrix4x4 &fatherGlobalTransformation, QMatrix4x4 &SkinGlobalTransformation);

	// Compute Joint vertex in global 
	void ComputeVertex(QVector3D vertices[], QMatrix4x4 &T_Mat, QMatrix4x4 &T_Mat_Skin, int &ivert, int N);

	// Compute M0 pour le skinning
	void Compute_offset(QMatrix4x4 &Transform);

	// Change Joint's values to the value of frame i
	void animate(int iframe=0);

	// Change joint calues to initial pos
	void reset();

	// Analysis of degrees of freedom :
	void nbDofs();

};
Joint* parse_joint(std::ifstream& file,Joint* parent, Joint* parsed);
void parse_channel(std::ifstream& file,Joint* joint);
void print_T_Mat(QMatrix4x4 M);

//std::ostream& operator<<(std::ostream& os, Joint &j);
void print_joint(Joint* j, int level);

#endif
