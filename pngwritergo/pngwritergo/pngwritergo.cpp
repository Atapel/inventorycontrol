#include <iostream>
#include <fstream>
#include <string>     // std::string, std::stoi
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <pngwriter.h>


using namespace std;
void save();
void header();
vector<string> codes, bins;
vector<double> qtys, revs;
vector<int> boxs;
int boxint;
int countme;


bool c = false;
bool b = false;
bool bo = false;
int listsize, x;
char ch;
int id;
int amount;

void printBarcode(string, string, double, int);
int enterInt();
double enterDouble();
int yesNo();


template<typename T> void printElement(T t)
{
	cout << left << setw(19) << setfill(' ') << t;
}

int main() {
	system("CLS");
	ifstream fin("inv.csv");
	string last;
	int i = 0;
	if (fin.is_open()) {
		cout << "Reading data from inv.csv  ..." << endl;
		getline(fin, last);
		while (getline(fin, last)) {
			stringstream strstr(last);
			string word = "";
			while (getline(strstr, word, ',')) {
				switch (i) {
				case 0:
					codes.push_back(word);
					break;
				case 1:
					bins.push_back(word);
					break;
				case 2:
					revs.push_back(stod(word));
					break;
				case 3:
					boxs.push_back(stoi(word));
					break;
				case 4:
					qtys.push_back(stoi(word));
					break;
				}

				if (i == 4) {
					i = 0;
				}
				else {
					i++;
				}
			}
		}
		cout << "Success!" << endl;
		system("pause");
	}
	else {
		cout << "Failed to open inv.csv!" << endl;
		cout << "Creating blank file..." << endl;
		system("pause");
	}
	fin.close();




mainmenu:
	system("CLS");
	int menu;
	string code, bin;
	double rev, qty;
	int box;
	//load data


	cout << "Select a menu option:" << endl;
	cout << "1. Add New Inventory" << endl;
	cout << "2. Withdraw Inventory" << endl;
	cout << "3. View All Inventory" << endl;
	cout << "4. Search Inventory (selective), Print Labels" << endl;
	cout << "5. Search Inventory (all fields)" << endl;
	cout << "6. Search Box" << endl << flush;

restart:
	if (cin >> menu && menu > 0 && menu < 7){
		system("CLS");
	}
	else {
		cout << "Please enter a valid menu option" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		goto restart;
	}

	switch (menu) {

	case 1:
		cout << "******* Add New Inventory *******" << endl;
		cout << "Scan item code:" << endl;
		cin >> code;
		cout << "Scan item bin:" << endl;
		cin >> bin;
		cout << "Scan revision number:" << endl;
		rev = enterDouble();
		listsize = codes.size();
		for (int i = 0; i < listsize; i++) {
			if (code == codes[i] && bin == bins[i] && rev == revs[i]) {
				cout << "Matching item found! There are currently " << qtys[i] << " units available." << endl << "How many additional units to add?" << endl;
				cin >> x;
				qtys[i] = qtys[i] + x;
				cout << "There are now " << qtys[i] << " units available." << endl;
				save();
				goto mainmenu;
			}
		}
		cout << "No matching items found. Enter new box number" << endl;
		cout << "Enter box number:" << endl;
		cin >> box;
		cout << "Enter quantity on hand:" << endl;
		cin >> qty;
		codes.push_back(code);
		bins.push_back(bin);
		revs.push_back(rev);
		qtys.push_back(qty);
		boxs.push_back(box);
		save();
		printBarcode(code, bin, rev, box);

		goto mainmenu;

		break;

	case 2:

		cout << "******* Withdraw Inventory *******" << endl;
		cout << "Scan box:" << endl;
		cin >> boxint;
		cout << "Scan item code:" << endl;
		cin >> code;
		cout << "Scan item bin:" << endl;
		cin >> bin;
		cout << "Scan revision number:" << endl;
		cin >> rev;

		listsize = codes.size();
		int id, withdraw;
		for (int i = 0; i < listsize; i++) {
			if (code == codes[i]) {
				c = true;
				if (bin == bins[i]) {
					b = true;
					if (rev == revs[i]) {
						bo = true;
						if (boxint == boxs[i]) {
							cout << "There are currently " << qtys[i] << " units available." << endl;
							cout << "Enter amount to withdraw:" << endl;
							cin >> withdraw;
							if (qtys[i] - withdraw < 0) {
								cout << "Error! This will result in a negative quantity!" << endl;
								system("pause");
								goto mainmenu;
							}

							else if (qtys[i] - withdraw == 0) {
								qtys[i] = qtys[i] - withdraw;
								cout << "There are now zero units available." << endl;
								codes.erase(codes.begin() + i);
								bins.erase(bins.begin() + i);
								revs.erase(revs.begin() + i);
								qtys.erase(qtys.begin() + i);
								boxs.erase(boxs.begin() + i);
								save();
								system("pause");
								goto mainmenu;
							}
							else {
								qtys[i] = qtys[i] - withdraw;
								cout << "Success! There are now " << qtys[i] << " units available" << endl;
								save();
								system("pause");
								goto mainmenu;
							}
						}
					}
				}
			}
		}
		cout << "No inventory found matching all parameters." << endl;
		cout << "However, the following parameters had matching pair(s). Use the search tool to match all criteria." << endl;
		if (c)
			cout << "Item code:" << code << endl;
		if (b)
			cout << "Item bin:" << bin << endl;
		if (bo)
			cout << "Item rev:" << rev << endl;
		system("pause");
		goto mainmenu;
		break;



	case 3:
		cout << "******* View All Inventory *******" << endl;
		listsize = codes.size();
		cout << "Displaying " << listsize << " results." << endl;
		header();
		for (int i = 0; i < listsize; i++) {
			printElement(i);
			printElement(boxs[i]);
			printElement(codes[i]);
			printElement(bins[i]);
			printElement(revs[i]);
			printElement(qtys[i]);
			cout << endl;
		}

		system("PAUSE");
		goto mainmenu;


		break;

	case 4:{
		cout << "******* Search Inventory (detailed) and Print Labels*******" << endl;
		cout << "Scan item code:" << endl;
		cin >> code;
		x = 0;
		listsize = codes.size();
		for (int i = 0; i < listsize; i++) {
			if (codes[i] == code)
				x++;
		}
		cout << "Found " << x << " results. Display them or specify more criteria? (y=display/n=scan bin)";
		int x = yesNo();
		cout << x;
		if (x==1) {
			header();
			for (int i = 0; i < listsize; i++) {
				if (codes[i] == code) {
					printElement(i);
					printElement(boxs[i]);
					printElement(codes[i]);
					printElement(bins[i]);
					printElement(revs[i]);
					printElement(qtys[i]);
					cout << endl;
				}
				system("PAUSE");
				goto mainmenu;
			}
		}


		else if (x==0) {
			cout << "Scan item bin:" << endl;
			cin >> bin;
			countme = 0;
			for (int i = 0; i < listsize; i++) {
				if (codes[i] == code && bins[i] == bin)
					countme++;
			}
			cout << "Found " << countme << " results. Display them or specify more criteria? (y=display/n=scan bin)";
			
			if (ch == 'y') {
				header();
				for (int i = 0; i < listsize; i++) {
					if (codes[i] == code && bins[i] == bin) {
						printElement(i);
						printElement(boxs[i]);
						printElement(codes[i]);
						printElement(bins[i]);
						printElement(revs[i]);
						printElement(qtys[i]);
						cout << endl;
					}
				}
			}
			else if (ch == 'n') {
				cout << "Scan item rev:" << endl;
				cin >> rev;
				cout << "Displaying matching results:" << endl;
				header();
				for (int i = 0; i < listsize; i++) {
					if (codes[i] == code && bins[i] == bin && revs[i] == rev) {
						printElement(i);
						printElement(boxs[i]);
						printElement(codes[i]);
						printElement(bins[i]);
						printElement(revs[i]);
						printElement(qtys[i]);
						cout << endl;
					}
				}
			}
			else {
				cout << "Invalid entry..." << endl;
			}
		}


		else {
			goto mainmenu;
		}


		cout << "Would you like to print a label? (y/n)" << endl;
		cin >> ch;
		if (ch == 'y') {
			cout << "Select ID:" << endl;
			id = enterInt();
			printBarcode(codes[id], bins[id], revs[id], boxs[id]);
		}

		system("PAUSE");
		goto mainmenu;
		break;
	}

	case 5:
		amount = 0;
		cout << "******* Search Inventory *******" << endl;
		cout << "Scan item code:" << endl;
		cin >> code;
		cout << "Scan item bin:" << endl;
		cin >> bin;
		cout << "Scan revision number:" << endl;
		cin >> rev;
		listsize = codes.size();
		header();
		for (int i = 0; i < listsize; i++) {
			if (codes[i] == code && bins[i] == bin && revs[i] == rev) {
				printElement(i);
				printElement(boxs[i]);
				printElement(codes[i]);
				printElement(bins[i]);
				printElement(revs[i]);
				printElement(qtys[i]);
				amount++;
				cout << endl;
			}
			
		}
		cout << "Displaying " << amount << " results.";
		system("PAUSE");
		goto mainmenu;

	case 6:
		amount = 0;
		cout << "******* Search Inventory *******" << endl;
		cout << "Scan box:" << endl;
		cin >> box;
		listsize = codes.size();
		header();
		for (int i = 0; i < listsize; i++) {
			if (boxs[i] == box){
				printElement(i);
				printElement(boxs[i]);
				printElement(codes[i]);
				printElement(bins[i]);
				printElement(revs[i]);
				printElement(qtys[i]);
				amount++;
				cout << endl;
			}
		}
		cout << "Displaying " << amount << " results.";
		system("PAUSE");
		goto mainmenu;
	}
	


}


