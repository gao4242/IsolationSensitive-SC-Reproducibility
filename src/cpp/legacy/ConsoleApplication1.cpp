// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include"Graph.h"


Graph G;
Graph G1;
Graph G2;
Graph G3;
SqQueue Q;

void readline(const char* strPath, int& line)
{
	ifstream is(strPath);
	char c;
	while (is.get(c))
	{
		if (c == '\n')
			line++;
	}
	is.close();
}
void linkLast(ENode* list, ENode* node)
{
	ENode* p = list;

	while (p->nextEdge)
		p = p->nextEdge;
	p->nextEdge = node;
}
int getPosition(int x, Graph G)
{
	int i;
	for (i = 0; i < G.Vexnum; i++)
		if (i == x)
			return i;
	return -1;
}
int InitQueue() {
	if (!&Q)
		return 0;
	Q.front = Q.rear = 0;
	return 1;
}

void EnQueue(int e)
{
	Q.queue[Q.rear] = e;
	Q.rear++;
}

int DeQueue()
{
	int e;
	if (Q.front == Q.rear)
		return 0;
	e = Q.queue[Q.front];
	Q.front = (Q.front + 1) % MAX_NUM;
	return  e;
}

int QueueEmpty()
{
	if (Q.front == Q.rear)
		return 1;
	else return 0;
}

void matirix() {//读入各区域工人任务位置数量生成工人矩阵、任务矩阵、权值矩阵
	vector<int> V;
	vector<int>::iterator it;

	const char* filename = "../60-10.txt";
	ifstream file(filename);

	int d;
	while (file >> d)
	{
		V.push_back(d);//将数据压入堆栈
	}
	file.close();
	int line = 1;
	readline(filename, line);
	for (it = V.begin(); it != V.end(); )
	{
		for (int i = 0; i < line; i++)//定义行循环
		{
			for (int j = 0; j < 4; j++)//定义列循环
			{
				aa[i][j] = *it;
				it++;
			}
		}break;
	}
	G.Vexnum = line;
	G.Arcnum = 0;
	G1.Vexnum = 0;
	G1.Arcnum = 0;
	ENode* node1;

	for (int i = 0; i < G.Vexnum; i++) {
		G.Vertexs[i].firstEdge = NULL;
		G.Vertexs[i].workerNum = aa[i][2] * l;
		G.Vertexs[i].questNum = aa[i][3];
		for (int j = 0; j < G.Vexnum; j++) {
			if (abs(aa[i][0] - aa[j][0]) < 2 && abs(aa[i][1] - aa[j][1]) < 2 && (i != j)) {
				edges[G.Arcnum][0] = i;
				edges[G.Arcnum][1] = j;
				G.Arcnum++;
			}
		}
	}
	
	clock_t startTime, endTime;
	startTime = clock();
	int p1, p2;
	for (int i = 0; i < G.Arcnum; i++) {
		p1 = getPosition(edges[i][0], G);
		p2 = getPosition(edges[i][1], G);
		node1 = new ENode();
		node1->ivex = p2;
		if (G.Vertexs[p1].firstEdge == NULL)
			G.Vertexs[p1].firstEdge = node1;
		else
			linkLast(G.Vertexs[p1].firstEdge, node1);
	}
	
	endTime = clock();
	cout << "建图G耗时: " << (endTime - startTime) << "ms" << endl;
}


