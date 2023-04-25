#pragma once
struct BasicMessage
{
    // 源节点，该节点的出度，链接节点
    int from, degree, to[893];
};
struct BasicDictionary
{
    // 网页编号，得分结果
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