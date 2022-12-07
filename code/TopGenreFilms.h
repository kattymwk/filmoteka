#pragma once
#include "TopFilms.h"

class TopGenreFilms : public TopFilms
{
public:
	 vector<Film*> GetTopGenre(string genre);
};