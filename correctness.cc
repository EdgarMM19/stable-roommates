#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>

using namespace std;

/*Una instancia aleatoria es una vector de vectores con 
las preferencias (una permutacion random) de cada persona.*/

#define DEBUG(CTR) cerr << #CTR << ": "; for (auto x : CTR) cerr << x << ", "; cerr << endl;

using vi = vector<int>;
using vvi = vector<vi>;
using ii = pair<int,int>;
using vii = vector<ii>;
using vb = vector<bool>;
using vvb = vector<vb>;

int randomNumber(int n);
vector<int> generateRandomPermutation(int len, bool startAtOne);
bool solveSR(int n, const vvi& preferencesOrder, vii& solution);
bool backSR(int n, const vvi& preferencesOrder);

void correct(int n_hab, int n_iter){
    //Experimentem per conjunts de 2 a num_persones persones
    for (int n = 1; n <= n_hab; ++n){
        //Fem num_iteracions experiments per conjunt
        int niter = n_iter;
        for (int i = 0; i < niter; ++i){ 
            vvi instancia;
            // Generem la instancia per un experiment en concret
            for (int j = 0; j < 2*n; j++){
                vi persona = generateRandomPermutation(2*n-1,false);
                for (int& x : persona){
                    if (x >= j) x++;
                    assert(x >= 0 and x < 2*n and x != j);
                }
                instancia.push_back(persona);
            }
            vii possible_sol;
            if (solveSR(2*n, instancia, possible_sol) != backSR(2*n, instancia)) {
                cout << "Problem with n = " << n << endl;
            } 
        }
        cout << n << " finish!" << endl;
    }
}

int main(int argc, char** argv){
    srand(0);
    int n_hab = stoi(argv[1]);
    int n_iter = stoi(argv[2]);
    correct(n_hab,n_iter);

}
