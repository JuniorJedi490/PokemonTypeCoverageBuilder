//============================================================================
// Name        : PokemonTypeCoverageBuilder.cpp
// Author      : JuniorJedi257
// Version     : 1.0
// Copyright   : MIT license
// Description :
//============================================================================

#include <iostream>
#include <string>
using namespace std;

const string PROGRAM_INIT_MESSAGE = string("--------Pokemon Type Coverage Builder--------\n") +
		"This program is meant to aid you with Pokemon moveset creation by telling you which type, " +
		"if added, will improve your coverage the most.\n\nWhen this program is asking for a type, " +
		"you can also enter \"License\" to view this program's license " +
		"(the MIT license), or \"Exit\" to close the program.";
const string MESSAGE_FIRST_TYPE_CHOICE = "Please enter a type to continue.";
const string MESSAGE_TYPE_CHOICE = "Which type of move would you like to add?";
const string MESSAGE_ENTER_TYPE = "Enter a type: ";
const string MESSAGE_TYPE_ALREADY_SELECTED = "You already have an attack of that type.";
const string MESSAGE_INVALID_TYPE = string("Invalid type.\n") +
		"Valid types are: Normal, Fighting, Flying, Poison, Ground, Rock, Bug, Ghost, Steel, " +
		"Fire, Water, Grass, Electric, Psychic, Ice, Dragon, Dark, and Fairy." +
		"\nYou can also type License to view the software license, or Exit to close the program.";

/** Prompts the user to enter a type, then outputs its ID using getTypeId(). */
int getUserType(bool first);

/** Used to convert the type (as a string) to an integer ID. */
int getTypeId(string type);

/** Inverse of getUserType, used to print the type name based on its ID. */
string getTypeFromId(int typeId);

/** Fills the provided array with the Gen VI+ type chart. */
void setupTypeChart(float typeChart[18][18]);

/** Prints the MIT License. */
void printLicense();

int main() {
	cout << PROGRAM_INIT_MESSAGE << endl;

	// Create and set up arrays and variables
	float typeChart[18][18];
	setupTypeChart(typeChart);
	bool selectedTypes[18];
	int numWeakCombinations[18];
	int numNonResistantCombinations[18];
	int numCurrWeakCombinations;
	int numCurrNonResistantCombinations;
	for (int i = 0; i < 18; i += 3) {
		selectedTypes[i] = false;
		selectedTypes[i + 1] = false;
		selectedTypes[i + 2] = false;
		numWeakCombinations[i] = 0;
		numWeakCombinations[i + 1] = 0;
		numWeakCombinations[i + 2] = 0;
		numNonResistantCombinations[i] = 0;
		numNonResistantCombinations[i + 1] = 0;
		numNonResistantCombinations[i + 2] = 0;
	}
	int alreadyWeak;
	int alreadyNotResisted;
	float effectiveness;
	char con;
	
	// Get initial type
	int nexttype = getUserType(true);

	while (nexttype != 255) {
		if (nexttype < 0 || nexttype > 17) return 1; // Should be impossible for this to occur
		if (selectedTypes[nexttype]) {
			cout << MESSAGE_TYPE_ALREADY_SELECTED << endl;
		} else {
			// Set user type as currently active
			selectedTypes[nexttype] = true;

			// Reset arrays
			for (int i = 0; i < 18; i += 3) {
				numWeakCombinations[i] = 0;
				numWeakCombinations[i + 1] = 0;
				numWeakCombinations[i + 2] = 0;
				numNonResistantCombinations[i] = 0;
				numNonResistantCombinations[i + 1] = 0;
				numNonResistantCombinations[i + 2] = 0;
			}
			numCurrWeakCombinations = 0;
			numCurrNonResistantCombinations = 0;

			// Calculate type combination stats
			for (int type1 = 0; type1 < 18; ++type1) {
			for (int type2 = 0; type2 < 18; ++type2) {
				if (type2 < type1) continue;
				alreadyWeak = 0;
				alreadyNotResisted = 0;
				// Mark as already weak / not resisted if so
				for (int selectedType = 0; selectedType < 18; ++selectedType) {
					if (selectedTypes[selectedType]) {
						effectiveness = typeChart[selectedType][type1] * typeChart[selectedType][type2];
						alreadyWeak = ((effectiveness > 1.0) || alreadyWeak);
						alreadyNotResisted = ((effectiveness >= 1.0) || alreadyNotResisted);
					}
				}
				numCurrWeakCombinations += alreadyWeak;
				numCurrNonResistantCombinations += alreadyNotResisted;
				
				// For each potential attacking type, check effectiveness
				for (int attackingType = 0; (attackingType < 18); ++attackingType) {
					if (!selectedTypes[attackingType]) {
						effectiveness = typeChart[attackingType][type1] * typeChart[attackingType][type2];
						numWeakCombinations[attackingType] += ((effectiveness > 1.0) && !alreadyWeak);
						numNonResistantCombinations[attackingType] += ((effectiveness >= 1.0) && !alreadyNotResisted);
					}
				}
			}}
			// Print active types
			cout << "You have of attacks of these types: ";
			bool first = true;
			for (int i = 0; i < 18; ++i) {
				if (selectedTypes[i]) {
					if (first) {
						first = false;
					} else {
						cout << ", ";
					}
					cout << getTypeFromId(i);
				}
			}
			// Print current covfefe stats and potential type additions
			cout << endl << endl << "Currently, " << numCurrWeakCombinations << " type combinations are weak to your attacks, and " <<
				(171 - numCurrNonResistantCombinations) << " type combinations resist your attacks.\n\nPotential types to add: " << flush;
			cout << "(<Type>: <Added neutral coverage> / <Added super effective coverage>)" << endl;
			for (int i = 0; i < 18; ++i) {
				if (!selectedTypes[i]) cout << getTypeFromId(i) << ": " << numNonResistantCombinations[i] << " / " << numWeakCombinations[i] << endl;
			}
		}
		cout << endl << "Add another type? (y/n):" << flush;
		cin >> con;
		if (con == 'n' || con == 'N') {
			nexttype = 255;
		} else {
			nexttype = getUserType(false);
		}
	}
	return 0;
}

