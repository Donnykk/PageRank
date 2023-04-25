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

// �������ҳ���ֲ�ֵ�ľ���ֵ֮��
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

// ��������������R
int updateR(int round, double Beta)
{
    round++;
    double add, sum, revise, difference;
    cout << "===========��" << round << "��===========" << endl;
    // ��ʼ����һ�ֵ�R����
    memset(R_new, 0.0, sizeof(double) * 8298);
    // ���㹫ʽ��ǰһ��
    for (int i = 0; i < 6110; i++)
    {
        add = Beta * (R_old[M[i].from] / (double)M[i].degree);
        for (int j = 0; j < M[i].degree; j++)
        {
            R_new[M[i].to[j]] += add;
        }
    }
    // ��������ǰ���ܵ÷�
    sum = 0.0;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate[i] == 1)
        {
            sum += R_new[i];
        }
    }
    // ��ɹ�ʽ��һ������
    revise = (1 - sum) / (double)counts;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate[i] == 1)
        {
            R_new[i] += revise;
        }
    }
    // ������������ܵ÷�
    sum = 0.0;
    for (int i = 0; i < 8298; i++)
    {
        if (isolate[i] == 1)
        {
            sum += R_new[i];
        }
    }
    cout << "���ֵ�������ʱ�ܵ÷�Ϊ��" << sum << endl;
    // �ж��Ƿ�Ҫ��������
    difference = calculateDifference();
    if (difference < 1e-10)
    {
        cout << "dif:" << difference << "  ��������!" << endl
             << endl;
        cout << "������" << round << "�ֵ�������!" << endl;
        return 0;
    }
    else
    {
        cout << "dif:" << difference << "  ��������..." << endl
             << endl;
        // ����R_old
        memcpy(R_old, R_new, sizeof(double) * 8298);
        // �ݹ����
        updateR(round, Beta);
    }
}

// ӳ��������R�ļ�����
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

// ����ȫ�����
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

// ��������
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

// ����ǰ100�Ľ��
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

    cout << "========PageRank�����㷨========" << endl
         << endl;
    cout << "���ݼ�·��:"
         << "./Data/MatrixData.txt" << endl
         << endl;
    cout << "Betaֵ:" << Beta << endl
         << endl;
    cout << "���ڶ�ȡ���ݼ�...";

    // ��ȡ����
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

    // ͳ�Ʒǹ����ڵ���Ŀ
    for (int i = 0; i < 8298; i++)
    {
        if (isolate[i] == 1)
        {
            counts++;
        }
    }
    cout << "�ǹ����ڵ�����" << counts << endl
         << endl;

    // ��ʼ��������R
    cout << "��ʼ��������R...";
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
    cout << " ��ʼ����ɣ�" << endl
         << endl;

    // ��ʼ��������
    updateR(round, Beta);

    // ��������ļ���
    system("cd Data");
    system("mkdir BasicResults");

    // ��������R�ļ��������浽�ֵ�
    BasicDictionary *D = convert();

    // ����ȫ�����
    storeWhole(D);

    // ��������
    D = sort(D);

    // ����ǰ100�Ľ��
    storeTop100(D);
}