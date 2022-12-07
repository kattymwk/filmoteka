#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Film.h"

bool ComparisonForScore1(Film* f1, Film* f2)
{
    return f1->score < f2->score;
}
bool ComparisonForScore2(Film* f1, Film* f2)
{
    return f1->score > f2->score;
}


bool ComparisonForTitle(Film* f1, Film* f2)
{
    return f2->title.compare(f1->title) == 1;
}


bool ComparisonForYear1(Film* f1, Film* f2)
{
    return f1->year < f2->year;
}
bool ComparisonForYear2(Film* f1, Film* f2)
{
    return f1->year > f2->year;
}

