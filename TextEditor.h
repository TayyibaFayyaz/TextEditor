#pragma once
#include<iostream>
#include<string>
using namespace std;

struct pos
{
	int li;
	int si;
	int ei;
};

class Line
{
private:
	char* Cs;
	int size;
	friend class TextEditor;
	friend class Document;
	static bool IsCharacter(char c);
	static bool IsWCharacter(char c);
	static bool IsSCharacter(char c);
	static bool IsLower(char c);
	static bool IsUpper(char c);
	static void ToLower(string& Word);
	static void ToLower(char*& C);
	void Addresses();
	void Clear();
public:
	Line();
	~Line();
	Line(const Line& A);
	void DeepCopy(const Line& A);
	void operator=(const Line& A);
	void PrintFromTo(int si, int ei);
	void InsertCharAt(int i, char ch);
	void DeleteACharAt(int i);
	void DeleteWordRightAt(int i);
	int DeleteWordLeftAt(int i);
	int GiveWordLenghtFromRightSide(int i);
	int GiveWordLenghtFromLeftSide(int i);
	bool SelectWord(int i, int& si, int& ei);
	void TOUPPER(int si, int ei);
	void TOLOWER(int si, int ei);
};


class Document
{
private:
	Line* Ls;
	int NOLs;
	string DName;
	bool IsChanged = false;
	bool Protected;
	string Code;
	bool PassOk;
	friend class TextEditor;
	static void ListOfFunctions();
	void ExitMenu();
	static void FindMenu();
	static void FindCaseMenu();
	void FindKeys();
	void Addresses();
	int CountSpecialChar();
	int SentencesCount();
	int WordCount();
	float AverageWordLenght(pos*& p, int*& WordSizes);
	int SmallestWordLen(int*& WordSizes);
	int LargestWordLen(int*& WordSizes);
	int ParagraphCount();
	bool PassWord(ifstream& Rdr);
public:
	Document(string Fname);
	Document();
	~Document();
	void PrintFromTo(int li, int si, int ei);
	void Load(string Fname);
	void InsertACharacter(int li, int ci, char ch);
	void InsertANewLineAt(int li, int ci);
	void MergeTwoLinesAt(int i);
	void DeleteACharAt(int li, int ci);
	void DeleteWordRightAt(int li, int ci);
	int DeleteWordLeftAt(int li, int ci);
	int GiveWordLenghtFromRightSide(int li, int ci);
	int GiveWordLenghtFromLeftSide(int li, int ci);
	void FindWordSentenceSubString(int& cr, int& cc);
	void FindWord(int& cr, int& cc, string word, int& d, bool& CaseSen);
	void FindSubString(int& cr, int& cc, string word, int& d, bool& CaseSen);
	void FindSentence(int& cr, int& cc, string word, int& d, bool& CaseSen);
	void TOUPPER(int li, int ci);
	void TOLOWER(int li, int ci);
	int FindNumberOfSubWords(int*& SubWords, pos*& p, int WordCount);
	void ShowStats();
	void SaveDocument();
	void PrintDocument(int Cfi, int TD);
};

class TextEditor
{
private:
	int cr = 0, cc = 0;
	Document** ADocument;
	int CDoc;
	int NODs;
	void Addresses();
	void MergeDocs(string FDoc, string SDoc);
public:
	TextEditor(const string& dname);
	TextEditor();
	void CreateNewDocument();
	void LoadFirst(const string& dname);
	~TextEditor();
	void OpenNewDocument();
	void MergeMultipleMenu();
	void CloseCurrentDocument();
	void Editing();
};

