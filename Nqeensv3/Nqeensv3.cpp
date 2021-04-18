#include "pch.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <vector>
#include <list>
#include<time.h> //srand(czas) 


int statesGenerated = 0;
int statesChecked = 0;
int bruteSwitch=0;


using namespace std;

class NQeens
{
public:
	short int* ChessBoard;//implementacja ulepszona
	//NQeens* pointer;//do przchodzenia na pzoiomie drzewa potrzebne?
	//NQeens** children;
	short int size;
	bool status;
	bool solution;

	NQeens() {}//konstuktor domyślny

	void display()
	{
		cout << "tree state:" << "\n";
		for (int i = 0; i < this->size; i++)
		{
			cout << this->ChessBoard[i] << " ; " << "\n";
		}
	}

	NQeens(int boardSize)//konstruktor 
	{
		size = boardSize;
		ChessBoard = new short int[boardSize];
		//children = new NQeens*[boardSize];		
		for (int i = 0; i < boardSize; i++)//wypelnic nullami na poczatku
			ChessBoard[i] = 0;
		status = true;

		solution = false;

		//pointer = NULL;
		statesGenerated++;
	}

	NQeens* newBoard(NQeens* &Qparent)//generuje nowa szachonice dla potomka
	{
		NQeens* Q1 = new NQeens();

		Q1->ChessBoard = new short int[Qparent->size];
		//Q1->children = new NQeens*[Qparent->size];
		Q1->size = size;
		Q1->status = true;

		for (int i = 0; i < Qparent->size; i++)//wypelnic nullami na poczatku
			Q1->ChessBoard[i] = Qparent->ChessBoard[i];

		//cout << ChessBoard[1] << "\n";
		//pointer = NULL;
		statesGenerated++;
		return Q1;
	}

	bool checkToInsert(NQeens* Q1,int column)
	{
		int check=column;
		int row;

		for (int j = 0; j < Q1->size; j++)//wstaw w pierwsze wolne miejsce
		{
			if (Q1->ChessBoard[j] != 0)
			{
				row = j+1;
			}

		}

		for (int k = 0; k < Q1->size; k++)//wstaw w pierwsze wolne miejsce
		{
			if (check + 1 == Q1->ChessBoard[k])//+1 bo od 0
			{
				return false;
			}
			
			if (abs(Q1->ChessBoard[k] - check) == abs(k - row))//warunek na ukos -1 bo 1 to wstawiony w 1 rzedzie
			{
				return false;
			}

		}

		return true;

		
	}

	void insertQeen(int row, int column)//wsatwianie proste
	{
		//if (ChessBoard[row] == 0)
		//{
		ChessBoard[row] = column + 1;
		//}
	}

	void insertQeenV2(int column, NQeens* &Q1)//wsatwianie proste
	{

		for (int k = 0; k < Q1->size; k++)//wstaw w pierwsze wolne miejsce
		{
			if (Q1->ChessBoard[k]==0)
			{
				Q1->ChessBoard[k] = column + 1;
				break;
			}
		}
	
	}

	void status_check(NQeens* &Q1)
	{

		for (int i = 0; i < Q1->size; i++)//petla do przechodzenia na poziomie dzrzewa
		{
			int check = 0;

			int row;

			//cout << "\n";

			int t1 = Q1->ChessBoard[i];

			if (t1 != check)//jesli jest wstawiony chetman
			{
				check = Q1->ChessBoard[i];//zapisz kolumne hetmana
				row = i;//zapisz rzad hetmana

				for (int k = i + 1; k < Q1->size; k++)//sprawdz wszystkie mozliwosci w szachonicy k=j+1 bo od poziomu w dol
				{
					if (Q1->ChessBoard[k] == check)
					{
						Q1->status = false;
					}
				}

				for (int k = i + 1; k < Q1->size; k++)//sprawdz wszystkie mozliwosci w szachonicy k=j+1 bo od poziomu w dol
				{
					int column = Q1->ChessBoard[k];//zapisz wiersz chetmana

					//cout << "teset co ejsst:" << tmp->ChessBoard[k] << "\n";

					if (column != 0)
					{
						if (abs(column - check) == abs(k - row))//warunek na ukos -1 bo 1 to wstawiony w 1 rzedzie
						{
							Q1->status = false;
						}
					}
				}

				check = 0;
			}
		}
		//Q1->status=true;
	}

