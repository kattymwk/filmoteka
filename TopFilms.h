#pragma once
#include "FilmCollection.h"

class TopFilms : public FilmCollection
{
public:
	std::vector<Film*> GetTop();
}; 
