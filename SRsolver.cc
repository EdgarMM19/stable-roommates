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

int randomNumber(int n);
vector<int> generateRandomPermutation(int len, bool startAtOne);

// irvings algorithm: https://iq.opengenus.org/stable-roommates-problem/
// https://uvacs2102.github.io/docs/roomates.pdf
// https://en.wikipedia.org/wiki/Stable_roommates_problem
vii solveSR(int n, const vvi& preferencesOrder, bool& hasSolution){
    cerr << "startSR" << endl;
    for(auto x : preferencesOrder){
        DEBUG(x)
    }
    // preferencesOrder[i][j] indicates the j preference of i 
    // preferencesMap[i][j] indicates the preference position of j in i lists
    vector<vector<int>> preferencesMap(n, vector<int>(n, -1));
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n-1; ++j)
            preferencesMap[i][preferencesOrder[i][j]] = j;
    }
    for (auto x : preferencesMap) {
        DEBUG(x)
    }
    vi nextProposal(n,0);
    vi actualAccepted(n,n);

    for (int i = 0; i < n; ++i){
        int actProposer = i;
        bool finish = false;
        do{
            if (nextProposal[actProposer] == n-1){
                hasSolution = false;
                return vii();
            }

            int nextChoice = preferencesOrder[actProposer][nextProposal[actProposer]];
            int howPreferedIsActForNext = preferencesMap[nextChoice][actProposer];
            nextProposal[actProposer]++;
                                                                cerr << "binary6" << endl;

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

    vvb preStable(n, vb(n-1, true));
    for (int i = 0; i < n; ++i){
        int whoAccepted = preferencesOrder[i][nextProposal[i]-1];
        for (int j = preferencesMap[whoAccepted][i] +1; j < n-1; ++j){
            preStable[whoAccepted][j] = false;
            int otherWay = preferencesOrder[whoAccepted][j];
            //cerr << n << " " << j << " " << whoAccepted << " " << otherWay << " " << preferencesMap[otherWay][whoAccepted] << endl;
            preStable[otherWay][preferencesMap[otherWay][whoAccepted]] = false;
        }

    }
    vvi stable(n);
    for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
        if (preStable[i][j])
            stable[i].push_back(preferencesOrder[i][j]);
 
    vi leftPref(n, 0), rightPref(n);
    for (int i = 0; i < n; ++i){
        if (stable[i].size() == 0){
            hasSolution = false;
            return vii(); 
        }
        rightPref[i] = stable[i].size()-1;
    }
    // JAVIER ENTEN FINS AQUI
    int finish = false;
    cerr << "stable phase" << endl;
    for(auto x : stable){
        DEBUG(x)
    }
    cerr << "stable phase1" << endl;

    while (not finish){
        finish = true;
        int pi = -1;
        for (int i = 0; i < n; ++i){
            if (rightPref[i] != leftPref[i]){
                finish = false;
                pi = i;
                break;
            }
        }
        if (pi == -1) break;
        cerr << "pi " << pi << endl;
        vi visited(n, -1);
        vi pis;
        int step = 0;

        while (visited[pi] == -1){

            pis.push_back(pi);
            visited[pi] = step;
            step++;
            // aixo esta mal:
            int qi = stable[pi][leftPref[pi]+1];
            pi = stable[qi][rightPref[qi]];
        }

        DEBUG(pis);

        int initSize = pis.size();
        reverse(pis.begin(), pis.end());
        while (pis.size() != initSize - visited[pi]) pis.pop_back();
        reverse(pis.begin(), pis.end());
        
        for (int j = 0; j < pis.size(); ++j){
            int ai = pis[j];
            int bi = stable[ai][leftPref[ai]];
            int ci = stable[ai][leftPref[ai]+1];
            while (stable[ci][rightPref[ci]] != ai) rightPref[ci]--;
            leftPref[ai]++;
            if (rightPref[bi] < leftPref[bi] or rightPref[ai] < leftPref[ai]) {
                hasSolution = false;
                return vii();
            }
        }
    }

    vii pairs;
    hasSolution = true;
    for (int i = 0; i < n; ++i)
        pairs.push_back(ii({i, stable[i][leftPref[i]]}));
    return pairs;
}


/*
int main(){
    
    int n;
    cin >> n; //has to be even
    cout << "??" << endl;
    vvi preferences(n);
    for (int i = 0; i < n; ++i){
        preferences[i] = generateRandomPermutation(n-1, 0);
        for (int& x : preferences[i]){
            if (x >= i) x++;
            assert(x >= 0 and x < n and x != i);
        }
    }
    cout << "!!" << endl;
    
    int n = 6;
    vvi preferences(n, vi(n-1));

    preferences[0][0] = 4;
    preferences[0][1] = 6;
    preferences[0][2] = 2;
    preferences[0][3] = 5;
    preferences[0][4] = 3;
    
    preferences[1][0] = 6;
    preferences[1][1] = 3;
    preferences[1][2] = 5;
    preferences[1][3] = 1;
    preferences[1][4] = 4;
    
    preferences[2][0] = 4;
    preferences[2][1] = 5;
    preferences[2][2] = 1;
    preferences[2][3] = 6;
    preferences[2][4] = 2;
    
    preferences[3][0] = 2;
    preferences[3][1] = 6;
    preferences[3][2] = 5;
    preferences[3][3] = 1;
    preferences[3][4] = 3;
    
    preferences[4][0] = 4;
    preferences[4][1] = 2;
    preferences[4][2] = 3;
    preferences[4][3] = 6;
    preferences[4][4] = 1;
    
    preferences[5][0] = 5;
    preferences[5][1] = 1;
    preferences[5][2] = 4;
    preferences[5][3] = 2;
    preferences[5][4] = 3;

    for(auto& x : preferences)
        for(auto& y : x) y--;
    bool hasSolution;
    vii matches = solveSR(n, preferences, hasSolution);
    if (hasSolution){
        cout << "yes" << endl;
        for (ii x : matches){
            if (x.F < x.S) cerr << x.F+1 << " <3 " << x.S+1 << endl;
        }
    }
    else{
        cerr << "no solution" << endl;
    }
}
*/
