#pragma once
struct BasicMessage
{
    // Դ�ڵ㣬�ýڵ�ĳ��ȣ����ӽڵ�
    int from, degree, to[893];
};
struct BasicDictionary
{
    // ��ҳ��ţ��÷ֽ��
    int id;
    double score;
};
double calculateDifference();
int updateR(int, double);
BasicDictionary *convert();
void storeWhole(BasicDictionary *);
BasicDictionary *sort(BasicDictionary *);
void storeTop100(BasicDictionary *);
void runBasic();