	void SolutionCheck(NQeens* &Q1)
	{
		int i = 0;

		for (int k = 0; k < Q1->size; k++)//sprawdz czy szchownca jest maksymalnie wypelniona
		{
			if (Q1->ChessBoard[k] == 0)
			{
				i = 1;
			}
		}

		if (i == 0 && Q1->status == true)//sprawdz czy szachownica nie ma bic i jest maksymalnie wypalniona
		{
			Q1->solution = true;//znalezione rozwiazanie
		}

		statesChecked++;
	}

	void BFS(NQeens* &Q1)
	{

		list <NQeens*> line;

		int row = 0;
		line.push_back(Q1);
		int rowCheck = Q1->size;
		int base = Q1->size;

		NQeens* Q2 = new NQeens();//do podejscia z lepsza funckja generujaca potomka bardzo male optymalne rozwiazanie
		Q2->ChessBoard = new short int[Q1->size];
		Q2->size = Q1->size;

		NQeens *tmp = new NQeens();
		int j = 0;
		bool stop = false;//warunek stopu bfs
		int jump = 0;//do skakania gdy nie generujemy potomkow

		while (stop != true)
		{
			tmp = line.front();//ponierz 1 z kolejki

			//cout << "z kolejki:" << "\n";
			//tmp->display();


			for (int k = 0; k < tmp->size; k++)//tworzenie dzieci dla wezla
			{
						
				if (bruteSwitch == 0)
				{
					NQeens* QCHILD = newBoard(tmp);

					QCHILD->insertQeenV2(k, QCHILD);//QCHILD->insertQeen(row,k);
					QCHILD->status_check(QCHILD);

					//if (QCHILD->status == true)//jesli nie ma bicia dodaj do kolejki
					//{
						QCHILD->SolutionCheck(QCHILD);

						if (QCHILD->solution == true)
						{
							stop = true;
							tmp = QCHILD;
						}

						
					//}

					line.push_back(QCHILD);//dodaj na koniec kolejki


				}
				else
				{
					
					for (int i = 0; i < tmp->size; i++)
						Q2->ChessBoard[i] = tmp->ChessBoard[i];

					Q2->status = true;
					Q2->insertQeenV2(k,Q2);
					Q2->status_check(Q2);

					if (Q2->status == true)//jesli nie ma bicia dodaj do kolejki
					{
						NQeens* QCHILD = newBoard(tmp);

						QCHILD->insertQeenV2(k, QCHILD);//QCHILD->insertQeen(row,k);
						QCHILD->status_check(QCHILD);

						//if (QCHILD->status == true)//jesli nie ma bicia dodaj do kolejki
						//{
							QCHILD->SolutionCheck(QCHILD);

							if (QCHILD->solution == true)
							{
								stop = true;
								tmp = QCHILD;
							}
							//QCHILD->display();
							line.push_back(QCHILD);//dodaj na koniec kolejki
						//}
					}
					

				}
				
				//tmp->children[k] = QCHILD;
							
			}

			line.pop_front();

		}

		cout << "rozwiazanie:" << "\n";
		cout << "ilsoc wygenerowanych stanow:" << statesGenerated<<"\n";
		cout << "ilsoc sprawdzonych  stanow:" << statesChecked << "\n";
		tmp->display();

	}

