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
                cout << nextChoice+1 << " rejected " << actProposer+1 << endl;
            }else{
                if(actualAccepted[nextChoice] == n){
                    cout << nextChoice+1 << " accepts for first time and its " << actProposer+1 << endl;
                    actualAccepted[nextChoice] = preferencesMap[nextChoice][actProposer];
                    finish = true;
                }else{
                    int aux = preferencesOrder[nextChoice][actualAccepted[nextChoice]];
                    cout << nextChoice+1 << " accepts " << actProposer+1 << " and rejects "  << aux+1 << endl;
                    actualAccepted[nextChoice] = preferencesMap[nextChoice][actProposer];
                    actProposer = aux;
                }
            }
        } while(not finish);
    }
    for(int i = 0; i < n; ++i)
        cout << actualAccepted[i]+1 << " " << i+1 << endl;
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
            
    for(auto x : preStable){
        for(auto y : x) cout << y << " ";
        cout << endl;
    }
    return vii();
}


int main(){
    int n;
    cin >> n; //has to be even
    vvi preferences(n);
    for(int i = 0; i < n; ++i){
        preferences[i] = generateRandomPermutation(n-1, 0);
        for(int& x : preferences[i]){
            if(x >= i) x++;
        }
    }
    bool hasSolution;
    n = 6;
    preferences = vvi(6, vi(5));
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
    preferences[4][1] = 6;
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
    vii matches = solveSR(n, preferences, hasSolution);
    if(hasSolution){
        for(ii x : matches){
            cout << x.F << " <3 " << x.S << endl;
        }
    }
    else{
        cout << "no solution" << endl;
    }
}