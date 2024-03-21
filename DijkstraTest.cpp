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

        //�ҳ������ʼ�ڵ�����ĵ㣬����һ��ֻ�ǽ��õ�ľ�����ǰ���ڵ�ȷ���ˣ���û�ж�����з��ʡ�
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

        //next_node û�б����ʹ����������Ѿ��Ǿ����ʼ�ڵ�����ĵ��ˣ���ʹ֮��������·�����Ե���õ㣬����Ҳ�϶�������Զ
        //next_node �����Ľڵ����̾����Ѿ�����ȷ����
        flag[next_node] = true;

        //������ɢ����next_node���Ե���Ľڵ�����ǳ���
        for (int j = 1; j < 8; ++j) 
        {
            int dis = graph[next_node][j] == INVALID_FLAG ? INVALID_FLAG : graph[next_node][j] + dist[next_node];

            //���jû�б����ʹ�(��ʹj�����ʹ�Ҳ�������dist[j]>dis�����)�����ҵ�ǰ�����Ǳ����ڵ�dist[j]С�ģ�Ĭ�ϾͿ��԰�next_node��Ϊǰ���ڵ㡣
            //�����ֱ�next_node��������Ľڵ�ʱ���Ϳ����滻next_node �ˡ�
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

        //��ӡ·��
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