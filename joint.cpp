#include "joint.h"
#include <QFile>
#include <QString>
#include <QDataStream>

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

QVector3D Joint::getGlobalPosition(QMatrix4x4 &fatherGlobalTransformation, QMatrix4x4 &SkinGlobalTransformation) 
{

    // create LOCAL 4x4 rotation matrix transformation for the Joint (Only the 3x3 upper matrix is filled)
    QMatrix4x4 localTransformation = QMatrix4x4(); // localTransformation initialized to Identity

	// Rotation transform
    switch (_rorder) {
        case roXYZ:
		    //cout << "case roXYZ" << endl;
            localTransformation.rotate((float) _curRx, 1, 0, 0);
            localTransformation.rotate((float) _curRy, 0, 1, 0);
            localTransformation.rotate((float) _curRz, 0, 0, 1);
            break;
        case roYZX:
		    //cout << "case 2" << endl;
            localTransformation.rotate((float) _curRy, 0, 1, 0);
            localTransformation.rotate((float) _curRz, 0, 0, 1);
            localTransformation.rotate((float) _curRx, 1, 0, 0);
            break;
        case roZXY:
		    //cout << "case 3" << endl;
            localTransformation.rotate((float) _curRz, 0, 0, 1);
            localTransformation.rotate((float) _curRx, 1, 0, 0);
            localTransformation.rotate((float) _curRy, 0, 1, 0);
            break;
        case roXZY:
		    //cout << "case 4" << endl;
            localTransformation.rotate((float) _curRx, 1, 0, 0);
            localTransformation.rotate((float) _curRz, 0, 0, 1);
            localTransformation.rotate((float) _curRy, 0, 1, 0);
            break;
        case roYXZ:
		    cout << "case 5" << endl;
            localTransformation.rotate((float) _curRy, 0, 1, 0);
            localTransformation.rotate((float) _curRx, 1, 0, 0);
            localTransformation.rotate((float) _curRz, 0, 0, 1);
            break;
        case roZYX:
		    //cout << "case 6" << endl;
            localTransformation.rotate((float) _curRz, 0, 0, 1);
            localTransformation.rotate((float) _curRy, 0, 1, 0);
            localTransformation.rotate((float) _curRx, 1, 0, 0);
            break;
        default:
		    //cout << "case default" << endl;
            break;
    }

	//if ((_name!="hips_dup")&(_name!="l_hip_dup")&(_name!="l_knee_dup")&(_name!="l_foot_dup")&(_name!="l_toes_dup")&(_name!="l_toes_End_dup")) 
	//{
	//	localTransformation.setToIdentity();
	//}

    // Construction of the entire 4x4 LOCAL by adding the translation component on the last column
    localTransformation.setColumn(3, QVector4D(
            (float) _offX + _curTx,
            (float) _offY + _curTy,
            (float) _offZ + _curTz,
            1));
	//localTransformation.translate(QVector3D((float) _offX + _curTx,(float) _offY + _curTy,(float) _offZ + _curTz));
	//localTransformation.translate(QVector3D((float) _curTx,(float) _curTy,(float) _curTz));


	//QVector3D vertice_position = fatherGlobalTransformation.map(QVector3D(_offX, _offY, _offZ));
    // Update father transformation matrix
    fatherGlobalTransformation = fatherGlobalTransformation * localTransformation;
	//_Translation = SkinGlobalTransformation.column(3);
	_T_Matrix = fatherGlobalTransformation;
	_R_Matrix = SkinGlobalTransformation;

	//cout << _name << endl;
	//print_T_Mat(fatherGlobalTransformation);
	//cout << fatherGlobalTransformation.column(3).x() << fatherGlobalTransformation.column(3).y() << fatherGlobalTransformation.column(3).z() << endl;
    
    // Apply transformation
    //QVector3D vertice_position = fatherGlobalTransformation.map(QVector3D(_offX, _offY, _offZ));
    QVector3D vertice_position(fatherGlobalTransformation.column(3));

    return vertice_position;
}

void print_T_Mat(QMatrix4x4 M)
{
	cout<< M.row(0).x() << " " << M.row(0).y() << " " << M.row(0).z() << " " << M.row(0).w() << " " <<endl;
	cout<< M.row(1).x() << " " << M.row(1).y() << " " << M.row(1).z() << " " << M.row(1).w() << " " <<endl;
	cout<< M.row(2).x() << " " << M.row(2).y() << " " << M.row(2).z() << " " << M.row(2).w() << " " <<endl;
	cout<< M.row(3).x() << " " << M.row(3).y() << " " << M.row(3).z() << " " << M.row(3).w() << " " <<endl;
	return;
}

void Joint::ComputeVertex(QVector3D (&vertices)[], QMatrix4x4& T_Mat, QMatrix4x4& T_Mat_Skin, int& ivert)
{
	// Compute position and update T_Mat
	vertices[ivert] = getGlobalPosition(T_Mat, T_Mat_Skin); 

	ivert++;
	for (unsigned int ichild=0; ichild<_children.size(); ichild++)
	{
	    // Save the parent transformation in T_Mat_copy
		_children[ichild]->ComputeVertex(vertices, T_Mat, T_Mat_Skin, ivert);
		T_Mat = _T_Matrix;
		T_Mat_Skin = _R_Matrix;
	}

}
void Joint::Compute_offset(QMatrix4x4 &Transform)
{
	QMatrix4x4 localTransform = QMatrix4x4();
    localTransform.rotate((float) _curRz, 0, 0, 1);
    localTransform.rotate((float) _curRy, 0, 1, 0);
    localTransform.rotate((float) _curRx, 1, 0, 0);
    localTransform.setColumn(3, QVector4D(
            (float) _offX,
            (float) _offY,
            (float) _offZ,
            1));
    _Offset = Transform.column(3);
	Transform = Transform * localTransform;
	std::cout << _Offset.x() << " " <<  _Offset.y() << " " <<  _Offset.z() << " " << std::endl;
	for(int ichild=0; ichild < _children.size(); ichild++)
	{
		QMatrix4x4 Transform_copy = Transform;
		_children[ichild]->Compute_offset(Transform);
		Transform = Transform_copy;
	}
}

