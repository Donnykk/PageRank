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

// ��ȡ��ҳ�ڵ�ĳ���
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

// �ֿ�
void divide(const char *dataFile)
{
    char buffer[1024];
    int i, from, to, block_id, now;

    cout << "=============�ֿ鲿��=============" << endl
         << endl;
    cout << "���ڶ�ȡ���ݼ�...";
    ifstream fin(dataFile);
    if (fin)
    {
        cout << " �ɹ���ȡ���ݼ���" << endl
             << endl;
    }
    else
    {
        cout << " ���ݼ���ȡʧ�ܣ�" << endl
             << endl;
    }
    block_num = 8298 / BLOCKSIZE + 1;
    cout << "���С��" << BLOCKSIZE << endl
         << endl;
    cout << "��������" << block_num << endl
         << endl;

    // �����ļ��洢�ֿ�;���
    fstream *fo = new fstream[block_num];
    fstream *fi = new fstream[block_num];
    system("mkdir /Data/Blocks");
    cout << "�����ļ�...";
    for (i = 0; i < block_num; i++)
    {
        sprintf(buffer, "./Data/Blocks/Block%s%d.txt", i < 10 ? "0" : "", i);
        fo[i].open(buffer, ios::out | ios::trunc | ios::in);
        sprintf(buffer, "./Data/Blocks/Matrix%s%d.txt", i < 10 ? "0" : "", i);
        fi[i].open(buffer, ios::out | ios::trunc | ios::in);
    }
    cout << "  �����ɹ���" << endl
         << endl;

    // �ֿ�
    cout << "��ʼ�ֿ�...";
    while (!fin.eof())
    {
        fin >> from >> to;
        block_id = to / BLOCKSIZE;
        fo[block_id] << from << " " << to << endl;
    }
    cout << "  �ֿ�ɹ���" << endl
         << endl;

    // ��������
    cout << "��������...";
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
                    // ��־ÿһ�н���
                    fi[i] << " -1" << endl;
                }
                fi[i] << from << " " << getDegree(from);
            }
            fi[i] << " " << to;
        }
        // ��־�ļ�����
        fi[i] << " -1";
        fo[i].close();
        fi[i].close();
    }
    cout << "  �����ɹ���" << endl
         << endl;
}

// ����ǰ������������ҳ���ֽ��֮��ľ���ֵ֮��
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

// ��������
int updateR(int round)
{
    round++;
    int from, to, degree;
    double R_i, add, sum, revise, difference;
    char buffer[1024];

    cout << "===============��" << round << "��===============" << endl;
    // ��ʼ����һ�ֵ�R����
    memset(R_new_, 0.0, sizeof(double) * 8298);
    // ��ʼ�ֿ����
    sprintf(buffer, "./Data/Dat/R%s%d.dat", (round - 1) < 10 ? "0" : "", round - 1);
    ifstream bi(buffer, ios::in | ios::binary);
    cout << "��ȡ��һ�ּ����R������ " << buffer << endl;
    for (int i = 0; i < block_num; i++)
    {
        sprintf(buffer, "./Data/Blocks/Matrix%s%d.txt", i < 10 ? "0" : "", i);
        ifstream fi(buffer);
        while (!fi.eof())
        {
            fi >> from >> degree;
            bi.seekg(from * sizeof(double), ios::beg);
            bi.read((char *)&R_i, sizeof(double));
            // ��ɹ�ʽǰһ��ļ���
            add = Beta * (R_i / (double)degree);
            while (true)
            {
                fi >> to;
                if (to == -1)
                {
                    // -1��־һ�����ݶ�ȡ���
                    break;
                }
                R_new_[to] += add;
            }
        }
        fi.close();
    }
    // ��������ǰ���ܵ÷�
    sum = 0.0;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate_[i] == 1)
        {
            sum += R_new_[i];
        }
    }
    // ��ɹ�ʽ��һ������
    revise = (1 - sum) / (double)counts_;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate_[i] == 1)
        {
            R_new_[i] += revise;
        }
    }
    // ������������ܵ÷�
    sum = 0.0;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate_[i] == 1)
        {
            sum += R_new_[i];
        }
    }
    cout << "���ֵ�������ʱ�ܵ÷�Ϊ��" << sum << endl;
    // �ж��Ƿ�Ҫ��������
    difference = calculateDifference_();
    if (difference < 1e-10)
    {
        cout << "dif:" << difference << "  ��������!" << endl
             << endl;
        cout << "������" << round << "�ֵ�������!" << endl;
        return 0;
    }
    else
    {
        cout << "dif:" << difference << "  ��������..." << endl;
        // ���汾��R�����ļ�����
        sprintf(buffer, "./Data/Dat/R%s%d.dat", (round) < 10 ? "0" : "", round);
        ofstream fo(buffer, ios::out | ios::binary | ios::trunc);
        cout << "���汾�ּ����R�������� " << buffer << endl
             << endl;
        for (int i = 0; i < 8298; i++)
        {
            fo.write((char *)&R_new_[i], sizeof(R_new_[i]));
        }
        fo.close();
        // ����R_old_
        memcpy(R_old_, R_new_, sizeof(double) * 8298);
        // �ݹ����
        updateR(round);
    }
}

void runBlock()
{
    Beta = 0.85;
    int from, to;
    int now = 0, index = -1, round = 0;

    cout << "==========PageRank�Ż��㷨==========" << endl
         << endl;
    cout << "���ݼ�·��:"
         << "./Data/MatrixData.txt" << endl
         << endl;
    cout << "Betaֵ:" << Beta << endl
         << endl;
    cout << "���ڶ�ȡ���ݼ�...";
    // ��ȡ���ݼ�
    ifstream fin("./Data/MatrixData.txt");
    if (fin)
    {
        cout << " �ɹ���ȡ���ݼ���" << endl
             << endl;
    }
    else
    {
        cout << " ���ݼ���ȡʧ�ܣ�" << endl
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

    // ͳ�Ʒǹ����ڵ���Ŀ
    for (int i = 0; i < 8298; i++)
    {
        if (isolate_[i] == 1)
        {
            counts_++;
        }
    }
    cout << "�ǹ����ڵ���Ŀ��" << counts_ << endl
         << endl;

    // �ֿ�
    divide("./Data/MatrixData.txt");

    // ��ʼ��������R
    cout << "��ʼ��������R...";
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
    cout << " ��ʼ����ɣ�" << endl
         << endl;

    // ��ʼ��������
    updateR(round);

    // ��������ļ���
    system("mkdir /Data/BlockResults");

    // ��������R�ļ��������浽�ֵ�
    BlockDictionary *D = convert();

    // ����ȫ�����
    storeWhole(D);

    // ��������
    D = sort(D);

    // ����ǰ100�Ľ��
    storeTop100(D);
}