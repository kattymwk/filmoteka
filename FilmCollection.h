#pragma once
#include <vector>
#include "Film.h"

using namespace std;
class FilmCollection
{
private:
	void SaveToFile();
public:

	vector<Film*> films;
	vector<Film*> views;
	vector<Film*> recomendations;

	FilmCollection();

	vector<Film*> GetCollection();
	vector<Film*> GetViews();
	vector<Film*> GetRecomendations();

	void Add(Film* film);

	void Delete(string title);

	Film* GetRandomFilm();

	void virtual ViewFilm(Film* film);

	void Search();
	void Sort();
}; 