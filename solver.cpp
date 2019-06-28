//#include <objidl.h>
#include "solver.h"
#include <bits/stdc++.h>

/*
 * Retorna a distancia entre dois pontos
 */

double getDistance(Point p1, Point p2){
	double delta_x = p1.x - p2.x;
	double delta_y = p1.y - p2.y;

	return sqrt(delta_x * delta_x + delta_y * delta_y); 
}

/*
 * Retorna uma matriz de distancia entre todos os pontos a partir de uma instancia
 */

vector<vector<double>> getDistances (Instance &instance) {
    vector<vector<double>> dist;

    // Aloca
    for (int i = 0; i<instance.n; i++){
        vector<double> aux;
        for (int j = 0; j<instance.n; j++){
            aux.push_back(0);
        }
        dist.push_back(aux);
    }

    // Calcula
    for (int i = 0; i< instance.n; i++) { //diagonal
        dist.at(i).at(i) = 0;
    }
    for (int i = 0; i < instance.n; i++) { // Pra cima da diagonal
        for (int j = i + 1; j < instance.n; j++) {
            Point p1 = instance.points.at(i);
            Point p2 = instance.points.at(j);

            dist.at(i).at(j) = getDistance(p1, p2);
        }
    }
    for (int i = 1; i < instance.n; i++) { // Pra baixo da diagonal
        for (int j = 0; j < i; j++) {
            dist.at(i).at(j) = dist.at(j).at(i);
        }
    }

    return dist;
}

/*
 * SOLVE BOTTOM UP
 */

vector<int> solveBottomUp(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started){
	vector<int> sol;
    vector<vector<double>> dmin;
    vector<vector<int>> anterior;

    // calcula as distancias
    vector<vector<double>> dist = getDistances(instance);

    // Aloca os vetores
    for (int i = 0; i < instance.n; i++) {
        sol.push_back(-1);
    }
    for (int i = 0; i<(1<<(instance.n)); i++){
        vector<double> aux;
        for (int j = 0; j<instance.n; j++){
            aux.push_back(DBL_MAX);
        }
        dmin.push_back(aux);
    }
    for (int i = 0; i<(1<<(instance.n)); i++){
        vector<int> aux;
        for (int j = 0; j<instance.n; j++){
            aux.push_back(0);
        }
        anterior.push_back(aux);
    }

    // seta a tabela do menor caminho com os valores máximos e, nas primeiras linhas, seta a distancia de cada ponto para o início
    for (int c = 0; c < instance.n; c++) {
        dmin.at(0).at(c) = dist.at(0).at(c);
        anterior.at(0).at(c) = 0;
    }

    // Calcula o menor caminho
    for (int l = 1; l < (1<<(instance.n)); l++) { // para cada linha l, analisaremos um conjunto de nós
        for (int c = 0; c < instance.n; c++) { // para cada coluna c, analisaremos um ponto final
            for (int p = l, patual = 0; p > 0; p = (p >> 1), patual++) {
                if ((p % 2) == 1) { // o último item pertence ao conjunto
                    double q = dist.at(patual).at(c) + dmin.at(l & (INT_MAX - ((1 << patual)))).at(patual);
                    if (dmin.at(l).at(c) > q) {
                        dmin.at(l).at(c) = q;
                        anterior.at(l).at(c) = patual;
                    }
                }
            }
        }
    }

    // Acha o caminho do menor caminho
    int l = ((1<<(instance.n))-1), c = instance.n - 1;

    for (int i = instance.n-1; i > 0; i--) {
        sol.at(i) = anterior.at(l).at(c);
        c = anterior.at(l).at(c);
        l = ( l & (INT_MAX-((1<<c))) );
    }
    for (int i = 0; i<sol.size()-1; i++) {
        sol.at(i) = sol.at(i+1);
    }
    sol.pop_back(); // tira o ponto inicial
    sol.pop_back(); // tira o ponto final

	return sol;
}

/*
 * SOLVE TOP DOWN
 */

struct Caminho {
    vector<int> sol;
    double distancia;
};

Caminho solveTopDownAux(Instance &instance, Caminho mc, int quantos, int ultimo) {
    quantos++;
    Caminho mcAux;
    Caminho menor;
    menor.distancia = DBL_MAX;
    int k = 0;


    if (quantos == instance.n) {
        mc.distancia = mc.distancia + getDistance(instance.points.at(ultimo), instance.points.at(instance.n-1));
        mc.sol.at(ultimo) = instance.n-1;
        menor = mc;
    }
    else {
        for (int i = 1; i < instance.n - 2; i++) {
            mcAux.sol = mc.sol;
            if (mc.sol.at(i) == -1){
                mcAux.distancia = mc.distancia + getDistance(instance.points.at(ultimo),instance.points.at(i));
                mcAux.sol.at(i) = ultimo;
                mcAux = solveTopDownAux(instance, mcAux, quantos, i);
                if (menor.distancia > mcAux.distancia) {
                    cout << "anus";
                    menor = mcAux;
                }
            }
        }
    }
    cout << "cu " << menor.sol.size()<< endl;

    return menor;
}

vector<int> solveTopDown(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started){
    Caminho mc;
    vector<int> sol;

    //Inicializa o vetor de solução
    sol.push_back(0); // começo =0
    for (int i = 1; i < instance.n; i++) {
        sol.push_back(-1);
    }
    sol.push_back(instance.n); // fim =instance.n-1

    mc.sol = sol;
    mc.distancia = 0;

    cout << "VERIFICANDO " << mc.sol.size()<< endl;
    // Chama recursao
    mc = solveTopDownAux(instance, mc, 0, 0);

    cout << "VERIFICANDO " << mc.sol.size()<< endl;
    for (int i=0; i < mc.sol.size(); i++) {
        cout << "s[" << i << "] = " << mc.sol.at(i) << endl;
    }
    cout << endl;



    /*   cout << "dmin "<< endl << "    ";
       for (int j = 0; j < instance.n; j++) {
           cout << j << "  ";
       }
       for (int i=0; i < (1<<(instance.n)); i++) {
           cout << endl << i << " | ";
           for (int j = 0; j < instance.n; j++) {
               cout << dmin.at(i).at(j) << "  ";
           }
       }
       cout << endl << endl;*/

    return mc.sol;
}