#pragma once
#include <string>

using namespace std;
class Film
{
public:
	int id;
	string title;
	string genre;
	double score;
	int year;
	string info;
	string actors;

	Film();

	Film(int _id, string _title, string _genre, double _score, int _year, string _info, string _actors);

	void Print();
	void PrintInfo();
};

