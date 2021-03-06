/*
 * =====================================================================================
 *
 *       Filename:  find_ladders.cpp
 *
 *        Version:  1.0
 *        Created:  01/01/2014 16:47:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 * =====================================================================================
 */

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>
using namespace std;

vector<string> vdict;
vector<vector<int> > adjList;

void printAdjList(vector<vector<int> > &adj) {
    for (int i = 0; i < adj.size(); i++) {
        cout << i << ": ";
        for (int j = 0; j < adj[i].size(); j++) {
            cout << adj[i][j] << " ";
        }
        cout << endl;
    }
}

void printVDict(vector<string> &vdict) {
    for (int i = 0; i < vdict.size(); i++) {
        cout << i << ": " << vdict[i] << endl;
    }
}

void printRet(vector<vector<string> > &ret) {
    for (int i = 0; i < ret.size(); i++) {
        cout << "[";
        for (int j = 0; j < ret[i].size(); j++) {
            cout << "\"" << ret[i][j] << "\",";
        }
        cout << "]," << endl;
    }
}

void buildAdjList(set<string> &dict) {
    map<string, int> m;
    vdict.clear();
    for (set<string>::iterator it = dict.begin(); it != dict.end(); it++) {
        m[*it] = vdict.size();
        vdict.push_back(*it);
        adjList.push_back(vector<int>(0));
    }
    for (int i = 0; i < vdict.size(); i++) {
        string word = vdict[i];
        for (int j = 0; j < vdict[i].length(); j++) {
            for (char ch = 'a'; ch <= 'z'; ch++) {
                if (ch != vdict[i][j]) {
                    word[j] = ch;
                    if (m.count(word)) {
                        adjList[i].push_back(m[word]);
                    }
                    word[j] = vdict[i][j];
                }
            }
        }
    }
}

void backtrack(vector<int> curr, vector<vector<int> > prev, vector<vector<string> > &ret, int dest, int depth) {
    if (curr.size() == depth) { // base case
        if (curr[depth - 1] == dest) {
            vector<string> tmp;
            for (int i = depth - 1; i >= 0; i--) {
                tmp.push_back(vdict[curr[i]]);
            }
            ret.push_back(tmp);
        }
        return;
    }

    int last = curr[curr.size() - 1];

    for (int i = 0; i < prev[last].size(); i++) {
        curr.push_back(prev[last][i]);
        backtrack(curr, prev, ret, dest, depth);
        curr.pop_back();
    }
}

vector<vector<string> > findLadders(string start, string end, set<string> &dict) {
    // build adjacency list
    dict.insert(start);
    dict.insert(end);
    buildAdjList(dict);

    // find the length of shortest path
    vector<bool> explored;
    vector<int> dist;
    for (int i = 0; i < vdict.size(); i++) {
        explored.push_back(false);
        dist.push_back(INT_MAX);
    }
    queue<int> q;
    vector<vector<int> > prev(vdict.size());
    int iStart, iEnd;
    for (iStart = 0; vdict[iStart] != start; iStart++);
    for (iEnd = 0; vdict[iEnd] != end; iEnd++);
    q.push(iStart);
    dist[iStart] = 0;
    explored[iStart] = true;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        for (int i = 0; i < adjList[curr].size(); i++) {
            int v = adjList[curr][i];
            if (!explored[v]) {
                prev[v].push_back(curr);
                dist[v] = dist[curr] + 1;
                explored[v] = true;
                q.push(v);
                // cout << "dist[" << adjList[curr][i] << "]: " << dist[adjList[curr][i]] << endl;
            }
            else {
                if (dist[v] == dist[curr] + 1) {
                    prev[v].push_back(curr);
                }
            }
        }
    }

    int shortest = dist[iEnd];
    // cout << "shortest path: " << shortest << endl;

    // backtrack
    vector<vector<string> > ret;
    explored.assign(vdict.size(), false);
    vector<int> curr;
    curr.push_back(iEnd);
    backtrack(curr, prev, ret, iStart, shortest + 1);
    return ret;
}

int main() {
    set<string> dict;
    dict.insert("hot");
    dict.insert("dot");
    dict.insert("dog");
    dict.insert("lot");
    dict.insert("log");
    vector<vector<string> > ret = findLadders("hit", "cog", dict);
    printRet(ret);
    return 0;
}
