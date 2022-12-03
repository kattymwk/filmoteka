#include <iostream>
#include <string>
#include <fstream>
#include "Film.h"
#include "FilmCollection.h"
#include "TopFilms.h"
#include "TopDetectivesFilms.h"
#include <iomanip>
using namespace std;

#pragma region Film

Film::Film() {}

Film::Film(int _id, string _title, string _genre, float _score, int _year)
{
	id = _id;
	title = _title;
	genre = _genre;
	score = _score;
	year = _year;
}

void Film::Print()
{
	cout << "_________________________________________________________\n";
	cout << setw(5) << left << id << setw(20) << left << title << setw(15) <<
		left << genre << setw(10) << left << score << setw(5) << left << year << endl;
}

#pragma endregion Film

#pragma region FilmCollection

FilmCollection::FilmCollection() {

	// формирую список фильмов из файла
	ifstream f("films.txt");

	while (f.good())
	{
		Film* tmp = new Film();
		f >> tmp->id;
		if (!f.good())
			break;
		f.get();
		getline(f, tmp->title);

		f >> tmp->genre;
		f >> tmp->score;
		f >> tmp->year;
		films.push_back(tmp);
	}

	f.close();

	f.open("views.txt");

	while (f.good())
	{
		int id;
		f >> id;
		if (!f.good())
			break;

		for (Film* film : films)
			if (film->id == id)
				views.push_back(film);
	}

	f.close();

	f.open("recomendations.txt");

	while (f.good())
	{
		int id = -1;
		f >> id;
		if (!f.good())
			break;

		for (Film* film : films)
			if (film->id == id)
				recomendations.push_back(film);
	}

	f.close();
}

vector<Film*> FilmCollection::GetCollection()
{
	return films;
}
vector<Film*> FilmCollection::GetViews()
{
	return views;
}
vector<Film*> FilmCollection::GetRecomendations()
{
	return recomendations;

}

void FilmCollection::Add(Film* film)
{
	int maxId = 1;
	for (Film* f : films)
	{
		if (f->id > maxId)
			maxId = f->id;
	}

	film->id = maxId + 1;
	films.push_back(film);

	SaveToFile();
}

void FilmCollection::Delete(string title)
{
	// указатель на найденный фильм
	for (Film* f : films)
	{
		if (!f->title.compare(title))
		{
			auto entity = find(films.begin(), films.end(), f);
			if (entity == films.end())
				return;
			// удаляет диапазон (в частности один элемент как сейчас)
			films.erase(entity, entity + 1);

			// delete from views
			entity = find(views.begin(), views.end(), f);
			if (entity != views.end())
				views.erase(entity, entity + 1);

			// delete from recs
			entity = find(recomendations.begin(), recomendations.end(), f);
			if (entity != recomendations.end())
				recomendations.erase(entity, entity + 1);

			SaveToFile();
			return;
		}
	}
}

Film* FilmCollection::GetRandomFilm()
{
	int random = rand() % recomendations.size();
	return recomendations[random];
}

void FilmCollection::ViewFilm(Film* film)
{
	views.push_back(film);
	auto entity = find(recomendations.begin(), recomendations.end(), film);
	if (entity == recomendations.end())
		return;

	recomendations.erase(entity, entity + 1);
	SaveToFile();
}

void FilmCollection::SaveToFile()
{
	ofstream f("films.txt");
	for (Film* film : films)
	{
		f << to_string(film->id) << endl;
		f << film->title << endl;
		f << film->genre << endl;
		f << to_string(film->score) << endl;
		f << to_string(film->year) << endl;
	}
	f.close();

	f.open("views.txt");
	for (Film* film : views)
		f << film->id << endl;
	f.close();

	f.open("recomendations.txt");
	for (Film* film : recomendations)
		f << film->id << endl;
	f.close();
}

#pragma endregion FilmCollection

#pragma region TopFilms

vector<Film*> TopFilms::GetTop()
{
	vector<Film*> arr;
	for (Film* f : films)
		if (f->score >= 8)
			arr.push_back(f);
	return arr;
}

#pragma endregion TopFilms

#pragma region TopDetectivesFilms

vector<Film*> TopDetectivesFilms::GetTopDetectives()
{
	vector<Film*> arr;
	for (Film* f : GetTop())
		if (!f->genre.compare("detective"))
			arr.push_back(f);
	return arr;
}

#pragma endregion TopDetectivesFilms

#pragma region Search
void FilmCollection::Search()
{
	int sel = 0;
	while (sel != 1 && sel != 2 && sel != 3)
	{
		cout << endl << "Find film by:" << endl
			<< "1. Title" << endl
			<< "2. Genre" << endl
			<< "3. Year" << endl
			<< "\n\n > ";

		rewind(stdin);
		cin.clear();
		cin >> sel;
	}

	switch (sel)
	{
	case 1:
	{
		bool badInputFlag = 1;

		string tit;
		cout << "Write title for search: ";
		cin.ignore();
		getline(cin, tit);

		vector<Film*> arr;
		for (Film* f : films)
		{
			if (f->title == tit)
			{
				f->Print();
				badInputFlag = 0;
			}
		}
		if (badInputFlag)cout << endl << "Film not found" << endl;
		break;
	}
	case 2:
	{
		bool badInputFlag = 1;

		string gen;
		cout << "Write genre for search: ";
		cin.clear();
		cin.ignore();
		getline(cin, gen);

		vector<Film*> arr;
		for (Film* f : films)
		{
			if (f->genre == gen)
			{
				f->Print();
				badInputFlag = 0;
			}
		}
		if (badInputFlag)cout << endl << "Films not found" << endl;
		break;
	}
	case 3:
	{
		bool badInputFlag = 1;

		int _year;
		cout << "Write year for search: ";
		cin.clear();
		cin.ignore();
		cin >> _year;

		vector<Film*> arr;
		for (Film* f : films)
		{
			if (f->year == _year)
			{
				f->Print();
				badInputFlag = 0;
			}
		}
		if (badInputFlag)cout << endl << "Films not found" << endl;
		break;
	}
	default:
		break;
	}
}


