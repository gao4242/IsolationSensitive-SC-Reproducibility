#pragma pack (1)
#include <iostream>
#include <vector>        //提供向量头文件
#include <algorithm>     // 算法头文件，提供迭代器
#include <fstream>       //提供文件头文件
#include <iomanip>       //C++输出精度控制需要
#include<queue>
#include<ctime>

const int MAXN = 12000;

const double INF = pow(10, 20);
int edges[MAXN][2];
int d;//工人和任务差值
int c;//同区约值
int l=1;//工人工作量
int lenn;
int lenm;

int E_val[MAXN][MAXN];   // 记录每条边的权值
int L_val[MAXN];      // 每个左顶点的值
int R_val[MAXN];       // 每个右顶点的值
bool L_vis[MAXN];    // 记录每一轮匹配匹配过的左顶点
bool R_vis[MAXN];     // 记录每一轮匹配匹配过的右顶点
int match[MAXN];        // 记录每个右顶点匹配到的左顶点 如果没有则为-1
double slack[MAXN];        // 记录每个右顶点如果能被左顶点匹配最少还需要多少值
int worker[MAXN][3];
int quest[MAXN][3];
int assignment[MAXN][MAXN];
int countW = 0;//工人数
int countQ = 0;//任务数
int countM = 0;//匹配数
using namespace std;

constexpr auto MAX_NUM = 10000;//最大顶点数目;
int aa[MAXN][4];
bool visited[MAX_NUM];
int Node[MAX_NUM][100];
int composition[MAX_NUM][MAX_NUM];//单个区域由哪些区域组成
int composition1[MAX_NUM][MAX_NUM];
int m;//生成新图的第几个节点
int m1;
int num;//由几个节点组成新的节点
int n;
int S=0;//总体匹配次数
int s;
int C;
int turn;//轮次

struct ENode {//弧的定义
	int ivex;//存放顶点在数组中的位置
	ENode* nextEdge;//指向下一条弧的指针
};
struct VNode {//顶点定义
	int workerNum;//工人数
	int questNum;//任务数
	ENode* firstEdge;//指向第一条依附该顶点的弧
};
struct Graph {//图的定义
	VNode Vertexs[MAX_NUM];//顶点表
	int Vexnum, Arcnum;//图的当前顶点数和边数
};

struct SqQueue {
	int queue[MAX_NUM];
	int front; //头指针，队非空时指向队头元素 
	int rear;//尾指针，队非空时指向队尾元素的下一位置 
};

