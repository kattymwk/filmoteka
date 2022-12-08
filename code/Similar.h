#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "FilmCollection.h"
#include "Exception.h"

class SimFilm
{
public:
    int idFilm;
    int idFilmTwo;

    SimFilm() {}
    ~SimFilm() {}
};

class SimilarFilms
{
public:
    int length = 0;
    int free = 0;
    SimFilm** similar;

    SimilarFilms() {}
    
    ~SimilarFilms()
    {
        delete[] similar;
    }

    void AddSFilm(SimFilm* elem) 
    {
        similar = (SimFilm**)realloc(similar, (free + 1) * sizeof(SimFilm*));
        similar[free++] = elem;
        length++;
    }

    bool LoadFromFile() 
    {
      
            ifstream f("similar.txt");

            string buff;

            while (!f.eof()) 
            {
                SimFilm* s = new SimFilm();

                string idOne;
                string idTwo;

                if (!getline(f, idOne) || !getline(f, idTwo))
                    break;

                s->idFilm = atoi(idOne.c_str());
                s->idFilmTwo = atoi(idTwo.c_str());

                AddSFilm(s);

                getline(f, buff);
            }

            return true;
    }

  

    void CheckSimilarFilms(FilmCollection* list) 
    {
        string name;

        cout << "\n\n\t[ Похожие фильмы ]\n\n";

        cout << "\nВведите название фильма. После этого вы сможете увидеть похожие фильмы!\n" << endl;

                getline(cin, name);
                cout << endl;

                int id = -1;
                int SimId = -1;


                for (int m = 0; m < list->films.size(); m++)
                {
                    if (list->films[m]->title == name)
                    {
                        id = list->films[m]->id;
                        break;
                    }
                }

                for (auto i = this->begin(); i != this->end(); i++)
                    if ((*i)->idFilm == id)
                    {
                        SimId = (*i)->idFilmTwo;
                        for (Film* f : list->films)
                        {
                            if (f->id == SimId) f->Print();
                        }
                    }

                if (id == -1)
                {
                    cout << endl << "Неверное название! Фильм не найден!" << endl;
                    return;
                }
                if (SimId == -1)
                {
                    cout << endl << name << " не имеет похожих фильмов." << endl;
                    return;
                }
            
    }

    struct Iterator 
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = SimFilm*;
        using pointer = SimFilm**;
        using reference = SimFilm*&;

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        // Prefix increment
        Iterator& operator++() { m_ptr = m_ptr++; return *this; }

        // Postfix increment
        Iterator operator++(int) { Iterator tmp = *this; m_ptr = m_ptr++; return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer m_ptr;
    };


    Iterator begin() { return Iterator(similar); }
    Iterator end() { return Iterator(similar + length); }
};