void save() {
	ofstream ofs("inv.csv", std::ofstream::out);
	if (ofs.is_open()) {
		ofs << "CODES, BINS, REVS, BOXS, QTYS" << endl;
		for (int i = 0; i < codes.size(); i++) {
			ofs << codes[i] << ',';
			ofs << bins[i] << ',';
			ofs << revs[i] << ',';
			ofs << boxs[i] << ',';
			ofs << qtys[i] << endl;
		}
	}
	else {
		cout << "ERROR! Couldn't save data!" << endl;
		system("PAUSE");
		goto fail;
	}
	cout << "Item successfully saved." << endl;
fail:
	ofs.close();
};

void header() {
	printElement("ID:");
	printElement("BOX:");
	printElement("ITEM");
	printElement("BIN");
	printElement("REV");
	printElement("QUANTITY");
	cout << endl;
}


int enterInt() {
	int x;
	if (cin >> x && x > 0) {
		return x;
	}
	else {
		cout << "Please enter a valid menu option" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return -1;
	}
}

double enterDouble() {
	double x;
	if (cin >> x && x > 0) {
		return x;
	}
	else {
		cout << "Please enter a valid menu option" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return -1;
	}
}


int yesNo() {
	char choice;
	if (cin >> choice) {
		if (tolower(choice) == 'y') {
			return 1;
		}
		else if(tolower(choice == 'n')){
			return 0;
		}

		else {
			cout << "Invalid input..." << endl;
			system("PAUSE");
			return -1;
		}
	}
	else {
		cout << "Invalid input..." << endl;
		system("PAUSE");
		return -1;
	}
};




