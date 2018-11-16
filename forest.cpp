#include <iostream>
#include <fstream>
#include <set>
#include <stdlib.h>
#include <time.h>
#include <sstream>

using namespace std;
const int log_size = 500;

bool g[30][30], allowed[30], mark[30];
set <string> graphs;
string graph[log_size];
int forest[log_size], edges[log_size], cnt, n;

string encode() {
    string ans = "";
    ans += char('A' + n - 1);
    int num = 0, po = 0;
    for (int j = 1; j < n; j++)
        for (int i = 0; i < j; i++, po++) {
            if (po % 7 == 0)
                ans += char(num + 63), num = 0;
            num *= 2;
            num += g[i][j];
            edges[cnt] += g[i][j];
        }
    if (num)
        ans += char(num + 63);
    return ans;
}

bool generate_graph() {
    n = rand() % 22 + 4;
    int density = rand() % 10 + 3;
    for (int i = 1; i < n; i++)
        for (int j = 0; j < i; j++)
            g[i][j] = g[j][i] = (rand() % 15 < density);
    graph[cnt] = encode();
    if (graphs.find(graph[cnt]) != graphs.end())
        return 0;
    graphs.insert(graph[cnt]);
    return 1;
}

void save_log(int log_num) {
    stringstream ss;
    ss << log_num;
    string file = "logs/log" + ss.str() + ".csv";
    ofstream fout(file);
    fout << "Graph, Number of vertices, Number of edges, Maximum forest size\n";
    for (int i = 0; i < log_size; i++) {
        fout << graph[i] << ',' << int(graph[i][0] - 'A') + 1 << ',' << edges[i] << ',' << forest[i] << '\n';
        forest[i] = edges[i] = 0;
    }
    cnt = 0;
}

bool dfs(int v, int par) {
    mark[v] = 1;
    for (int i = 0; i < n; i++)
        if (g[v][i] && allowed[i] && i != par) {
            if (mark[i])
                return 0;
            if (!dfs(i, v))
                return 0;
        }
    return 1;
}

void find_forest() {
    for (int mask = 1; mask < (1 << n); mask++) {
        for (int i = 0; i < n; i++)
            mark[i] = 0, allowed[i] = mask & (1 << i);
        bool ok = true;
        for (int i = 0; i < n; i++)
            if (allowed[i] && !mark[i])
                ok &= dfs(i, -1);
        if (ok)
            forest[cnt] = max(forest[cnt] , __builtin_popcount(mask));
    }
}

int main(){
    int log_num = 1;
    srand (time(NULL));
    while (true) {
        if (cnt == log_size)
            save_log(log_num++);
        if (!generate_graph())
            continue;
        find_forest();
        cnt++;
    }
}
