#include <iostream>
#include <string>

using namespace std;

//Nachdem ich den ersten Algorithmus mit Brute-Force ueberarbeitet habe
//hatte ich dann einen Algorithmus genutzt bei dem von dem gesetzten Stein 3 Felder geshiftet wurden
//und dann jedes Feld zwischen shift und Startpunkt geprueft wird. Daraufhin habe ich gemerkt das so Felder
//doppelt geprueft werden, weshalb ich eine Rekursiven Algorithmus geschrieben habe der Schrittweise vom Startpunkt
//das naechste Feld prueft, mit integrierten Richtungswechsel, wenn ein Rand erreicht ist

//Dynamische Spielfeldgroesse hinzugefuegt
//Pruefung der Eingabe fuer 1 char hinzugefuegt

//*===================================
//*===	Deklaration Globale Variable
//*===================================
//Dynamisches Spielfeld
const int rowSize = 6;
const int columSize = 7;
string readLine = "";
//maximaler columSize sollte 26 nicht ueberschreiten

//*===================================
//*===	Definition der Punkt-Struktur
//*===================================
struct Point {
	int x;
	int y;
};

//*===================================
//*===	Ausgabe des Begruessungstextes
//*===================================
void welcome(int round) {
	cout << "============================" << endl;
	cout << "Willkommen zu Vier gewinnt, ein Spiel fuer 2 Spieler" << endl;
	cout << "Ziel ist es 4 Steine in einer Reihe zu bekommen." << endl;
	cout << "Diese koennen Waagerecht, Senkrecht oder Diagonal sein!" << endl;
	cout << "Copyright © 2020 Timm Buengel" << endl;
	cout << "============================" << endl << endl;
	cout << "Runde: " << round << endl;
}

//*===================================
//*===	Zeichne Spielfeld
//*===================================
void drawGameField(char gameField[][columSize], int rowSize, char colum[], int columSize) {
	cout << "" << endl;
	//Trennlinie
	for (int i = 0; i < columSize; i++) {
		if (i == 0)
			cout << "\t\t ";
		cout << "-" << " ";
	} cout << "" << endl;

	//Feld darstellen
	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < columSize; j++) {
			if (j == 0)
				cout << "\t\t|";
			cout << gameField[i][j] << "|";
		} cout << "" << endl;
	}

	//Trennlinie
	for (int i = 0; i < columSize; i++) {
		if (i == 0)
			cout << "\t\t ";
		cout << "-" << " ";
	} cout << "" << endl;

	//Splatenbezeichnung
	for (int i = 0; i < columSize; i++) {
		if (i == 0)
			cout << "\t\t|";
		cout << colum[i] << "|";
	} cout << "" << endl << endl;
}

//*===================================
//*===	Convertierung von Grossbuchstaben zu Kleinbuchstaben
//*===================================
char switchToLowerCase(char c) {
	int n = int(c);
	if (n >= 65 && n <= 90)
		return char(n + 32);

	return c;
}

//*===================================
//*===	Convertierung von Grossbuchstaben zu Kleinbuchstaben
//*===================================
string directionToString(int x, int y) {
	if (x == -1 && y == -1) return "[links-oben]";
	else if (x == 0 && y == -1) return "[links]";
	else if (x == 1 && y == -1) return "[links-unten]";
	else if (x == 1 && y == 0) return "[unten]";
	else if (x == 1 && y == 1) return "[rechts-unten]";
	else if (x == 0 && y == 1) return "[rechts]";
	else if (x == -1 && y == 1) return "[rechts-oben]";
	else if (x == 1 && y == 0) return "[oben]";
	else return "[Richtung nicht bekannt]";
}

//*===================================
//*===	Convertiere Spaltenbezeichnung zum Int-Wert
//*===================================
int getValue(char value) {
	value = switchToLowerCase(value);
	int n = int(value) - 97;
	if (!(n >= 0 && n < columSize))
		n = -1;

	return n;
}

//*===================================
//*===	Spalten-Input solange lesen bis Korrekte Eingabe
//*===================================
int readInput(char colum[], int columSize) {
	char input = ' ';
	int countInput = 0;

	do {
		if (countInput != 0) {
			cout << "Das Zeichen muss zwischen '" << colum[0] << "' und '" << colum[columSize - 1] << "' liegen:"; cin >> readLine; input = readLine[0];
		}
		else {
			cout << "Bitte geben Sie eine Spalte an, in das Sie Ihr Zeichen setzen moechten (" << colum[0] << "-" << colum[columSize - 1] << "): "; cin >> readLine; input = readLine[0];
		}
		countInput++;
	} while (getValue(input) == -1);

	return getValue(input);
}

