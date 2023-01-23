#include"bvh_writer.h"

using namespace std;

void write_bvh(std::vector<std::string> files, std::string name)//créate bvh file
{
	vector<vector<QQuaternion>> quat_matrix;
	vector<QQuaternion> quat_offset;
	QQuaternion q;
	bool is_first=true;

	// Lecture des fichiers
	for(int ifile=0; ifile<files.size(); ifile++)
	{
		read_file(files[ifile], quat_matrix, quat_offset);
	}

	// Calcule
	for(int frame=0; frame< quat_matrix[0].size();frame++)
	{
		for (int icap=0; icap<quat_matrix.size()-1; icap++)
		{
			QQuaternion q_loc;
			cout << frame << ", " << icap<< endl;
			QQuaternion q_1 = quat_matrix[icap][frame]   * quat_offset[icap].inverted(); 
			QQuaternion q_2 = quat_matrix[icap+1][frame] * quat_offset[icap+1].inverted(); 

			q_loc = q_1 * q_2 * q_1.inverted();
			print_quat(q_loc);
		}
	}

	// Write bvh
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
	if (verbose) cout << "Reading "<< file_name <<"..."<< endl;
	vector<QQuaternion> quat_vect;
	QQuaternion q;
	bool is_first = false;
	fstream file(file_name.data());
	if (!file.good())
	{
		cout << "erreur lecture dans " << file_name << endl;
		return;
	}
	string token;
	string line;
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