void parse_channel(std::ifstream& file,
   Joint* joint){
		int num;
		file >> num ;
		joint->_dofs.resize(num); 
		string token;
		int i = 0;
		while( i < num){
			file >> token ;

			if( token == kXpos || token == kYpos || token == kZpos){
			        joint->_dofs[i].name = token; 
			}
			else if ( token == kZrot){
			        joint->_dofs[i].name = token; 
				file >> token ;
				i++;
				if( token == kYrot){
			        	joint->_dofs[i].name = token; 
					file >> token ;
			        	joint->_dofs[i+1].name = token; 
					joint->_rorder = roZYX ;
					break;
				}
				else if ( token == kXrot){
			        	joint->_dofs[i].name = token; 
					file >> token ;
			        	joint->_dofs[i+1].name = token; 
					joint->_rorder = roZXY ;
					break;
				}

			}
			else if ( token == kXrot){
			        joint->_dofs[i].name = token; 
				file >> token ;
				i++;
				if( token == kYrot){
			        	joint->_dofs[i].name = token; 
					file >> token ;
			        	joint->_dofs[i+1].name = token; 
					joint->_rorder = roXYZ ;
					break;
				}
				else if ( token == kZrot){
			        	joint->_dofs[i].name = token; 
					file >> token ;
			        	joint->_dofs[i+1].name = token; 
					joint->_rorder = roXZY ;
					break;
				}

			}
			else if ( token == kYrot){
			        joint->_dofs[i].name = token; 
				file >> token ;
				i++;
				if( token == kXrot){
			        	joint->_dofs[i].name = token; 
					file >> token ;
			        	joint->_dofs[i+1].name = token; 
					joint->_rorder = roYXZ ;
					break;
				}
				else if ( token == kZrot){
			        	joint->_dofs[i].name = token; 
					file >> token ;
			        	joint->_dofs[i+1].name = token; 
					joint->_rorder = roYZX ;
					break;
				}
			}
			i++;
		}

	}
Joint* parse_joint(std::ifstream& file,
    Joint* parent, Joint* parsed){
	Joint* joint = new Joint();
	std::string name;
	file >> name ;
	if (verbose) cout << "Parsing joint : " << name << endl;
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
			if (verbose) cout << name_end<<  endl;
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
			parsed = joint;
			return parsed;
			
			
		}
		file >> token;
		
	}
	parsed = joint; // securité mais le code n'est pas censé executer ces lignes	
	return parsed;	
		
	
	
}



void Joint::read_dof(ifstream& file, int iframe)
{
	for (unsigned int idof = 0 ; idof < _dofs.size() ; idof++) { // Pour chaque dofs
		double dof_value;
		file >> dof_value;
		_dofs[idof]._values.push_back(dof_value);
		if ((iframe==1)&(verbose)) cout << "We set "<< _dofs[idof].name << " of " << _name << " at " << dof_value << endl;

	}
	for (unsigned int ichild = 0 ; ichild < _children.size() ; ichild++) {
		_children[ichild]->read_dof(file, iframe);
	}

}

void parse_frames(std::ifstream& file, Joint* root)
{

    string token;
	file >> token;
	// place cursor at MOTION
	while (token != kMotion)
	{ 
		file >> token;
	}
	file >> token;
	file >> root->nb_frames;
	file >> token;
	file >> token;
	file >> root->frame_time;
	if (verbose) 
	{
	    cout << endl << "There is "<< root->nb_frames << "frames." << endl;
	    cout << "The time of each frame is "<< root->frame_time<< "." << endl << endl;
	}
	// Set dofs
	for (int frame=1; frame <= root->nb_frames; frame++) // Pour chaque ligne (ou frame)
	{
		if ((frame==1)&(verbose)) cout << "Frame : " <<frame<<endl;
		root->read_dof(file, frame);
         	if ((frame==1)&(verbose)) cout << "Frame : 2 \netc ..." << endl;
	}  
	return;
}


Joint* Joint::createFromFile(std::string fileName) {
	Joint* root = NULL;
	if (verbose) cout << "Loading from " << fileName << endl;

	ifstream inputfile(fileName.data());
	ifstream file(fileName.data());
	if(inputfile.good()) {
		string buf;	
		inputfile >> buf;
		if (verbose) cout << "Parsing" << endl;
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
			return root;
		}
		if (verbose) cout << "Fin du parsing" << endl << endl;
		inputfile.close();
	} else {
		std::cerr << "Failed to load the file " << fileName.data() << std::endl;
		fflush(stdout);
	}

	cout << "File loaded" << endl;
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

void Joint::reset()
{
	//Reset dofs values
	_curTx = 0; _curTy = 0; _curTz = 0;
	_curRx = 0; _curRy = 0; _curRz = 0;
	for (unsigned int ichild = 0 ; ichild < _children.size() ; ichild++) {
		_children[ichild]->reset();
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

void print_joint(Joint* j, int level)
{
    string space;
	for (int k=0; k<level; k++) space += " ";
	cout << space << j->_name << "(il a "<< j->_children.size() <<" children)" << endl;
	for (int ichild=0; ichild<j->_children.size(); ichild++)
	{
		level ++;
		print_joint(j->_children[ichild], level);
		level --;
	}
	return;
}
