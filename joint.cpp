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
    std::shared_ptr <Joint> joint){
		cout << "Parsing channels " << endl;
		int num;
		file >> num ;
		std :: string token;
		int i = 0;
		while( i < num){
			file >> token ;
			i++;
			if( token == kXpos || token == kYpos || token == kZpos){

			}
			else if ( token == kZrot){
				file >> token ;
				if( token == kYrot){
					joint->_rorder = roZYX ; 
					break;
				}
				else if ( token == kXrot){
					joint->_rorder = roZXY ;
					break;
				}
				
			}
			else if ( token == kXrot){
				file >> token ;
				if( token == kYrot){
					joint->_rorder = roXYZ ; 
					break;
				}
				else if ( token == kZrot){
					joint->_rorder = roXZY ;
					break;
				}
				
			}
			else if ( token == kYrot){
				file >> token ;
				if( token == kXrot){
					joint->_rorder = roYXZ ; 
					break;
				}
				else if ( token == kZrot){
					joint->_rorder = roYZX ;
					break;
				}
				
			}
		}

	}
Joint* parse_joint(std::ifstream& file,
    std::shared_ptr <Joint> parent, std::shared_ptr <Joint>& parsed){
	cout << "Parsing joint " << fileName << endl;
	std::shared_ptr<Joint> joint = std::make_shared<Joint>();
	std::string name;
	file >> name ;
	joint->name = name;
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
		if ( token == kChannels){
			parse_channel(file,joint);
			file >> token ;
			while(file.good()){
				if( token == kJoint){
					std::shared_ptr <Joint> child;
					child = parse_joint(file , joint , child);
					joint->_children.push_back(child);
			    }
			    else if( token = kEnd) {
				  std::string name_end ;
				  file >> name_end ;
				  file >> token; 
				  std::shared_ptr <Joint> end_joint = std::make_shared <Joint> ();
				  end_joint->name = name_end;
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
					parsed = joint ;
					return parsed ;
				}
			}
			
			

			
		}
		else{
			std::cerr << "Bad structure of bvh file "<< std::endl;
			return parsed;
		}
		
	}else{
		std::cerr << "Bad structure of bvh file "<< std::endl;
		return parsed ;
	}
	
}

Joint* Joint::createFromFile(std::string fileName) {
	Joint* root = NULL;
	cout << "Loading from " << fileName << endl;

	ifstream inputfile(fileName.data());
	if(inputfile.good()) {
		while(!inputfile.eof()) {
			string buf;	
			inputfile >> buf;
			// TODO : construire la structure de donn�es root � partir du fichier
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