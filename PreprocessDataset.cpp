#include"AvtivityCodeGen.h"
#include<iostream>
#include<vector>
#include<map>
#include<list>
#include<io.h>
#include<direct.h>
using namespace std;

AvtivityCodeGen cg;
//获取文件夹下所有的文件名
void GetFiles(string dirpath, list<string>& files) {
	files.clear();
	long hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(dirpath).append("\\*").c_str(), &fileinfo)) == -1)
	{
		cout << dirpath << "没有找到！" << endl;
	}
	else
	{
		while (_findnext(hFile, &fileinfo) == 0)
		{
			//是目录文件的话继续
			if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
				continue;//这里也可以进行递归
			//加入列表
			files.push_back(p.assign(dirpath).append("\\").append(fileinfo.name));
		}
	}
	_findclose(hFile);
}

void _FC2Code2Pseudocode(string question_path, string output_question_path)
{
	int split = question_path.find_last_of("\\");
	string question_name = question_path.substr(split);
	
	list<string> flowchart_dir_list;
	GetFiles(question_path, flowchart_dir_list);

	for (string flowchart_dir : flowchart_dir_list)
	{
		int split = flowchart_dir.find_last_of("\\");
		string file_name = flowchart_dir.substr(split + 1);

		if (file_name == "info.txt")
		{
			ifstream in_file(flowchart_dir, ios::in);
			ofstream out_file(output_question_path + "\\" + file_name, ios::out);
			string line;
			while (getline(in_file, line))
			{
				out_file << line << endl;
			}
			continue;
		}

		list<string> files;
		GetFiles(flowchart_dir, files);
		split = flowchart_dir.find_last_of("\\");

		string flowchart_dir_name = flowchart_dir.substr(split + 1);

		for (string file : files) {
			
			if (_access(output_question_path.c_str(), 0) == -1)		//如果文件夹不存在
				_mkdir(output_question_path.c_str());				//则创建

			string dir = output_question_path + "\\" + flowchart_dir_name;
			_mkdir(dir.c_str());


			int split = file.find_last_of("\\");
			file_name = file.substr(split + 1);
			/*
			if (file_name == "code.txt")
			{


				ifstream in_file(file, ios::in);
				ofstream out_file(dir + "\\" + file_name, ios::out);
				string line;
				while (getline(in_file, line))
				{
					out_file << line << endl;
				}
				continue;
			}*/

			if (file_name == "flowchart.txt")
			{
				cg.FC2Pseudocode(file, dir + "\\pseudo_code.txt");
			}
		}
	}

}

void FC2Code2Pseudocode(string dataset_path, string output_path) {
	list<string> questions;
	vector<string> sDataset = {"\\train", "\\valid", "\\test" };
	for (string dataset : sDataset)
	{
		string _dataset_path = dataset_path + "\\" + dataset;
		string _output_path = output_path + "\\" + dataset;

		_mkdir(_output_path.c_str());
		GetFiles(_dataset_path, questions);
		for (string question_path : questions)
		{
			int split = question_path.find_last_of("\\");
			string question_name = question_path.substr(split);
			_FC2Code2Pseudocode(_dataset_path + "\\" + question_name, _output_path + "\\" + question_name);
		}
	}

}

int main2() {
	string dataset_path = "D:\\FC2Code";
	string output_path = "D:\\FC2Code";
	FC2Code2Pseudocode(dataset_path, output_path);

	system("pause");
	return 0;
}