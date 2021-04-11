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

double Pn (int n){
    //Pn ~ e*(1/sqrt(pi))*(2/n)^1/4
    return exp(1.0) * (double)1.0/sqrt(1.0*M_PI) * pow(2.0/n,1.0/4.0);
}

map<int,vector<double>> experiment(int n_hab, int n_iter, double maxError){
    //Experimentem per conjunts de 2 a num_persones persones
    map<int,vector<double>> resultats; 
    for (int n = 1; n <= n_hab; ++n){
        double amb_solucio = 0;
        //Probabilitat estimada de tenir solució
        double prob_n = Pn(n);
        //Fem num_iteracions experiments per conjunt
        int niter = n_iter;
        if(maxError != 0){
            niter = int(prob_n*(1-prob_n)*16./(maxError*maxError));
        }
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
            if (solveSR(2*n, instancia, possible_sol)) {
                amb_solucio++;
            } 
        }
        //Afegim els resultats a l'estructura de dades
        cerr << niter << endl;
        double prob_real = amb_solucio/double(niter);
        double error = abs(prob_real-prob_n);
        vector<double> res = {prob_n,prob_real,error};
        resultats.insert({n,res});
        cerr << n << ", " << prob_n << ", " << prob_real << ", " << error << endl;
        cout << n << ", " << prob_n << ", " << prob_real << ", " << error << endl;
    }
    return resultats;
}

int main(int argc, char** argv){
    srand(0);
    int n_hab = stoi(argv[1]);
    int n_iter = stoi(argv[2]);
    int rang1000 = stoi(argv[3]);
    map<int,vector<double>> resultats = experiment(n_hab,n_iter, rang1000/1000.0);
}
