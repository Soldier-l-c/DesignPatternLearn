#include "stdafx.h"
#include "DijkstraTest.h"
#include "DesignBase.h"

using namespace std;

#define INVALID_FLAG INT_MAX

void GetPath(vector<vector<int>>& graph, vector<int>& prve, vector<bool> flag, vector<int>& dist, int center_node)
{
    flag[center_node] = 0;
    dist[center_node] = 0;

    int Min = 0;
    while(Min != INVALID_FLAG)
    {
        Min = INVALID_FLAG;

        int next_node = 1;

        //找出距离初始节点最近的点，再上一步只是将该点的距离与前导节点确定了，并没有对其进行访问。
        for (int j = 1; j < 8; ++j) 
        {  
            if (!flag[j] && Min > dist[j]) 
            {
                Min = dist[j];
                next_node = j;
            }
        }

        if (Min == INVALID_FLAG)
            break;

        //next_node 没有被访问过，但是它已经是距离初始节点最近的点了，即使之后有其他路径可以到达该点，距离也肯定比现在远
        //next_node 到中心节点的最短距离已经可以确定了
        flag[next_node] = true;

        //向外扩散，把next_node可以到达的节点距离标记出来
        for (int j = 1; j < 8; ++j) 
        {
            int dis = graph[next_node][j] == INVALID_FLAG ? INVALID_FLAG : graph[next_node][j] + dist[next_node];

            //如果j没有被访问过(即使j被访问过也不会出现dist[j]>dis的情况)，并且当前距离是比现在的dist[j]小的，默认就可以把next_node作为前导节点。
            //当出现比next_node距离更近的节点时，就可以替换next_node 了。
            if (!flag[j] && dis < dist[j]) 
            {
                prve[j] = next_node;
                dist[j] = dis;
            }
        }
    }
}
int StartTestDijstra() 
{
    const auto center_node = 4;

    string str = " ABCDEFG";
    vector<int>prve_node(8, INVALID_FLAG);
    vector<bool>visit_flag(8, false);
    vector<vector<int>>graph(8, prve_node);

    graph[1][2] = 12;
    graph[1][6] = 16;
    graph[1][7] = 14;
    graph[2][1] = 12;
    graph[2][3] = 10;
    graph[2][6] = 7;
    graph[3][2] = 10;
    graph[3][4] = 3;
    graph[3][5] = 5;
    graph[3][6] = 6;
    graph[4][3] = 3;
    graph[4][5] = 4;
    graph[5][3] = 5;
    graph[5][4] = 4;
    graph[5][6] = 2;
    graph[5][7] = 8;
    graph[6][2] = 7;
    graph[6][1] = 16;
    graph[6][3] = 6;
    graph[6][5] = 2;
    graph[6][7] = 9;
    graph[7][1] = 14;
    graph[7][6] = 9;
    graph[7][5] = 8;

    vector<int>dist(8, INVALID_FLAG);

    GetPath(graph, prve_node, visit_flag, dist, center_node);

    for (int i = 1; i < 8; ++i) 
    {
        cout << str[center_node] << ": " << str[i] << ":" << dist[i] << endl;

        //打印路径
        auto pre = i;
        while (pre != INVALID_FLAG)
        {
            cout << str[pre];
            pre = prve_node[pre];
            if (pre == INVALID_FLAG)
            {
                cout << endl;
                break;
            }
            cout << "->";
        }
    }

}

void DijkstraTest::StartTest()
{
    StartTestDijstra();
}
