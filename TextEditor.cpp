#include "TextEditor.h"
#include<conio.h>
#include<iostream>
#include<windows.h>
#include<string>
#include<fstream>
#include<iomanip>
using namespace std;

void gotoRowCol(int rpos, int cpos)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	int xpos = cpos, ypos = rpos;
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos;
	scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void Line::ToLower(string& Word)
{
	for (int i = 0; i < Word.size(); i++)
	{
		if (IsUpper(Word.at(i)))
		{
			Word.at(i) += 32;
		}
	}
}

void Line::ToLower(char*& C)
{
	int i = 0;
	while (C[i] != '\0')
	{
		if (IsUpper(C[i]))
		{
			C[i] += 32;
		}
		i++;
	}
}

Line::Line()
{
	Cs = new char[1]{};
	size = 0;
}

void Line::Clear()
{
	delete[] Cs;
	size = 0;
}

Line::~Line()
{
	Clear();
}

Line::Line(const Line& A)
{
	DeepCopy(A);
}

void Line::DeepCopy(const Line& A)
{
	Cs = new char[A.size + 1];
	size = A.size;
	for (int i = 0; i <= size; i++)
	{
		Cs[i] = A.Cs[i];
	}
}

void Line::operator=(const Line& A)
{
	Clear();
	DeepCopy(A);
}

void Line::PrintFromTo(int si, int ei)
{
	for (int i = si; i <= ei; i++)
	{
		cout << Cs[i];
	}
}

void Line::InsertCharAt(int i, char ch)
{
	char* HA = new char[size + 2];
	for (int ci = 0; ci < i; ci++)
	{
		HA[ci] = Cs[ci];
	}
	HA[i] = ch;
	for (int ci = i + 1; ci <= size + 1; ci++)
	{
		HA[ci] = Cs[ci - 1];
	}
	delete[]Cs;
	Cs = HA;
	size++;
}

void Line::DeleteACharAt(int i)
{
	char* HA = new char[size];
	for (int ci = 0, hi = 0; ci <= size; ci++, hi++)
	{
		if (ci == i)
		{
			ci++;
		}
		HA[hi] = Cs[ci];
	}
	delete[] Cs;
	Cs = HA;
	size--;
}

void Line::DeleteWordRightAt(int i)
{
	int wordsize = GiveWordLenghtFromRightSide(i);
	char* Vs = new char[size - wordsize + 1];
	int vi = 0;
	for (int j = 0; j <= size; j++)
	{
		if (j >= i && j < i + wordsize)
		{
			continue;
		}
		else
		{
			Vs[vi] = Cs[j];
			vi++;
		}
	}
	delete[]Cs;
	Cs = Vs;
	size -= wordsize;
}

int Line::DeleteWordLeftAt(int i)
{
	int wordsize = GiveWordLenghtFromLeftSide(i);
	char* Vs = new char[size - wordsize + 1];
	int vi = size - wordsize;
	for (int j = size; j >= 0; j--)
	{
		if (j <= i && j > i - wordsize)
		{
			continue;
		}
		else
		{
			Vs[vi] = Cs[j];
			vi--;
		}
	}
	delete[]Cs;
	Cs = Vs;
	size -= wordsize;
	return wordsize;
}

int Line::GiveWordLenghtFromRightSide(int i)
{
	int wordsize = 0;
	for (int j = i; j < size; j++)
	{
		wordsize++;
		if (Cs[j] == 32 && Cs[j + 1] != 32)
		{
			break;
		}
	}
	return wordsize;
}

int Line::GiveWordLenghtFromLeftSide(int i)
{
	int wordsize = 0;
	bool LF = false;
	for (int j = i; j >= 0; j--)
	{
		if (Cs[j] != 32)
		{
			LF = true;
		}
		if (Cs[j] == 32 && j != i && LF)
		{
			break;
		}
		wordsize++;
	}
	return wordsize;
}

bool Line::IsCharacter(char c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
	{
		return true;
	}
	return false;
}

bool Line::IsWCharacter(char c)
{
	if (IsCharacter(c) || c == '-' || c == '\'')
	{
		return true;
	}
	return false;
}

bool Line::IsSCharacter(char c)
{
	if (!((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57) || c == ' '))
	{
		return true;
	}
	return false;
}

bool Line::IsLower(char c)
{
	if (c >= 97 && c <= 122)
	{
		return true;
	}
	return false;
}

bool Line::IsUpper(char c)
{
	if (c >= 65 && c <= 90)
	{
		return true;
	}
	return false;
}

void Line::Addresses()
{
	cout << "Cs (" << (void*)&Cs << ") :: \"" << Cs << "\" is Pointing Towards: " << (void*)&Cs << endl;
	cout << "Size (" << &size << ") ::" << size << endl;
	for (int i = 0; i <= size; i++)
	{
		cout << "\tCs[" << i << "] :: (" << (void*)&Cs[i] << ") " << Cs[i] << endl;
	}
}

bool Line::SelectWord(int i, int& si, int& ei)
{
	si = 0;
	ei = 0;
	if (!IsCharacter(Cs[i]))
	{
		return false;
	}
	if (!IsCharacter(Cs[i + 1]) && !IsCharacter(Cs[i - 1]))
	{
		si = i; ei = i;
		return true;
	}
	for (int j = i; j >= 0; j--)
	{
		if (!IsWCharacter(Cs[j - 1]) && IsWCharacter(Cs[j]))
		{
			si = j;
			break;
		}
	}
	for (int k = i; k <= size; k++)
	{
		if (!IsWCharacter(Cs[k + 1]) && IsWCharacter(Cs[k]))
		{
			ei = k;
			return true;
		}
	}
}

void Line::TOUPPER(int si, int ei)
{
	for (int i = si; i <= ei; i++)
	{
		if (IsLower(Cs[i]))
		{
			Cs[i] -= 32;
		}
	}
	return;
}

void Line::TOLOWER(int si, int ei)
{
	for (int i = si; i <= ei; i++)
	{
		if (IsUpper(Cs[i]))
		{
			Cs[i] += 32;
		}
	}
	return;
}

Document::Document(string Fname)
{
	NOLs = 0;
	Load(Fname);
}

Document::Document()
{
	NOLs = 0;
	DName = {};
	Ls = {};
}

void Document::Load(string Fname)
{
	DName = Fname;
	char ch;
	ifstream Rdr(DName);
	bool IsPass;
	Rdr >> IsPass;
	Protected = IsPass;
	string temp;
	if (IsPass)
	{
		if (PassWord(Rdr))
		{
			PassOk = true;
			while (!Rdr.eof())
			{
				ch = Rdr.get();
				if (ch == '\n' || Rdr.eof())
				{
					NOLs++;
				}
			}
			Rdr.close();
			if (NOLs == 0)
			{
				NOLs = 1;
			}
			Rdr.open(DName);
			Ls = new Line[NOLs];
			int ri = 0, ci = 0;
			getline(Rdr,temp);
			while (!Rdr.eof())
			{
				ch = Rdr.get();
				if (ch == '\n' || Rdr.eof())
				{
					ri++;
					ci = 0;
				}
				else
				{
					InsertACharacter(ri, ci, ch - 2);
					ci++;
				}
			}
		}
		else
		{
			PassOk = false;
			NOLs = 1;
			Ls = new Line[NOLs];
			string Message = "Invalid PassWord Entered! You Can't See the Original Document. Any Changes Will Not Be Saved!";
			Ls[0].Cs = new char[Message.size() + 1];
			for (int i = 0; i < Message.size(); i++)
			{
				InsertACharacter(0, i, Message.at(i));
			}
			Ls[0].Cs[Message.size()] = '\0';
		}
		return;
	}
	else
	{
		PassOk = true;
		ch = Rdr.get();
		Code = {};
		while (!Rdr.eof())
		{
			ch = Rdr.get();
			if (ch == '\n' || Rdr.eof())
			{
				NOLs++;
			}
		}
		Rdr.close();
		if (NOLs == 0)
		{
			NOLs = 1;
		}
		Rdr.open(DName);
		Ls = new Line[NOLs];
		int ri = 0, ci = 0;
		getline(Rdr, temp);
		while (!Rdr.eof())
		{
			ch = Rdr.get();
			if (ch == '\n' || Rdr.eof())
			{
				ri++;
				ci = 0;
			}
			else
			{
				InsertACharacter(ri, ci, ch);
				ci++;
			}
		}
	}
	
	
}

Document::~Document()
{
	delete[] Ls;
	Ls = nullptr;
}

void Document::Addresses()
{
	cout << "Ls (" << &Ls << ") ::" << Ls << endl;
	cout << "NOLs (" << &NOLs << ") ::" << NOLs << endl << endl << endl;
	for (int i = 0; i < NOLs; i++)
	{
		cout << "Ls[" << i << "] ::" << endl << endl;
		Ls[i].Addresses();
		cout << endl << endl;
	}
}

void Document::InsertACharacter(int li, int ci, char ch)
{
	Ls[li].InsertCharAt(ci, ch);
}

void Document::InsertANewLineAt(int li, int ci)
{
	Line* HLs = new Line[NOLs + 1];
	for (int i = 0, hi = 0; i < NOLs; i++, hi++)
	{
		if (i == li)
		{
			for (int j = 0, si = 0; j < ci; j++, si++)
			{
				HLs[hi].InsertCharAt(si, Ls[i].Cs[j]);
			}
			hi++;
			for (int j = ci, si = 0; j < Ls[i].size; j++, si++)
			{
				HLs[hi].InsertCharAt(si, Ls[i].Cs[j]);
			}
		}
		else
		{
			HLs[hi] = Ls[i];
		}
	}
	delete[] Ls;
	NOLs++;
	Ls = HLs;
}

void Document::MergeTwoLinesAt(int i)
{
	Line* HLs = new Line[NOLs - 1];
	for (int j = 0, hi = 0; j < NOLs; j++, hi++)
	{
		if (j == i)
		{
			HLs[hi] = Ls[j];
			j++;
			for (int k = 0; k < Ls[j].size; k++)
			{
				HLs[hi].InsertCharAt(HLs[hi].size, Ls[j].Cs[k]);
			}
		}
		else
		{
			HLs[hi] = Ls[j];
		}
	}
	delete[] Ls;
	Ls = HLs;
	NOLs--;
}

void Document::DeleteACharAt(int li, int ci)
{
	Ls[li].DeleteACharAt(ci);
}

void Document::DeleteWordRightAt(int li, int ci)
{
	Ls[li].DeleteWordRightAt(ci);
}