void createline(Graph& G1) {
	int edge[MAX_NUM][3] = {};
	bool visited1[MAXN];
	ENode* node1, * node2;
	for (int i = 0; i < m; i++) {
		visited1[i] = false;
	}
	int a = 0, b = 1;
	if (m > 0) {
		for (int i = 0; i < G.Arcnum; i++) {
			for (int j = 1; j < composition[m][0]+1; j++) {
				if (edges[i][0] == composition[m][j] && visited[edges[i][1]]) {
					int z = 1;
					while (b) {
						if (edges[i][1] == composition[m][z]) {
							b = 0;
							break;
						}
						else {
							z++;
						}
						if (z == composition[m][0] + 1 && b == 1) {
							edge[a][0] = edges[i][0];
							edge[a][1] = edges[i][1];
							a++;
							break;
						}

					}
					b = 1;

				}

			}
		}

	}
	for (int z = 0; z < a; z++) {
		for (int i = 0; i < m; i++) {
			for (int j = 1; j < composition[i][0] + 1; j++) {
				if (edge[z][1] == composition[i][j]) {
					edge[z][2] = i;

				}

			}
		}
	}
	for (int i = 0; i < a; i++) {
		if (!visited1[edge[i][2]]) {
			node1 = new ENode();
			node1->ivex = edge[i][2];
			if (G1.Vertexs[m].firstEdge == NULL)
				G1.Vertexs[m].firstEdge = node1;
			else
				linkLast(G1.Vertexs[m].firstEdge, node1);
			node2 = new ENode();
			node2->ivex = m;
			if (G1.Vertexs[edge[i][2]].firstEdge == NULL)
				G1.Vertexs[edge[i][2]].firstEdge = node2;
			else
				linkLast(G1.Vertexs[edge[i][2]].firstEdge, node2);
			visited1[edge[i][2]] = true;
			//cout << edge[i][0]  << edge[i][1] << edge[i][2] << endl;
		}
		/*if (edge[i][2]  == m) {
			for (int i = 0; i < m; i++) {
				visited1[i] = true;
			}
		}*/
	}

}
bool dfs(int now)
{

	L_vis[now] = true;
	for (int i = 0; i < lenm; i++)
	{
		if (R_vis[i]) continue; // 每一轮匹配 每个右顶点只尝试一次
		double tmp = L_val[now] + R_val[i] - E_val[now][i];

		if (tmp == 0) {  // 如果符合要求
			R_vis[i] = true;
			n++;
			if (match[i] == -1 || dfs(match[i])) {    // 找到一个没有匹配的右顶点 或者该右顶点当前匹配的左顶点可以找到其它匹配
				match[i] = now;
				return true;
			}
		}
		else {
			slack[i] = min(slack[i], tmp);
		}

	}

	return false;
}
int KM(Graph G,Graph g) {
	int bb[MAXN][4] = {};
	int a = 0,b=0;
	countW =0, countM=0, countQ = 0;
	for (int i = 0; i < g.Vexnum; i++) {
		
			//cout << i << "(WN:" << g.Vertexs[i].workerNum << ",QN:" << g.Vertexs[i].questNum << ")" << endl;
			bb[a][2] = g.Vertexs[i].workerNum;
			bb[a][3] = g.Vertexs[i].questNum;
			if (turn == 1) {
				b=getPosition(composition[i][composition[i][0]], G);
			}
			else if (turn == 0) {
				b = i;
			}
			else if (turn == 2) {
				int x = 0;
				x=getPosition(composition[i][composition[i][0]], G1);
				for (int j = 0; j < m1; j++) {
					for (int z = 1; z < composition1[j][0] + 1; z++) {
						if (x == j) {
							b = getPosition(composition1[j][composition1[j][0]], G);

						}
					}
				}
			}
			//cout <<"原" << b << endl;
			bb[a][0] = b / 10 % 10;
			bb[a][1] = b % 10;
			
			//cout << bb[a][0] << bb[a][1] <<"w" << bb[a][2] <<"q" << bb[a][3] << endl;
			for (int j = 0; j < bb[a][3]; j++) {
				countQ++;
				quest[countQ - 1][1] = bb[a][0];
				quest[countQ - 1][2] = bb[a][1];
				quest[countQ - 1][0] = countQ;
			}
			
			for (int j = 0; j < bb[a][2]; j++) {
				countW++;
				worker[countW-1][1] = bb[a][0];
				worker[countW-1][2] = bb[a][1];
				worker[countW-1][0] = countW;
				
			}
		a++;
	}
	if (countQ == 0) {
		 C = 0;
		return 0;
	}
	cout << "工人数" << countW << endl;
	/*for (int i = 0; i < countW; i++) {
		cout << "工人" << i << "(" << worker[i][1] << "," << worker[i][2] << ")" << setprecision(1) << endl;
	}*/
	cout << "任务数" << countQ << endl;
	
	
	/*for (int i = 0; i < countQ; i++) {
		cout << "任务" << i << "(" << quest[i][1] << "," << quest[i][2] << ")" << setprecision(1) << endl;
	}*/
	if ( countW >= countQ) {
		countM = countW;
		for (int i = 0; i < countW; i++) {
			for (int j = 0; j < countQ; j++) {
				assignment[i][j] = -abs(pow((pow((worker[i][1] - quest[j][1]), 2) + pow((worker[i][2] - quest[j][2]), 2)), 0.5));
				//assignment[countW + i][j] = assignment[i][j];
			}
		}
		for (int i = 0; i < countM; i++) {
			for (int j = countQ; j < countM; j++) {
				assignment[i][j] = -10000;
			}
			for (int j = 0; j < countM; j++) {

				E_val[i][j] = assignment[i][j];
				//cout << E_val[i][j] << "   ";
			}
			//cout << endl;
		}
	}
	lenm = lenn = countM;
	memset(match, -1, sizeof(match));    // 初始每个右顶点都没有匹配的左顶点
	memset(R_val, 0, sizeof(R_val));   // 初始每个右顶点的值为0
	for (int i = 0; i < lenm; ++i) {             //初始每个左顶点为与其相连边的最大权值
		L_val[i] = E_val[i][0];
		for (int j = 1; j < lenm; ++j) {
			L_val[i] = max(L_val[i], E_val[i][j]);
		}
	}
	int p = 0;
	// 尝试为每一个左顶点匹配
	for (int i = 0; i < lenm; ++i) {
		fill(slack, slack + lenm, INF);    // 因为要取最小值 初始化为无穷大
		while (1) {
			// 为每个左顶点匹配的方法是 ：如果找不到就降低期望值，直到找到为止
			// 记录每轮匹配中左右顶点是否被尝试匹配过
			memset(L_vis, false, sizeof(L_vis));
			memset(R_vis, false, sizeof(R_vis));
			p++;
			if (dfs(i)) break;  // 找到匹配 退出
			// 如果不能找到 就降低期望值
			// 最小可降低的期望值
			double d = INF;
			for (int j = 0; j < lenm; ++j)
				if (!R_vis[j]) d = min(d, slack[j]);
			for (int j = 0; j < lenm; ++j) {
				// 所有访问过的(被涉及的)左顶点降低值
				if (L_vis[j]) L_val[j] -= d;
				// 所有访问过(被涉及的)的右顶点增加值
				if (R_vis[j]) R_val[j] += d;
				// 没有访问过的右顶点 因为左顶点的期望值降低，距离被左顶点匹配又进了一步
				else slack[j] -= d;
			}
		}
	}
	// 匹配完成 求出所有匹配的权值和
	
	int count = 0;
	 C = 0;
	for (int i = 0; i < lenm; ++i) {
		if (E_val[match[i]][i] != -INF) {
			count++;
		}

	}
	for (int i = 0; i < lenm; i++) {
		C += E_val[match[i]][i];
			//cout << "工人" << match[i] << "匹配任务" << i << "权值" << E_val[match[i]][i] << endl;
	}
	C = C + 10000 * (countW - countQ);
	cout << "匹配次数" << p << endl;
	cout << "跨区域代价：" << -C << endl;
	cout << "匹配结果数：" << count << endl;
	return 0;
}
void createClique(VNode v, int x, Graph G, Graph& G1) {
	ENode* node, * node1;
	node = v.firstEdge;
	int w = v.workerNum;
	int q = v.questNum;
	Node[MAX_NUM][100] = {};
	num = 0, n = 2;
	int a = 1, b = 1;
	while (node != NULL) {
		/*if (G.Vertexs[node->ivex].questNum == 0 && G.Vertexs[node->ivex].workerNum == 0) {
			node = node->nextEdge;
		}*/
		if (node != NULL && !visited[node->ivex]) {
			
			node1 = G.Vertexs[node->ivex].firstEdge;//初始点中找
			if (node->nextEdge == NULL)
				break;
			while (a) {
				while (node1 != NULL) {
					if (node == NULL || node->nextEdge == NULL)
						break;
					if (node1->ivex == node->nextEdge->ivex) {
						w = w + G.Vertexs[node->ivex].workerNum;
						q = q + G.Vertexs[node->ivex].questNum;
						Node[m][num] = node->ivex;
						num++;
						node = node->nextEdge;
						a = 0;
						break;
					}
					else {
						node1 = node1->nextEdge;
					}
				}
				if (a == 1) {
					if (node == NULL || node->nextEdge == NULL)
						break;
					node1 = G.Vertexs[node->ivex].firstEdge;
					node = node->nextEdge;
					if (visited[node->ivex]) {
						node = node->nextEdge;
					}
				}
			}
			a = 1;

		}
		else if (node != NULL) node = node->nextEdge;
	}
	if (w >= 0 && q >= 0 && abs(w - q) <= x) {
		G1.Vertexs[G1.Vexnum].workerNum = w;
		G1.Vertexs[G1.Vexnum].questNum = q;
		G1.Vertexs[G1.Vexnum].firstEdge = NULL;
		for (int i = 0; i < num; i++) {
			
				visited[Node[m][i]] = true;
				composition[m][n] = Node[m][i];
				n++;
				//cout << "V" << Node[m][i] << " ";
			
		}
		//cout << endl;
		G1.Vexnum++;
	}
	else {
		int i = 0;
		while (1) {
			if (G.Vertexs[Node[m][i]].workerNum == 0 && G.Vertexs[Node[m][i]].questNum == 0) {
				i++;
			}
			int w1 = w - G.Vertexs[Node[m][i]].workerNum;
			int q1 = q - G.Vertexs[Node[m][i]].questNum;
			if (w1>=0&&q1>=0&&abs(w1 - q1) <= x) {
				G1.Vertexs[G1.Vexnum].workerNum = w1;
				G1.Vertexs[G1.Vexnum].questNum = q1;
				G1.Vertexs[G1.Vexnum].firstEdge = NULL;
				for (int j = 0; j < num; j++) {
					visited[Node[m][j]] = true;
				}
				visited[Node[m][i]] = false;
				for (int j = 0; j < num; j++) {
					if (visited[Node[m][j]] == true) {
						
							composition[m][n] = Node[m][j];
							n++;
							//cout << "V" << Node[m][j] << " ";
						
					}
				}
				//cout << endl;
				G1.Vexnum++;
				break;
			}
			i++;
			if (i >= num - 1) {
				b = 0;
				break;
			}

		}
		if (b == 0) {
			int i = 0, j = 0;
			if (G.Vertexs[Node[m][i]].workerNum == 0 && G.Vertexs[Node[m][i]].questNum == 0) {
				i++;
			}
			int w1 = w - G.Vertexs[Node[m][i]].workerNum;
			int q1 = q - G.Vertexs[Node[m][i]].questNum;
			while (1) {
				if (i == j||(G.Vertexs[Node[m][j]].workerNum == 0 && G.Vertexs[Node[m][j]].questNum == 0)) {
					j++;
				}
				int w2 = w1 - G.Vertexs[Node[m][j]].workerNum;
				int q2 = q1 - G.Vertexs[Node[m][j]].questNum;
				if (w1 >= 0 && q1 >= 0 && w2 >= 0 && q2 >= 0 && abs(w2 - q2) <= x) {
					G1.Vertexs[G1.Vexnum].workerNum = w2;
					G1.Vertexs[G1.Vexnum].questNum = q2;
					G1.Vertexs[G1.Vexnum].firstEdge = NULL;
					for (int z = 0; z < num; z++) {
						visited[Node[m][z]] = true;
					}
					visited[Node[m][i]] = visited[Node[m][j]] = false;
					for (int z = 0; z < num; z++) {
						if (visited[Node[m][z]] == true) {
							
								composition[m][n] = Node[m][z];
								n++;
								//cout << "V" << Node[m][z] << " ";
							
						}
					}
					//cout << endl;
					G1.Vexnum++;
					break;
				}
				j++;
				if (j >= num) {
					i++;
					j = 0;
				}
				if (i >= num) {
					G1.Vertexs[G1.Vexnum].workerNum = G.Vertexs[composition[m][1]].workerNum;
					G1.Vertexs[G1.Vexnum].questNum = G.Vertexs[composition[m][1]].questNum;
					G1.Vertexs[G1.Vexnum].firstEdge = NULL;
					G1.Vexnum++;
					//cout << endl;
					break;
				}
			}
		}
	}
}

