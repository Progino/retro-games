#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <cctype>
#include <limits>
#include <chrono>
#define MINES 15
//#define EASYMINES 15
//#define MEDIUMMINES 20
//#define HARDMINES 30
#define ROWS 10
#define COLUMNS 10

using namespace std;
using namespace std::chrono;

string formatTimeStr(int totalSeconds) {
	int hours = totalSeconds / 3600;
	int minutes = (totalSeconds % 3600) / 60;
	int seconds = totalSeconds % 60;

	string hh = (hours < 10 ? "0" : "") + to_string(hours);
	string mm = (minutes < 10 ? "0" : "") + to_string(minutes);
	string ss = (seconds < 10 ? "0" : "") + to_string(seconds);

	return hh + ":" + mm + ":" + ss;
}

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

void addMines(int l[ROWS][COLUMNS],int cx,int cy){

	for (int j = 0; j < ROWS; j++) {
		for (int i = 0; i < COLUMNS; i++) {
			l[j][i] = 0;
		}
	}

	int mines = 0;

	

	while (mines < MINES) {
		int randomRow = rand() % 10;
		int randomColumn = rand() % 10;

		if ((randomRow != cx && randomRow !=(cx-1) && randomRow != (cx+1)) ||( randomColumn != cy && randomColumn !=(cy-1) && randomColumn != (cy +1)) && l[randomRow][randomColumn] != -1) {
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

bool addFlag(char v[ROWS][COLUMNS], int cx, int cy) {
	bool occupied = false;
	if (v[cx][cy] == '.') {
		v[cx][cy] = 'F';
		return occupied;
	}
	else {
		cout << "Cell already discovered"<<endl;
		occupied = true;
		return occupied;
	}
}

bool revealCell(int l[ROWS][COLUMNS], char v[ROWS][COLUMNS], int cx, int cy) {
	bool gameOver = false;
	
	if (v[cx][cy] != '.') {
		if (v[cx][cy] != 'F') {
			return false;
		}
	}

	if (l[cx][cy] == -1) {
		gameOver = true;
		v[cx][cy] = 'X';
		return gameOver;
	}
	else if (l[cx][cy] == 0) {
		v[cx][cy] = '0';
		if (cy > 0) {
			revealCell(l, v, cx, cy - 1);
		}
		if (cy > 0 && cx > 0) {
			revealCell(l, v, cx - 1, cy - 1);
		}
		if (cx > 0) {
			revealCell(l, v, cx-1, cy);
		}
		if (cx > 0 && cy < 9) {
			revealCell(l, v, cx - 1, cy + 1);
		}
		if (cy < 9){
			revealCell(l, v, cx, cy + 1);
		}
		if (cx < 9 && cy < 9) {
			revealCell(l, v, cx + 1, cy + 1);
		}
		if (cx < 9) {
			revealCell(l, v, cx + 1, cy);
		}
		if (cx < 9 && cy > 0) {
			revealCell(l, v, cx + 1, cy - 1);
		}
	}
	else {
		v[cx][cy] = l[cx][cy]+'0';
	}
	return gameOver;
}

bool winControl(int l[ROWS][COLUMNS], char v[ROWS][COLUMNS]) {
	bool won = false;
	int flags = 0;
	int safe = 0;
	for (int j = 0; j < ROWS; j++) {
		for (int i = 0; i < COLUMNS; i++) {
			if (v[j][i] == 'F' && l[j][i]== -1) {
				flags++;
			}
			if (v[j][i] != '.' && v[j][i] != 'F') {
				safe++;
			}
		}
	}
	
	if (safe == 100 - MINES) {
		won = true;
	}
	return won;
}

int main() {
	srand(time(0));
	bool gameOver = false;
	bool won = false;
	bool firstMove = true;
	int discoveredCells = 0;
	char cellC;
	int cellCI;
	char coordinates[10]{ 'A','B','C','D','E','F','G','H','I','J' };
	int cellI;

	int logicGrid[ROWS][COLUMNS];
	char visualGrid[ROWS][COLUMNS];

	steady_clock::time_point start_time;

	while (!gameOver && !won) {
		if (!firstMove) {
			auto current_time = steady_clock::now();
			auto elapsed_now = duration_cast<seconds>(current_time - start_time).count();
			cout << "Current Time: " << formatTimeStr(elapsed_now) << endl;
		}

		if (firstMove) {
			printGrid(visualGrid, firstMove);
			while (true) {
				cout << "select the first cell(letter)" << endl;

				string letterInput;
				cin >> letterInput;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				if (letterInput.length() != 1) {
					cout << "Insert a valid coordinate" << endl;
					continue;
				}
				cellC = letterInput[0];
				cellC = toupper(cellC);
				if (cellC != 'A' && cellC != 'B' && cellC != 'C' && cellC != 'D' && cellC != 'E' && cellC != 'F' && cellC != 'G' && cellC != 'H' && cellC != 'I' && cellC != 'J') {
					cout << "Insert a valid coordinate" << endl;
					continue;
				}
				break;
			}
			for (int k = 0;k < ROWS;k++) {
				if (cellC == coordinates[k]) {
					cellCI = k;
					break;
				}
			}
			while (true) {
				cout << "select the first cell(number)" << endl;
				cin >> cellI;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Insert a valid coordinate" << endl;
					continue;
				}
				else {
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				if (cellI > 10 || cellI < 1) {
					cout << "Insert a valid coordinate" << endl;
					continue;
				}
				cellI--;
				break;
			}
			addMines(logicGrid, cellCI,cellI);
			firstMove = false;
			start_time = steady_clock::now();
			system("cls");
			gameOver = revealCell(logicGrid, visualGrid, cellCI, cellI);
			printGrid(visualGrid, firstMove);
		}
		else {
			int decision;
			while (true) {
				cout << "Add a flag or dig" << endl;
				cout << "1. dig" << endl << "2. flag" << endl;
				cin >> decision;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Insert a valid action" << endl;
					continue;
				}
				else {
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				if (decision != 1 && decision != 2) {
					cout << "Insert a valid action" << endl;
					continue;
				}
				break;
			}
			if (decision == 1) {
				while (true) {
					cout << "select the cell(letter)" << endl;
					string letterInput;
					cin >> letterInput;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					if (letterInput.length() != 1) {
						cout << "Insert a valid coordinate" << endl;
						continue;
					}
					cellC = letterInput[0];
					cellC = toupper(cellC);
					if (cellC != 'A' && cellC != 'B' && cellC != 'C' && cellC != 'D' && cellC != 'E' && cellC != 'F' && cellC != 'G' && cellC != 'H' && cellC != 'I' && cellC != 'J') {
						cout << "Insert a valid coordinate" << endl;
						continue;
					}
					break;
				}

				for (int k = 0;k < ROWS;k++) {
					if (cellC == coordinates[k]) {
						cellCI = k;
						break;
					}
				}
				while (true) {
					cout << "select the cell(number)" << endl;
					cin >> cellI;
					if (cin.fail()) {
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Insert a valid coordinate" << endl;
						continue;
					}
					else {
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
					}
					if (cellI > 10 || cellI < 1) {
						cout << "Insert a valid coordinate" << endl;
						continue;
					}
					cellI--;
					break;
				}
				system("cls");
				gameOver = revealCell(logicGrid, visualGrid, cellCI, cellI);
				printGrid(visualGrid, firstMove);

				if (gameOver) {
					cout << "You lost" << endl;
					return 0;
				}
			}
			else if (decision == 2) {
				 bool occupied = true;
				while (occupied) {
					while (true) {
						cout << "select the cell(letter)" << endl;
						string letterInput;
						cin >> letterInput;
						cin.ignore(numeric_limits<streamsize>::max(), '\n');

						if (letterInput.length() != 1) {
							cout << "Insert a valid coordinate" << endl;
							continue;
						}
						cellC = letterInput[0];
						cellC = toupper(cellC);
						if (cellC != 'A' && cellC != 'B' && cellC != 'C' && cellC != 'D' && cellC != 'E' && cellC != 'F' && cellC != 'G' && cellC != 'H' && cellC != 'I' && cellC != 'J') {
							cout << "Insert a valid coordinate" << endl;
							continue;
						}
						break;
					}

					for (int k = 0;k < ROWS;k++) {
						if (cellC == coordinates[k]) {
							cellCI = k;
							break;
						}
					}
					while (true) {
						cout << "select the cell(number)" << endl;
						cin >> cellI;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							cout << "Insert a valid coordinate" << endl;
							continue;
						}
						else {
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
						}
						if (cellI > 10 || cellI < 1) {
							cout << "Insert a valid coordinate" << endl;
							continue;
						}
						cellI--;
						break;
					}
					occupied = addFlag(visualGrid, cellCI, cellI);
				}
				system("cls");
				printGrid(visualGrid, firstMove);
				won = winControl(logicGrid, visualGrid);

				if (won) {
					cout << "You Won" << endl;
					auto end_time = steady_clock::now();
					auto elapsed = duration_cast<seconds>(end_time - start_time).count();
					cout << "Time: "<<formatTimeStr(elapsed)<<endl;
					return 0;
				}
			}
		}

	}
	return 0;
}