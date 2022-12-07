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
	vector<Film*> temp;

	FilmCollection();

	vector<Film*> GetCollection();
	vector<Film*> GetViews();
	vector<Film*> GetRecomendations();

	void Add(Film* film);
	void Delete(string title);
	void Edit();

	Film* GetRandomFilm();

	void ViewRandFilmFromRec(Film* film);
	void ViewFilm();
	void ViewInfo();

	void Search();
	void Sort();

	friend class SimilarFilms;

}; 

