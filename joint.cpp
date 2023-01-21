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

QVector4D Joint::getGlobalPosition(Joint *joint, QMatrix4x4 fatherGlobalTransformation) {

    // create LOCAL 4x4 rotation matrix transformation for the Joint (Only the 3x3 upper matrix is filled)
    QMatrix4x4 localTransformation = QMatrix4x4(); // localTransformation initialized to Identity
    switch (joint->_rorder) {
        case roXYZ:
            localTransformation.rotate((float) joint->_curRx, 1, 0, 0);
            localTransformation.rotate((float) joint->_curRy, 0, 1, 0);
            localTransformation.rotate((float) joint->_curRz, 0, 0, 1);
            break;
        case roYZX:
            localTransformation.rotate((float) joint->_curRy, 0, 1, 0);
            localTransformation.rotate((float) joint->_curRz, 0, 0, 1);
            localTransformation.rotate((float) joint->_curRx, 1, 0, 0);
            break;
        case roZXY:
            localTransformation.rotate((float) joint->_curRz, 0, 0, 1);
            localTransformation.rotate((float) joint->_curRx, 1, 0, 0);
            localTransformation.rotate((float) joint->_curRy, 0, 1, 0);
            break;
        case roXZY:
            localTransformation.rotate((float) joint->_curRx, 1, 0, 1);
            localTransformation.rotate((float) joint->_curRz, 0, 0, 1);
            localTransformation.rotate((float) joint->_curRy, 0, 1, 0);
            break;
        case roYXZ:
            localTransformation.rotate((float) joint->_curRy, 0, 1, 0);
            localTransformation.rotate((float) joint->_curRx, 1, 0, 0);
            localTransformation.rotate((float) joint->_curRz, 0, 0, 1);
            break;
        case roZYX:
            localTransformation.rotate((float) joint->_curRz, 0, 0, 1);
            localTransformation.rotate((float) joint->_curRy, 0, 1, 0);
            localTransformation.rotate((float) joint->_curRx, 0, 0, 0);
            break;
        default:
            break;
    }

    // Construction of the entire 4x4 LOCAL by adding the translation component on the last column
    localTransformation.setColumn(3, QVector4D(
            (float) joint->_offX,
            (float) joint->_offY,
            (float) joint->_offZ,
            1));

    // Calculate the global transformation matrix from the ROOT, the global position is the last column
    QMatrix4x4 childGlobalTransformation = fatherGlobalTransformation * localTransformation;

    return childGlobalTransformation.column(3);
}


void parse_channel(std::ifstream& file,
   Joint* joint){
		cout << "Parsing channels " << endl;
		int num;
		file >> num ;
		joint->_dofs.resize(num); //
		cout << "  joint " << joint->_name << endl;
    		cout << "  dofs size : " << joint->_dofs.size() << endl;
		std :: string token;
		int i = 0;
		while( i < num){
			file >> token ;

			if( token == kXpos || token == kYpos || token == kZpos){
			        joint->_dofs[i].name = token; //
       				cout <<i<< "name : " << token << endl;
				//i++;
			}
			else if ( token == kZrot){
			        joint->_dofs[i].name = token; //
       				cout <<i<< "name : " << token << endl;
				file >> token ;
				i++;
				if( token == kYrot){
			        	joint->_dofs[i].name = token; //
       					cout <<i<< "name : " << token << endl;
					file >> token ;
			        	joint->_dofs[i+1].name = token; //
       					cout << "name : " << token << endl;
					joint->_rorder = roZYX ;
					break;
				}
				else if ( token == kXrot){
			        	joint->_dofs[i].name = token; //
					file >> token ;
			        	joint->_dofs[i+1].name = token; //
					joint->_rorder = roZXY ;
					break;
				}

			}
			else if ( token == kXrot){
			        joint->_dofs[i].name = token; //
				file >> token ;
				i++;
				if( token == kYrot){
			        	joint->_dofs[i].name = token; //
					file >> token ;
			        	joint->_dofs[i+1].name = token; //
					joint->_rorder = roXYZ ;
					break;
				}
				else if ( token == kZrot){
			        	joint->_dofs[i].name = token; //
					file >> token ;
			        	joint->_dofs[i+1].name = token; //
					joint->_rorder = roXZY ;
					break;
				}

			}
			else if ( token == kYrot){
			        joint->_dofs[i].name = token; //
				file >> token ;
				i++;
				if( token == kXrot){
			        	joint->_dofs[i].name = token; //
					file >> token ;
			        	joint->_dofs[i+1].name = token; //
					joint->_rorder = roYXZ ;
					break;
				}
				else if ( token == kZrot){
			        	joint->_dofs[i].name = token; //
					file >> token ;
			        	joint->_dofs[i+1].name = token; //
					joint->_rorder = roYZX ;
					break;
				}
			}
			i++;
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
			//cout << "im here " <<  endl;
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
	cout << "joint "<< _name << endl;
        cout << "dof size : " << _dofs.size() << endl;
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

void parse_frames(std::ifstream& file, Joint* root)
{

        std:string token;
	file >> token;
	while (token != kMotion)
	{ 
		file >> token;
		cout << token << endl;
	}
	//inputfile;
	file >> token;
	cout << token << endl;
	file >> root->nb_frames;
	file >> token;
	file >> token;
	file >> root->frame_time;
	cout << endl << "There is "<< root->nb_frames << "frames." << endl;
	cout << "The time of each frame is "<< root->frame_time<< "." << endl;
	// init _dofs
	for (int frame=1; frame <= root->nb_frames; frame++) // Pour chaque ligne (ou frame)
	{
		cout << "frame : " <<frame<<endl;
		root->read_dof(file, frame);
	}  
	return;
}


Joint* Joint::createFromFile(std::string fileName) {
	Joint* root = NULL;
	cout << "Loading from " << fileName << endl;

	ifstream inputfile(fileName.data());
	ifstream file(fileName.data());
	if(inputfile.good()) {
		while(!inputfile.eof()) {
			string buf;	
			inputfile >> buf;
			cout << "Parsing" << endl;
			// Construire la structure de donn�es root partir du fichier
			if( buf == kHierarchy){
				if(inputfile.good() ){
					inputfile >> buf;
					if( buf == kRoot){
						root = parse_joint(inputfile, nullptr, root);
						parse_frames(file, root);
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
			cout << "Fin du parsing" << endl;
		 	// Stock les valeurs pour chaque keyframe
			cout << "toke : " << buf << endl;
			std::string token;
			inputfile >> token;

			cout << token << endl;
			if (!buf.compare("MOTION")) {
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