//*===================================
//*===	Spielstein auf dem Spielfeld hinzufuegen
//*===================================
Point addPlayerStone(char gameField[][columSize], int fieldSize, char player, int value, char colum[], int columSize) {
	Point result;

	for (int i = fieldSize - 1; i >= 0; i--) {
		if (gameField[i][value] == ' ') {
			gameField[i][value] = player;
			result.x = i;
			result.y = value;
			break;
		}
		else {
			if (i == 0) { // Pruefen ob die Spalte voll ist
				char input = ' ';
				cout << "Die eingegebenen Spalte ist bereits voll!" << endl;
				int newValue = readInput(colum, columSize);
				result = addPlayerStone(gameField, fieldSize, player, newValue, colum, columSize);
			}
		}
	}

	return result;//da recursiver Aufruf, wird result immer initialisiert
}

//*===================================
//*===	Vorzeichen wechseln fuer Richtungswechsel
//*===================================
void switchSign(int* value) {
	if (*value == -1) *value = 1;
	else if (*value == 1) *value = -1;
	else if (*value == 0) *value = 0;
}

//*===================================
//*===	Algorithmus zum pruefen auf 4 aufeinander folgende Steine
//*===================================
/**
*	@param gameField	enthaelt das die Zweidimensionale Array des Spielfeldes
*	@param fieldSize	gibt die maximale groesse des Arrays an
*	@param x			x-Wert des eingegebenen Spielsteines im Array
*	@param y			y-Wert des eingegebenen Spielsteines im Array
*	@param offSetX		Wert um den x im Array geshiftet wird
*	@param offSetY		Wert um den y im Array geshiftet wird
*	@param player		char-Zeichen des Spielers
*	@param count		Zaehler Variable fuer rekursiven Aufruf, welche die Anzahl der Steine in einer Reihe beinhaltet
*	@param startPos		Startposition der auch nach rekursiven Aufruf nicht veraendert wird
*	@param flagDirectionChange		false, wenn kein Richtungswechsel stattgefunden hat
*	@param direction	Enthaelt string lediglich zum testen
*/
bool checkDirection(char gameField[][columSize], int fieldSize, int x, int y, int offSetX, int offSetY, char player, int count, Point startPos, bool flagDirectionChange, string direction) {
	//cout << endl << "Aktuelle Position im Array(" << x << y << "). Es geht in Richtung [" << direction << "]. Gezaehlt sind [" << count << "].Richtige" << endl;
	bool result = false;

	if (count == 4) { return true; } // Wenn 4 in einer Reihe gezaehlt werden, wird true zurueck gegeben
	int valueX = x + offSetX, valueY = y + offSetY;

	if (valueX >= 0 && valueX < fieldSize && valueY >= 0 && valueY < fieldSize) { // pruefen ob der naechste anliegende Stein Innerhalb der Array Grenze liegt
		//cout << "naechste Stelle zu pruefen(" << valueX << valueY << ")" << endl;
		if (gameField[valueX][valueY] == player) {
			count++;
			//cout << "Der Stein an der Stelle(" << valueX << valueY << ") [ist gleich] " << player << " gehe weiter!" << endl;
			// rekursiver aufruf
			if (!flagDirectionChange) { // pruefen ob ein Richtungswechsel stattgefunden hat
				return checkDirection(gameField, fieldSize, valueX, valueY, offSetX, offSetY, player, count, startPos, false, directionToString(offSetX, offSetY));
			}
			else {
				return checkDirection(gameField, fieldSize, valueX, valueY, offSetX, offSetY, player, count, startPos, true, directionToString(offSetX, offSetY));
			}
		}
		else {
			//cout << "[FALSE] (" << valueX << valueY << ") ist [NICHT gleich] " << player << " gehe weiter!" << endl;
			if (!flagDirectionChange) {//kann nur durchgefuehrt werden wenn noch kein Richtungswechsel stattgefunden hat
				//cout << "[Wechsel] die Richtung!" << endl;
				switchSign(&offSetX); switchSign(&offSetY);
				valueX = startPos.x; valueY = startPos.y;
				//cout << "Gehe zu Startpunkt(" << valueX << valueY << ")" << endl;
				return checkDirection(gameField, fieldSize, valueX, valueY, offSetX, offSetY, player, count, startPos, true, directionToString(offSetX, offSetY));
			}
			else {
				//cout << "Es gab bereits ein Richtungswechsel!" << endl;
				return false;
			}
		}
	}
	else { //Wenn der Rand des Spielfeldes erreicht wurde, wird zunaechst ein Ridchtungswechsel durchgefuehrt
		if (!flagDirectionChange) {//kann nur durchgefuehrt werden wenn noch kein Richtungswechsel stattgefunden hat
			//cout << "[Wechsel] die Richtung!" << endl;
			switchSign(&offSetX); switchSign(&offSetY);
			valueX = startPos.x; valueY = startPos.y;
			//cout << "Gehe zu Startpunkt(" << valueX << valueY << ")" << endl;
			return checkDirection(gameField, fieldSize, valueX, valueY, offSetX, offSetY, player, count, startPos, true, directionToString(offSetX, offSetY));
		}
		else {
			//cout << "Die Stelle(" << valueX << valueY << ") liegt ausserhalb des Arrays" << endl;
			return false;
		}
	}
}


