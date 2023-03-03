#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

enum Cores {BRANCO, CINZA, PRETO};

class vertice{
	public:
    	vertice();
    	virtual ~vertice() {}
    	void setCor(Cores cor) {this->cor = cor;};
    	Cores getCor() const {return this->cor;};
    	void setAdj(int vAdj) {this->adj.push_back(vAdj);}; // recebe o indice do vertice que é adjacente
    	int getAdj(int j) const {return this->adj[j];};
    	int getNumAdj() const {return adj.size();};
    	void setnumPred(int numPi){this->numPred = numPi;};
    	int getnumPred() const {return numPred;};
    	void setPi(int p) {this->pi = p;};
    	int getPi() const {return pi;};
	protected:
    	Cores cor;
    	int numPred;
    	int pi;
    	vector<int> adj; // os vertices adjacentes a este
};

vertice::vertice(){
	this->cor = BRANCO;
	this->numPred = 0;
	this->pi = -1;
	this->adj = {};
}

class grafo{
	public:
    	grafo(int n);
    	virtual ~grafo() {};
    	void dfs();
    	void dfs_visit(vertice &vert, int j);
    	void setAdj(int ind, int vAdj) {this->u[ind].setAdj(vAdj);}; // recebe o índice do vetor de vertices e o indice do vertice que é adjacente
    	int getAdj(int i, int j) const {return this->u[i].getAdj(j);};
    	int getNumAdj(int i) const {return u[i].getNumAdj();};
    	void setnumPred(int i, int numPi){this->u[i].setnumPred(numPi);};
    	void setU(vector<vertice> &v) {this->u = v;}
    	vector<vertice> getU() const {return this->u;}
    	void setSeq(list<int> &s) {this->seq = s;}
    	list<int> getSeq() const {return this->seq;}
    	void setnumV(int n) {this->numV = n;};
    	int getnumV() const {return this->numV;};
    	int topological_sort();
	private:
    	vector<vertice> u;
    	list<int> seq;
    	int numV;
};

grafo::grafo(int n){
    u.resize(n);
    seq = {};
    setnumV(n);
}

void grafo::dfs(){
	for (int i = 0 ; i < getnumV(); i++){
    	u[i].setCor(BRANCO);
	}
	for (int i = 0 ; i < getnumV(); i++){
    	if (u[i].getCor() == BRANCO && u[i].getnumPred() == 0){ // só posso visitar se for branco e não possuir nenhuma dependência de outros vertices
            dfs_visit(u[i], i+1);
    	}
	}
}

void grafo::dfs_visit(vertice &vert, int j){ // j é o índice correspondente ao vértice
    vert.setCor(CINZA);
    for (int i = 0; i < vert.getNumAdj(); i++){ // percorre os vértices adjacentes
        if (u[vert.getAdj(i)-1].getCor() == BRANCO){
            u[vert.getAdj(i)-1].setPi(j); // recebe como predecessor o indice de vert
            dfs_visit(u[vert.getAdj(i)-1], vert.getAdj(i)); // executa a recursão para aprofundar
        }
    }
    vert.setCor(PRETO); // marco que finalizei de visitar o vertice
    seq.push_front(j); // coloco na lista que representa a sequência correta da ordenação topológica
}

int grafo::topological_sort(){
    dfs();
    int cont = 1; // para contar o tempo em cada sequencia de dependencias
    int j;
    int tempo = 1; // guardará o maior tempo registrado
    vector<int> aux(seq.begin(), seq.end()); // crio um vetor com os dados da lista
    int i = getnumV()-1; // vou começar pelo final da lista
    while (i >= 0 && tempo <= i){ // percorro o vetor até chegar a primeira posição e o tempo calculado ate o momento for menor que o numero de vertices que ainda tenho para analisar
        if (u[aux[i]-1].getPi() != -1){ // calculando as dependências
            cont++;
            j = u[aux[i]-1].getPi();
            while (u[j-1].getPi() != -1){ // enquanto tiver predecessor incremento
                cont++;
                j = u[j-1].getPi();
            }
            tempo = max(tempo, cont); // coloco em tempo o maior valor entre o maior tempo que já foi obtido ou o novo tempo calculado em cont
            cont = 1; //  reinicio a variável auxiliar para começar a contar o tempo novamente
        }
        i--;
    }
    return tempo;
}

int main() {
    int n, m, a, b, nAd, t; // n representa os arquivos, a e b representa as dependências
	while (cin >> n){ //
        grafo g(n);
        m = 0;
        for(int j = 0; j < n; j++){ // lendo a descrição dos arquivos
            cin >> a;
            for (int i = 0; i < a; i++){
                cin >> b;
                g.setAdj(b-1, j+1); // grafo direcional, posso ir apenas de b-1 para j+1 e não o contrário
                nAd = g.getNumAdj(j); // pega quantos vizinhos possui o vértice até o momento
            	for(int k = 0; k<nAd; k++){ // identifica se há ciclo
                    if(b == g.getAdj(j, k)){
                        m = 1;
                        break;
                    }
                }
            }
            g.setnumPred(j, a); // seto o número de dependências que o j possui
        }
        if (m == 1){ // caso houver ciclo não vou realizar a busca topologica
            cout << "-1" << endl;
        } else { // caso não haja ciclo a busca funcionará
            t = g.topological_sort();
            cout << t << endl; // mostra o tempo de execução
        }
	}
    return 0;
}
