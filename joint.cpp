#include <QtGui/QMatrix4x4>
#include <sstream>
#include "joint.h"

using namespace std;

Joint* Joint::createFromFile(std::string fileName) {
	Joint* root = NULL;
	cout << "Loading from " << fileName << endl;

	ifstream inputfile(fileName.data());
	int nb_frames;
	float frame_time;
	if(inputfile.good()) {
		while(!inputfile.eof()) {
			string buf;	
			inputfile >> buf;
			// TODO : construire la structure de données root à partir du fichier
			// TODO : fill Anim curve for each fram
			cout << buf << endl;
			if (!buf.compare("MOTION")) {
				//inputfile;
				inputfile >> buf;
				inputfile >> nb_frames;
				inputfile >> buf;
				inputfile >> frame_time;
				cout << "There is "<< nb_frames << "frames." << endl;
				cout << "The time of each frame is "<<frame_time<< "." << endl;
				for (int frame=1; frame <= nb_frames; frame++) // Pour chaque ligne (ou frame)
				{
					cout << "frame : " <<frame<<endl;
					root->init_dof(inputfile, frame);
					inputfile.close();
				}
				inputfile.close();
			}
		}
		inputfile.close();
	} else {
		std::cerr << "Failed to load the file " << fileName.data() << std::endl;
		fflush(stdout);
	}

	cout << "file loaded" << endl;

	return root;
}

void Joint::init_dof(ifstream& input, int iframe)
{
	cout << "Core dumped root est un pointeur nul" <<endl;
	for (unsigned int idof = 0 ; idof < _dofs.size() ; idof++) { // Pour chaque dofs
		cout << "idof : "<< idof << endl;
		cout << "Core dumped car _dofs n'est pas encore parsé" <<endl;
		cout << "name : " << _dofs[idof].name << endl;
		double dof_value;
		input >> dof_value;
		_dofs[idof]._values.push_back(dof_value);
		cout << "We set "<< _dofs[idof].name << " of " << _name << " at " << dof_value << endl;

	}
	for (unsigned int ichild = 0 ; ichild < _children.size() ; ichild++) {
		_children[ichild]->init_dof(input, iframe);
	}
	
}

void Joint::animate(int iframe) 
{
	// Update dofs :
	_curTx = 0; _curTy = 0; _curTz = 0;
	_curRx = 0; _curRy = 0; _curRz = 0;
	for (unsigned int idof = 0 ; idof < _dofs.size() ; idof++) {
		if(!_dofs[idof].name.compare("Xposition")) _curTx = _dofs[idof]._values[iframe];
		if(!_dofs[idof].name.compare("Yposition")) _curTy = _dofs[idof]._values[iframe];
		if(!_dofs[idof].name.compare("Zposition")) _curTz = _dofs[idof]._values[iframe];
		if(!_dofs[idof].name.compare("Zrotation")) _curRz = _dofs[idof]._values[iframe];
		if(!_dofs[idof].name.compare("Yrotation")) _curRy = _dofs[idof]._values[iframe];
		if(!_dofs[idof].name.compare("Xrotation")) _curRx = _dofs[idof]._values[iframe];
	}	
	// Animate children :
	for (unsigned int ichild = 0 ; ichild < _children.size() ; ichild++) {
		_children[ichild]->animate(iframe);
	}
}


void Joint::nbDofs() {
	if (_dofs.empty()) return;

	double tol = 1e-4;

	int nbDofsR = -1;

	// TODO :
	for (unsigned int idof=0 ; idof < _dofs.size() ; idof++) {
		for (unsigned int frame=0 ; frame < _dofs[idof]._values.size()-1 ; frame++) {
			if ((!_dofs[idof].name.compare("rotation")) 
			& (abs(_dofs[idof]._values[frame]-_dofs[idof]._values[frame+1]) > tol)) { // si il y a roation dans le nom
				nbDofsR += 1;
			}
		}
	}
	cout << _name << " : " << nbDofsR << " degree(s) of freedom in rotation\n";

	// Propagate to children :
	for (unsigned int ichild = 0 ; ichild < _children.size() ; ichild++) {
		_children[ichild]->nbDofs();
	}

}
