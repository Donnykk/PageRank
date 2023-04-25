#define _CRT_SECURE_NO_WARNINGS
#include "Basic.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

using namespace std;

BasicMessage M[6110] = {0, 0, {0}};

int isolate[8298];
int counts = 0;
double R_old[8298], R_new[8298];

// 计算各网页评分差值的绝对值之和
double calculateDifference()
{
    double difference = 0;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate[i] == 1)
        {
            difference += (double)fabs(R_new[i] - R_old[i]);
        }
    }
    return difference;
}

// 迭代更新列向量R
int updateR(int round, double Beta)
{
    round++;
    double add, sum, revise, difference;
    cout << "===========第" << round << "轮===========" << endl;
    // 初始化新一轮的R向量
    memset(R_new, 0.0, sizeof(double) * 8298);
    // 计算公式的前一半
    for (int i = 0; i < 6110; i++)
    {
        add = Beta * (R_old[M[i].from] / (double)M[i].degree);
        for (int j = 0; j < M[i].degree; j++)
        {
            R_new[M[i].to[j]] += add;
        }
    }
    // 计算修正前的总得分
    sum = 0.0;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate[i] == 1)
        {
            sum += R_new[i];
        }
    }
    // 完成公式后一半运算
    revise = (1 - sum) / (double)counts;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate[i] == 1)
        {
            R_new[i] += revise;
        }
    }
    // 计算修正后的总得分
    sum = 0.0;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate[i] == 1)
        {
            sum += R_new[i];
        }
    }
    cout << "本轮迭代结束时总得分为：" << sum << endl;
    // 判断是否要继续迭代
    difference = calculateDifference();
    if (difference < 1e-10)
    {
        cout << "dif:" << difference << "  迭代结束!" << endl
             << endl;
        cout << "共经过" << round << "轮迭代结束!" << endl;
        return 0;
    }
    else
    {
        cout << "dif:" << difference << "  继续迭代..." << endl
             << endl;
        // 更新R_old
        memcpy(R_old, R_new, sizeof(double) * 8298);
        // 递归迭代
        updateR(round, Beta);
    }
}

// 映射列向量R的计算结果
BasicDictionary *convert()
{
    BasicDictionary *D = new BasicDictionary[counts];
    int index = 0;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate[i] == 1)
        {
            D[index].id = i;
            D[index].score = R_old[i];
            index++;
        }
    }
    return D;
}

// 保存全部结果
void storeWhole(BasicDictionary *D)
{
    ofstream fwhole("./Data/BasicResults/Whole.txt", ios::out | ios::trunc);
    for (int i = 0; i < counts; i++)
    {
        if (i == counts - 1)
        {
            fwhole << D[i].id << "\t" << D[i].score;
            break;
        }
        fwhole << D[i].id << "\t" << D[i].score << endl;
    }
    fwhole.close();
}

// 降序排序
BasicDictionary *sort(BasicDictionary *D)
{
    for (int i = 0; i < counts; i++)
    {
        for (int j = i + 1; j < counts; j++)
        {
            if (D[i].score < D[j].score)
            {
                BasicDictionary tmp;
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
void storeTop100(BasicDictionary *D)
{
    ofstream f100("./Data/BasicResults/Top100.txt", ios::out | ios::trunc);
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

void runBasic()
{
    double Beta = 0.85;
    int from, to;
    int now = 0, round = 0, index = -1;

    cout << "========PageRank基础算法========" << endl
         << endl;
    cout << "数据集路径:"
         << "./Data/MatrixData.txt" << endl
         << endl;
    cout << "Beta值:" << Beta << endl
         << endl;
    cout << "正在读取数据集...";

    // 读取数据
    ifstream fin("./Data/MatrixData.txt");
    if (fin)
    {
        cout << " 成功读取数据集！" << endl
             << endl;
    }
    else
    {
        cout << " 数据集读取失败！" << endl
             << endl;
    }
    while (!fin.eof())
    {
        fin >> from >> to;
        isolate[from] = 1;
        isolate[to] = 1;
        if (from != now)
        {
            index++;
            M[index].from = from;
            now = from;
        }
        M[index].to[M[index].degree] = to;
        M[index].degree++;
    }
    fin.close();

    // 统计非孤立节点数目
    for (int i = 0; i < 8298; i++)
    {
        if (isolate[i] == 1)
        {
            counts++;
        }
    }
    cout << "非孤立节点数：" << counts << endl
         << endl;

    // 初始化列向量R
    cout << "初始化列向量R...";
    for (int i = 0; i < 8298; i++)
    {
        if (isolate[i] == 1)
        {
            R_old[i] = 1.0 / counts;
        }
        else
        {
            R_old[i] = 0.0;
        }
    }
    cout << " 初始化完成！" << endl
         << endl;

    // 开始迭代计算
    updateR(round, Beta);

    // 创建结果文件夹
    system("cd Data");
    system("mkdir BasicResults");

    // 将列向量R的计算结果保存到字典
    BasicDictionary *D = convert();

    // 保存全部结果
    storeWhole(D);

    // 降序排序
    D = sort(D);

    // 保存前100的结果
    storeTop100(D);
}