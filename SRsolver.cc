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
    /*cerr << "preferencesMap is: " << endl;
    for (auto x : preferencesMap) {
        DEBUG(x)
    }*/
    vi nextProposal(n,0);
    vi actualAccepted(n,n);

    for (int i = 0; i < n; ++i){
        

        int actProposer = i;
        bool finish = false;
        do{
            //DEBUG(nextProposal);
            //cerr << actProposer << endl;
            if (nextProposal[actProposer] == n-1){
                return false;
            }

            int nextChoice = preferencesOrder[actProposer][nextProposal[actProposer]];
            int howPreferedIsActForNext = preferencesMap[nextChoice][actProposer];
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

    /*for (auto pst : preStable) {
        DEBUG(pst);
    }*/

    vvi stable(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n-1; ++j)
            if (preStable[i][j])
                stable[i].push_back(preferencesOrder[i][j]);
 
    preferencesMap = vvi(n, vi(n, -1));
    for (int i=0; i<n; i++) {
        for (int j=0; j<stable[i].size(); j++) preferencesMap[i][stable[i][j]] = j;
    }

    vi leftPref(n, 0), secondPref(n, 1), rightPref(n);
    for (int i = 0; i < n; ++i){
        if (stable[i].size() == 0){
            return false;
        }
        rightPref[i] = stable[i].size()-1;
        if (rightPref[i] == 0) secondPref[i] = 0;
    }
    /*cerr << "stable phase" << endl;
    for(auto st : stable){
        DEBUG(st)
    }
    DEBUG(leftPref);
    DEBUG(rightPref);
    cerr << "stable phase1" << endl;
    */
    while (true){
        int pi = -1;
        for (int i = 0; i < n; ++i){
            if (rightPref[i] != leftPref[i]){
                pi = i;
                break;
            }
        }
        if (pi == -1) break;
        vi visited(n, -1);
        vi pis;
        int step = 0;

        /*cerr << "Pi: " << pi << endl;
        for (int k=0; k<n; k++) {
            cerr << "stable right: ";
            for (int d=leftPref[k]; d<=rightPref[k]; d++) {
                if(stable[k][d] != -1) cerr << stable[k][d] << ", ";
            }
            cerr << endl;
        }*/

        while (visited[pi] == -1){
            pis.push_back(pi);
            visited[pi] = step;
            step++;
            // aixo esta mal:
            if (leftPref[pi] == rightPref[pi]) {
                cout << "Input basura" << endl;
                char bas; cin >> bas;
                return false;
            }
            int punter = secondPref[pi];
            int qi = stable[pi][punter];
            pi = stable[qi][rightPref[qi]];
            //DEBUG(pis)
        }

        //DEBUG(pis);

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

        //DEBUG(pis);
        for (int j = 0; j < pis.size(); ++j){
            //DEBUG(leftPref);
            //DEBUG(rightPref);

            /*for (int k=0; k<n; k++) {
                cerr << "stable right inner: ";
                for (int d=leftPref[k]; d<=rightPref[k]; d++) {
                    cerr << stable[k][d] << ", ";
                }
                cerr << endl;
            }*/

            int ai = pis[j];
            int bi = Bs[j];
            int ci = Cs[j];
            //cerr << ai << "  ) " << bi << " - " << ci << endl;
            //DEBUG(pis);

            while (stable[ci][rightPref[ci]] != ai) {
                int bo = stable[ci][rightPref[ci]];
                if (bo != -1) stable[bo][preferencesMap[bo][ci]] = -1;
                rightPref[ci]--;
            }
            leftPref[ai] += (stable[ai][leftPref[ai]] == bi);

            for (int i=0; i<n; i++) {
                while (stable[i][rightPref[i]] == -1) rightPref[i]--;
                while (stable[i][leftPref[i]] == -1) leftPref[i]++;
                secondPref[i] = leftPref[i]+1;
                while (secondPref[i] < rightPref[i] and stable[i][secondPref[i]] == -1) secondPref[i]++;
             
                if (rightPref[i] < leftPref[i] or rightPref[i] < leftPref[i]) {
                    return false;
                }
            }
            //cerr << endl;

            //DEBUG(leftPref);
            //DEBUG(rightPref);
        }
    }

    vii pairs;
    for (int i = 0; i < n; ++i)
        pairs.push_back(ii({i, stable[i][leftPref[i]]}));
    solution = pairs;
    return true;
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
