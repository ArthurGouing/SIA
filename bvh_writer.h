#include<QQuaternion>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

extern bool verbose;

void write_bvh(std::vector<std::string> files, std::string name);//créate bvh file

QQuaternion fill_quat(std::string line); 

void read_file(std::string file_name, std::vector<std::vector<QQuaternion>>& quat_matrix, std::vector<QQuaternion>& quat_offset);


void print_quat(QQuaternion q);
