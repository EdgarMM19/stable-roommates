#include <iostream>
#include <vector>
#include <map>
using namespace std;


#define F first
#define S second

using vi = vector<int>;
using vvi = vector<vi>;
using ii = pair<int,int>;
using vii = vector<ii>;
using vb = vector<bool>;
using vvb = vector<vb>;

int randomNumber(int n);
vector<int> generateRandomPermutation(int len, bool startAtOne);

// irvings algorithm: https://iq.opengenus.org/stable-roommates-problem/
// https://uvacs2102.github.io/docs/roomates.pdf
// https://en.wikipedia.org/wiki/Stable_roommates_problem
vii solveSR(int n, const vvi& preferencesOrder, bool& hasSolution){

    // preferencesOrder[i][j] indicates the j preference of i 
    // preferencesMap[i][j] indicates the preference position of j in i lists
    vector<map<int, int> > preferencesMap(n);
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n-1; ++j)
            preferencesMap[i][preferencesOrder[i][j]] = j;
    }
    vi nextProposal(n,0);
    vi actualAccepted(n,n);
    for(int i = 0; i < n; ++i){
        int actProposer = i;
        bool finish = false;
        do{
            if(nextProposal[actProposer] == n){
                    hasSolution = false;
                    return vii();
            }
            int nextChoice = preferencesOrder[actProposer][nextProposal[actProposer]];
            nextProposal[actProposer]++;
            if(actualAccepted[nextChoice] < preferencesMap[nextChoice][actProposer]){
                // not accepted has to continue
            }else{
                if(actualAccepted[nextChoice] == n){
                    // accepted by someone that is its first proposal
                    actualAccepted[nextChoice] = preferencesMap[nextChoice][actProposer];
                    finish = true;
                }else{
                    // accepted by someone that has to reject other proposal
                    int aux = preferencesOrder[nextChoice][actualAccepted[nextChoice]];
                    actualAccepted[nextChoice] = preferencesMap[nextChoice][actProposer];
                    actProposer = aux;
                }
            }
        } while(not finish);
    }
    
    vvb preStable(n, vb(n-1, true));
    for(int i = 0; i < n; ++i){
        int whoAccepted = preferencesOrder[i][nextProposal[i]-1];
        for(int j = preferencesMap[whoAccepted][i] +1; j < n-1; ++j){
            preStable[whoAccepted][j] = false;
            int otherWay = preferencesOrder[whoAccepted][j];
            preStable[otherWay][preferencesMap[otherWay][whoAccepted]] = false;
        }
    }
    vvi stable(n);
    for(int i = 0; i < n; ++i)
    for(int j = 0; j < n; ++j)
        if(preStable[i][j])
            stable[i].push_back(preferencesOrder[i][j]);
 
   
    vi leftPref(n), rightPref(n);
    for(int& x : leftPref) x = 0;
    for(int i = 0; i < n; ++i){
        if(stable[i] == 0){
            hasSolution = false;
            return vii(); 
        }
        rightPref[i] = stable[i].size()-1;

    }
    
    int finish = false;
    while(not finish){
        finish = true;
        for(int i = 0; i < n; ++i){
            if(rightPref[i] != leftPref[i]){
                finish = false;
                vi visited(n, -1);
                int pi = i;
                vi pis;
                int step = 0;

                while(visited[pi] == -1){

                    pis.push_back(pi);
                    visited[pi] = step;
                    step++;
                    // aixo esta mal:
                    int qi = stable[pi][leftPref[pi]+1];
                    pi = stable[qi][rightPref[qi]];
                }

                for(int j = visited[pi]; j < pis.size(); ++j){
                    // aixo esta mal:
                    int ai = pis[j];
                    leftPref[ai]++;
                    int bi = stable[ai][leftPref];

                    // is this okey??
                    while(rightPref[bi] >= leftPref[bi] and stable[bi][rightPref[bi]] != ai){
                        rightPref[bi]--;
                   }
                    // and this?
                    if(rightPref[bi] < leftPref[bi]){
                       hasSolution = false;
                       return vii();     
                   }
                    //assert(rightPref[bi] >= 0);
                    //rightPref[bi]--;

                }
                for(int i = 0; i < n; ++i)
                    if(leftPref[i] > rightPref[i]){
                        hasSolution = false;
                        return vii();
                    }
                
                break;
            }
        }
    }
    vii pairs;
    hasSolution = true;
    for(int i = 0; i < n; ++i)
        pairs.push_back(ii({i, stable[i][leftPref[i]]}));
    return pairs;
}


int main(){
    for(int n = 2; n < 100; n+=2){
    //int n;
    //cin >> n; //has to be even
        cout << "??" << endl;
    vvi preferences(n);
    for(int i = 0; i < n; ++i){
        preferences[i] = generateRandomPermutation(n-1, 0);
        for(int& x : preferences[i]){
            if(x >= i) x++;
            assert(x >= 0 and x < n and x != i);
        }
    }
    cout << "!!" << endl;
    bool hasSolution;
   /*n = 6;
    preferences = vvi(6, vi(5));
    preferences[0][0] = 2;
    preferences[0][1] = 6;
    preferences[0][2] = 4;
    preferences[0][3] = 3;
    preferences[0][4] = 5;

    preferences[1][0] = 3;
    preferences[1][1] = 5;
    preferences[1][2] = 1;
    preferences[1][3] = 6;
    preferences[1][4] = 4;

    preferences[2][0] = 1;
    preferences[2][1] = 6;
    preferences[2][2] = 2;
    preferences[2][3] = 5;
    preferences[2][4] = 4;

    preferences[3][0] = 5;
    preferences[3][1] = 2;
    preferences[3][2] = 3;
    preferences[3][3] = 6;
    preferences[3][4] = 1;

    preferences[4][0] = 6;
    preferences[4][1] = 1;
    preferences[4][2] = 3;
    preferences[4][3] = 4;
    preferences[4][4] = 2;
    
    preferences[5][0] = 4;
    preferences[5][1] = 2;
    preferences[5][2] = 5;
    preferences[5][3] = 1;
    preferences[5][4] = 3;*/

    //for(auto& x : preferences)
    //    for(auto& y : x) y--;
    cout << n << ": " << endl;
    vii matches = solveSR(n, preferences, hasSolution);
    if(hasSolution){
        cout << "yes" << endl;
       /* for(ii x : matches){
            if(x.F < x.S)
            cerr << x.F+1 << " <3 " << x.S+1 << endl;
        }
*/    }
    else{
        cerr << "no solution" << endl;
    }
    }
}