#pragma endregion Search

//#pragma region Sort
//void FilmCollection::Sort()
//{
//	int ans = 0;
//	while (ans != 1 && ans != 2 && ans != 3)
//	{
//		cout << endl << "Sort films by:" << endl
//			<< "1. Title" << endl
//			<< "2. Score" << endl
//			<< "3. Year" << endl
//			<< "\n\n > ";
//
//		rewind(stdin);
//		cin.clear();
//		cin >> ans;
//	}
//	switch (ans)
//	{
//	case 1:
//	{
//		cout << "Sort films by title: ";
//		vector<Film*> arr;
//		vector<Film*> _arr;
//		for (Film* f : films)
//		{
//			if (f->title == tit)
//			{
//				f->Print();
//			}
//		}
//		break;
//	}
//	case 2:
//	{
//		break;
//	}
//	case 3:
//	{
//		break;
//	}
//	default:
//		break;
//	}
//}
//#pragma endregion Sort

void PrintMenu()
{
	cout << "\n\t[ M E N U ]\n";
	cout << "\n";
	cout << "1 - show films\n";
	cout << "2 - show viewed films\n";
	cout << "3 - show recomendations\n";
	cout << "4 - add film\n";
	cout << "5 - remove film\n";
	cout << "6 - search film\n";
	cout << "7 - view random film from recomendations\n";
	cout << "8 - top films\n";
	cout << "9 - top detectives\n";
	cout << "10 - exit\n";

}

int main()
{
	FilmCollection* x = new FilmCollection();

	int code;

	while (true)
	{
		system("cls");
		PrintMenu();
		cin >> code;
		getchar(); // get '\n' after 'Enter' press
		fflush(stdin);
		system("cls");

		if (code == 1)
		{
			cout << "_________________________________________________________\n";
			cout << setw(5) << left << "ID" << setw(20) << left << "TITLE" << setw(15) <<
				left << "GENRE" << setw(10) << left << "SCORE" << setw(5) << left << "YEAR" << endl;
			for (Film* f : x->GetCollection())
				f->Print();
			cout << "_________________________________________________________\n";
			//cout << f->ToString() << endl;
		}
		else if (code == 2)
		{
			cout << "_________________________________________________________\n";
			cout << setw(5) << left << "ID" << setw(20) << left << "TITLE" << setw(15) <<
				left << "GENRE" << setw(10) << left << "SCORE" << setw(5) << left << "YEAR" << endl;
			for (Film* f : x->GetViews())
				f->Print();
			cout << "_________________________________________________________\n";
			//cout << f->ToString() << endl;
		}
		else if (code == 3)
		{
			cout << "_________________________________________________________\n";
			cout << setw(5) << left << "ID" << setw(20) << left << "TITLE" << setw(15) <<
				left << "GENRE" << setw(10) << left << "SCORE" << setw(5) << left << "YEAR" << endl;
			for (Film* f : x->GetRecomendations())
				f->Print();
			cout << "_________________________________________________________\n";
			//cout << f->ToString() << endl;
		}
		else if (code == 4)
		{
			Film* tmp = new Film();
			cout << "input title: ";

			getline(cin, tmp->title);

			cout << "input genre: ";
			cin >> tmp->genre;
			cout << "input score: ";
			cin >> tmp->score;
			cout << "input year: ";
			cin >> tmp->year;

			x->Add(tmp);
		}
		else if (code == 5)
		{
			cout << "input film title: ";
			string title;
			getline(cin, title);

			x->Delete(title);
		}
		else if (code == 6) x->Search();
		else if (code == 7)
		{
			Film* film = x->GetRandomFilm();
			cout << "Film '" << film->title << "' will be viewed!";
			x->ViewFilm(film);
		}
		else if (code == 8)
		{
			TopFilms* top = new TopFilms();
			cout << "_________________________________________________________\n";
			cout << setw(5) << left << "ID" << setw(20) << left << "TITLE" << setw(15) <<
				left << "GENRE" << setw(10) << left << "SCORE" << setw(5) << left << "YEAR" << endl;
			for (Film* f : top->GetTop())
				f->Print();
			cout << "_________________________________________________________\n";
			//cout << f->ToString() << endl;
		}
		else if (code == 9)
		{
			TopDetectivesFilms* top = new TopDetectivesFilms();
			cout << "_________________________________________________________\n";
			cout << setw(5) << left << "ID" << setw(20) << left << "TITLE" << setw(15) <<
				left << "GENRE" << setw(10) << left << "SCORE" << setw(5) << left << "YEAR" << endl;
			for (Film* f : top->GetTopDetectives())
				f->Print();
			cout << "_________________________________________________________\n";
			//cout << f->ToString() << endl;
		}
		else if (code == 10) return 0;

		cout << "\npress any key for exit to menu\n";
		rewind(stdin);
		getchar();
	}

	return 0;
}