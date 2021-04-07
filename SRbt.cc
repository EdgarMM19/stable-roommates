#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#define F first
#define S second

using vi = vector<int>;
using vvi = vector<vi>;
using ii = pair<int,int>;
using vii = vector<ii>;
using vb = vector<bool>;
using vvb = vector<vb>;

#define DEBUG(CTR) cerr << #CTR << ": "; for (auto x : CTR) cerr << x << ", "; cerr << endl;

// irvings algorithm: https://iq.opengenus.org/stable-roommates-problem/
// https://uvacs2102.github.io/docs/roomates.pdf
// https://en.wikipedia.org/wiki/Stable_roommates_problem

bool checkSolution(vi& pairs, const vvi& preferencesMap,const int n){
    for(int i = 0; i < n; ++i){
        for(int j = i+1; j < n; ++j){
            if(preferencesMap[i][j] < preferencesMap[i][pairs[i]] and preferencesMap[j][i] < preferencesMap[j][pairs[j]])
                return false;
        }
    }
    return true;
}

bool makePairs(vi& pairs, bool& sol, const int n, int pos, const vvi& preferencesMap){
    if(sol) return true;
    if(pos == n){
        if(checkSolution(pairs, preferencesMap, n)){
            return sol = true;
        }
        return false;
    }
    if(pairs[pos] != -1) return makePairs(pairs, sol, n, pos+1, preferencesMap);
    for(int i = pos+1; i < n; ++i){
        if(pairs[i] == -1){
            pairs[pos] = i;
            pairs[i] = pos;
            if(makePairs(pairs, sol, n, pos+1, preferencesMap)) return true;
            pairs[pos] = -1;
            pairs[i] = -1;
        }
    }
    return false;
}
bool backSR(int n, const vvi& preferencesOrder){
    /*cerr << "start solveSR... preferencesOrder is:" << endl;
    for(auto x : preferencesOrder){
        DEBUG(x)
    }*/
    // preferencesOrder[i][j] indicates the j preference of i 
    // preferencesMap[i][j] indicates the preference position of j in i lists
    vector<vector<int>> preferencesMap(n, vector<int>(n, -1));
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n-1; ++j)
            preferencesMap[i][preferencesOrder[i][j]] = j;
    }
    vi pairs(n,-1);
    bool sol = false;
    return makePairs(pairs, sol, n, 0, preferencesMap);
}