void nodeMerge(Graph G, Graph& G1) {
	ENode* node;
	InitQueue();
	m = 0;
	composition[MAX_NUM][MAX_NUM] = {};
	for (int i = 0; i < G.Vexnum; i++) {

		if (!visited[i]) {
			visited[i] = true;
			composition[m][1] = i;
			//cout << "第" << m << "个节点包含G中" << "V" << i;
			EnQueue(i);
			while (!QueueEmpty()) {
				i = DeQueue();
				createClique(G.Vertexs[i], d, G, G1);
				composition[m][0] = n - 1;//节点中有几个点组成
				createline(G1);
			}
			m++;
		}
	}
	/*for (int i = 0; i < m; i++) {
		for (int j = 1; j < composition[i][0]+1; j++) {
			cout << composition[i][j];
		}
		cout << endl;
	}*/
	/*
	cout <<"G1节点数：" << G1.Vexnum << endl;
	cout << "G1节点"<< endl;
	for (int i = 0; i < G1.Vexnum; i++)
	{
		cout << "V" << i + 1 << "(" << "WN:" << G1.Vertexs[i].workerNum << ",QN:" << G1.Vertexs[i].questNum << ")";
		cout << endl;
	}*/
	//for (int i = 0; i < G1.Vexnum; i++)
	//{
	//	cout << "V" << i << "(" << "WN:" << G1.Vertexs[i].workerNum << ",QN:" << G1.Vertexs[i].questNum << "): ";
	//	node = G1.Vertexs[i].firstEdge;
	//	while (node != NULL)
	//	{
	//		//cout << "V" << node->ivex << "  ";
	//		node = node->nextEdge;
	//	}
	//	cout << endl;
	//}
}





