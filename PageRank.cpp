#include "DataAnalyzer.h"
#include "Basic.h"
#include "Block.h"
#include <stdlib.h>

using namespace std;

int main()
{
    // 数据集分析
    process();
    analyze();
    show();
    // Basic PageRank
    runBasic();
    // Block PageRank
    runBlock();
    system("pause");
    return 0;
}