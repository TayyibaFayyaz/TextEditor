#include<iostream>
#include"TextEditor.h"
#include<conio.h>
using namespace std;

int mains()
{
	int c;
	while (true)
	{
		c = _getch();
		cout << c << endl << endl;
	}
}

int main()
{
	cout << "1. Open an Existing File." << endl;
	cout << "2. Create a New File." << endl;
	int option;
	string Fname;
	cin >> option;
	cin.ignore();
	TextEditor TD;
	switch (option)
	{
	case 1:
		cout << "Enter Filename: ";
		getline(cin, Fname);
		system("Color F0");
		TD.LoadFirst(Fname);
		TD.Editing();
		break;
	case 2:
		TD.CreateNewDocument();
		system("Color F0");
		TD.Editing();
		break;
	default:
		break;
	}
	
	return 0;
	
}