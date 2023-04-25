#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "Block.h"
#include "store.h"
using namespace std;

#define BLOCKSIZE 100

BlockMessage DegreeList[6110];

double R_old_[8298], R_new_[8298];
double Beta;
int counts_, block_num;
int isolate_[8298] = {0};

// 获取网页节点的出度
int getDegree(int id)
{
    for (int i = 0; i < 6110; i++)
    {
        if (DegreeList[i].from == id)
        {
            return DegreeList[i].degree;
        }
    }
}

// 分块
void divide(const char *dataFile)
{
    char buffer[1024];
    int i, from, to, block_id, now;

    cout << "=============分块部分=============" << endl
         << endl;
    cout << "正在读取数据集...";
    ifstream fin(dataFile);
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
    block_num = 8298 / BLOCKSIZE + 1;
    cout << "块大小：" << BLOCKSIZE << endl
         << endl;
    cout << "块数量：" << block_num << endl
         << endl;

    // 创建文件存储分块和矩阵
    fstream *fo = new fstream[block_num];
    fstream *fi = new fstream[block_num];
    system("mkdir /Data/Blocks");
    cout << "创建文件...";
    for (i = 0; i < block_num; i++)
    {
        sprintf(buffer, "./Data/Blocks/Block%s%d.txt", i < 10 ? "0" : "", i);
        fo[i].open(buffer, ios::out | ios::trunc | ios::in);
        sprintf(buffer, "./Data/Blocks/Matrix%s%d.txt", i < 10 ? "0" : "", i);
        fi[i].open(buffer, ios::out | ios::trunc | ios::in);
    }
    cout << "  创建成功！" << endl
         << endl;

    // 分块
    cout << "开始分块...";
    while (!fin.eof())
    {
        fin >> from >> to;
        block_id = to / BLOCKSIZE;
        fo[block_id] << from << " " << to << endl;
    }
    cout << "  分块成功！" << endl
         << endl;

    // 构建矩阵
    cout << "构建矩阵...";
    for (i = 0; i < block_num; i++)
    {
        fo[i].seekg(0, ios::beg);
        now = -1;
        while (!fo[i].eof())
        {
            fo[i] >> from >> to;
            if (fo[i].tellp() < 0)
            {
                break;
            }
            if (from != now)
            {
                now = from;
                if (fi[i].tellp() > 0)
                {
                    // 标志每一行结束
                    fi[i] << " -1" << endl;
                }
                fi[i] << from << " " << getDegree(from);
            }
            fi[i] << " " << to;
        }
        // 标志文件结束
        fi[i] << " -1";
        fo[i].close();
        fi[i].close();
    }
    cout << "  构建成功！" << endl
         << endl;
}

// 计算前后两轮所有网页评分结果之差的绝对值之和
double calculateDifference_()
{
    double difference = 0.0;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate_[i] == 1)
        {
            difference += (double)fabs(R_new_[i] - R_old_[i]);
        }
    }
    return difference;
}

// 迭代计算
int updateR(int round)
{
    round++;
    int from, to, degree;
    double R_i, add, sum, revise, difference;
    char buffer[1024];

    cout << "===============第" << round << "轮===============" << endl;
    // 初始化新一轮的R向量
    memset(R_new_, 0.0, sizeof(double) * 8298);
    // 开始分块计算
    sprintf(buffer, "./Data/Dat/R%s%d.dat", (round - 1) < 10 ? "0" : "", round - 1);
    ifstream bi(buffer, ios::in | ios::binary);
    cout << "读取上一轮计算的R向量： " << buffer << endl;
    for (int i = 0; i < block_num; i++)
    {
        sprintf(buffer, "./Data/Blocks/Matrix%s%d.txt", i < 10 ? "0" : "", i);
        ifstream fi(buffer);
        while (!fi.eof())
        {
            fi >> from >> degree;
            bi.seekg(from * sizeof(double), ios::beg);
            bi.read((char *)&R_i, sizeof(double));
            // 完成公式前一半的计算
            add = Beta * (R_i / (double)degree);
            while (true)
            {
                fi >> to;
                if (to == -1)
                {
                    // -1标志一行数据读取完毕
                    break;
                }
                R_new_[to] += add;
            }
        }
        fi.close();
    }
    // 计算修正前的总得分
    sum = 0.0;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate_[i] == 1)
        {
            sum += R_new_[i];
        }
    }
    // 完成公式后一半运算
    revise = (1 - sum) / (double)counts_;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate_[i] == 1)
        {
            R_new_[i] += revise;
        }
    }
    // 计算修正后的总得分
    sum = 0.0;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate_[i] == 1)
        {
            sum += R_new_[i];
        }
    }
    cout << "本轮迭代结束时总得分为：" << sum << endl;
    // 判断是否要继续迭代
    difference = calculateDifference_();
    if (difference < 1e-10)
    {
        cout << "dif:" << difference << "  迭代结束!" << endl
             << endl;
        cout << "共经过" << round << "轮迭代结束!" << endl;
        return 0;
    }
    else
    {
        cout << "dif:" << difference << "  继续迭代..." << endl;
        // 保存本轮R向量的计算结果
        sprintf(buffer, "./Data/Dat/R%s%d.dat", (round) < 10 ? "0" : "", round);
        ofstream fo(buffer, ios::out | ios::binary | ios::trunc);
        cout << "保存本轮计算的R向量到： " << buffer << endl
             << endl;
        for (int i = 0; i < 8298; i++)
        {
            fo.write((char *)&R_new_[i], sizeof(R_new_[i]));
        }
        fo.close();
        // 更新R_old_
        memcpy(R_old_, R_new_, sizeof(double) * 8298);
        // 递归迭代
        updateR(round);
    }
}

void runBlock()
{
    Beta = 0.85;
    int from, to;
    int now = 0, index = -1, round = 0;

    cout << "==========PageRank优化算法==========" << endl
         << endl;
    cout << "数据集路径:"
         << "./Data/MatrixData.txt" << endl
         << endl;
    cout << "Beta值:" << Beta << endl
         << endl;
    cout << "正在读取数据集...";
    // 读取数据集
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
        isolate_[from] = 1;
        isolate_[to] = 1;
        if (from != now)
        {
            index++;
            DegreeList[index].from = from;
            now = from;
        }
        DegreeList[index].degree++;
    }
    fin.close();

    // 统计非孤立节点数目
    for (int i = 0; i < 8298; i++)
    {
        if (isolate_[i] == 1)
        {
            counts_++;
        }
    }
    cout << "非孤立节点数目：" << counts_ << endl
         << endl;

    // 分块
    divide("./Data/MatrixData.txt");

    // 初始化列向量R
    cout << "初始化列向量R...";
    for (int i = 0; i < 8298; i++)
    {
        if (isolate_[i] == 1)
        {
            R_old_[i] = 1.0 / counts_;
        }
        else
        {
            R_old_[i] = 0.0;
        }
    }
    system("mkdir /Data/Dat");
    ofstream fo("./Data/Dat/R00.dat", ios::out | ios::binary | ios::trunc);
    for (int i = 0; i < 8298; i++)
    {
        fo.write((char *)&R_old_[i], sizeof(R_old_[i]));
    }
    fo.close();
    cout << " 初始化完成！" << endl
         << endl;

    // 开始迭代计算
    updateR(round);

    // 创建结果文件夹
    system("mkdir /Data/BlockResults");

    // 将列向量R的计算结果保存到字典
    BlockDictionary *D = convert();

    // 保存全部结果
    storeWhole(D);

    // 降序排序
    D = sort(D);

    // 保存前100的结果
    storeTop100(D);
}