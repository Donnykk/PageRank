#include "Block.h"
#include <iostream>
#include <fstream>
using namespace std;

extern int isolate_[8298];
extern double R_old_[8298];
extern int counts_;

// 将列向量R的计算结果保存到字典
BlockDictionary *convert()
{
	BlockDictionary *D = new BlockDictionary[counts_];
	int index = 0;
	for (int i = 0; i < 8298; i++)
	{
		if (isolate_[i] == 1)
		{
			D[index].id = i;
			D[index].score = R_old_[i];
			index++;
		}
	}
	return D;
}

// 保存全部结果
void storeWhole(BlockDictionary *D)
{
	ofstream fwhole("./Data/BlockResults/Whole.txt", ios::out | ios::trunc);
	for (int i = 0; i < counts_; i++)
	{
		if (i == counts_ - 1)
		{
			fwhole << D[i].id << "\t" << D[i].score;
			break;
		}
		fwhole << D[i].id << "\t" << D[i].score << endl;
	}
	fwhole.close();
}

// 降序排序
BlockDictionary *sort(BlockDictionary *D)
{
	for (int i = 0; i < counts_; i++)
	{
		for (int j = i + 1; j < counts_; j++)
		{
			if (D[i].score < D[j].score)
			{
				BlockDictionary tmp;
				tmp.id = D[i].id;
				tmp.score = D[i].score;
				D[i].id = D[j].id;
				D[i].score = D[j].score;
				D[j].id = tmp.id;
				D[j].score = tmp.score;
			}
		}
	}
	return D;
}

// 保存前100的结果
void storeTop100(BlockDictionary *D)
{
	ofstream f100("./Data/BlockResults/Top100.txt", ios::out | ios::trunc);
	for (int i = 0; i < 100; i++)
	{
		if (i == 99)
		{
			f100 << D[i].id << "\t" << D[i].score;
			break;
		}
		f100 << D[i].id << "\t" << D[i].score << endl;
	}
	f100.close();
}