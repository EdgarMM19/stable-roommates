# stable-roommates
Projecte Algorismia-FIB Primavera 2021

Per a fer un experiment fins a mida *n*, *i* iteracions per mida i que les dades es guardin al arxiu *b.txt* s'ha d'utilitzar la comanda:

```  g++ -std=c++14 experiment.cc randomGenerators.cc SRsolver.cc && ./a.out n i 0 > b.txt```  

Si es vol que el càlcul de iteracions es faci per a que amb probabilitat 95% es quedi amb un error menor a *x/1000* on *x* és enter: 

```  g++ -std=c++14 experiment.cc randomGenerators.cc SRsolver.cc && ./a.out n 0 x > b.txt```  

Si es vol comprovar que l'algorisme de Irving funciona correctament fins a mida *n*, *i* iteracions per mida:

```  g++ -std=c++14 correctness.cc SRsolver.cc SRbt.cc randomGenerators.cc && ./a.out n i```  

