#include "DataAnalyzer.h"
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

int FromNodeID, ToNodeID;
int NowNodeID = 0;
int data_row = 0;
int _isolate[10000];
int min_id = 999999, max_id = 0;
int max_degree = 0, now_degree = 0;
int num_src = 0;
int data_num = 0;

void process() 
{
	printf("���ݾ��� ... ... \n");
	ifstream fin("./data/Data.txt");
	ofstream fout("./data/MatrixData.txt");
	int from_id, to_id;
	map<int, vector<int>> maps;
	while (!fin.eof())
	{
		fin >> from_id >> to_id;
		maps[from_id].push_back(to_id);
	}
	for (int i = 0; i < maps.size(); i++)
	{
		if (maps[i].empty())
			continue;
		for (int j = 0; j < maps[i].size(); j++)
			fout << i << " " << maps[i][j] << endl;
	}
	fout.close();
	printf("���ݴ������ \n");
}

void analyze()
{
	ifstream fin("./data/MatrixData.txt");
	while (!fin.eof())
	{
		fin >> FromNodeID >> ToNodeID;
		_isolate[FromNodeID] = 1;
		_isolate[ToNodeID] = 1;
		if (FromNodeID != NowNodeID)
		{
			num_src++;
			NowNodeID = FromNodeID;
			max_degree = max(now_degree, max_degree);
			now_degree = 1;
		}
		else
		{
			now_degree++;
		}
		min_id = min(min(FromNodeID, min_id), min(ToNodeID, min_id));
		max_id = max(max(FromNodeID, max_id), max(ToNodeID, max_id));
		data_row++;
	}
	for (int i = 0; i < 10000; i++)
		if (_isolate[i])
			data_num++;
}

void show()
{
	printf("Read Data ......\n");
	printf("���ݼ�������%d\n", data_row);
	printf("�ǹ����ڵ���Ŀ��%d\n", data_num);
	printf("��С��ID��%d\n", min_id);
	printf("����ID��%d\n", max_id);
	printf("���ĳ��ȣ�%d\n", max_degree);
	printf("Դ�ڵ㼯����Ԫ��������%d\n", num_src);
	printf("Read Data Finished.\n");
}