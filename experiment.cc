#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>
//#include "randomGenerators.cc"
//#include "SRsolver.cc"
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

#define num_persones 300
#define num_iteracions 2000

int randomNumber(int n);
vector<int> generateRandomPermutation(int len, bool startAtOne);
bool solveSR(int n, const vvi& preferencesOrder, vii& solution);

double Pn (int n){
    //Pn ~ e*(1/sqrt(pi))*(2/n)^1/4
    return exp(1.0) * (double)1.0/sqrt(1.0*M_PI) * pow(2.0/n,1.0/4.0);
}

map<int,vector<double>> experiment(){
    //Experimentem per conjunts de 2 a 100 persones
    map<int,vector<double>> resultats; 
    for (int n = 1; n <= num_persones; ++n){
        double amb_solucio = 0;
        //Probabilitat estimada de tenir solució
        double prob_n = Pn(n);
        //Fem 20 experiments per conjunt
        for (int i = 0; i < num_iteracions; ++i){ 
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
            if (solveSR(2*n, instancia, possible_sol)) amb_solucio++;
            //cout << "Ins results: " << has_solution;
            //char bas; cin >> bas;
        }
        //Afegim els resultats a l'estructura de dades
        double prob_real = amb_solucio/num_iteracions;
        double error = abs(prob_real-prob_n);
        vector<double> res = {prob_n,prob_real,error};
        resultats.insert({n,res});
        cout << n << " " << prob_n << " " << prob_real << " " << error << endl;
    }
    return resultats;
}

int main(){
    srand(0);

    map<int,vector<double>> resultats = experiment();
    for(auto x : resultats){
        cout << x.first << " " << x.second[0] << " " << x.second[1] << " " << x.second[2] << endl;
    }
}