int Document::DeleteWordLeftAt(int li, int ci)
{
	return Ls[li].DeleteWordLeftAt(ci);
}

int Document::GiveWordLenghtFromRightSide(int li, int ci)
{
	return Ls[li].GiveWordLenghtFromRightSide(ci);
}

int Document::GiveWordLenghtFromLeftSide(int li, int ci)
{
	return Ls[li].GiveWordLenghtFromLeftSide(ci);
}

void Document::ListOfFunctions()
{
	system("cls");
	gotoRowCol(0, 0);
	cout << setw(20) << left << "Ctrl + L" << "\tTo Show List of Functions." << endl;
	cout << setw(20) << left << "Ctrl + S, Ctrl + S" << "\tTo Save The Current File." << endl;
	cout << setw(20) << left << "Ctrl + Alt + O" << "\tTo Open New File." << endl;
	cout << setw(20) << left << "Ctrl + Alt + C" << "\tTo Create New File." << endl;
	cout << setw(20) << left << "Ctrl + Alt + S" << "\tTo Show Current Document Stats." << endl;
	cout << setw(20) << left << "Ctrl + X" << "\tTo Close the Current File." << endl;
	cout << setw(20) << left << "Escape Key" << "\tTo Close the Editor." << endl;
	cout << setw(20) << left << "Ctrl + U" << "\tTo Convert the Current Word To Upper Case." << endl;
	cout << setw(20) << left << "Ctrl + O" << "\tTo Convert the Current Word To Lower Case." << endl;
	cout << endl;
	cout << setw(20) << left << "Ctrl + T" << "\tTo To Find A Word, Sentence, Substring." << endl;
	cout << "\t" << setw(20) << left << "Ctrl + S" << "\tTo Find A Sentence." << endl;
	cout << "\t" << setw(20) << left << "Ctrl + W" << "\tTo Find A Word." << endl;
	cout << "\t" << setw(20) << left << "Ctrl + B" << "\tTo Find A SubString." << endl;
	cout << "\t\t" << setw(20) << left << "Ctrl + A" << "\tTo Select All Found." << endl;
	cout << "\t\t" << setw(20) << left << "Ctrl + R" << "\tTo Select Current Found." << endl;
	cout << "\t\t" << setw(20) << left << "Ctrl + N" << "\tTo Select Next Found." << endl;
	cout << "\t\t" << setw(20) << left << "Ctrl + P" << "\tTo Select Previous Found." << endl;
	cout << "\t\t" << setw(20) << left << "Ctrl + Alt + N" << "\tTo Select Next All Found." << endl;
	cout << "\t\t" << setw(20) << left << "Ctrl + Alt + P" << "\tTo Select Previous All Found." << endl;
	cout << "\t\t\t" << setw(20) << left << "Ctrl + R" << "\tTo Replace." << endl;
	cout << "\t\t\t" << setw(20) << left << "Ctrl + P" << "\tTo Add Insert At Start/Prefix." << endl;
	cout << "\t\t\t" << setw(20) << left << "Ctrl + S" << "\tTo Add Insert At End/Suffix(Postfix)." << endl;
	cout << "\t\t\t\t" << setw(20) << left << "Ctrl + C" << "\tTo To Toggle Case Sensitivity." << endl;
	cout << endl;
}

void Document::FindMenu()
{
	system("cls");
	gotoRowCol(0, 0);
	cout << setw(20) << left << "Ctrl + S" << "\tTo Find A Sentence." << endl;
	cout << setw(20) << left << "Ctrl + W" << "\tTo Find A Word." << endl;
	cout << setw(20) << left << "Ctrl + B" << "\tTo Find A SubString." << endl;
	cout << endl << "Press Any Other Key To Cancel." << endl;
}

void Document::FindCaseMenu()
{
	system("cls");
	gotoRowCol(1, 0);
	cout << setw(20) << left << "Ctrl + C" << "\tFor Case Sensitive." << endl;
	cout << setw(20) << left << "Ctrl + N" << "\tFor Case In-Sensitive." << endl;
}

void Document::FindKeys()
{
	cout << "Ctrl + R\tTo Replace" << endl;
	cout << "Ctrl + S\tTo Insert At End/Suffix" << endl;
	cout << "Ctrl + P\tTo Insert At Start/Prefix" << endl;
}

void Document::ExitMenu()
{
	system("cls");
	gotoRowCol(0, 0);
	cout << "The File \"" << DName << "\" is not Saved after Changes::" << endl;
	cout << setw(20) << left << "Ctrl + S" << "\tTo Save and Close." << endl;
	cout << setw(20) << left << "Ctrl + X" << "\tTo Close Without Saving." << endl;
	cout << endl << "Press Any Other Key To Cancel." << endl;
}

void Document::TOUPPER(int li, int ci)
{
	int si, ei;
	bool CursorOnWord = Ls[li].SelectWord(ci, si, ei);
	if (CursorOnWord)
	{
		Ls[li].TOUPPER(si, ei);
		PrintFromTo(li, si, ei);
		gotoRowCol(li, ci);
	}
	
}

void Document::TOLOWER(int li, int ci)
{
	int si, ei;
	bool CursorOnWord = Ls[li].SelectWord(ci, si, ei);
	if (CursorOnWord)
	{
		Ls[li].TOLOWER(si, ei);
		PrintFromTo(li, si, ei);
		gotoRowCol(li, ci);
	}
}

void Document::SaveDocument()
{
	if (PassOk)
	{
		ofstream Wtr(DName);
		
		if (Protected)
		{
			string encode = Code;
			for (int i = 0; i < encode.size(); i++)
			{
				encode.at(i) = encode.at(i) + 3;
			}
			Wtr << Protected << encode << endl;
			char ch;
			for (int i = 0; i < NOLs; i++)
			{
				for (int j = 0; j < Ls[i].size; j++)
				{
					ch = (Ls[i].Cs[j] + 2);
					Wtr << ch;
				}
				if (i != NOLs - 1)
					Wtr << endl;
			}
		}
		else
		{
			Wtr << Protected << Code << endl;
			for (int i = 0; i < NOLs; i++)
			{
				Wtr << Ls[i].Cs;
				if (i != NOLs - 1)
					Wtr << endl;
			}
		}
	}
	else
	{
		gotoRowCol(NOLs + 2, 0);
		cout << "File Not Save. Illegal Operation. Password was not Correctly Entered." << endl;
	}
}

void Document::PrintDocument(int Cfi, int TD)
{
	system("cls");
	gotoRowCol(0, 0);
	for (int i = 0; i < NOLs; i++)
	{
		cout << Ls[i].Cs << endl;
	}
	gotoRowCol(NOLs + 3, 0);
	cout << "On Document ::" << setw(4) << right << Cfi + 1 << " of " << setw(4) << left << TD << endl;
}

void Document::PrintFromTo(int li, int si, int ei)
{
	gotoRowCol(li, si);
	Ls[li].PrintFromTo(si, ei);
}

