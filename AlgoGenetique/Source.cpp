#include <iostream>
#include <ctime>
#include <string>
#include <Windows.h>
#include <cmath>
#include <list>
#include <sstream>
#include <fstream>

using namespace std;

#define NB_POP 500 //Population
#define NB_IT 10000 //Nombre de générations dans l'évolution
#define NB_CH 100 //Nombre de chromosomes dans un individu
#define SELECT_POWER 2
#define NB_CH_MIN 0
#define FILE

void init_pop(list<string> *pop);	//Initialisation de la population
void aff_pop(list<string> *pop);	//Affichage de la population actuelle
void write_pop(list<string> *pop, ofstream *flx);
int nb_int_rec(string str, int arg);//Nombre de récurence d'un chromosome donné dans un individu
void init_proba(int prob[NB_POP], list<string> *pop, int const s); //Initialisation du tableau de probabilité
int select_n(int r, int prob[NB_POP]); //Selectionne un individu au hasard en fonction de son nombre de chromosomes forts

int main(){
	srand(time(NULL));	//Graine
	string const file("out.txt");
	ofstream flx_out(file.c_str());
	int const succ = rand() % 10; //Selection du chromosome fort
	//string population[NB_POP];	//Population totale
	list<string> population;
	init_pop(&population);	//Initialisation de la population initiale
#ifdef FILE
	flx_out << "ETAPE 0 : " << endl;
	write_pop(&population, &flx_out);
#elif
	cout << "ETAPE 0 : " << endl;
	aff_pop(&population); //Affichage de la population initiale
#endif

	
	for (int i = 0; i < NB_IT; i++){
		//Sleep(500);	//Délai entre les générations
		list<string> new_pop;	//Population enfants
		for (int j = 0; j < rand() % (population.size() - 1) + (population.size() / 2) + 2; j++){
			int p[NB_POP];	//Tableau de probas
			init_proba(p, &population, succ);	//Initialisation des probas
			int r = rand() % p[population.size() - 1] + 1;
			int id = select_n(r, p);	//Selection du père N
			list<string>::iterator it = population.begin();
			for (int g = 0; g < id; g++){
				it++;
			}
			string n = *it;
			int id2;
			do{
				r = rand() % p[population.size() - 1];
				id2 = select_n(r, p);	//Selection de la mère N'
			} while (id2 == id);
			it = population.begin();
			for (int g = 0; g < id2; g++){
				it++;
			}
			string n_p = *it;
			r = rand() % (NB_CH - 2) + 1;
			string e = "";	//Création du fils N*
			for (int t = 0; t < r; t++){
				e.push_back(n[t]);	//Le fils recois n chromosomes du père
			}
			for (int t = r; t < NB_CH; t++){
				e.push_back(n_p[t]); //Le fils recois n chromosomes de la mère
			}
			for (int q = 0; q < rand() % ((NB_CH / 10) + 1); q++){
				r = rand() % NB_CH;
				char c = e.at(r);
				if (e.at(r) != succ + '0'){
					e.at(r) = rand() % 10 + '0';	//Mutation d'un chromosome du fils au hasard
					if (c == e.at(r)){
						e.at(r) = succ + '0';
					}
				}
			}

			if (nb_int_rec(e, succ) > NB_CH_MIN){
				new_pop.push_back(e); //Le fils rejoins la nouvelle population
			}
		}
		if (new_pop.size() < 2){
			new_pop.push_back(to_string(rand() % (int)(pow(10, NB_CH))));
			for (list<string>::iterator it = new_pop.begin(); it != new_pop.end(); it++){
				while (it->size() < NB_CH)
				{
					it->insert(0, "0");
				}
			}
		}
		//list<string>::iterator itP = population.begin();
		population.clear();
		list<string>::iterator itE = new_pop.begin();
		for (int k = 0; k < new_pop.size(); k++){
			if (itE != new_pop.end()){
				population.push_back(*itE); //La population de parents est remplacée par la population d'enfants
				itE++;
				//itP++;
			}

		}
#ifdef FILE
		flx_out << "ETAPE " << i + 1 << " : " << endl;
		flx_out << "ESTIMATION : " << (i + 1) * 20 << " ANS" << endl;
		write_pop(&population, &flx_out);
#elif
		cout << "ETAPE " << i + 1 << " : " << endl;
		cout << "ESTIMATION : " << (i + 1) * 20 << " ANS" << endl;
		aff_pop(&population);	//Affichage de la population enfant
#endif
		
		int s = 0;
		for (list<string>::iterator it = population.begin(); it != population.end(); it++){
			s += nb_int_rec(*it, succ);
		}
		if (s >= NB_CH*population.size() || population.size() < 2){	//Condition d'arret
			i = NB_IT;
		}
	}
	cout << succ << endl;	//Affichage du chromosome fort
	system("PAUSE");
	return 0;	//FIN
}

void init_pop(list<string> *pop){
	for (int i = 0; i < rand() % (NB_POP - 2) + (NB_POP / 2); i++){
		pop->push_back(to_string(rand() % (int)(pow(10, NB_CH))));
	}
	for (list<string>::iterator it = pop->begin(); it != pop->end(); it++){
		while (it->size() < NB_CH)
		{
			stringstream ss;
			string s;
			char c = rand() % 10 + '0';
			ss << c;
			ss >> s;
			it->insert(0, s);
		}
	}
}

void aff_pop(list<string> *pop){
	int i = 0;
	for (list<string>::iterator it = pop->begin(); it != pop->end(); it++){
		cout << i + 1 << ":" << *it << endl;
		i++;
	}
	cout << "   |   " << endl;
	cout << "  \\|/  " << endl;
}

void write_pop(list<string> *pop, ofstream *flx){
	int i = 0;
	for (list<string>::iterator it = pop->begin(); it != pop->end(); it++){
		*flx << i + 1 << ":" << *it << endl;
		i++;
	}
	*flx << "   |   " << endl;
	*flx << "  \\|/  " << endl;
}

int nb_int_rec(string str, int arg){
	int cpt = 0;
	for (int i = 0; i < str.size(); i++){
		if (str.at(i) - '0' == arg){
			cpt++;
		}
	}
	return cpt;
}

void init_proba(int prob[NB_POP], list<string> *pop, int const s){
	list<string>::iterator it = pop->begin();
	for (int i = 0; i < NB_POP; i++){
		if (it != pop->end()){
			prob[i] = ((100 * nb_int_rec(*it, s)*SELECT_POWER) / NB_CH) + 1;
			if (i != 0){
				prob[i] += prob[i - 1];
			}
			it++;
		}
		else{
			break;
		}
	}
}

int select_n(int r, int prob[NB_POP]){
	for (int i = 0; i < NB_POP; i++){
		if (r <= prob[i]){
			return i;
		}
	}
	return 0;
}