int getUserType(bool first) {
	if (first) {
		cout << MESSAGE_FIRST_TYPE_CHOICE << endl;
	} else {
		cout << MESSAGE_TYPE_CHOICE << endl;
	}

	int type = -1;
	string typeString;
	while (type == -1) {
		cout << MESSAGE_ENTER_TYPE << flush;
		cin >> typeString;
		type = getTypeId(typeString);
		if (type == -1) {
			cout << MESSAGE_INVALID_TYPE << endl;
		}
		if (type == 127) {
			printLicense();
			if (first) {
				cout << MESSAGE_FIRST_TYPE_CHOICE << endl;
			} else {
				cout << MESSAGE_TYPE_CHOICE << endl;
			}
			type = -1;
		}
	}
	return type;
}

int getTypeId(string type) {
	for (unsigned int i = 0; i < type.length(); ++i) {
		type[i] = tolower(type[i]);
	}
	if (type == "normal") { return 0; }
	else if (type == "fighting") { return 1; }
	else if (type == "flying") { return 2; }
	else if (type == "poison") { return 3; }
	else if (type == "ground") { return 4; }
	else if (type == "rock") { return 5; }
	else if (type == "bug") { return 6; }
	else if (type == "ghost") { return 7; }
	else if (type == "steel") { return 8; }
	else if (type == "fire") { return 9; }
	else if (type == "water") { return 10; }
	else if (type == "grass") { return 11; }
	else if (type == "electric") { return 12; }
	else if (type == "psychic") { return 13; }
	else if (type == "ice") { return 14; }
	else if (type == "dragon") { return 15; }
	else if (type == "dark") { return 16; }
	else if (type == "fairy") { return 17; }
	else if (type == "exit") { return 255; }
	else if (type == "license") { return 127; }
	else { return -1; }
}

string getTypeFromId(int typeId) {
	switch (typeId) {
	case 0: return "Normal";
	case 1: return "Fighting";
	case 2: return "Flying";
	case 3: return "Poison";
	case 4: return "Ground";
	case 5: return "Rock";
	case 6: return "Bug";
	case 7: return "Ghost";
	case 8: return "Steel";
	case 9: return "Fire";
	case 10: return "Water";
	case 11: return "Grass";
	case 12: return "Electric";
	case 13: return "Psychic";
	case 14: return "Ice";
	case 15: return "Dragon";
	case 16: return "Dark";
	case 17: return "Fairy";
	default: return "Invalid type";
	}
}

