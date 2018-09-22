//#include <bits/stdc++.h>

//FALTA:

//MODULARIZAR
//LIBERAR MEMORIA (?)
//MELHORAR SIMULATED ANNEALING (?)

#include "/Users/brunaparacat1/stdc++.h"

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>
#include "ListaAdjacencia.h"
#define min(a,b) a<b?a:b

using namespace std;


typedef  vector < vector < pair <int,int> > > matrizCidades;
typedef  vector <int> listaSolucao;

int qt_cidades = 0;
matrizCidades matriz_lida;


void imprime(listaSolucao solucao){
    for(int i=0; i< qt_cidades; i++)
    {
        cout<< solucao[i] << " --> ";
    }
    cout<<endl<<endl;
}

//retorna tempo de latencia total de uma solucao
double eval( listaSolucao solucao)
{
    double latenciaTotal = 0;
    double caminho = 0;
    for(int i=0; i<qt_cidades; i++){
            if(solucao[i+1] == -1)
                break;
            
            caminho+= matriz_lida[solucao[i]][solucao[i+1]].second;
            latenciaTotal +=caminho;
        
    }
    return latenciaTotal;
}

//retorna vetor solucao contendo a ordem das visitas `as cidades
listaSolucao busca_gulosa(matrizCidades listaAdjacencia,listaSolucao sol_Inicial,  int cidade_de_origem )
{
    vector <int> cidades_visitadas;
	cidades_visitadas.resize(qt_cidades);
    
	int aux_cidade_de_origem = cidade_de_origem;
	int end = 0;
	cidades_visitadas[cidade_de_origem] = 1;

	while(!end){
		for (int i = 0; i < qt_cidades; i++){
			if (!cidades_visitadas[listaAdjacencia[aux_cidade_de_origem][i].first] && listaAdjacencia[aux_cidade_de_origem][i].second ){
			  
                //insere cidade atual e custo para a proxima, em ordem
                sol_Inicial.push_back(aux_cidade_de_origem);
				cidades_visitadas[listaAdjacencia[aux_cidade_de_origem][i].first] = 1;
				aux_cidade_de_origem = listaAdjacencia[aux_cidade_de_origem][i].first;
				break;
			}
		}
		end = 1;
		for (int i = 0; i < qt_cidades; i++){
			if (!cidades_visitadas[i]) {
				end = 0;
				break;
			}
        }
	}
    
    //insere ultima cidade na solucao
    sol_Inicial.push_back(aux_cidade_de_origem);
    sol_Inicial.push_back(-1);

	return sol_Inicial;
}


listaSolucao caminho_otimo(matrizCidades listaAdjacencia, listaSolucao sol_Inicial){
	int menor = INT_MAX;
    int origem =0;
    
    sol_Inicial = busca_gulosa(listaAdjacencia, sol_Inicial, 0);
    return sol_Inicial;
}

listaSolucao SWAP(listaSolucao solucao, int A, int B )
{
    listaSolucao nova = solucao;
    int temp = nova[A];
    nova[A] = nova[B];
    nova[B] = temp;

    return nova;
}

listaSolucao gera_nova_solucao(listaSolucao sol_Inicial){
    int A, B;
    listaSolucao nova = sol_Inicial;
    
        A = rand() % (sol_Inicial.size()-1);
        B = A;
        while(B == A)
            B=rand() % (sol_Inicial.size()-1);
        
        nova = SWAP(nova, A,B );
    
    return nova;
}


//a partir de solucao inicial s
//selecionamos aleatoriamente um estado t a partir dos vizinhos de s
//se eval(t) < eval (s)   s = t
//se nao , o estado t deve passar em um teste de problabilidade para ver se vai ser aceito
vector <int> SimulatedAnnealing(listaSolucao solucao)
{
    cout<< "Latencia antes simulated annealing:" << eval(solucao)<<endl;
    
    listaSolucao nova;
    listaSolucao melhor = solucao;
    double delta;
    double latencia_Ini, latencia_nova ;
    double temp =10000.0;
    double cooling_rate = 1;
    int aceitou =0;
    while( temp > 1)
    {
        for(int k =0; k < 30; k++){
            nova=gera_nova_solucao(solucao);
            
            latencia_Ini= eval(solucao);
            latencia_nova = eval(nova);
            delta = latencia_nova - latencia_Ini ;
            
            //printf( "delta:%lf  exp:%lf \n random: %d" ,delta, exp((latencia_Ini-latencia_nova)/temp), random);
            
            if(delta <=0 )
                solucao = nova;
            else
            {
                if(exp(-delta/temp) > rand() % 100)
                {
                    solucao=nova; 
                    aceitou++;

                }
            }
            
            if( eval(solucao) <= eval(melhor))
                melhor = solucao;
        }
        temp -= cooling_rate;
    }
    cout<<" aceitou "<< aceitou<< endl;
    cout<<" latencia final: "<<eval(melhor)<<endl;
    return melhor;
}


int main(int argc, char *argv[]){
	
    clock_t tStart;

    matrizCidades matriz_ordenada;
    listaSolucao solucao;
    
    
    //leitura do arquivo
	matriz_lida = le_arquivo(argv[1], &qt_cidades);
    matriz_ordenada = matriz_lida;
    
    //inicio contagem de tempo de execucao
    tStart= clock();
    
    //transforma matriz triangularizada em matriz completa
    matriz_lida= copia_superior(matriz_lida);
    matriz_ordenada = copia_superior(matriz_ordenada);
    
    // Ordenacao de listaAdjacencia
    for (int i = 0; i < matriz_ordenada.size(); i++){
		sort(matriz_ordenada[i].begin(), matriz_ordenada[i].end(), sortbysec);
	}
    
    //imprime_listaAdjacencia(matriz_lida);

    //Constroi Solucao inicial utilizando algoritmo guloso e matriz ordenada
    solucao = caminho_otimo(matriz_ordenada,solucao);
    
    cout<<" solucao inicial: ";
    imprime(solucao);
    
    //A partir de solucao inicial e lista de Adjacencias Utiliza Algoritmo de Simulated Anealing para encontrar nova solucao
    solucao = SimulatedAnnealing(solucao);
   // imprime(solucao);
    
    printf("\nTempo de execucao: %.2f  ms \n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	return 0;
}
