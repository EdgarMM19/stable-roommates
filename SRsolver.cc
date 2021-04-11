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
bool solveSR(int n, const vvi& preferencesOrder, vii& solution){
    
    // preferencesOrder[i][j] indicates the j preference of i 
    // preferencesMap[i][j] indicates the preference position of j in i lists
    vector<vector<int>> preferencesMap(n, vector<int>(n, -1));
    //O(n^2)
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n-1; ++j)
            preferencesMap[i][preferencesOrder[i][j]] = j;
    }

    vi nextProposal(n,0);
    vi actualAccepted(n,n);
    // O(n^2) amortized
    for (int i = 0; i < n; ++i){
        

        int actProposer = i;
        bool finish = false;
        do{
            if (nextProposal[actProposer] == n-1){
                return false;
            }

            int nextChoice = preferencesOrder[actProposer][nextProposal[actProposer]];
            int howPreferedIsActForNext = preferencesMap[nextChoice][actProposer];

            //amortized O(n^2)
            nextProposal[actProposer]++;

            if (actualAccepted[nextChoice] < howPreferedIsActForNext){
                // not accepted has to continue

            } else {
                if (actualAccepted[nextChoice] == n){

                    // accepted by someone that is its first proposal
                    actualAccepted[nextChoice] = howPreferedIsActForNext;
                    finish = true;

                } else {

                    // accepted by someone that has to reject other proposal
                    int aux = preferencesOrder[nextChoice][actualAccepted[nextChoice]];
                    actualAccepted[nextChoice] = howPreferedIsActForNext;
                    actProposer = aux;
                }
            }

        } while (not finish);
    }
    //O(n^2)
    vvb preStable(n, vb(n-1, true));
    for (int i = 0; i < n; ++i){
        int whoAccepted = preferencesOrder[i][nextProposal[i]-1];
        for (int j = preferencesMap[whoAccepted][i] +1; j < n-1; ++j){
            preStable[whoAccepted][j] = false;
            int otherWay = preferencesOrder[whoAccepted][j];
            preStable[otherWay][preferencesMap[otherWay][whoAccepted]] = false;
        }

    }

    //O(n^2)
    vvi stable(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n-1; ++j)
            if (preStable[i][j])
                stable[i].push_back(preferencesOrder[i][j]);
    
    //O(n^2)
    preferencesMap = vvi(n, vi(n, -1));
    for (int i=0; i<n; i++) {
        for (int j=0; j<stable[i].size(); j++) preferencesMap[i][stable[i][j]] = j;
    }
    
    //O(n)
    vi leftPref(n, 0), secondPref(n, 1), rightPref(n);
    for (int i = 0; i < n; ++i){
        if (stable[i].size() == 0){
            return false;
        }
        rightPref[i] = stable[i].size()-1;
        if (rightPref[i] == 0) secondPref[i] = 0;
    }
    int pi = 0;
    while (true){
        while(pi < n and rightPref[pi] == leftPref[pi])
            pi++;
        if (pi == n) break;
        vi visited(n, -1);
        vi pis;
        int step = 0;


        while (visited[pi] == -1){
            //amortized sum(pi(size)) = O(n^2)
            pis.push_back(pi);
            visited[pi] = step;
            step++;
            int punter = secondPref[pi];
            int qi = stable[pi][punter];
            pi = stable[qi][rightPref[qi]];
        }

        int initSize = pis.size();
        reverse(pis.begin(), pis.end());
        while (pis.size() != initSize - visited[pi]) pis.pop_back();
        reverse(pis.begin(), pis.end());
        
        vi Bs, Cs;
        for (auto ai : pis) {
            Bs.push_back(stable[ai][leftPref[ai]]);
            int punter = secondPref[ai];
            Cs.push_back(stable[ai][punter]);   
        }
        //amortized sum(pi(size)) = O(n^2)
        for (int j = 0; j < pis.size(); ++j){
            int ai = pis[j];
            int bi = Bs[j];
            int ci = Cs[j];
            while (stable[ci][rightPref[ci]] != ai) {
                int bo = stable[ci][rightPref[ci]];
                if (bo != -1){
                    int todelete = preferencesMap[bo][ci];
                    stable[bo][todelete] = -1;
                    while (stable[bo][rightPref[bo]] == -1) rightPref[bo]--;
                    while (stable[bo][leftPref[bo]] == -1) leftPref[bo]++;
                    secondPref[bo] = leftPref[bo]+1;
                    while (secondPref[bo] < rightPref[bo] and stable[bo][secondPref[bo]] == -1) secondPref[bo]++;
                    if (rightPref[bo] < leftPref[bo] or rightPref[bo] < leftPref[bo]) {
                        return false;
                    }
                }
                //amortized O(n^2)
                rightPref[ci]--;
                while (stable[ci][rightPref[ci]] == -1) rightPref[ci]--;             
                if (rightPref[ci] < leftPref[ci]) {
                    return false;
                }
            }
            leftPref[ai] += (stable[ai][leftPref[ai]] == bi);
            while (stable[ai][leftPref[ai]] == -1) leftPref[ai]++;
            secondPref[ai] = leftPref[ai]+1;
            while (secondPref[ai] < rightPref[ai] and stable[ai][secondPref[ai]] == -1) secondPref[ai]++;
            if (rightPref[ai] < leftPref[ai]) {
                return false;
            }
        }
    }

    vii pairs;
    //O(n)
    for (int i = 0; i < n; ++i)
        pairs.push_back(ii({i, stable[i][leftPref[i]]}));
    solution = pairs;
    return true;
}

