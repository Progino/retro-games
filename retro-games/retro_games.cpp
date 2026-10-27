#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <cctype>
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

	for (int j = 0; j < ROWS; j++) {
		for (int i = 0; i < COLUMNS; i++) {
			l[j][i] = 0;
		}
	}

	int mines = 0;
	int cx;
	char coordinates[10]{'A','B','C','D','E','F','G','H','I','J' };
	for (int k = 0;k < ROWS;k++) {
		if (c == coordinates[k]) {
			cx = k;
			cy--;
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

	for (int j = 0; j < ROWS; j++) {
		for (int i = 0; i < COLUMNS; i++) {
			int number = 0;
			if (l[j][i] == -1) {
				continue;
			}
			if (i>0 && l[j][i - 1] == -1) {
				number++;
			}
			if (i > 0 && j>0 && l[j-1][i-1] == -1) {
				number++;
			}
			if (j > 0 && l[j-1][i] == -1) {
				number++;
			}
			if (i<9 && j>0 && l[j-1][i + 1] == -1) {
				number++;
			}
			if (i < 9 && l[j][i + 1] == -1) {
				number++;
			}
			if (i < 9 && j<9 && l[j+1][i + 1] == -1) {
				number++;
			}
			if (j < 9 && l[j+1][i] == -1) {
				number++;
			}
			if (i > 0 && j < 9 && l[j+1][i - 1] == -1) {
				number++;
			}
			l[j][i] = number;
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
			cellC = toupper(cellC);
			if (cellC != 'A' && cellC != 'B' && cellC != 'C' && cellC != 'D' && cellC != 'E' && cellC != 'F' && cellC != 'G' && cellC != 'H' && cellC != 'I' && cellC != 'J') {
				cout << "Insert a valid coordinate" << endl;
				continue;
			}
			cout << "select the first cell(number)" << endl;
			cin >> cellI;
			if (cellI > 10) {
				cout << "Insert a valid coordinate" << endl;
				continue;
			}
			addMines(logicGrid, cellC,cellI);
			firstMove = false;
		}
		return 0;
	}
}