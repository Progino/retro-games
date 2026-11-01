#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <cctype>
#include <limits>
#include <chrono>
#include <fstream>
#include <sstream>
#define ROWS 10
#define COLUMNS 10

using namespace std;
using namespace std::chrono;

const string FILENAME = "minesweeper.txt";

void saveGame(int l[ROWS][COLUMNS], char v[ROWS][COLUMNS], int time) {
	ofstream file(FILENAME);
	if (!file.is_open()) {
		cout << "Error: Could not save data to file!" << endl;
		return;
	}
	file << time << endl;
	for (int j = 0;j < ROWS;j++) {
		for (int i = 0;i < COLUMNS;i++) {
			file << l[j][i] << ",";
		}
	}
	file  << endl;
	for (int j = 0;j < ROWS;j++) {
		for (int i = 0;i < COLUMNS;i++) {
			file << (int)v[j][i] << ",";
		}
	}
	file <<endl;
	file.close();
}

int loadTime() {
	ifstream file(FILENAME);
	if (!file.is_open()) {
		return 0;
	}
	string time;
	if (getline(file, time)) {
		file.close();
		if (time.empty()) return 0;
		return stoi(time);
	}
	file.close();
	return 0;
}

void loadLogicGrid(int l[ROWS][COLUMNS]) {
	ifstream file(FILENAME);
	if (!file.is_open()) {
		return;
	}
	
	string line;
	getline(file, line);

	if (getline(file, line)) {
		stringstream ss(line);
		string cell;
		for (int j = 0; j < ROWS; j++) {
			for (int i = 0; i < COLUMNS; i++) {
				if (getline(ss, cell, ',')) {
					l[j][i] = stoi(cell);
				}
			}
		}
	}
	file.close();
}

void loadVisualGrid(char v[ROWS][COLUMNS]) {
	ifstream file(FILENAME);
	if (!file.is_open()) {
		return;
	}

	string line;
	getline(file, line);
	getline(file, line);

	if (getline(file, line)) {
		stringstream ss(line);
		string cell;
		for (int j = 0; j < ROWS; j++) {
			for (int i = 0; i < COLUMNS; i++) {
				if (getline(ss, cell, ',')) {
					v[j][i] = (char)stoi(cell);
				}
			}
		}
	}
	file.close();
}

string formatTimeStr(int totalSeconds) {
	int hours = totalSeconds / 3600;
	int minutes = (totalSeconds % 3600) / 60;
	int seconds = totalSeconds % 60;

	string hh = (hours < 10 ? "0" : "") + to_string(hours);
	string mm = (minutes < 10 ? "0" : "") + to_string(minutes);
	string ss = (seconds < 10 ? "0" : "") + to_string(seconds);

	return hh + ":" + mm + ":" + ss;
}

void printGrid(char v[ROWS][COLUMNS],bool fM,int minesP) {
	char coordinates[10]{ 'A','B','C','D','E','F','G','H','I','J' };

	int minesLeft = minesP;
	if (fM) {
		for (int j = 0; j < ROWS; j++) {
			for (int i = 0; i < COLUMNS; i++) {
				v[j][i] = '.';
			}
		}
	}

	cout << "     1  2  3  4  5  6  7  8  9  10" <<endl;
	cout << "    ------------------------------" << endl;
	for (int j = 0;j < ROWS;j++) {
		cout << " " << coordinates[j] << " | ";
		for (int i = 0;i < COLUMNS;i++) {
			if (v[j][i] == 'F') {
				cout << "\033[32m";
				minesLeft--;
			}
			cout << v[j][i]<<"  "<<"\033[0m";
		}
		cout << endl;
	}
	cout << endl;
	cout << "            MINES LEFT: " << minesLeft << "            "<<endl << endl;
}

void printFinalGrid(char v[ROWS][COLUMNS], int l[ROWS][COLUMNS]) {
	char coordinates[10]{ 'A','B','C','D','E','F','G','H','I','J' };

	cout << "     1  2  3  4  5  6  7  8  9  10" << endl;
	cout << "    ------------------------------" << endl;
	for (int j = 0;j < ROWS;j++) {
		cout << " " << coordinates[j] << " |";
		for (int i = 0;i < COLUMNS;i++) {
			if (l[j][i] == -1) {
				cout << "\033[31m";
				v[j][i] = 'X';
			}
			cout << v[j][i] << "  "<< "\033[0m";
		}
		cout << endl;
	}
}

