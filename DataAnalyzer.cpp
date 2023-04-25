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
	printf("数据矩阵化 ... ... \n");
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
	printf("数据处理完成 \n");
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
	printf("数据集行数：%d\n", data_row);
	printf("非孤立节点数目：%d\n", data_num);
	printf("最小的ID：%d\n", min_id);
	printf("最大的ID：%d\n", max_id);
	printf("最大的出度：%d\n", max_degree);
	printf("源节点集合中元素数量：%d\n", num_src);
	printf("Read Data Finished.\n");
}