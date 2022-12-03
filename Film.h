#pragma once
#include <string>

using namespace std;
class Film
{
public:
	int id;
	string title;
	string genre;
	float score;
	int year;

	Film();

	Film(int _id, string _title, string _genre, float _score, int _year);

	void Print();
};

