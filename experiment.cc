#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include "randomGenerators.cc"
#include "SRsolver.cc"
using namespace std;

/*Una instancia aleatoria es una vector de vectores con 
las preferencias (una permutacion random) de cada persona.*/

using vi = vector<int>;
using vvi = vector<vi>;
using ii = pair<int,int>;
using vii = vector<ii>;
using vb = vector<bool>;
using vvb = vector<vb>;

#define num_persones 50
#define num_iteracions 20

int randomNumber(int n);
vector<int> generateRandomPermutation(int len, bool startAtOne);
vii solveSR(int n, const vvi& preferencesOrder, bool& hasSolution);



double Pn (int n){
    //Pn ~ e*(1/sqrt(pi))*(2/n)^1/4
    return exp(1) * (double)1/sqrt(M_PI) * pow(2/n,1/4);
}

map<int,vector<double>> experiment(){
    //Experimentem per conjunts de 2 a 100 persones
    map<int,vector<double>> resultats; 
    for (int n = 1; n < num_persones; ++n){
        double amb_solucio = 0;
        //Probabilitat estimada de tenir solució
        double prob_n = Pn(n);

        //Fem 20 experiments per conjunt
        for (int i = 0; i < num_iteracions; ++i){ 
            vvi instancia;
            // Generem la instancia per un experiment en concret
            for (int j = 0; j < 2*n; j++){
                vi persona = generateRandomPermutation(2*n,true);
                instancia.push_back(persona);
            }
            bool has_solution;
            vii possible_sol = solveSR(2*n,instancia,has_solution);
            if (has_solution) amb_solucio++;
        }
        //Afegim els resultats a l'estructura de dades
        double prob_real = amb_solucio/num_iteracions;
        double error = abs(prob_real-prob_n);
        vector<double> res = {prob_n,prob_real,error};
        resultats.insert({n,res});
    }
    return resultats;
}

/*int main(){
    map<int,vector<double>> resultats = experiment();
}*/