int main() {
	char player1 = 'X', player2 = 'O', player = ' ', stat = 'y';
	char gameField[rowSize][columSize];
	char colum[columSize];
	int directions[4][2] = { {-1, -1}, { 0, -1}, { 1, -1}, { 1,  0} };

	//ColumArray Initialisieren
	for (int i = 0; i < columSize; i++) {
		colum[i] = char(i + 65);
	}
	do {
		bool win = false;
		int round = 1, count = 1;

		// Array Initialisieren/Spielfeld bereinigen
		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < columSize; j++) {
				gameField[i][j] = ' ';
			}
		}

		//Die Schleife fuer das eigentliche Spiel
		do {
			if (round % 2 == 1)
				player = player1;
			else
				player = player2;

			welcome(round);
			drawGameField(gameField, rowSize, colum, columSize);

			//Spielfeld ist voll (9*9+1) Unentschieden
			if (round == (rowSize * rowSize) + 1) { cout << "Das Feld ist voll, Niemand hat gewonnen" << endl; break; }
			cout << "Spieler '" << player << "' ist an der Reihe!" << endl;

			//Angabe der Spalte	/ Zeichen des Spielers einlesen
			int value = readInput(colum, columSize);

			// Zeichen zum Feld hinzufuegen, returnWert ist die Postion des gesetzten Steines
			Point lastStonePosition = addPlayerStone(gameField, rowSize, player, value, colum, columSize);

			//nach hinzufuegen des Spielsteines wird das Feld neu gezeichnet
			system("cls");
			welcome(round);
			drawGameField(gameField, rowSize, colum, columSize);

			//Pruefe vom gesetzten Stein die eine Folge von Reihen, mit Richtungswechsel
			if (round > 6) {
				for (int i = 0; i < 4; i++) {
					int count = 1;
					if (!win)
						win = checkDirection(gameField, rowSize, lastStonePosition.x, lastStonePosition.y, directions[i][0], directions[i][1], player, count, lastStonePosition, false, directionToString(directions[i][0], directions[i][1]));
				}
				/*
				// links-oben
				if (!win)
					win = checkDirection(gameField, rowSize, lastStonePosition.x, lastStonePosition.y, -1, -1, player, 1, lastStonePosition, false, "[links-oben]");

				// links
				if (!win)
					win = checkDirection(gameField, rowSize, lastStonePosition.x, lastStonePosition.y, 0, -1, player, 1, lastStonePosition, false, "[links]");

				// links-unten
				if (!win)
					win = checkDirection(gameField, rowSize, lastStonePosition.x, lastStonePosition.y, +1, -1, player, 1, lastStonePosition, false, "[links-unten]");

				// unten
				if (!win)
					win = checkDirection(gameField, rowSize, lastStonePosition.x, lastStonePosition.y, +1, 0, player, 1, lastStonePosition, false, "[unten]");
				//Diese fallen weg da ein Richtungswechsel implementiert ist
				// unten-rechts		x=+1 y=+1
				// rechts			x= 0 y=+1
				// rechts-oben		x=-1 y=+1
				// oben faellt eh weg da ueber dem gesetzten Stein niemals ein anderer Stein liegen kann
				*/
			}

			//Ausstiegspunkt wenn Spieler gewonnen hat
			if (win) {
				cout << "" << endl;
				cout << "\t===========================================" << endl;
				cout << "\t>>> Spieler '" << player << "' hat das Spiel gewonnen! <<<" << endl;
				cout << "\t===========================================" << endl;
				cout << endl << "\tWollen Sie ein neues Spiel Starten? y/n" << endl;
				do {
					cin >> readLine;
					stat = switchToLowerCase(readLine[0]);
				} while (stat != 'n' && stat != 'y');
				system("pause");
				break;
			}
			system("cls");
			round++;
		} while (!win);

		system("cls");//neues Spiel Console Komplett leeren
	} while (stat == 'y');
	return 0;
}