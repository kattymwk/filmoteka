#pragma once
#include "FilmCollection.h"

class TopFilms : public FilmCollection
{
public:
	vector<Film*> GetTop();
}; 
