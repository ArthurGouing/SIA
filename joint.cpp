#include "joint.h"
#include <QtGui/QMatrix4x4>

using namespace std;
namespace {

const std::string kChannels = "CHANNELS";
const std::string kEnd = "End";
const std::string kEndSite = "End Site";
const std::string kFrame = "Frame";
const std::string kFrames = "Frames:";
const std::string kHierarchy = "HIERARCHY";
const std::string kJoint = "JOINT";
const std::string kMotion = "MOTION";
const std::string kOffset = "OFFSET";
const std::string kRoot = "ROOT";

const std::string kXpos = "Xposition";
const std::string kYpos = "Yposition";
const std::string kZpos = "Zposition";
const std::string kXrot = "Xrotation";
const std::string kYrot = "Yrotation";
const std::string kZrot = "Zrotation";

}
void parse_channel(std::ifstream& file,
   Joint* joint){
		cout << "Parsing channels " << endl;
		int num;
		file >> num ;
		joint->_dofs.resize(num); //
		std :: string token;
		int i = 0;
		while( i < num){
			file >> token ;
			i++;
			joint->_dofs[i].name = token; //
			
			if( token == kXpos || token == kYpos || token == kZpos){
			}
			else if ( token == kZrot){
				file >> token ;
				i++;
				if( token == kYrot){
					file >> token ;
					joint->_rorder = roZYX ; 
					break;
				}
				else if ( token == kXrot){
					file >> token ;
					joint->_rorder = roZXY ;
					break;
				}
				
			}
			else if ( token == kXrot){
				file >> token ;
				i++;
				if( token == kYrot){
					file >> token ;
					joint->_rorder = roXYZ ; 
					break;
				}
				else if ( token == kZrot){
					file >> token ;
					joint->_rorder = roXZY ;
					break;
				}
				
			}
			else if ( token == kYrot){
				file >> token ;
				i++;
				if( token == kXrot){
					file >> token ;
					joint->_rorder = roYXZ ; 
					break;
				}
				else if ( token == kZrot){
					file >> token ;
					joint->_rorder = roYZX ;
					break;
				}
				
			}
		}

	}
Joint* parse_joint(std::ifstream& file,
    Joint* parent, Joint* parsed){
	cout << "Parsing joint " <<  endl;
	Joint* joint = new Joint();
	std::string name;
	file >> name ;
	cout << name <<  endl;
	joint->_name = name;
	std::string token ;
	file >> token ; // consommer '{'
	file >> token ; 
	if( token == kOffset ){
		double offX;
		double offY;
		double offZ;
		file >> offX;
		joint->_offX = offX;
		file >> offY;
		joint->_offY = offY;
		file >> offZ;
		joint->_offZ = offZ;
		file >> token ;
		
		
	}
	
	if ( token == kChannels){
			parse_channel(file,joint);
			file >> token ;
			

			}
	
	while(file.good()){
		if ( token == kMotion){
			cout << "im here " <<  endl;
			break;
			
		}
		if( token == kJoint){
			Joint* child = new Joint();
			child = parse_joint(file , joint , child);
			joint->_children.push_back(child);
		}
		else if( token == kEnd) {
			std::string name_end ;
			file >> name_end ;
			cout << name_end<<  endl;
			file >> token; 
			Joint* end_joint = new Joint();
			end_joint->_name = name_end;
			joint->_children.push_back(end_joint);
			file >> token ; 
			if ( token == kOffset){
			double offX;
			double offY;
			double offZ;
			file >> offX;
			end_joint->_offX = offX;
			file >> offY;
			end_joint->_offY = offY;
			file >> offZ;
			end_joint->_offZ = offZ;
			file >> token ;
			}


		}
		else if ( token == "}"){
			
			
		}
		file >> token;
		
	}
	parsed = joint ;	
	return parsed;	
		
	
	
}

void Joint::read_dof(ifstream& file, int iframe)
{
	cout << "Core dumped root est un pointeur nul" <<endl;
	for (unsigned int idof = 0 ; idof < _dofs.size() ; idof++) { // Pour chaque dofs
		cout << "idof : "<< idof << endl;
		cout << "name : " << _dofs[idof].name << endl;
		double dof_value;
		file >> dof_value;
		_dofs[idof]._values.push_back(dof_value);
		cout << "We set "<< _dofs[idof].name << " of " << _name << " at " << dof_value << endl;

	}
	for (unsigned int ichild = 0 ; ichild < _children.size() ; ichild++) {
		_children[ichild]->read_dof(file, iframe);
	}

}


Joint* Joint::createFromFile(std::string fileName) {
	Joint* root = NULL;
	int nb_frames;
	float frame_time; 
	cout << "Loading from " << fileName << endl;

	ifstream inputfile(fileName.data());
	if(inputfile.good()) {
		while(!inputfile.eof()) {
			string buf;	
			inputfile >> buf;
			// Construire la structure de donn�es root � partir du fichier
			if( buf == kHierarchy){
				if(inputfile.good() ){
					inputfile >> buf;
					if( buf == kRoot){
						root = parse_joint(inputfile, nullptr, root );
					}
					else{
						std::cerr << "Bad structure of bvh file "<< std::endl;
						fflush(stdout);
					}
				}
			}
			else{
				std::cerr << "Bad structure of bvh file "<< std::endl;
				fflush(stdout);
			}
		 	// Stock les valeurs pour chaque keyframe
			inputfile >> buf;
			std::string token;
			inputfile >> token;

			cout << token << endl;
			if (!buf.compare("MOTION")) {
				//inputfile;
				int nb_frames =0;
				float frame_time=0;
				inputfile >> buf;
				inputfile >> nb_frames;
				inputfile >> buf;
				inputfile >> frame_time;
				cout << "There is "<< nb_frames << "frames." << endl;
				cout << "The time of each frame is "<<frame_time<< "." << endl;
				// init _dofs
				for (int frame=1; frame <= nb_frames; frame++) // Pour chaque ligne (ou frame)
				{
					cout << "frame : " <<frame<<endl;
					root->read_dof(inputfile, frame);
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
	cout << _name << " : " << nbDofsR << " degree(s) of freedom in rotation\n";

	// Propagate to children :
	for (unsigned int ichild = 0 ; ichild < _children.size() ; ichild++) {
		_children[ichild]->nbDofs();
	}

}