void setupTypeChart(float typeChart[18][18]) {
	// Default to neutral damage
	for (int i = 0; i < 18; i += 3) {
		for (int j = 0; j < 18; j += 3) {
			typeChart[i][j] = 1.0;
			typeChart[i+1][j] = 1.0;
			typeChart[i+2][j] = 1.0;
			typeChart[i][j+1] = 1.0;
			typeChart[i+1][j+1] = 1.0;
			typeChart[i+2][j+1] = 1.0;
			typeChart[i][j+2] = 1.0;
			typeChart[i+1][j+2] = 1.0;
			typeChart[i+2][j+2] = 1.0;
		}
	}

	// Type weaknesses
	typeChart[1][0] = 2.0;  // Fighting beats Normal
	typeChart[1][5] = 2.0;  // Fighting beats Rock
	typeChart[1][8] = 2.0;  // Fighting beats Steel
	typeChart[1][14] = 2.0; // Fighting beats Ice
	typeChart[1][16] = 2.0; // Fighting beats Dark
	typeChart[2][1] = 2.0;  // Flying beats Fighting
	typeChart[2][6] = 2.0;  // Flying beats Bug
	typeChart[2][11] = 2.0; // Flying beats Grass
	typeChart[3][11] = 2.0; // Poison beats Grass
	typeChart[3][17] = 2.0; // Poison beats Fairy
	typeChart[4][3] = 2.0;  // Ground beats Poison
	typeChart[4][5] = 2.0;  // Ground beats Rock
	typeChart[4][8] = 2.0;  // Ground beats Steel
	typeChart[4][9] = 2.0;  // Ground beats Fire
	typeChart[4][12] = 2.0; // Ground beats Electric
	typeChart[5][2] = 2.0;  // Rock beats Flying
	typeChart[5][6] = 2.0;  // Rock beats Bug
	typeChart[5][9] = 2.0;  // Rock beats Fire
	typeChart[5][14] = 2.0; // Rock beats Ice
	typeChart[6][11] = 2.0; // Bug beats Grass
	typeChart[6][13] = 2.0; // Bug beats Psychic
	typeChart[6][16] = 2.0; // Bug beats Dark
	typeChart[7][7] = 2.0;  // Ghost beats itself
	typeChart[7][13] = 2.0; // Ghost beats Psychic
	typeChart[8][5] = 2.0;  // Steel beats Rock
	typeChart[8][14] = 2.0; // Steel beats Ice
	typeChart[8][17] = 2.0; // Steel beats Psychic
	typeChart[9][6] = 2.0;  // Fire beats Bug
	typeChart[9][8] = 2.0;  // Fire beats Steel
	typeChart[9][11] = 2.0; // Fire beats Grass
	typeChart[9][14] = 2.0; // Fire beats Ice
	typeChart[10][4] = 2.0; // Water beats Ground
	typeChart[10][5] = 2.0; // Water beats Rock
	typeChart[10][9] = 2.0; // Water beats Fire
	typeChart[11][4] = 2.0; // Grass beats Ground
	typeChart[11][5] = 2.0; // Grass beats Rock
	typeChart[11][10] = 2.0;// Grass beats Water
	typeChart[12][2] = 2.0; // Electric beats Flying
	typeChart[12][10] = 2.0;// Electric beats Water
	typeChart[13][1] = 2.0; // Psychic beats Fighting
	typeChart[13][3] = 2.0; // Psychic beats Poison
	typeChart[14][2] = 2.0; // Ice beats Flying
	typeChart[14][4] = 2.0; // Ice beats Ground
	typeChart[14][11] = 2.0;// Ice beats Grass
	typeChart[14][15] = 2.0;// Ice beats Dragon
	typeChart[15][15] = 2.0;// Dragon beats only itself
	typeChart[16][7] = 2.0; // Dark beats Ghost
	typeChart[16][13] = 2.0;// Dark beats Psychic
	typeChart[17][1] = 2.0; // Fairy beats Fighting
	typeChart[17][15] = 2.0;// Fairy beats Dragon
	typeChart[17][16] = 2.0;// Fairy beats Dark

	// Type resistances
	typeChart[0][5] = 0.5;  // Rock resists Normal
	typeChart[0][8] = 0.5;  // Steel resists Normal
	typeChart[1][2] = 0.5;  // Flying resists Fighting
	typeChart[1][3] = 0.5;  // Poison resists Fighting
	typeChart[1][6] = 0.5;  // Bug resists Fighting
	typeChart[1][13] = 0.5; // Psychic resists Fighting
	typeChart[1][17] = 0.5; // Fairy resists Fighting
	typeChart[2][5] = 0.5;  // Rock resists Flying
	typeChart[2][8] = 0.5;  // Steel resists Flying
	typeChart[2][12] = 0.5; // Electric resists Flying
	typeChart[3][3] = 0.5;  // Poison resists itself
	typeChart[3][4] = 0.5;  // Ground resists Poison
	typeChart[3][5] = 0.5;  // Rock resists Poison
	typeChart[3][7] = 0.5;  // Ghost resists Poison
	typeChart[4][6] = 0.5;  // Bug resists Ground
	typeChart[4][11] = 0.5; // Grass resists Ground
	typeChart[5][1] = 0.5;  // Fighting resists Rock
	typeChart[5][4] = 0.5;  // Ground resists Rock
	typeChart[5][8] = 0.5;  // Steel resists Rock
	typeChart[6][1] = 0.5;  // Fighting resists Bug
	typeChart[6][2] = 0.5;  // Flying resists Bug
	typeChart[6][3] = 0.5;  // Poison resists Bug
	typeChart[6][7] = 0.5;  // Ghost resists Bug
	typeChart[6][8] = 0.5;  // Steel resists Bug
	typeChart[6][9] = 0.5;  // Fire resists Bug
	typeChart[6][17] = 0.5; // Fairy resists Bug
	typeChart[7][16] = 0.5; // Dark resists Ghost
	typeChart[8][8] = 0.5;  // Steel resists itself
	typeChart[8][9] = 0.5;  // Fire resists Steel
	typeChart[8][10] = 0.5; // Water resists Steel
	typeChart[8][12] = 0.5; // Electric resists Steel
	typeChart[9][5] = 0.5;  // Rock resists Fire
	typeChart[9][9] = 0.5;  // Fire resists itself
	typeChart[9][10] = 0.5; // Water resists Fire
	typeChart[9][15] = 0.5; // Dragon resists Fire
	typeChart[10][10] = 0.5;// Water resists itself
	typeChart[10][11] = 0.5;// Grass resists Water
	typeChart[10][15] = 0.5;// Dragon resists Water
	typeChart[11][2] = 0.5; // Flying resists Grass
	typeChart[11][3] = 0.5; // Poison resists Grass
	typeChart[11][6] = 0.5; // Bug resists Grass
	typeChart[11][8] = 0.5; // Steel resists Grass
	typeChart[11][9] = 0.5; // Fire resists Grass
	typeChart[11][11] = 0.5;// Grass resists itself
	typeChart[11][15] = 0.5;// Dragon resists Grass
	typeChart[12][11] = 0.5;// Grass resists Electric
	typeChart[12][12] = 0.5;// Electric resists itself
	typeChart[12][15] = 0.5;// Dragon resists Electric
	typeChart[13][8] = 0.5; // Steel resists Psychic
	typeChart[13][13] = 0.5;// Psychic resists itself
	typeChart[14][8] = 0.5; // Steel resists Ice
	typeChart[14][9] = 0.5; // Fire resists Ice
	typeChart[14][10] = 0.5;// Water resists Ice
	typeChart[14][14] = 0.5;// Ice resists itself
	typeChart[15][8] = 0.5; // Steel resists Dragon
	typeChart[16][1] = 0.5; // Fighting resists Dark
	typeChart[16][16] = 0.5;// Dark resists itself
	typeChart[16][17] = 0.5;// Fairy resists Dark
	typeChart[17][3] = 0.5; // Poison resists Fairy
	typeChart[17][8] = 0.5; // Steel resists Fairy
	typeChart[17][9] = 0.5; // Fire resists Fairy

	// Type immunities
	typeChart[0][7] = 0.0;  // Ghost is immune to Normal
	typeChart[1][7] = 0.0;  // Ghost is immune to Fighting
	typeChart[3][8] = 0.0;  // Steel is immune to Poison
	typeChart[4][2] = 0.0;  // Flying is immune to Ground
	typeChart[7][0] = 0.0;  // Normal is immune to Ghost
	typeChart[12][4] = 0.0; // Ground is immune to Electric
	typeChart[13][16] = 0.0;// Dark is immune to Psychic
	typeChart[15][17] = 0.0;// Fairy is immune to Dragon
}

void printLicense() {
	string dummy;
	cout << "MIT License" << endl << endl;
	cout << "Copyright (c) 2021 JuniorJedi490" << endl << endl;
	cout << (string("Permission is hereby granted, free of charge, to any person obtaining a copy of this software ") +
			"and associated documentation files (the \"Software\"), to deal in the Software without restriction, " +
			"including without limitation the rights to use, copy, modify, merge, publish, distribute, " +
			"sublicense, and/or sell copies of the Software, and to permit persons to whom the Software " +
			"is furnished to do so, subject to the following conditions:\n\nThe above copyright notice " +
			"and this permission notice shall be included in all copies or substantial portions of the Software.\n\n" +
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, " +
			"INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE " +
			"AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, " +
			"DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, " +
			"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.") << endl << endl;
	cout << "End license text\nType something, then press Enter to continue" << endl;
	cin >> dummy;
}
