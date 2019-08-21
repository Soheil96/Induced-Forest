#include <iostream>
#include <fstream>
#include <string.h>
#include <set>
#include <stdlib.h>
#include <time.h>
#include <sstream>

using namespace std;

int g[10][10], n, sumd;
bool allowed[10], mark[10];

void build_graph(string graph) {
    memset(g, 0, sizeof(g));
    n = graph[0] - 'A' + 2;
    sumd = 0;
    string s = "";
    for (int i = graph.size() - 1; i >= 1; i--) {
        int k = graph[i] - 63;
        for (int j = 0; j < 6; j++) {
            s += char(int(k % 2) + '0');
            sumd += k % 2;
            k = int(k / 2);
        }
    }
    int po = s.size() - 1;
    for (int j = n - 1; j >= 0; j--)
        for (int i = j - 1; i >= 0; i--)
            g[j][i] = g[i][j] = s[po--] - '0';
}

bool dfs(int v, int par) {
    mark[v] = 1;
    int child = (par == -1? 0 : 1);
    for (int i = 0; i < n; i++)
        if (g[v][i] && allowed[i] && i != par) {
            if (mark[i])
                return 0;
            if (!dfs(i, v))
                return 0;
            child += 1;
        }
    return (child <= 2);
}

double func(string graph) {
    build_graph(graph);
    double t_g = 0;
    for (int i = 0; i < n; i++) {
        double degree = 1;
        for (int j = 0; j < n; j++)
            degree += g[i][j];
        t_g += 1 / degree;
    }

    int lif_g = 0;
    for (int mask = 1; mask < (1 << n); mask++) {
        for (int i = 0; i < n; i++)
            mark[i] = 0, allowed[i] = mask & (1 << i);
        bool ok = true;
        for (int i = 0; i < n; i++)
            if (allowed[i] && !mark[i])
                ok &= dfs(i, -1);
        if (ok)
            lif_g = max(lif_g, __builtin_popcount(mask));
    }
    //cout << graph << "  " << lif_g << " " << t_g << " " << lif_g / t_g << endl;
    return lif_g / t_g;
}

void check(int v) {
    mark[v] = 1;
    for (int i = 0; i < n; i++)
        if (g[v][i] && !mark[i])
            check(i);
}

bool connected() {
    for (int i = 0; i < n; i++)
        mark[i] = 0;
    check(0);
    for (int i = 0; i < n; i++)
        if (mark[i] == 0)
            return 0;
    return 1;
}

int main() {
    for (int node = 3; node < 10; node++) {
        string address = "data/graph";
        address += char(node + '0');
        address += ".g6";
        ifstream fin (address);
        string graph;
        double min_f = 10000000.0;
        string ans_g = " ";
        while (fin >> graph) {
            double tmp = func(graph);
            if(!connected())
                continue;
            if (min_f > tmp) {
                min_f = tmp;
                ans_g = graph;
            }
        }
        cout << node << "   " << min_f << "   " << ans_g << endl;
    }
}
