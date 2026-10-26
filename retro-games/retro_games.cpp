#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>
#define MINES 15
#define ROWS 10
#define COLUMNS 10

using namespace std;

void printGrid(char v[ROWS][COLUMNS],bool fM) {
	char coordinates[10]{ 'A','B','C','D','E','F','G','H','I','J' };

	if (fM) {
		for (int j = 0; j < ROWS; j++) {
			for (int i = 0; i < COLUMNS; i++) {
				v[j][i] = '.';
			}
		}
	}

	cout << "     1  2  3  4  5  6  7  8  9  10" << endl;
	cout << "    ------------------------------" << endl;
	for (int j = 0;j < ROWS;j++) {
		cout << " " << coordinates[j] << " | ";
		for (int i = 0;i < COLUMNS;i++) {
			cout << v[j][i]<<"  ";
		}
		cout << endl;
	}
}

void addMines(int l[ROWS][COLUMNS],char c,int cy){
	int mines = 0;
	int cx;
	char coordinates[10]{'A','B','C','D','E','F','G','H','I','J' };
	for (int k = 0;k < ROWS;k++) {
		if (c == coordinates[k]) {
			cx = k;
			cx--;
			break;
		}
	}
	

	while (mines < MINES) {
		int randomRow = rand() % 10;
		int randomColumn = rand() % 10;

		if ((randomRow != cx || randomColumn != cy)&&(randomRow != -1 || randomColumn != -1)) {
			l[randomRow][randomColumn] = -1;

			mines++;
		}
	}
}

int main() {
	srand(time(0));
	bool gameOver = false;
	bool firstMove = true;
	int discoveredCells = 0;
	char cellC;
	int cellI;

	int logicGrid[ROWS][COLUMNS];
	char visualGrid[ROWS][COLUMNS];

	while (!gameOver) {
		printGrid(visualGrid,firstMove);
		if (firstMove) {
			cout << "select the first cell(letter)" << endl;
			cin >> cellC;
			cout << "select the first cell(number)" << endl;
			cin >> cellC;
			addMines(logicGrid, cellC,cellI);
			firstMove = false;


		}
		return 0;
	}







	return 0;
}