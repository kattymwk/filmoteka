#pragma once
#include "TopFilms.h"

class TopDetectivesFilms : public TopFilms
{
public:
	std::vector<Film*> GetTopDetectives();
};