	void DFS(NQeens* &Q1)
	{

		list <NQeens*> line;

		NQeens* Q2 = new NQeens();//do podejscia z lepsza funckja generujaca potomka bardzo male optymalne rozwiazanie
		Q2->ChessBoard = new short int[Q1->size];
		Q2->size = Q1->size;


		int row = 0;
		line.push_back(Q1);
		int rowCheck = Q1->size;
		int base = Q1->size;


		NQeens *tmp=Q1;


		//for (int i = 0; i < Q1->size; i++)//wypelnic nullami na poczatku
			// tmp->ChessBoard[i]= Q1->ChessBoard[i];


		//int j = 0;
		bool stop = false;//warunek stopu bfs
		//int jump = 0;//do skakania gdy nie generujemy potomkow


		for (int k = 0; k < Q1->size; k++)//tworzenie dzieci 1 krok taki sam ja bfs
		{
			NQeens* QCHILD = newBoard(tmp);
			QCHILD->insertQeen(row, k);

			line.push_back(QCHILD);
			QCHILD->display();

		}

		while (stop != true)
		{
			
			tmp = line.front();
			line.pop_front();
			//cout << "z kolejki:" << "\n";
			//tmp->display();

			for (int k = 0; k < tmp->size; k++)//tworzenie dzieci dla wezla
			{
				//ponierz 1 z kolejki

				if (bruteSwitch == 0)
				{
					NQeens* QCHILD = newBoard(tmp);//kpowiowanie danych z tmp do nowego obiektu
					QCHILD->insertQeenV2(k, QCHILD);

					//QCHILD->display();

					QCHILD->status_check(QCHILD);

					if (QCHILD->status == true)//jesli nie ma bicia dodaj do kolejki
					{
						QCHILD->SolutionCheck(QCHILD);

						if (QCHILD->solution == true)
						{
							stop = true;
							tmp = QCHILD;
						}

						line.push_front(QCHILD);
					}
				}
				else
				{
					for (int i = 0; i < tmp->size; i++)
						Q2->ChessBoard[i] = tmp->ChessBoard[i];

					Q2->status = true;
					Q2->insertQeenV2(k, Q2);
					Q2->status_check(Q2);
					if (Q2->status == true)//jesli nie ma bicia dodaj do kolejki
					{
						NQeens* QCHILD = newBoard(tmp);//kpowiowanie danych z tmp do nowego obiektu
						QCHILD->insertQeenV2(k, QCHILD);

						//QCHILD->display();

						QCHILD->status_check(QCHILD);

						if (QCHILD->status == true)//jesli nie ma bicia dodaj do kolejki
						{
							QCHILD->SolutionCheck(QCHILD);

							if (QCHILD->solution == true)
							{
								stop = true;
								tmp = QCHILD;
							}

							line.push_front(QCHILD);
						}

					}

				}
				
			}
		}

		cout << "rozwiazanie:" << "\n";
		cout << "ilsoc wygenerowanych stanow:" << statesGenerated << "\n";
		cout << "ilsoc sprawdzonych  stanow:" << statesChecked << "\n";
		tmp->display();

	}
};

int main()
{
	bruteSwitch = 0;
	NQeens Q1(4);
	NQeens *tmp = &Q1;

	srand(time(0));//do miezenia czasu
	time_t t;/**/
	time(&t);//konieczne dla dzialania rand
	srand(t);/**/
	clock_t begin, end;//do miezenia czasu
	double time_spent;//do miezenia czasu
	begin = clock();//do miezenia czasu
	
	
	tmp->BFS(tmp);
	//tmp->DFS(tmp);

	end = clock();//do miezenia czasu
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;//do miezenia czasu
	cout << "czas wykonania:" << time_spent << "\n";//do miezenia czasu



	//tmp->ChessBoard[0] = 1;
	//tmp->ChessBoard[1] = 2;
	//tmp->ChessBoard[2] = 3;
	//tmp->ChessBoard[3] = 4;

	//NQeens* w=Q1.newBoard(tmp);
	//w->insertQeen(1,2);
	//w->display();
	//cout << "test1 " << w->ChessBoard[0] << "\n";


}