void addMines(int l[ROWS][COLUMNS],int cx,int cy,int minesP){

	for (int j = 0; j < ROWS; j++) {
		for (int i = 0; i < COLUMNS; i++) {
			l[j][i] = 0;
		}
	}

	int mines = 0;

	

	while (mines < minesP) {
		int randomRow = rand() % 10;
		int randomColumn = rand() % 10;

		if (!(randomRow >= cx -1 && randomRow <= cx + 1 && randomColumn >= cy - 1 && randomColumn <= cy + 1)){
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
			if (i < (COLUMNS -1) && j>0 && l[j - 1][i + 1] == -1) {
				number++;
			}
			if (i < (COLUMNS -1) && l[j][i + 1] == -1) {
				number++;
			}
			if (i < (COLUMNS - 1) && j<(ROWS-1) && l[j+1][i + 1] == -1) {
				number++;
			}
			if (j < (ROWS - 1) && l[j+1][i] == -1) {
				number++;
			}
			if (i > 0 && j < (ROWS - 1) && l[j+1][i - 1] == -1) {
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

bool revealCell(int l[ROWS][COLUMNS], char v[ROWS][COLUMNS], int cx, int cy, bool manualClick = false) {
	bool gameOver = false;
	

	if (v[cx][cy] == 'F') {
		return false;
	}
	
	if (v[cx][cy] >= '1' && v[cx][cy] <= '8') {
		if (manualClick) {
			goto checkNear;
		}
		else {
			return false;
		}
		
	}

	if (v[cx][cy] == '0') {
		return false;
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
	else if (l[cx][cy] < 8 && l[cx][cy] > 0) {
		v[cx][cy] = l[cx][cy] + '0';

		checkNear:

		int flags = 0;
		if (cy > 0 && v[cx][cy - 1] == 'F') {
			flags++;
		}
		if (cy > 0 && cx > 0 && v[cx - 1][cy - 1] == 'F') {
			flags++;
		}
		if (cx > 0 && v[cx - 1][cy] == 'F') {
			flags++;
		}
		if (cx > 0 && cy < 9 && v[cx - 1][cy + 1] == 'F') {
			flags++;
		}
		if (cy < 9 && v[cx][cy + 1] == 'F') {
			flags++;
		}
		if (cx < 9 && cy < 9 && v[cx + 1][cy + 1] == 'F') {
			flags++;
		}
		if (cx < 9 && v[cx + 1][cy] == 'F') {
			flags++;
		}
		if (cx < 9 && cy > 0 && v[cx + 1][cy - 1] == 'F') {
			flags++;
		}
		if (flags == l[cx][cy]) {
			if (cy > 0 && revealCell(l, v, cx, cy - 1)) {
				gameOver = true;
			}
			if (cy > 0 && cx > 0 && revealCell(l, v, cx - 1, cy - 1)) {
				gameOver = true;
			}
			if (cx > 0 && revealCell(l, v, cx - 1, cy)) {
				gameOver = true;
			}
			if (cx > 0 && cy < 9 && revealCell(l, v, cx - 1, cy + 1)) {
				gameOver = true;
			}
			if (cy < 9 && revealCell(l, v, cx, cy + 1)) {
				gameOver = true;
			}
			if (cx < 9 && cy < 9 && revealCell(l, v, cx + 1, cy + 1)) {
				gameOver = true;
			}
			if (cx < 9 && revealCell(l, v, cx + 1, cy)) {
				gameOver = true;
			}
			if (cx < 9 && cy > 0 && revealCell(l, v, cx + 1, cy - 1)) {
				gameOver = true;
			}
		}
	}
	else {
		v[cx][cy] = l[cx][cy]+'0';
	}
	return gameOver;
}

bool winControl(int l[ROWS][COLUMNS], char v[ROWS][COLUMNS],int minesP) {
	bool won = false;
	int flags = 0;
	int mines = 0;
	for (int j = 0; j < ROWS; j++) {
		for (int i = 0; i < COLUMNS; i++) {
			if (v[j][i] == 'F' && l[j][i]== -1) {
				flags++;
			}
			if (v[j][i] == '.') {
				mines++;
			}
		}
	}
	
	if (mines+flags == minesP) {
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

	int easyMines = 5;
	int mediumMines = 15;
	int hardMines = 25;
	int mines = easyMines;
	int difficulty;

	int time = 0;

	while (true) {
		int loadOption;
		cout << endl << "      MINESWEEPER" << endl;
		cout << "------------------------" << endl;
		cout << "1. New game" << endl << "2. Load game" << endl << "3. Exit" << endl;
		cin >>loadOption;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Insert a valid option" << endl;
			continue;
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		if (loadOption == 1) {
			system("cls");
		}
		if (loadOption == 2) {
			system("cls");
			time = loadTime();
			if (time == 0) {
				cout << "No game saved" << endl;
				continue;
			}
			loadLogicGrid(logicGrid);
			loadVisualGrid(visualGrid);
			
			mines = 0;
			for (int j = 0;j < ROWS;j++) {
				for (int i = 0;i < COLUMNS;i++) {
					if (logicGrid[j][i] == -1) mines++;
				}
			}
			if (mines == 0) mines = easyMines;

			
			firstMove = false;
			printGrid(visualGrid, firstMove, mines);
			start_time = steady_clock::now();
		}
		if (loadOption == 3) {
			return 0;

		}
		if (loadOption != 1 && loadOption != 2 && loadOption != 3) {
			cout << "Insert a valid option" << endl;
			continue;
		}

		while (firstMove) {
			cout << "Select the difficulty: " << endl;
			cout << "1. Easy(5 mines)" << endl;
			cout << "2. Medium(15 mines)" << endl;
			cout << "3. Hard(25 mines)" << endl;
			cin >> difficulty;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Insert a valid option" << endl;
				continue;
			}
			else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			switch (difficulty) {
			case 1:
				mines = easyMines;
				break;
			case 2:
				mines = mediumMines;
				break;
			case 3:
				mines = hardMines;
				break;
			default:
				cout << "Insert a valid option" << endl;
				continue;
			}
			system("cls");
			break;
		}

		while (!gameOver && !won) {
			if (!firstMove) {
				auto current_time = steady_clock::now();
				int elapsed_now = duration_cast<seconds>(current_time - start_time).count() + time;
				cout << "Current Time: " << formatTimeStr(elapsed_now) << endl;
			}

			if (firstMove) {
				printGrid(visualGrid, firstMove, mines);
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
				addMines(logicGrid, cellCI, cellI, mines);
				firstMove = false;
				start_time = steady_clock::now();
				system("cls");
				gameOver = revealCell(logicGrid, visualGrid, cellCI, cellI);
				printGrid(visualGrid, firstMove, mines);
				saveGame(logicGrid, visualGrid, time);
			}
			else {
				int decision;
				while (true) {
					cout << endl << "Add a flag or dig" << endl;
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
						cout << endl << "select the cell(letter)" << endl;
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
					gameOver = revealCell(logicGrid, visualGrid, cellCI, cellI,true);
					printGrid(visualGrid, firstMove, mines);

					if (gameOver) {
						system("cls");
						printFinalGrid(visualGrid, logicGrid);
						cout << endl << "Game over" << endl;
						saveGame(logicGrid, visualGrid, 0);
						return 0;
					}
					won = winControl(logicGrid, visualGrid, mines);

					if (won) {
						cout << endl << "You win" << endl;
						auto end_time = steady_clock::now();
						auto elapsed = duration_cast<seconds>(end_time - start_time).count();
						cout << "Time: " << formatTimeStr(elapsed) << endl;
						saveGame(logicGrid, visualGrid, 0);
						return 0;
					}
					int current_seconds = duration_cast<seconds>(steady_clock::now() - start_time).count() + time;
					saveGame(logicGrid, visualGrid, current_seconds);
				}
				else if (decision == 2) {
					bool occupied = true;
					while (occupied) {
						while (true) {
							cout << endl << "select the cell(letter)" << endl;
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
					printGrid(visualGrid, firstMove, mines);
					won = winControl(logicGrid, visualGrid, mines);

					if (won) {
						cout << endl << "You Win" << endl;
						auto end_time = steady_clock::now();
						auto elapsed = duration_cast<seconds>(end_time - start_time).count();
						cout << "Time: " << formatTimeStr(elapsed) << endl;
						saveGame(logicGrid, visualGrid, 0);
						return 0;
					}
					int current_seconds = duration_cast<seconds>(steady_clock::now() - start_time).count() + time;
					saveGame(logicGrid, visualGrid, current_seconds);
				}
			}

		}
	}
}