int SpatialHM(Graph& G) {
	 s = 0;
	
	for (int i = 0; i < G.Vexnum; i++) {
		visited[i] = false;
	}
	for (int i = 0; i < G.Vexnum; i++) {
		
//cout << "起始V" << i << "(WN:" << G.Vertexs[i].workerNum << ",QN:" << G.Vertexs[i].questNum << ")" << endl;
		if (G.Vertexs[i].workerNum >= G.Vertexs[i].questNum) {
			G.Vertexs[i].workerNum = G.Vertexs[i].workerNum - G.Vertexs[i].questNum;
			s = s + G.Vertexs[i].questNum;
			G.Vertexs[i].questNum = 0;
		}
		else if(G.Vertexs[i].workerNum < G.Vertexs[i].questNum){
			if ((G.Vertexs[i].questNum - G.Vertexs[i].workerNum <= c)&& G.Vertexs[i].workerNum>0){
				s = s + G.Vertexs[i].workerNum;
				G.Vertexs[i].workerNum = 0;
					G.Vertexs[i].questNum = 0;
			}
			G.Vertexs[i].questNum = G.Vertexs[i].questNum - G.Vertexs[i].workerNum;
			s = s + G.Vertexs[i].workerNum;
			G.Vertexs[i].workerNum = 0;
		}
		
		//cout << i << "(WN:" << G.Vertexs[i].workerNum << ",QN:" << G.Vertexs[i].questNum << ")" << endl;
		/*if (G.Vertexs[i].questNum == 0 && G.Vertexs[i].workerNum == 0) {
			visited[i] = true;
		}*/
		
	}
	
	//cout << "匹配次数" << s << endl;

}

