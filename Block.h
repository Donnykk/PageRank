#pragma once
struct BlockMessage
{
    // 源节点，该节点的出度
    int from, degree;
};
struct BlockDictionary
{
    // 网页节点编号，评分结果
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