void Document::FindWord(int& cr, int& cc, string word, int& d, bool& CaseSen)
{
	int si, ei;
	int count = 0;
	Line L;
	if (!CaseSen)
	{
		Line::ToLower(word);
	}
	for (int i = 0; i < NOLs; i++)//Finds Count
	{
		if (!CaseSen)
		{
			L.DeepCopy(Ls[i]);
			Line::ToLower(L.Cs);
		}
		else
		{
			L.DeepCopy(Ls[i]);
		}
		for (int j = 0; j < L.size; j++)
		{
			if (L.SelectWord(j, si, ei))
			{
				if (ei - si + 1 == word.size())
				{
					int temp = 0;
					for (int k = 0; k < word.size(); k++)
					{
						if (L.Cs[si + k] == word.at(k))
						{
							temp++;
						}
						else
						{
							break;
						}
					}
					if (temp == word.size())
					{
						count++;
						j = ei + 1;
					}
				}
				else
				{
					j = ei + 1;
					continue;
				}
			}
		}
	}
	pos* p = new pos[count];
	int pi = 0;
	for (int i = 0; i < NOLs; i++)//Stores Starting and Ending Indexes of All Words
	{
		if (!CaseSen)
		{
			L.DeepCopy(Ls[i]);
			Line::ToLower(L.Cs);
		}
		else
		{
			L.DeepCopy(Ls[i]);
		}
		for (int j = 0; j < L.size; j++)
		{
			if (L.SelectWord(j, si, ei))
			{
				if (ei - si + 1 == word.size())
				{
					int temp = 0;
					for (int k = 0; k < word.size(); k++)
					{
						if (L.Cs[si + k] == word.at(k))
						{
							temp++;
						}
						else
						{
							break;
						}
					}
					if (temp == word.size())
					{
						p[pi].li = i;
						p[pi].si = si;
						p[pi].ei = ei;
						pi++;
						j = ei + 1;
					}
				}
				else
				{
					continue;
				}
			}
		}
	}
	system("cls");
	gotoRowCol(0, 0);
	pi = 0;
	for (int i = 0; i < NOLs; i++)//Prints the Document With Highlighted Words
	{
		for (int j = 0; j < Ls[i].size; j++)
		{
			if (p[pi].li == i && j >= p[pi].si && j <= p[pi].ei)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xB4);
				cout << Ls[i].Cs[j];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF0);
				if (j == p[pi].ei)
				{
					pi++;
				}
			}
			else
			{
				cout << Ls[i].Cs[j];
			}
		}
		cout << endl;
	}
	for (pi = 0; pi < count; pi++)//Selects cc,cr to next found word starting index from previous cc,cr
	{
		if (cr <= p[0].li && cc <= p[0].si)
		{
			cr = p[0].li;
			cc = p[0].si;
			break;
		}
		else if (p[pi - 1].li < cr && p[pi].li >= cr && ((cr == p[pi].li) ? p[pi].si >= cc : 1) && pi > 0)
		{
			cr = p[pi].li;
			cc = p[pi].si;
			break;
		}
		else if (pi == count - 1)
		{
			cr = p[pi].li;
			cc = p[pi].si;
			break;
		}
	}
	gotoRowCol(NOLs + 2, 0);
	cout << "On " << setw(3) << right << ((count==0)? 0 :pi + 1) << " of " << setw(3) << left << count << endl;
	cout << "Case Sensitivity: " << ((CaseSen) ? "On" : "Off") << endl;
	cout << "String Entered: \"" << word << "\"";
	gotoRowCol(cr, cc);
	while (true)//Main Loop
	{
		if (_kbhit)
		{
			d = _getch();
			if (d == 224)
			{
				d = _getch();
				if (d == 77/*Right Arrow*/)
				{
					if (count != 0)
					{
						if (pi < count - 1)
							pi++;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << ((count == 0) ? 0 : pi + 1) << " of " << setw(3) << left << count << endl;
					}
				}
				else if (d == 75/*Left Arrow*/)
				{
					if (count != 0)
					{
						if (pi > 0)
							pi--;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << pi + 1 << " of " << setw(3) << left << count << endl;
					}
				}
				else if (d == 116/*Ctrl + Right Arrow*/)
				{
					if (count != 0)
					{
						pi = count - 1;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << pi + 1 << " of " << setw(3) << left << count << endl;

					}
				}
				else if (d == 115/*Ctrl + Left Arrow*/)
				{
					if (count != 0)
					{
						pi = 0;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << pi + 1 << " of " << setw(3) << left << count << endl;
					}
				}
			}
			else if (d == 1/*Ctrl + A Select All*/)
			{
				gotoRowCol(NOLs + 5, 0);
				cout << "Selected :: All" << endl;
				FindKeys();
				d = _getch();
				if (d == 18 /*Ctrl + R Replace With Word*/)
				{
					system("cls");
					cout << "Enter Word To Replace with::";
					string replace;
					cin >> replace;
					for (int k = 0; k < count; k++)
					{
						for (int i = p[k].si; i <= p[k].ei; i++)
						{
							Ls[p[k].li].DeleteACharAt(p[k].si);
						}
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size() - word.size();
								p[y].ei += replace.size() - word.size();
							}
							else
							{
								break;
							}
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
				else if (d == 16/*Ctrl + P Prefix*/)
				{
					system("cls");
					cout << "Enter Prefix for the word::";
					string replace;
					cin >> replace;
					for (int k = 0; k < count; k++)
					{
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
				else if (d == 19/*Ctrl + S Suffix (Postfix)*/)
				{
					system("cls");
					cout << "Enter Suffix for the word::";
					string replace;
					cin >> replace;
					for (int k = 0; k < count; k++)
					{
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].ei + i + 1, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
			}
			else if (d == 18/*Ctrl + R Select Current*/)
			{
			gotoRowCol(NOLs + 5, 0);
			cout << "Selected :: Current" << endl;
			FindKeys();
				d = _getch();
				if (d == 18 /*Ctrl + R Replace With Word*/)
				{
					system("cls");
					cout << "Enter Word To Replace with::";
					string replace;
					cin >> replace;
					for (int i = p[pi].si; i <= p[pi].ei; i++)
					{
						Ls[p[pi].li].DeleteACharAt(p[pi].si);
					}
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi].li].InsertCharAt(p[pi].si + i, replace.at(i));
					}
					for (int y = pi + 1; y < count; y++)
					{
						if (p[pi].li == p[y].li)
						{
							p[y].si += replace.size() - word.size();
							p[y].ei += replace.size() - word.size();
						}
						else
						{
							break;
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
				else if (d == 16/*Ctrl + P Prefix*/)
				{
					system("cls");
					cout << "Enter Prefix for the word::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi].li].InsertCharAt(p[pi].si + i, replace.at(i));
					}
					for (int y = pi + 1; y < count; y++)
					{
						if (p[pi].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
				else if (d == 19/*Ctrl + S Suffix(Postfix)*/)
				{
					system("cls");
					cout << "Enter Suffix for the word::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi].li].InsertCharAt(p[pi].ei + i + 1, replace.at(i));
					}
					for (int y = pi + 1; y < count; y++)
					{
						if (p[pi].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
			}
			else if (d == 14/*Ctrl + N Select Next*/)
			{
			gotoRowCol(NOLs + 5, 0);
			cout << "Selected :: Next" << endl;
			FindKeys();
			d = _getch();
				if (pi < count - 1 && pi >= 0)
				{
					if (d == 18 /*Ctrl + R Replace With Word*/)
				{

					system("cls");
					cout << "Enter Word To Replace with::";
					string replace;
					cin >> replace;
					for (int i = p[pi + 1].si; i <= p[pi + 1].ei; i++)
					{
						Ls[p[pi + 1].li].DeleteACharAt(p[pi + 1].si);
					}
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi + 1].li].InsertCharAt(p[pi + 1].si + i, replace.at(i));
					}
					for (int y = pi + 2; y < count; y++)
					{
						if (p[pi + 1].li == p[y].li)
						{
							p[y].si += replace.size() - word.size();
							p[y].ei += replace.size() - word.size();
						}
						else
						{
							break;
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
					else if (d == 16/*Ctrl + P Prefix*/)
				{
					system("cls");
					cout << "Enter Prefix for the word::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi + 1].li].InsertCharAt(p[pi + 1].si + i, replace.at(i));
					}
					for (int y = pi + 2; y < count; y++)
					{
						if (p[pi + 1].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
					else if (d == 19/*Ctrl + S Suffix(Postfix)*/)
				{
					system("cls");
					cout << "Enter Suffix for the word::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi + 1].li].InsertCharAt(p[pi + 1].ei + i + 1, replace.at(i));
					}
					for (int y = pi + 2; y < count; y++)
					{
						if (p[pi + 1].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
				}
			}
			else if (d == 16/*Ctrl + P Select Previous*/)
			{
			gotoRowCol(NOLs + 5, 0);
			cout << "Selected :: Previous" << endl;
			FindKeys();
			d = _getch();
				if (pi > 0 && pi <= count - 1)
				{
					if (d == 18 /*Ctrl + R Replace With Word*/)
				{

					system("cls");
					cout << "Enter Word To Replace with::";
					string replace;
					cin >> replace;
					for (int i = p[pi - 1].si; i <= p[pi - 1].ei; i++)
					{
						Ls[p[pi - 1].li].DeleteACharAt(p[pi - 1].si);
					}
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi - 1].li].InsertCharAt(p[pi - 1].si + i, replace.at(i));
					}
					for (int y = pi; y < count; y++)
					{
						if (p[pi - 1].li == p[y].li)
						{
							p[y].si += replace.size() - word.size();
							p[y].ei += replace.size() - word.size();
						}
						else
						{
							break;
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
					else if (d == 16/*Ctrl + P Prefix*/)
				{
					system("cls");
					cout << "Enter Prefix for the word::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi - 1].li].InsertCharAt(p[pi - 1].si + i, replace.at(i));
					}
					for (int y = pi ; y < count; y++)
					{
						if (p[pi - 1].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
					else if (d == 19/*Ctrl + S Suffix(Postfix)*/)
				{
					system("cls");
					cout << "Enter Suffix for the word::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi - 1].li].InsertCharAt(p[pi - 1].ei + i + 1, replace.at(i));
					}
					for (int y = pi; y < count; y++)
					{
						if (p[pi - 1].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
				}
			}
			else if (d == 0/*Ctrl + Alt*/)
			{
			d = _getch();
			if (d == 49 /*Ctrl + Alt + N Select Next All*/)
			{
				gotoRowCol(NOLs + 5, 0);
				cout << "Selected :: Next All" << endl;
				FindKeys();
				d = _getch();
				if (d == 18 /*Ctrl + R Replace With Word*/)
				{
					system("cls");
					cout << "Enter Word To Replace with::";
					string replace;
					cin >> replace;
					for (int k = pi + 1; k < count; k++)
					{
						for (int i = p[k].si; i <= p[k].ei; i++)
						{
							Ls[p[k].li].DeleteACharAt(p[k].si);
						}
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size() - word.size();
								p[y].ei += replace.size() - word.size();
							}
							else
							{
								break;
							}
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
				else if (d == 16/*Ctrl + P Prefix*/)
				{
					system("cls");
					cout << "Enter Prefix for the word::";
					string replace;
					cin >> replace;
					for (int k = pi + 1; k < count; k++)
					{
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
				else if (d == 19/*Ctrl + S Suffix (Postfix)*/)
				{
					system("cls");
					cout << "Enter Suffix for the word::";
					string replace;
					cin >> replace;
					for (int k = pi + 1; k < count; k++)
					{
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].ei + i + 1, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
			}
			else if (d == 25/*Ctrl + Alt + P Select Previous All*/)
			{
				gotoRowCol(NOLs + 5, 0);
				cout << "Selected :: Previous All" << endl;
				FindKeys();
				d = _getch();
				if (d == 18 /*Ctrl + R Replace With Word*/)
				{
					system("cls");
					cout << "Enter Word To Replace with::";
					string replace;
					cin >> replace;
					for (int k = 0; k < pi; k++)
					{
						for (int i = p[k].si; i <= p[k].ei; i++)
						{
							Ls[p[k].li].DeleteACharAt(p[k].si);
						}
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size() - word.size();
								p[y].ei += replace.size() - word.size();
							}
							else
							{
								break;
							}
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
				else if (d == 16/*Ctrl + P Prefix*/)
				{
					system("cls");
					cout << "Enter Prefix for the word::";
					string replace;
					cin >> replace;
					for (int k = 0; k < pi; k++)
					{
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
				else if (d == 19/*Ctrl + S Suffix (Postfix)*/)
				{
					system("cls");
					cout << "Enter Suffix for the word::";
					string replace;
					cin >> replace;
					for (int k = 0; k < pi; k++)
					{
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].ei + i + 1, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
					}
					FindWord(cr, cc, word, d, CaseSen);
				}
			}
			}
			else if (d == 3 /*Ctrl + C Change CaseSen*/)
			{
				CaseSen = !CaseSen;
				FindWord(cr, cc, word, d, CaseSen);
			}
			if (d == 24)/*Ctrl + X To Return From Nested Conditions*/
			{
				return;
			}
			gotoRowCol(cr, cc);
		}
	}
}

void Document::FindSubString(int& cr, int& cc, string word, int& d, bool& CaseSen)
{
	int si, ei;
	int count = 0;
	Line L;
	if (!CaseSen)
	{
		Line::ToLower(word);
	}
	for (int i = 0; i < NOLs; i++)//Finds Count
	{
		if (!CaseSen)
		{
			L.DeepCopy(Ls[i]);
			Line::ToLower(L.Cs);
		}
		else
		{
			L.DeepCopy(Ls[i]);
		}
		for (int j = 0; j < L.size; j++)
		{
			int temp = 0;
			for (int k = 0; k < word.size(); k++)
			{
				if (L.Cs[j + k] == word.at(k))
				{
					temp++;
				}
				else
				{
					break;
				}
			}
			if (temp == word.size())
			{
				count++;
			}
		}
	}
	pos* p = new pos[count];
	int pi = 0;
	for (int i = 0; i < NOLs; i++)//Stores Starting and Ending Indexes of All SubString
	{
		if (!CaseSen)
		{
			L.DeepCopy(Ls[i]);
			Line::ToLower(L.Cs);
		}
		else
		{
			L.DeepCopy(Ls[i]);
		}
		for (int j = 0; j < L.size; j++)
		{
			int temp = 0;
			for (int k = 0; k < word.size(); k++)
			{
				if (L.Cs[j + k] == word.at(k))
				{
					temp++;
				}
				else
				{
					break;
				}
			}
			if (temp == word.size())
			{
				p[pi].li = i;
				p[pi].si = j;
				p[pi].ei = j + word.size() - 1;
				pi++;
			}
		}
	}
	system("cls");
	gotoRowCol(0, 0);
	pi = 0;
	for (int i = 0; i < NOLs; i++)//Prints the Document With Highlighted Words
	{
		for (int j = 0; j < Ls[i].size; j++)
		{
			if (p[pi].li == i && j >= p[pi].si && j <= p[pi].ei)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xB4);
				cout << Ls[i].Cs[j];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF0);
				if (j == p[pi].ei)
				{
					pi++;
				}
			}
			else
			{
				cout << Ls[i].Cs[j];
			}
		}
		cout << endl;
	}
	for (pi = 0; pi < count; pi++)//Selects cc,cr to next found word starting index from previous cc,cr
	{
		if (cr <= p[0].li && cc <= p[0].si)
		{
			cr = p[0].li;
			cc = p[0].si;
			break;
		}
		else if (p[pi - 1].li < cr && p[pi].li >= cr && ((cr == p[pi].li) ? p[pi].si >= cc : 1) && pi > 0)
		{
			cr = p[pi].li;
			cc = p[pi].si;
			break;
		}
		else if (pi == count - 1)
		{
			cr = p[pi].li;
			cc = p[pi].si;
			break;
		}
	}
	gotoRowCol(NOLs + 2, 0);
	cout << "On " << setw(3) << right << ((count == 0) ? 0 : pi + 1) << " of " << setw(3) << left << count << endl;
	cout << "Case Sensitivity: " << ((CaseSen) ? "On" : "Off") << endl;
	cout << "String Entered: \"" << word << "\"";
	gotoRowCol(cr, cc);
	while (true)//Main Loop
	{
		if (_kbhit)
		{
			d = _getch();
			if (d == 224)
			{
				d = _getch();
				if (d == 77/*Right Arrow*/)
				{
					if (count != 0)
					{
						if (pi < count - 1)
							pi++;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << ((count == 0) ? 0 : pi + 1) << " of " << setw(3) << left << count << endl;
					}
				}
				else if (d == 75/*Left Arrow*/)
				{
					if (count != 0)
					{
						if (pi > 0)
							pi--;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << pi + 1 << " of " << setw(3) << left << count << endl;
					}
				}
				else if (d == 116/*Ctrl + Right Arrow*/)
				{
					if (count != 0)
					{
						pi = count - 1;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << pi + 1 << " of " << setw(3) << left << count << endl;

					}
				}
				else if (d == 115/*Ctrl + Left Arrow*/)
				{
					if (count != 0)
					{
						pi = 0;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << pi + 1 << " of " << setw(3) << left << count << endl;
					}
				}
			}
			else if (d == 1/*Ctrl + A Select All*/)
			{
				gotoRowCol(NOLs + 5, 0);
				cout << "Selected :: All" << endl;
				FindKeys();
				d = _getch();
				if (d == 18 /*Ctrl + R Replace With Word*/)
				{
					system("cls");
					cout << "Enter Word To Replace All SubStrings with::";
					string replace;
					cin >> replace;
					for (int k = 0; k < count; k++)
					{
						for (int i = p[k].si; i <= p[k].ei; i++)
						{
							Ls[p[k].li].DeleteACharAt(p[k].si);
						}
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size() - word.size();
								p[y].ei += replace.size() - word.size();
							}
							else
							{
								break;
							}
						}
					}
					FindSubString(cr, cc, word, d, CaseSen);
				}
				else if (d == 16/*Ctrl + P Insert at start of Substring*/)
				{
					system("cls");
					cout << "Enter what to Insert at Start of All Substrings::";
					string replace;
					cin >> replace;
					for (int k = 0; k < count; k++)
					{
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
					}
					FindSubString(cr, cc, word, d, CaseSen);
				}
				else if (d == 19/*Ctrl + S Insert at end of Substring)*/)
				{
					system("cls");
					cout << "Enter what to Insert at End of All Substrings::";
					string replace;
					cin >> replace;
					for (int k = 0; k < count; k++)
					{
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].ei + i + 1, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
					}
					FindSubString(cr, cc, word, d, CaseSen);
				}
			}
			else if (d == 18/*Ctrl + R Select Current*/)
			{
			gotoRowCol(NOLs + 5, 0);
			cout << "Selected :: Current" << endl;
			FindKeys();
				d = _getch();
				if (d == 18 /*Ctrl + R Replace With Word*/)
				{
					system("cls");
					cout << "Enter Word To Replace Sub String with::";
					string replace;
					cin >> replace;
					for (int i = p[pi].si; i <= p[pi].ei; i++)
					{
						Ls[p[pi].li].DeleteACharAt(p[pi].si);
					}
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi].li].InsertCharAt(p[pi].si + i, replace.at(i));
					}
					for (int y = pi + 1; y < count; y++)
					{
						if (p[pi].li == p[y].li)
						{
							p[y].si += replace.size() - word.size();
							p[y].ei += replace.size() - word.size();
						}
						else
						{
							break;
						}
					}
					FindSubString(cr, cc, word, d, CaseSen);
				}
				else if (d == 16/*Ctrl + P Insert at start of Substring*/)
				{
					system("cls");
					cout << "Enter what to Insert at Start of Substring::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi].li].InsertCharAt(p[pi].si + i, replace.at(i));
					}
					for (int y = pi + 1; y < count; y++)
					{
						if (p[pi].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindSubString(cr, cc, word, d, CaseSen);
				}
				else if (d == 19/*Ctrl + S Insert at end of Substring)*/)
				{
					system("cls");
					cout << "Enter what to Insert at End of Substring::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi].li].InsertCharAt(p[pi].ei + i + 1, replace.at(i));
					}
					for (int y = pi + 1; y < count; y++)
					{
						if (p[pi].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindSubString(cr, cc, word, d, CaseSen);
				}
			}
			else if (d == 14/*Ctrl + N Select Next*/)
			{
			gotoRowCol(NOLs + 5, 0);
			cout << "Selected :: Next" << endl;
			FindKeys();
				d = _getch();
				if (pi < count - 1 && pi >= 0)
				{
					if (d == 18 /*Ctrl + R Replace With Word*/)
					{
						system("cls");
						cout << "Enter Word To Replace Sub String with::";
						string replace;
						cin >> replace;
						for (int i = p[pi + 1].si; i <= p[pi + 1].ei; i++)
						{
							Ls[p[pi + 1].li].DeleteACharAt(p[pi + 1].si);
						}
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[pi + 1].li].InsertCharAt(p[pi + 1].si + i, replace.at(i));
						}
						for (int y = pi + 2; y < count; y++)
						{
							if (p[pi + 1].li == p[y].li)
							{
								p[y].si += replace.size() - word.size();
								p[y].ei += replace.size() - word.size();
							}
							else
							{
								break;
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
					else if (d == 16/*Ctrl + P Insert at start of Substring*/)
					{
						system("cls");
						cout << "Enter what to Insert at Start of Substring::";
						string replace;
						cin >> replace;
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[pi + 1].li].InsertCharAt(p[pi + 1].si + i, replace.at(i));
						}
						for (int y = pi + 2; y < count; y++)
						{
							if (p[pi + 1].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
					else if (d == 19/*Ctrl + S Insert at end of Substring)*/)
					{
						system("cls");
						cout << "Enter what to Insert at End of Substring::";
						string replace;
						cin >> replace;
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[pi + 1].li].InsertCharAt(p[pi + 1].ei + i + 1, replace.at(i));
						}
						for (int y = pi + 2; y < count; y++)
						{
							if (p[pi + 1].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
				}
			}
			else if (d == 16/*Ctrl + P Select Previous*/)
			{
			gotoRowCol(NOLs + 5, 0);
			cout << "Selected :: Previous" << endl;
			FindKeys();
				d = _getch();
				if (pi > 0 && pi <= count - 1)
				{
					if (d == 18 /*Ctrl + R Replace With Word*/)
					{
						system("cls");
						cout << "Enter Word To Replace Sub String with::";
						string replace;
						cin >> replace;
						for (int i = p[pi - 1].si; i <= p[pi - 1].ei; i++)
						{
							Ls[p[pi - 1].li].DeleteACharAt(p[pi - 1].si);
						}
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[pi - 1].li].InsertCharAt(p[pi - 1].si + i, replace.at(i));
						}
						for (int y = pi; y < count; y++)
						{
							if (p[pi - 1].li == p[y].li)
							{
								p[y].si += replace.size() - word.size();
								p[y].ei += replace.size() - word.size();
							}
							else
							{
								break;
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
					else if (d == 16/*Ctrl + P Insert at start of Substring*/)
					{
						system("cls");
						cout << "Enter what to Insert at Start of Substring::";
						string replace;
						cin >> replace;
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[pi - 1].li].InsertCharAt(p[pi - 1].si + i, replace.at(i));
						}
						for (int y = pi; y < count; y++)
						{
							if (p[pi - 1].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
					else if (d == 19/*Ctrl + S Insert at end of Substring)*/)
					{
						system("cls");
						cout << "Enter what to Insert at End of Substring::";
						string replace;
						cin >> replace;
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[pi - 1].li].InsertCharAt(p[pi - 1].ei + i + 1, replace.at(i));
						}
						for (int y = pi; y < count; y++)
						{
							if (p[pi - 1].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
				}
			}
			else if (d == 0/*Ctrl + Alt*/)
			{
				d = _getch();
				if (d == 49 /*Ctrl + Alt + N Select Next All*/)
				{
					gotoRowCol(NOLs + 5, 0);
					cout << "Selected :: Next All" << endl;
					FindKeys();
					d = _getch();
					if (d == 18 /*Ctrl + R Replace With Word*/)
					{
						system("cls");
						cout << "Enter Word To Replace All SubStrings with::";
						string replace;
						cin >> replace;
						for (int k = pi + 1; k < count; k++)
						{
							for (int i = p[k].si; i <= p[k].ei; i++)
							{
								Ls[p[k].li].DeleteACharAt(p[k].si);
							}
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size() - word.size();
									p[y].ei += replace.size() - word.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
					else if (d == 16/*Ctrl + P Insert at start of Substring*/)
					{
						system("cls");
						cout << "Enter what to Insert at Start of All Substrings::";
						string replace;
						cin >> replace;
						for (int k = pi + 1; k < count; k++)
						{
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size();
									p[y].ei += replace.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
					else if (d == 19/*Ctrl + S Insert at end of Substring)*/)
					{
						system("cls");
						cout << "Enter what to Insert at End of All Substrings::";
						string replace;
						cin >> replace;
						for (int k = pi + 1; k < count; k++)
						{
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].ei + i + 1, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size();
									p[y].ei += replace.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
				}
				else if (d == 25/*Ctrl + Alt + P Select Previous All*/)
				{
					gotoRowCol(NOLs + 5, 0);
					cout << "Selected :: Previous All" << endl;
					FindKeys();
					d = _getch();
					if (d == 18 /*Ctrl + R Replace With Word*/)
					{
						system("cls");
						cout << "Enter Word To Replace All SubStrings with::";
						string replace;
						cin >> replace;
						for (int k = 0; k < pi; k++)
						{
							for (int i = p[k].si; i <= p[k].ei; i++)
							{
								Ls[p[k].li].DeleteACharAt(p[k].si);
							}
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size() - word.size();
									p[y].ei += replace.size() - word.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
					else if (d == 16/*Ctrl + P Insert at start of Substring*/)
					{
						system("cls");
						cout << "Enter what to Insert at Start of All Substrings::";
						string replace;
						cin >> replace;
						for (int k = 0; k < pi; k++)
						{
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size();
									p[y].ei += replace.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
					else if (d == 19/*Ctrl + S Insert at end of Substring)*/)
					{
						system("cls");
						cout << "Enter what to Insert at End of All Substrings::";
						string replace;
						cin >> replace;
						for (int k = 0; k < pi; k++)
						{
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].ei + i + 1, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size();
									p[y].ei += replace.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSubString(cr, cc, word, d, CaseSen);
					}
				}
			}
			else if (d == 3 /*Ctrl + C Change CaseSen*/)
			{
				CaseSen = !CaseSen;
				FindSubString(cr, cc, word, d, CaseSen);
			}
			if (d == 24)/*Ctrl + X To Return From Nested Conditions*/
			{
				return;
			}
			gotoRowCol(cr, cc);
		}
	}
}

void Document::FindSentence(int& cr, int& cc, string word, int& d, bool& CaseSen)
{
	int si, ei;
	int count = 0;
	Line L;
	if (!CaseSen)
	{
		Line::ToLower(word);
	}
	for (int i = 0; i < NOLs; i++)//Finds Count
	{
		if (!CaseSen)
		{
			L.DeepCopy(Ls[i]);
			Line::ToLower(L.Cs);
		}
		else
		{
			L.DeepCopy(Ls[i]);
		}
		for (int j = 0; j < L.size; j++)
		{
			int temp = 0;
			for (int k = 0; k < word.size(); k++)
			{
				if (L.Cs[j + k] == word.at(k))
				{
					temp++;
				}
				else
				{
					break;
				}
			}
			if (temp == word.size())
			{
				count++;
			}
		}
	}
	pos* p = new pos[count];
	int pi = 0;
	for (int i = 0; i < NOLs; i++)//Stores Starting and Ending Indexes of All SubString
	{
		if (!CaseSen)
		{
			L.DeepCopy(Ls[i]);
			Line::ToLower(L.Cs);
		}
		else
		{
			L.DeepCopy(Ls[i]);
		}
		for (int j = 0; j < L.size; j++)
		{
			int temp = 0;
			for (int k = 0; k < word.size(); k++)
			{
				if (L.Cs[j + k] == word.at(k))
				{
					temp++;
				}
				else
				{
					break;
				}
			}
			if (temp == word.size())
			{
				p[pi].li = i;
				p[pi].si = j;
				p[pi].ei = j + word.size() - 1;
				pi++;
			}
		}
	}
	system("cls");
	gotoRowCol(0, 0);
	pi = 0;
	for (int i = 0; i < NOLs; i++)//Prints the Document With Highlighted Words
	{
		for (int j = 0; j < Ls[i].size; j++)
		{
			if (p[pi].li == i && j >= p[pi].si && j <= p[pi].ei)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xB4);
				cout << Ls[i].Cs[j];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF0);
				if (j == p[pi].ei)
				{
					pi++;
				}
			}
			else
			{
				cout << Ls[i].Cs[j];
			}
		}
		cout << endl;
	}
	for (pi = 0; pi < count; pi++)//Selects cc,cr to next found word starting index from previous cc,cr
	{
		if (cr <= p[0].li && cc <= p[0].si)
		{
			cr = p[0].li;
			cc = p[0].si;
			break;
		}
		else if (p[pi - 1].li < cr && p[pi].li >= cr && ((cr == p[pi].li) ? p[pi].si >= cc : 1) && pi > 0)
		{
			cr = p[pi].li;
			cc = p[pi].si;
			break;
		}
		else if (pi == count - 1)
		{
			cr = p[pi].li;
			cc = p[pi].si;
			break;
		}
	}
	gotoRowCol(NOLs + 2, 0);
	cout << "On " << setw(3) << right << ((count == 0) ? 0 : pi + 1) << " of " << setw(3) << left << count << endl;
	cout << "Case Sensitivity: " << ((CaseSen) ? "On" : "Off") << endl;
	cout << "String Entered: \"" << word << "\"";
	gotoRowCol(cr, cc);
	while (true)//Main Loop
	{
		if (_kbhit)
		{
			d = _getch();
			if (d == 224)
			{
				d = _getch();
				if (d == 77/*Right Arrow*/)
				{
					if (count != 0)
					{
						if (pi < count - 1)
							pi++;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << ((count == 0) ? 0 : pi + 1) << " of " << setw(3) << left << count << endl;
					}
				}
				else if (d == 75/*Left Arrow*/)
				{
					if (count != 0)
					{
						if (pi > 0)
							pi--;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << pi + 1 << " of " << setw(3) << left << count << endl;
					}
				}
				else if (d == 116/*Ctrl + Right Arrow*/)
				{
					if (count != 0)
					{
						pi = count - 1;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << pi + 1 << " of " << setw(3) << left << count << endl;

					}
				}
				else if (d == 115/*Ctrl + Left Arrow*/)
				{
					if (count != 0)
					{
						pi = 0;
						cr = p[pi].li;
						cc = p[pi].si;
						gotoRowCol(NOLs + 2, 0);
						cout << "On " << setw(3) << right << pi + 1 << " of " << setw(3) << left << count << endl;
					}
				}
			}
			else if (d == 1/*Ctrl + A Select All*/)
			{
				gotoRowCol(NOLs + 5, 0);
				cout << "Selected :: All" << endl;
				FindKeys();
				d = _getch();
				if (d == 18 /*Ctrl + R Replace With Word*/)
				{
					system("cls");
					cout << "Enter Word To Replace All Sentences with::";
					string replace;
					cin >> replace;
					for (int k = 0; k < count; k++)
					{
						for (int i = p[k].si; i <= p[k].ei; i++)
						{
							Ls[p[k].li].DeleteACharAt(p[k].si);
						}
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size() - word.size();
								p[y].ei += replace.size() - word.size();
							}
							else
							{
								break;
							}
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
				else if (d == 16/*Ctrl + P Insert at start of Sentences*/)
				{
					system("cls");
					cout << "Enter what to Insert at Start of All Sentences::";
					string replace;
					cin >> replace;
					for (int k = 0; k < count; k++)
					{
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
				else if (d == 19/*Ctrl + S Insert at end of Substring)*/)
				{
					system("cls");
					cout << "Enter what to Insert at End of All Sentences::";
					string replace;
					cin >> replace;
					for (int k = 0; k < count; k++)
					{
						for (int i = 0; i < replace.size(); i++)
						{
							Ls[p[k].li].InsertCharAt(p[k].ei + i + 1, replace.at(i));
						}
						for (int y = k + 1; y < count; y++)
						{
							if (p[k].li == p[y].li)
							{
								p[y].si += replace.size();
								p[y].ei += replace.size();
							}
							else
							{
								break;
							}
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
			}
			else if (d == 18/*Ctrl + R Select Current*/)
			{
			gotoRowCol(NOLs + 5, 0);
			cout << "Selected :: Current" << endl;
			FindKeys();
				d = _getch();
				if (d == 18 /*Ctrl + R Replace With Word*/)
				{
					system("cls");
					cout << "Enter Word To Replace Sub String with::";
					string replace;
					cin >> replace;
					for (int i = p[pi].si; i <= p[pi].ei; i++)
					{
						Ls[p[pi].li].DeleteACharAt(p[pi].si);
					}
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi].li].InsertCharAt(p[pi].si + i, replace.at(i));
					}
					for (int y = pi + 1; y < count; y++)
					{
						if (p[pi].li == p[y].li)
						{
							p[y].si += replace.size() - word.size();
							p[y].ei += replace.size() - word.size();
						}
						else
						{
							break;
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
				else if (d == 16/*Ctrl + P Insert at start of Sentences*/)
				{
					system("cls");
					cout << "Enter what to Insert at Start of Substring::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi].li].InsertCharAt(p[pi].si + i, replace.at(i));
					}
					for (int y = pi + 1; y < count; y++)
					{
						if (p[pi].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
				else if (d == 19/*Ctrl + S Insert at end of Sentences)*/)
				{
					system("cls");
					cout << "Enter what to Insert at End of Substring::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi].li].InsertCharAt(p[pi].ei + i + 1, replace.at(i));
					}
					for (int y = pi + 1; y < count; y++)
					{
						if (p[pi].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
			}
			else if (d == 14/*Ctrl + N Select Next*/)
			{
			gotoRowCol(NOLs + 5, 0);
			cout << "Selected :: Next" << endl;
			FindKeys();
			d = _getch();
			if (pi < count - 1 && pi >= 0)
			{
				if (d == 18 /*Ctrl + R Replace With Word*/)
				{
					system("cls");
					cout << "Enter Word To Replace Sub String with::";
					string replace;
					cin >> replace;
					for (int i = p[pi+1].si; i <= p[pi+1].ei; i++)
					{
						Ls[p[pi+1].li].DeleteACharAt(p[pi+1].si);
					}
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi+1].li].InsertCharAt(p[pi+1].si + i, replace.at(i));
					}
					for (int y = pi + 2; y < count; y++)
					{
						if (p[pi+1].li == p[y].li)
						{
							p[y].si += replace.size() - word.size();
							p[y].ei += replace.size() - word.size();
						}
						else
						{
							break;
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
				else if (d == 16/*Ctrl + P Insert at start of Sentences*/)
				{
					system("cls");
					cout << "Enter what to Insert at Start of Substring::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi+1].li].InsertCharAt(p[pi+1].si + i, replace.at(i));
					}
					for (int y = pi + 2; y < count; y++)
					{
						if (p[pi+1].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
				else if (d == 19/*Ctrl + S Insert at end of Sentences)*/)
				{
					system("cls");
					cout << "Enter what to Insert at End of Substring::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi+1].li].InsertCharAt(p[pi+1].ei + i + 1, replace.at(i));
					}
					for (int y = pi + 2; y < count; y++)
					{
						if (p[pi+1].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
			}
			}
			else if (d == 16/*Ctrl + P Select Previous*/)
			{
			gotoRowCol(NOLs + 5, 0);
			cout << "Selected :: Previous" << endl;
			FindKeys();
			d = _getch();
			if (pi > 0 && pi <= count - 1)
			{
				if (d == 18 /*Ctrl + R Replace With Word*/)
				{
					system("cls");
					cout << "Enter Word To Replace Sub String with::";
					string replace;
					cin >> replace;
					for (int i = p[pi - 1].si; i <= p[pi - 1].ei; i++)
					{
						Ls[p[pi - 1].li].DeleteACharAt(p[pi - 1].si);
					}
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi - 1].li].InsertCharAt(p[pi - 1].si + i, replace.at(i));
					}
					for (int y = pi; y < count; y++)
					{
						if (p[pi - 1].li == p[y].li)
						{
							p[y].si += replace.size() - word.size();
							p[y].ei += replace.size() - word.size();
						}
						else
						{
							break;
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
				else if (d == 16/*Ctrl + P Insert at start of Sentences*/)
				{
					system("cls");
					cout << "Enter what to Insert at Start of Substring::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi - 1].li].InsertCharAt(p[pi - 1].si + i, replace.at(i));
					}
					for (int y = pi; y < count; y++)
					{
						if (p[pi - 1].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
				else if (d == 19/*Ctrl + S Insert at end of Sentences)*/)
				{
					system("cls");
					cout << "Enter what to Insert at End of Substring::";
					string replace;
					cin >> replace;
					for (int i = 0; i < replace.size(); i++)
					{
						Ls[p[pi - 1].li].InsertCharAt(p[pi - 1].ei + i + 1, replace.at(i));
					}
					for (int y = pi + 2; y < count; y++)
					{
						if (p[pi - 1].li == p[y].li)
						{
							p[y].si += replace.size();
							p[y].ei += replace.size();
						}
						else
						{
							break;
						}
					}
					FindSentence(cr, cc, word, d, CaseSen);
				}
			}
			}
			else if (d == 0/*Ctrl + Alt*/)
			{
				d = _getch();
				if (d == 49 /*Ctrl + Alt + N Select Next All*/)
				{
					gotoRowCol(NOLs + 5, 0);
					cout << "Selected :: Next All" << endl;
					FindKeys();
					d = _getch();
					if (d == 18 /*Ctrl + R Replace With Word*/)
					{
						system("cls");
						cout << "Enter Word To Replace All Sentences with::";
						string replace;
						cin >> replace;
						for (int k = pi + 1; k < count; k++)
						{
							for (int i = p[k].si; i <= p[k].ei; i++)
							{
								Ls[p[k].li].DeleteACharAt(p[k].si);
							}
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size() - word.size();
									p[y].ei += replace.size() - word.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSentence(cr, cc, word, d, CaseSen);
					}
					else if (d == 16/*Ctrl + P Insert at start of Sentences*/)
					{
						system("cls");
						cout << "Enter what to Insert at Start of All Sentences::";
						string replace;
						cin >> replace;
						for (int k = pi + 1; k < count; k++)
						{
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size();
									p[y].ei += replace.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSentence(cr, cc, word, d, CaseSen);
					}
					else if (d == 19/*Ctrl + S Insert at end of Substring)*/)
					{
						system("cls");
						cout << "Enter what to Insert at End of All Sentences::";
						string replace;
						cin >> replace;
						for (int k = pi + 1; k < count; k++)
						{
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].ei + i + 1, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size();
									p[y].ei += replace.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSentence(cr, cc, word, d, CaseSen);
					}
				}
				else if (d == 25/*Ctrl + Alt + P Select Previous All*/)
				{
					gotoRowCol(NOLs + 5, 0);
					cout << "Selected :: Previous All" << endl;
					FindKeys();
					d = _getch();
					if (d == 18 /*Ctrl + R Replace With Word*/)
					{
						system("cls");
						cout << "Enter Word To Replace All Sentences with::";
						string replace;
						cin >> replace;
						for (int k = 0; k < pi; k++)
						{
							for (int i = p[k].si; i <= p[k].ei; i++)
							{
								Ls[p[k].li].DeleteACharAt(p[k].si);
							}
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size() - word.size();
									p[y].ei += replace.size() - word.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSentence(cr, cc, word, d, CaseSen);
					}
					else if (d == 16/*Ctrl + P Insert at start of Sentences*/)
					{
						system("cls");
						cout << "Enter what to Insert at Start of All Sentences::";
						string replace;
						cin >> replace;
						for (int k = 0; k < pi; k++)
						{
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].si + i, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size();
									p[y].ei += replace.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSentence(cr, cc, word, d, CaseSen);
					}
					else if (d == 19/*Ctrl + S Insert at end of Substring)*/)
					{
						system("cls");
						cout << "Enter what to Insert at End of All Sentences::";
						string replace;
						cin >> replace;
						for (int k = 0; k < pi; k++)
						{
							for (int i = 0; i < replace.size(); i++)
							{
								Ls[p[k].li].InsertCharAt(p[k].ei + i + 1, replace.at(i));
							}
							for (int y = k + 1; y < count; y++)
							{
								if (p[k].li == p[y].li)
								{
									p[y].si += replace.size();
									p[y].ei += replace.size();
								}
								else
								{
									break;
								}
							}
						}
						FindSentence(cr, cc, word, d, CaseSen);
					}
				}
			}
			else if (d == 3 /*Ctrl + C Change CaseSen*/)
			{
				CaseSen = !CaseSen;
				FindSubString(cr, cc, word, d, CaseSen);
			}
			if (d == 24)/*Ctrl + X To Return From Nested Conditions*/
			{
				return;
			}
			gotoRowCol(cr, cc);
		}
	}
}

void Document::FindWordSentenceSubString(int& cr, int& cc)
{
	int d, h;
	FindMenu();
	d = _getch();
	bool CaseSen;
	if (d == 19)//Ctrl + S (SENTENCE)
	{
		FindCaseMenu();
		gotoRowCol(0, 0);
		cout << "Finding::Sentence";
		h = _getch();
		if (h == 3)//Ctrl + C (CASE SENSITIVE)
		{
			CaseSen = true;
		}
		else if (h == 14)//Ctrl + N (CASE INSENSITIVE)
		{
			CaseSen = false;
		}
		else
		{
			CaseSen = false;
		}
		string word;
		gotoRowCol(8, 0);
		cout << "Case Sensitive: " << ((CaseSen) ? "On" : "Off") << endl;
		gotoRowCol(6, 0);
		cin.clear();
		cout << "Enter the Sentence: ";
		getline(cin, word);
		FindSentence(cr, cc, word, d, CaseSen);
	}
	else if (d == 23)//Ctrl + W (WORD)
	{
		FindCaseMenu();
		gotoRowCol(0, 0);
		cout << "Finding::Word";
		h = _getch();
		if (h == 3)//Ctrl + C (CASE SENSITIVE)
		{
			CaseSen = true;
		}
		else if (h == 14)//Ctrl + N (CASE INSENSITIVE)
		{
			CaseSen = false;
		}
		else
		{
			CaseSen = false;
		}
		string word;
		gotoRowCol(8, 0);
		cout << "Case Sensitive: " << ((CaseSen) ? "On" : "Off") << endl;
		cout << "Exact Words will be found." << endl;
		cout << "If more than one word entered, First word would be considered.";
		gotoRowCol(6, 0);
		cin.clear();
		cout << "Enter the Word: ";
		cin >> word;
		FindWord(cr, cc, word, d, CaseSen);
	}
	else if (d == 2)//Ctrl + B (SUB STRING)
	{
		FindCaseMenu();
		gotoRowCol(0, 0);
		cout << "Finding::SubString";
		h = _getch();
		if (h == 3)//Ctrl + C (CASE SENSITIVE)
		{
			CaseSen = true;
		}
		else if (h == 14)//Ctrl + N (CASE INSENSITIVE)
		{
			CaseSen = false;
		}
		else
		{
			CaseSen = false;
		}
		string word;
		gotoRowCol(8, 0);
		cout << "Case Sensitive: " << ((CaseSen) ? "On" : "Off") << endl;
		cout << "Exact Substring would be found." << endl;
		cout << "Substring will be considered till first Space or Enter.";
		gotoRowCol(6, 0);
		cin.clear();
		cout << "Enter the Substring: ";
		cin >> word;
		FindSubString(cr, cc, word, d, CaseSen);
	}
}

int Document::CountSpecialChar()
{
	int count = 0;
	for (int i = 0; i < NOLs; i++)
	{
		for (int j = 0; j < Ls[i].size; j++)
		{
			if (Line::IsSCharacter(Ls[i].Cs[j]))
			{
				count++;
			}
		}
	}
	return count;
}

int Document::SentencesCount()
{
	int count = 0;
	for (int i = 0; i < NOLs; i++)
	{
		for (int j = 0; j < Ls[i].size ; j++)
		{
			if (Ls[i].Cs[j] == '.')
			{
				count++;
			}
		}
		count++;
		if (Ls[i].Cs[Ls[i].size - 1] == '.')
		{
			count--;
		}
		if (Ls[i].Cs[0] == '.')
		{
			count--;
		}
	}
	return count;
}

int Document::WordCount()
{
	int wordcount = 0;
	int si = 0, ei = 0;
	for (int i = 0; i < NOLs; i++)
	{
		for (int j = 0; j < Ls[i].size; j++)
		{
			if (Ls[i].SelectWord(j, si, ei))
			{
				wordcount++;
				j = ei + 1;
			}
		}
	}
	return wordcount;
}

float Document::AverageWordLenght(pos*& p,int*& WordSizes)
{
	float WordC = WordCount();
	int si = 0, ei = 0;
	int temp = 0;
	for (int i = 0; i < NOLs; i++)
	{
		for (int j = 0; j < Ls[i].size; j++)
		{
			if (Ls[i].SelectWord(j,si,ei))
			{
				p[temp].li = i;
				p[temp].si = si;
				p[temp].ei = ei;
				WordSizes[temp] = p[temp].ei - p[temp].si + 1;
				temp++;
				j = ei + 1;
			}
		}
	}
	int TotalSizes = 0;
	for (int i = 0; i < WordCount(); i++)
	{
		TotalSizes += WordSizes[i];
	}
	float AvgLen = TotalSizes / WordC;
	return AvgLen;
}

int Document::SmallestWordLen(int*& WordSizes)
{
	int SML = INT_MAX;
	for (int i = 0; i < WordCount(); i++)
	{
		if (SML > WordSizes[i])
		{
			SML = WordSizes[i];
		}
	}
	return SML;
}

int Document::LargestWordLen(int*& WordSizes)
{
	int LML = 0;
	for (int i = 0; i < WordCount(); i++)
	{
		if (LML < WordSizes[i])
		{
			LML = WordSizes[i];
		}
	}
	return LML;
}

int Document::FindNumberOfSubWords(int*& SubWords, pos*& p, int WordCount)
{
	SubWords = new int[WordCount] {};
	char* CWord;
	int CWSize = 0;
	int si = 0, ei = 0;
	for (int a = 0; a < WordCount; a++)
	{
		//Assigns CWord word by word from document
		CWSize = p[a].ei - p[a].si + 1;
		CWord = new char[CWSize + 1];
		for (int k = p[a].si, c = 0; k <= p[a].ei; k++, c++)
		{
			CWord[c] = Ls[p[a].li].Cs[k];
		}
		CWord[CWSize] = '\0';
		//Assigns CWord word by word from document
		//Finds Number of Subwords in CWord from document
		for (int i = 0; i < NOLs; i++)
		{
			for (int j = 0; j < Ls[i].size; j++)
			{
				if (Ls[i].SelectWord(j, si, ei))
				{
					bool Found = false;
					for (int r = si; r <= ei; r++)
					{
						Found = false;
						for (int q = 0; q <= CWSize; q++)
						{
							if (Ls[i].Cs[r] == CWord[q])
							{
								Found = true;
							}
							if (Found)
							{
								break;
							}
						}
						if (!Found)
						{
							break;
						}
					}
					if (Found)
					{
						SubWords[a]++;
					}
					j = ei + 1;
				}
			}
		}

	}
	int Highest = 0;
	int HInd;
	for (int i = 0; i < WordCount; i++)
	{
		if (SubWords[i]>Highest)
		{
			Highest = SubWords[i];
			HInd = i;
		}
	}
	return HInd;
}

int Document::ParagraphCount()
{
	int Paracount = 1;
	for (int i = 0; i < NOLs; i++)
	{
		if ((Ls[i].Cs[0] == ' ' || Ls[i].Cs[0] == '\t') and i > 0)
		{
			Paracount++;
		}

	}
	return Paracount;
}

void Document::ShowStats()
{
	system("cls");
	cout << "Following are the stats of \"" << DName << "\" File." << endl << endl;
	int SpecialCharCount = CountSpecialChar();
	cout << "Special Characters Count :: " << SpecialCharCount << endl;
	int NOSentences = SentencesCount();
	cout << "Number of Sentences :: " << NOSentences << endl;
	int WordC = WordCount();
	int* WordSizes = new int[WordC];
	cout << "WordCount :: " << WordC << endl;
	pos* p = new pos[WordC];
	float AvgWordLen = AverageWordLenght(p, WordSizes);
	cout << "Average Word Lenght :: " << AvgWordLen << endl;
	int SmolWLen = SmallestWordLen(WordSizes);
	cout << "Smallest Word Size :: " << SmolWLen << endl;
	int LargeWLen = LargestWordLen(WordSizes);
	cout << "Largest Word Size :: " << LargeWLen << endl;
	////---------------------------------
	int* WordSubWords;
	int MSInd = FindNumberOfSubWords(WordSubWords, p, WordC);
	int HSsize = p[MSInd].ei - p[MSInd].si + 1;
	char* HSWord = new char[HSsize + 1];
	for (int i = p[MSInd].si, c = 0; i <= p[MSInd].ei; i++, c++)
	{
		HSWord[c] = Ls[p[MSInd].li].Cs[i];
	}
	HSWord[HSsize] = '\0';
	cout << "The Word With Highest Sub Words Is :: " << HSWord << " With " << WordSubWords[MSInd] << " Sub-Words" << endl;
	//-----------------------------------
	int ParaCount = ParagraphCount();
	cout << "The number of paragraphs in the file are: " << ParaCount << endl;
}

bool Document::PassWord(ifstream& Rdr)
{
	getline(Rdr, Code);
	for (int i = 0; i < Code.size(); i++)
	{
		Code.at(i) = Code.at(i) - 3;
	}
	cout << "Enter The PassWord For The File\"" << DName << "\"" << endl;
	string word;
	getline(cin, word);
	if (Code.size() == Code.size())
	{
		for (int i = 0; i < Code.size(); i++)
		{
			if (Code.at(i) != word.at(i))
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

TextEditor::TextEditor()
{
	ADocument = nullptr;
	CDoc = 0;
	NODs = 0;
}

TextEditor::TextEditor(const string& dname)
{
	LoadFirst(dname);
}

void TextEditor::CreateNewDocument()
{
	system("cls");
	cout << "Enter Name of New Document: " << endl;
	string dname;
	getline(cin, dname);
	ofstream Wtr(dname);
	Wtr << 0 << endl;
	Document** ND = new Document * [1];
	ND[0] = new Document[NODs + 1];
	for (int i = 0; i < NODs; i++)
	{
		ND[0][i] = ADocument[0][i];
	}
	ND[0][NODs].Load(dname);
	delete[] ADocument;
	ADocument = ND;
	NODs++;
	CDoc = NODs - 1;
}

void TextEditor::LoadFirst(const string& dname)
{
	ADocument = new Document * [1];
	ADocument[0] = new Document(dname);
	CDoc = 0;
	NODs = 1;
}


void TextEditor::OpenNewDocument()
{
	Document** ND = new Document * [1];
	ND[0] = new Document[NODs + 1];
	for (int i = 0; i < NODs; i++)
	{
		ND[0][i] = ADocument[0][i];
	}
	system("cls");
	gotoRowCol(0, 0);
	cout << "Enter Document Name: ";
	string name;
	getline(cin,name);
	ND[0][NODs].Load(name);
	delete[] ADocument;
	ADocument = ND;
	NODs++;
	CDoc++;
}

void TextEditor::CloseCurrentDocument()
{
	Document** ND = new Document * [1];
	ND[0] = new Document[NODs - 1];
	for (int i = 0, j = 0; i < NODs; i++)
	{
		if (i==CDoc)
		{
			delete[] ADocument[0][i].Ls;
		}
		else
		{
			ND[0][j] = ADocument[0][i];
			j++;
		}
	}
	delete[] ADocument;
	ADocument = ND;
	NODs--;
	if (CDoc > 0)
	{
		CDoc--;
	}
}

TextEditor::~TextEditor()
{
	delete[] ADocument[0];
	delete[] ADocument;
}

void TextEditor::Addresses()
{
	system("cls");
	gotoRowCol(0, 0);
	cout << "ADocument (" << &ADocument << ") :: " << ADocument << endl << endl;
	ADocument[0][CDoc].Addresses();
}

void TextEditor::MergeDocs(string FDoc, string SDoc)
{
	
	ofstream Wtr(FDoc, ios::app);
	Document T(SDoc);
	Wtr << endl;
	for (int i = 0; i < T.NOLs; i++)
	{
		Wtr << T.Ls[i].Cs;
		if (i != T.NOLs - 1)
			Wtr << endl;
	}
	Wtr.close();
}

void TextEditor::MergeMultipleMenu()
{
	system("cls");
	cout << "----------MERGING MULTIPLE FILES----------" << endl;
	cout << "1. To Select Current Document (Other File will be placed at end)." << endl;
	cout << "2. To Select Document By Filename (Other File will be place at end)." << endl;
	cout << "Enter: _\b";
	int option;
	cin >> option;
	if (option == 1)
	{
		int fd = CDoc;
		cout << "----------------------------------------" << endl;
		cout << "1. To Enter the Document Number Opened in Text Editor (To Place at End of First Document)." << endl;
		cout << "2. To Enter the Document Filename (To Place at End of First Document)." << endl;
		cout << "Enter: _\b";
		cin >> option;
		cin.ignore();
		if (option == 1)
		{
			cout << "Enter Second Document Number::" ;
			int sd;
			cin >> sd;
			cin.ignore();
			sd--;
			if (sd < NODs && sd >= 0)
			{
				MergeDocs(ADocument[0][fd].DName, ADocument[0][sd].DName);
				cout << "Files has been Merged!" << endl;
				return;
			}
		}
		else if (option == 2)
		{
			cout << "Enter Second FileName::" ;
			string sfile;
			cin >> sfile;
			MergeDocs(ADocument[0][fd].DName, sfile);
			cout << "Files has been Merged!" << endl;
			return;
		}
	}
	else if (option == 2)
	{
		cout<< "Enter First FileName::";
		string ffile;
		cin >> ffile;
		cin.ignore();
		int fd = CDoc;
		cout << "----------------------------------------" << endl;
		cout << "1. To Enter the Document Number Opened in Text Editor (To Place at End of First Document)." << endl;
		cout << "2. To Enter the Document Filename (To Place at End of First Document)." << endl;
		cout << "Enter: _\b";
		cin >> option;
		if (option == 1)
		{
			cout << "Enter Second Document Number::";
			int sd;
			cin >> sd;
			cin.ignore();
			sd--;
			if (sd < NODs && sd >= 0)
			{
				MergeDocs(ffile , ADocument[0][sd].DName);
				cout << "Files has been Merged!" << endl;
				return;
			}
		}
		else if (option == 2)
		{
			cout << "Enter Second FileName::";
			string sfile;
			cin >> sfile;
			MergeDocs(ffile , sfile);
			cout << "Files has been Merged!" << endl;
			return;
		}
	}
	cout << "Invalid Options Entered!" << endl;
}

void TextEditor::Editing()
{
	int d;
	int h;
	char ch;
	cc = 0, cr = 0;
	ADocument[0][CDoc].PrintDocument(CDoc,NODs);
	gotoRowCol(cr, cc);
	while (true)
	{
		if (_kbhit())
		{
			d = _getch();
			if (d == 224)//Special Characters
			{
				d = _getch();
				if (d == 72/*Up Arrow*/)
				{
					if (cr > 0)
					{
						cr--;
					}
					if (ADocument[0][CDoc].Ls[cr].size - 1 < cc)
					{
						cc = ADocument[0][CDoc].Ls[cr].size;
					}
				}
				else if (d == 80/*Down Arrow*/)
				{
					if (ADocument[0][CDoc].NOLs - 1 > cr)
					{
						cr++;
					}
					if (ADocument[0][CDoc].Ls[cr].size - 1 < cc)
					{
						cc = ADocument[0][CDoc].Ls[cr].size;
					}
				}
				else if (d == 77/*Right Arrow*/)
				{
					cc++;
					if (ADocument[0][CDoc].Ls[cr].size < cc)
					{
						if (ADocument[0][CDoc].NOLs - 1 > cr)
						{
							cr++;
							cc = 0;
						}
						else
						{
							cc--;
						}
					}
				}
				else if (d == 75/*Left Arrow*/)
				{
					cc--;
					if (cc < 0)
					{
						if (cr > 0)
						{
							cr--;
							cc = ADocument[0][CDoc].Ls[cr].size;
						}
						else
						{
							cc++;
						}
					}
				}
				else if (d == 83/*Delete Key*/)
				{
					if (cc == ADocument[0][CDoc].Ls[cr].size && cr != ADocument[0][CDoc].NOLs - 1)
					{
						ADocument[0][CDoc].MergeTwoLinesAt(cr);
						ADocument[0][CDoc].PrintDocument(CDoc,NODs);
					}
					else if (cr != ADocument[0][CDoc].NOLs - 1 || cc != ADocument[0][CDoc].Ls[cr].size)
					{
						ADocument[0][CDoc].DeleteACharAt(cr, cc);
						ADocument[0][CDoc].PrintDocument(CDoc,NODs);
					}
					ADocument[0][CDoc].IsChanged = true;
				}
				else if (d == 147/*Ctrl + Delete*/)
				{
					if (cc == ADocument[0][CDoc].Ls[cr].size && cr != ADocument[0][CDoc].NOLs - 1)
					{
						ADocument[0][CDoc].MergeTwoLinesAt(cr);
						ADocument[0][CDoc].PrintDocument(CDoc,NODs);
					}
					else if (cr != ADocument[0][CDoc].NOLs - 1 || cc != ADocument[0][CDoc].Ls[cr].size)
					{
						ADocument[0][CDoc].DeleteWordRightAt(cr, cc);
						ADocument[0][CDoc].PrintDocument(CDoc,NODs);
					}
					ADocument[0][CDoc].IsChanged = true;
				}
				else if (d == 115/*Ctrl + Left Arrow*/)
				{
					if (cc == 0 && cr != 0)
					{
						cr--;
						cc = ADocument[0][CDoc].Ls[cr].size;
					}
					else
					{
						cc -= ADocument[0][CDoc].GiveWordLenghtFromLeftSide(cr, cc - 1);
					}
				}
				else if (d == 116/*Ctrl + Right Arrow*/)
				{
					if (cc == ADocument[0][CDoc].Ls[cr].size && cr != ADocument[0][CDoc].NOLs - 1)
					{
						cr++;
						cc = 0;
					}
					else
					{
						cc += ADocument[0][CDoc].GiveWordLenghtFromRightSide(cr, cc);
					}
				}
				else if (d == 73/*Page UP (Show Next Document)*/)
				{
					if (CDoc < NODs - 1)
					{
						CDoc++;
						ADocument[0][CDoc].PrintDocument(CDoc, NODs);
					}
					else
					{
						CDoc = 0;
						ADocument[0][CDoc].PrintDocument(CDoc, NODs);
					}
					cc = 0, cr = 0;
				}
				else if (d == 81/*Page DOWN (Show Previous Document*/)
				{
					if (CDoc > 0)
					{
						CDoc--;
						ADocument[0][CDoc].PrintDocument(CDoc, NODs);
					}
					else
					{
						CDoc = NODs - 1;
						ADocument[0][CDoc].PrintDocument(CDoc, NODs);
					}
					cc = 0, cr = 0;
				}
			}
			else if (d == 13/*Enter*/)
			{
				ADocument[0][CDoc].InsertANewLineAt(cr, cc);
				cr++;
				cc = 0;
				ADocument[0][CDoc].PrintDocument(CDoc,NODs);
				ADocument[0][CDoc].IsChanged = true;
			}
			else if (d == 8/*Backspace*/)
			{
				if (cc == 0 && cr != 0)
				{
					cr--;
					cc = ADocument[0][CDoc].Ls[cr].size;
					ADocument[0][CDoc].MergeTwoLinesAt(cr);
					ADocument[0][CDoc].PrintDocument(CDoc,NODs);
				}
				else if (cc != 0)
				{
					cc--;
					ADocument[0][CDoc].DeleteACharAt(cr, cc);
					ADocument[0][CDoc].PrintDocument(CDoc,NODs);
				}
				ADocument[0][CDoc].IsChanged = true;
			}
			else if (d == 127/*Ctrl + Backspace*/)
			{
				if (cc == 0 && cr != 0)
				{
					cr--;
					cc = ADocument[0][CDoc].Ls[cr].size;
					ADocument[0][CDoc].MergeTwoLinesAt(cr);
					ADocument[0][CDoc].PrintDocument(CDoc,NODs);
				}
				else if (cc != 0)
				{
					cc -= ADocument[0][CDoc].DeleteWordLeftAt(cr, cc - 1);
					ADocument[0][CDoc].PrintDocument(CDoc,NODs);
				}
				ADocument[0][CDoc].IsChanged = true;
			}
			else if (d == 12/*Ctrl + L (LIST OF FUNCTIONS)*/)
			{
				ADocument[0][CDoc].ListOfFunctions();
				system("pause");
				ADocument[0][CDoc].PrintDocument(CDoc,NODs);
			}
			else if (d == 21/*Ctrl + U (TOUPPER)*/)
			{
				ADocument[0][CDoc].TOUPPER(cr, cc);
				ADocument[0][CDoc].IsChanged = true;
			}
			else if (d == 15/*Ctrl + O (TOLOWER)*/)
			{
				ADocument[0][CDoc].TOLOWER(cr, cc);
				ADocument[0][CDoc].IsChanged = true;
			}
			else if (d == 19/*Ctrl + S (SAVE)*/)
			{
				d = _getch();
				if (d == 19)//Ctrl + S
				{
					ADocument[0][CDoc].SaveDocument();
					ADocument[0][CDoc].IsChanged = false;
				}
			}
			else if (d == 24/*Ctrl + X (EXIT)*/)
			{
				if (ADocument[0][CDoc].IsChanged)
				{
					ADocument[0][CDoc].ExitMenu();
					d = _getch();
					if (d == 19/*Ctrl + S (SAVE)*/)
					{
						ADocument[0][CDoc].SaveDocument();
						cout << endl << "File \"" << ADocument[0][CDoc].DName << "\" Saved And Closing." << endl;
						CloseCurrentDocument();
						system("pause");
						if (NODs == 0)
						{
							cout << "All Documents Closed. Text Editor Closed." << endl;
							break;
						}
						else
						{
							ADocument[0][CDoc].PrintDocument(CDoc,NODs);
						}
					}
					else if (d == 24/*Ctrl + X (Close)*/)
					{
						cout << endl << "Closing \"" << ADocument[0][CDoc].DName << "\" Without Saving." << endl;
						CloseCurrentDocument();
						system("pause");
						if (NODs == 0)
						{
							cout << "All Documents Closed. Text Editor Closed." << endl;
							break;
						}
						else
						{
							ADocument[0][CDoc].PrintDocument(CDoc,NODs);
						}
					}
					else
					{
						ADocument[0][CDoc].PrintDocument(CDoc,NODs);
					}
				}
				else
				{
					system("cls");
					cout << "Current Document \"" << ADocument[0][CDoc].DName << "\" Closed." << endl;
					CloseCurrentDocument();
					system("pause");
					if (NODs == 0)
					{
						cout << "All Documents Closed. Text Editor Closed." << endl;
						break;
					}
					else
					{
						ADocument[0][CDoc].PrintDocument(CDoc,NODs);
					}
				}
			}
			else if (d == 2/*Ctrl + B (ADDRESSES)*/)
			{
				Addresses();
				system("pause");
				ADocument[0][CDoc].PrintDocument(CDoc,NODs);
			}
			else if (d == 20/*Ctrl + T (FIND)*/)
			{
				ADocument[0][CDoc].FindWordSentenceSubString(cr, cc);
				ADocument[0][CDoc].PrintDocument(CDoc,NODs);
				ADocument[0][CDoc].IsChanged = true;
			}
			else if (d == 0/*Ctrl + Alt*/)
			{
				d = _getch();
				if (d == 24/* + O (Open New Document)*/)
				{
					OpenNewDocument();
					ADocument[0][CDoc].PrintDocument(CDoc,NODs);
					cc = 0, cr = 0;
				}
				else if (d == 31/* + S (Show Stats)*/)
				{
					ADocument[0][CDoc].ShowStats();
					system("pause");
					ADocument[0][CDoc].PrintDocument(CDoc,NODs);
				}
				else if (d == 50/* + M (Merge Files)*/)
				{
					MergeMultipleMenu();
					system("pause");
					ADocument[0][CDoc].PrintDocument(CDoc, NODs);
				}
				else if (d == 46/* + C (Create New Document)*/)
				{
					CreateNewDocument();
					ADocument[0][CDoc].PrintDocument(CDoc, NODs);
					cc = 0; cr = 0;
				}
			}			
			else if (d == 27/*Escape Key (Exit Editor)*/)
			{
				break;
			}
			else
			{
				ch = char(d);
				ADocument[0][CDoc].IsChanged = true;
				ADocument[0][CDoc].InsertACharacter(cr, cc, ch);
				ADocument[0][CDoc].PrintFromTo(cr, cc, ADocument[0][CDoc].Ls[cr].size);
				cc++;
			}
			gotoRowCol(cr, cc);
		}
	}
}