int main()
{
	clock_t startTime, endTime,Sa,E;
	Sa = clock();
	startTime = clock();
	matirix();
	endTime = clock();//计时结束
	cout << "建图G耗时: " << (endTime - startTime) << "ms" << endl;
    c = 4;
	d = 4;
	//c = 2;
	startTime = clock();
	SpatialHM(G);
	//cout << s;
	endTime = clock();//计时结束
	cout << "起始同区匹配耗时: " << (endTime - startTime) << "ms" << endl;

	startTime = clock();
	turn = 0;
	KM(G, G);
	S = S-C;
	endTime = clock();//计时结束
	cout << "km: " << (endTime - startTime) << "ms" << endl;

	
	cout << "第一轮！" << endl;
	startTime = clock();
	nodeMerge(G, G1);
	endTime = clock();//计时结束
	cout << "建图G1耗时: " << (endTime - startTime) << "ms" << endl;
	startTime = clock();
	SpatialHM(G1);
	turn = 1;
	endTime = clock();//计时结束
	cout << "同区匹配耗时: " << (endTime - startTime) << "ms" << endl;

	startTime = clock();
	KM(G,G1);
	endTime = clock();//计时结束
	cout << "km算法耗时: " << (endTime - startTime) << "ms" << endl;

	//S = S + s - C;
	//cout << "第一轮代价:" << s << endl;
	//
	//cout << "第二轮！" << endl;
	//startTime = clock();
	//for (int i = 0; i < m; i++) {
	//	for (int j = 0; j < composition[i][0] + 1; j++) {
	//		composition1[i][j]=composition[i][j];
	//	}
	//}
	//m1 = m;
	//nodeMerge(G1, G2);
	//endTime = clock();
	//cout << "建G2: " << (endTime - startTime) << "ms" << endl;
	//startTime = clock();
	//SpatialHM(G2);
	//turn = 2;
	//endTime = clock();
	//cout << "同区匹配: " << (endTime - startTime) << "ms" << endl;
	//startTime = clock();
	//KM(G,G2);
	//endTime = clock();//计时结束
	//cout << "km: " << (endTime - startTime) << "ms" << endl;
	//S = S + 2*s - C;
	//cout << "第二轮代价:" << 2 * s << endl;


	cout << "总体代价:" << S << endl;
	E = clock();//计时结束
	cout << "总体耗时: " << (E-Sa)<< "ms" << endl;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
