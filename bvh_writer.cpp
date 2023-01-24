#include"bvh_writer.h"

using namespace std;

void write_bvh(std::vector<std::string> files, std::string name)//créate bvh file
{
	vector<vector<QQuaternion>> quat_matrix;
	vector<QQuaternion> quat_offset;

	// Lecture des fichiers
	for(int ifile=0; ifile<files.size(); ifile++)
	{
		read_file(files[ifile], quat_matrix, quat_offset);
	}

	// Calcule
	vector<vector<QQuaternion>> quat_matrix_loc; // quat_matrix_loc[frame][joint]
	for(int frame=0; frame< quat_matrix[0].size();frame++)
	{
		vector<QQuaternion> quat_loc;
		quat_loc.push_back(quat_matrix[0][frame] * quat_offset[0].inverted());
		for (int icap=1; icap<quat_matrix.size(); icap++)
		{
			QQuaternion q_loc;
			QQuaternion q_1 = quat_matrix[icap-1][frame] * quat_offset[icap-1].inverted(); 
			QQuaternion q_2 = quat_matrix[icap][frame]   * quat_offset[icap].inverted(); 

			q_loc = q_1.inverted() * q_2;
			quat_loc.push_back(q_loc);
			//if (icap == 1) {
			//	cout << "frame : " << frame << endl;
			//	print_quat(q_loc);
			//}
		}
		quat_matrix_loc.push_back(quat_loc);
	}

	// Write bvh
	ifstream bvh_parse("ressources/human_template.bvh");
	ofstream bvh_file(name);
	string line;
	// Write parsingj
	while(bvh_parse.good())
	{
		getline(bvh_parse, line);	
		
		bvh_file << line << endl;
	}

	cout << "fin du parsing" << endl;
	// Fill value for each frame
	for(int frame=0; frame<quat_matrix_loc.size(); frame++)
	{
		for(int i=0; i<42; i++) bvh_file << 0.000 << " ";
		// set Spine4
		float Rx, Ry, Rz;
		QQuaternion q = quat_matrix_loc[frame][0];
		quat_matrix_loc[frame][0].getEulerAngles(&Rz, &Rx, &Ry); //pitch, yaw, roll 
		bvh_file << Rx << " " << Ry << " " << Rz << " ";

		for(int i=0; i<15; i++) bvh_file << 0.000 << " ";
		// set r_shoulder to r_hand
		for(int ijoint=1; ijoint<quat_matrix_loc[0].size(); ijoint++)
		{
			float Rx, Ry, Rz;
			QQuaternion q = quat_matrix_loc[frame][ijoint];
			quat_matrix_loc[frame][ijoint].getEulerAngles(&Rz, &Rx, &Ry); //pitch, yaw, roll 
			bvh_file << -Rx << " " << Ry << " " << Rz << " ";
			cout << Rx << " " << Ry << " " << Rz << " ";
			//bvh_file << q.scalar() << " " << q.x() << " " << q.y() << " " << q.z() << " " <<endl;
		}
		// set 0 to right arm
		for(int i=0; i<3; i++) bvh_file << 0.000 << " ";
		bvh_file << 75.0 << " ";
		for(int i=0; i<8; i++) bvh_file << 0.000 << " ";
		
		bvh_file << endl;
	}
}

QQuaternion fill_quat(string line) 
{
	float q0, q1, q2, q3;
	string token;
	stringstream ss(line);
	for(int i=0; i<7; i++) ss >> token;
	ss >> q0;
	ss >> q1;
	ss >> q2;
	ss >> q3;
	QQuaternion q(q0, q1, q2, q3);
	return q;
}

 void read_file(string file_name, std::vector<std::vector<QQuaternion>>& quat_matrix, std::vector<QQuaternion>& quat_offset)
{
	// Initialize
	vector<QQuaternion> quat_vect;
	QQuaternion q;
	bool is_first = true;
	string token;
	string line;

	//Open file
	fstream file(file_name.data());
	if (verbose) cout << "Reading "<< file_name <<"..."<< endl;
	if (!file.good())
	{
		cout << "erreur lecture dans " << file_name << endl;
		return;
	}

	// Read file
	file >> token;
	while(getline(file, line))
	{
		stringstream ss(line);
		ss >> token;
		if (token =="PacketCounter")
		break;
	}
	while(getline(file, line))
	{
		q = fill_quat(line);
		if (is_first)
		{
			is_first = false;
			quat_offset.push_back(q);
			continue;
		}
		quat_vect.push_back(q);
	}
	quat_matrix.push_back(quat_vect);
	if (verbose) cout << "End of read" << endl;
}

void print_quat(QQuaternion q)
{
	cout << q.scalar() << " " <<  q.x() << " " << q.y() << " " << q.z() << " " <<endl;
}