void printBarcode(string codem, string binm, double revm, int boxm)
{
	char barcode[] = "barcode.ttf";
	char arial[] = "arial.ttf";
	double gradstart = 0.0;
	double gradend = 0.3;

	//Creating the PNGwriter instance
	pngwriter image(400, 700, 1.0, "out.png");

	char code[100];
	char bin[100];
	char rev[10];
	char box[10];

	string temp;
	string revms, boxms;

	codem.insert(0, "*");
	codem.push_back('*');
	binm.insert(0, "*");
	binm.push_back('*');
	
	stringstream ss;
	ss << revm; //need to add the * * to front and back of box/rev
	revms = ss.str();
	revms.insert(0, "*");
	revms.push_back('*');
	temp = "REV:" + ss.str();
	char revtext[20];
	strcpy_s(revtext, temp.c_str());
	ss.str("");

	ss << boxm;
	boxms = ss.str();
	boxms.insert(0, "*");
	boxms.push_back('*');
	temp = "BOX:" + ss.str();
	char boxtext[20];
	strcpy_s(boxtext, temp.c_str());

	/*
	PADDING WITH 0's
	stringstream ss;
	ss << "*" << setfill('0') << setw(4) << revm << "*";
	revm = ss.str();

	ss.str("");
	ss << "*" << setfill('0') << setw(4) << boxm << "*";
	boxm = ss.str();
	*/

	strcpy_s(code, codem.c_str());
	strcpy_s(bin, binm.c_str());
	strcpy_s(rev, revms.c_str());
	strcpy_s(box, boxms.c_str());

	//This writes the code in the uppper left corner.
	system("PAUSE");
	image.plot_text(barcode, 40, 10, 350, 0.0, code, 0.0, 0.0, 0.0);
	temp = "CODE:" + codem;
	char codetext[20];
	strcpy_s(codetext, temp.c_str());
	image.plot_text(arial, 40, 25, 460, 0.0, codetext, 0.0, 0.0, 0.0);

	//This writes the bin in the middle.
	image.plot_text(barcode, 40, 10, 180, 0.0, bin, 0.0, 0.0, 0.0);
	temp = "BIN:" + binm;
	char bintext[20];
	strcpy_s(bintext, temp.c_str());
	image.plot_text(arial, 40, 25, 290, 0.0, bintext, 0.0, 0.0, 0.0);

	
	//This writes the rev in the lower left corner.
	image.plot_text(barcode, 40, 10, 10, 0.0, rev, 0.0, 0.0, 0.0);
	image.plot_text(arial, 40, 25, 120, 0.0, revtext, 0.0, 0.0, 0.0);
	
	image.plot_text(barcode, 40, 10, 520, 0.0, box, 0.0, 0.0, 0.0);
	image.plot_text(arial, 40, 25, 630, 0.0, boxtext, 0.0, 0.0, 0.0);
	
	image.close();
	system("out.png");
}