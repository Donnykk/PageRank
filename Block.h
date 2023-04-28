#pragma once
struct BlockMessage
{
    // Դ�ڵ㣬�ýڵ�ĳ���
    int from, degree;
};
struct BlockDictionary
{
    // ��ҳ�ڵ��ţ����ֽ��
    int id;
    double score;
};
int getDegree(int);
void divide(const char *);
double calculateDifference_();
int updateR(int);
BlockDictionary* block_convert();
void storeWhole(BlockDictionary*);
BlockDictionary* sort(BlockDictionary*);
void storeTop100(BlockDictionary*);
void runBlock();