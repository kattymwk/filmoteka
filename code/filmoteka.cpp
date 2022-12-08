#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <exception>
#include <locale.h>
#include <windows.h>

#include "Film.h"
#include "FilmCollection.h"
#include "TopFilms.h"
#include "TopGenreFilms.h"
#include "Sorting.h"
#include "Similar.h"
#include "Exception.h"

using namespace std;

bool BadInputFlag = 1;

#pragma region Film

Film::Film() {}

Film::Film(int _id, string _title, string _genre, double _score, int _year,string _info,string _actors)
{
	id = _id;
	title = _title;
	genre = _genre;
	score = _score;
	year = _year;
	info = _info;
	actors = _actors;
}

void Film::Print()
{
	cout << "______________________________________________________________\n";
	cout << setw(5) << left << id << setw(20) << left << title << setw(15) <<
		left << genre << setw(10) << left << score << setw(15) << left << year << endl;
}

void Film::PrintInfo()
{
	cout << "\nИнформация о фильме:\n" << info << endl << "\nАктеры:\n" << actors << endl << endl;
}

void PrintHeader()
{
	cout << "______________________________________________________________\n";
	cout << setw(5) << left << "ID" << setw(20) << left << "НАЗВАНИЕ" << setw(15) <<
		left << "ЖАНР" << setw(10) << left << "РЕЙТИНГ" << setw(15) << left << "ГОД ВЫПУСКА" << endl;
}

#pragma endregion Film

#pragma region FilmCollection

FilmCollection::FilmCollection()
{

	// формирую список фильмов из файла
	ifstream f("films.txt");

	if (!f.is_open() || f.bad())
	{
		throw Exception("Файл не открыт!");
	}

	while (f.good())
	{
		Film* tmp = new Film();
		f >> tmp->id;
		if (!f.good())
			break;
		f.get();
		getline(f, tmp->title);

		getline(f, tmp->genre);
		f >> tmp->score;
		f >> tmp->year;
		f.get();
		getline(f, tmp->info);
		getline(f, tmp->actors);
		films.push_back(tmp);
	}

	f.close();

	ifstream p("films.txt");

	if (!p.is_open() || p.bad())
	{
		throw Exception("Файл не открыт!");
	}

	while (p.good())
	{
		Film* tmp = new Film();
		p >> tmp->id;
		if (!p.good())
			break;
		p.get();
		getline(p, tmp->title);

		getline(p, tmp->genre);
		p >> tmp->score;
		p >> tmp->year;
		p .get();
		getline(p, tmp->info);
		getline(p, tmp->actors);
		temp.push_back(tmp);
	}

	f.close();

	f.open("views.txt");

	if (!f.is_open() || f.bad())
	{
		throw Exception("Файл не открыт!");
	}

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

	if (!f.is_open() || f.bad())
	{
		throw Exception("Файл не открыт!");
	}

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
	int newId = films.size() + 1;
	film->id = newId;
	films.push_back(film);
	SaveToFile();
	cout << endl<<"Вы добавили новый фильм!" << endl << endl;
	while (true)
	{
		try
		{
	int otv = 0;
	while (otv != 1 && otv != 2)
	{
		cout << endl<<"Хотите отменить последнее действие? (1 - да | 2 - нет)" << endl;
		cin.clear();
		if(!(cin >> otv)) throw(exception("Некорректный ввод! Повторите попытку!\n"));
		cin.ignore();
	}
	switch (otv)
	{
	case 1: { films = temp; SaveToFile(); break; }

	case 2: {temp.push_back(film); break; }
	}
	break;
		}
		catch (const exception& ex)
		{
			cout << ex.what() << endl;
			cin.clear();
			cin.ignore(100, '\n');
			system("pause");
		}
	}
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

			entity = find(temp.begin(), temp.end(), f);
			if (entity != temp.end())
				temp.erase(entity, entity + 1);

			SaveToFile();

			cout << "\n\nВы удалили фильм!\n\n";

			return;
		}
	}
}

Film* FilmCollection::GetRandomFilm()
{
	int random = rand() % recomendations.size();
	return recomendations[random];
}

void FilmCollection::ViewRandFilmFromRec(Film* film)
{
	views.push_back(film);
	auto entity = find(recomendations.begin(), recomendations.end(), film);
	if (entity == recomendations.end())
		return;

	recomendations.erase(entity, entity + 1);
	SaveToFile();
}

void FilmCollection::ViewFilm()
{
	BadInputFlag = 1;
	while (BadInputFlag)
	{
		Search();
	};

	cout << "\nВведите название для просмотра фильма (введите 'exit', чтобы вернуться в меню): " << endl;
	string name;
	getline(cin, name);
	if (name == "exit") return;
	system("cls");
	for (Film* f : films)
	{
		if (f->title == name)
		{
			cout << "Вы хотите посмотреть " << endl;
			f->Print();
			views.push_back(f);
			SaveToFile();
		}
	}
}

void FilmCollection::SaveToFile()
{
	ofstream f("films.txt");

	if (!f.is_open() || f.bad())
	{
		throw Exception("Файл не открыт!");
	}

	for (Film* film : films)
	{
		f << to_string(film->id) << endl;
		f << film->title << endl;
		f << film->genre << endl;
		f << film->score << endl;
		f << to_string(film->year) << endl;
		f << film->info << endl;
		f << film->actors << endl;
	}
	f.close();

	f.open("views.txt");
	if (!f.is_open() || f.bad())
	{
		throw Exception("Файл не открыт!");
	}
	for (Film* film : views)
		f << film->id << endl;
	f.close();

	f.open("recomendations.txt");
	if (!f.is_open() || f.bad())
	{
		throw Exception("Файл не открыт!");
	}

	for (Film* film : recomendations)
		f << film->id << endl;
	f.close();
}

void FilmCollection::Search()
{
	while (true)
	{
		try
		{
			int sel = 0;
			while (sel != 1 && sel != 2 && sel != 3 && sel != 4)
			{
				cout << endl << "Поиск фильма по:" << endl
					<< "1. Названию" << endl
					<< "2. Жанру" << endl
					<< "3. Году выпуска" << endl
					<< "4. Вернуться в меню" << endl
					<< "\n\n > ";

				rewind(stdin);
				cin.clear();
				if(!(cin >> sel)) throw(exception("Некорректный ввод! Повторите попытку!"));
			}

			switch (sel)
			{
			case 1:
			{

				string tit;
				cout << "\nВведите название фильма для поиска (введите 'exit', чтобы вернуться в меню): " << endl;
				cin.ignore();
				getline(cin, tit);
				if (tit == "exit")
				{
					BadInputFlag = 0;
					return;
				}

				vector<Film*> arr;
				for (Film* f : films)
				{
					if (f->title == tit)
					{
						f->Print();
						BadInputFlag = 0;
					}
				}
				if (BadInputFlag)cout << endl << "Фильм не найден!" << endl;
				break;
			}
			case 2:
			{

				string gen;
				cout << "\nВведите жанр фильма для поиска (введите 'exit', чтобы вернуться в меню): " << endl;
				cin.clear();
				cin.ignore();
				getline(cin, gen);
				if (gen == "exit")
				{
					BadInputFlag = 0;
					return;
				}

				vector<Film*> arr;
				for (Film* f : films)
				{
					if (f->genre == gen)
					{
						f->Print();
						BadInputFlag = 0;
					}
				}
				if (BadInputFlag)cout << endl << "Фильмы не найдены!" << endl;
				break;
			}
			case 3:
			{

				int _year;
				cout << "\nВведите год выпуска фильма для поиска (введите '0', чтобы вернуться в меню): " << endl;
				cin.clear();
				if(!(cin >> _year)) throw(exception("Некорректный ввод! Повторите попытку!"));
				cin.ignore();
				if (_year == 0)
				{
					BadInputFlag = 0;
					return;
				}

				vector<Film*> arr;
				for (Film* f : films)
				{
					if (f->year == _year)
					{
						f->Print();
						BadInputFlag = 0;
					}
				}
				if (BadInputFlag)cout << endl << "Фильмы не найдены!" << endl;
				break;
			}
			case 4:
			{
				BadInputFlag = 0;
				break;
			}
			default:
				break;
			}
			break;
		}
		catch (const exception& ex)
		{
			cout << ex.what() << endl;
			cin.clear();
			cin.ignore(100, '\n');
			system("pause");
		}
	}
}

void FilmCollection::Edit()
{
	while (true)
	{
		try
		{
			string tmp1;
			string tmp2;
			double tmp3;
			int tmp4;
			PrintHeader();
			for (Film* f : films) f->Print();
			cout << "\nВведите ID для редактирования записи (введите '0', чтобы вернуться в меню): \n\n";
			int _id;
			cin.clear();
			if(!(cin >> _id)) throw(exception("Некорректный ввод! Повторите попытку!"));
			cin.ignore();
			system("cls");
			if (_id == 0) return;

			for (Film* f : films)
			{
				tmp1 = f->title;
				tmp2 = f->genre;
				tmp3 = f->score;
				tmp4 = f->year;

				if (f->id == _id)
				{
					cout << "Вы хотите редактировать запись?" << endl;
					f->Print();
					int sel = 0;
					while (sel != 1 && sel != 2)
					{
						cout << "1 - да | 2 - нет\n\n";
						cin.clear();
						if(!(cin >> sel)) throw(exception("Некорректный ввод! Повторите попытку!"));
						cin.ignore();
					}
					if (sel == 2) return;

					bool Flag = true;

					do
					{
						int ed = 0;
						while (ed != 1 && ed != 2 && ed != 3 && ed != 4 && ed != 5)
						{
							std::cout << "Редактировать:\n\n" << "1. Название;\n" <<
								"2. Жанр;\n3. Рейтинг;\n" <<
								"4. Год выпуска;\n5. Вернуться в меню.\n\n";
							cin.clear();
							if(!(cin >> ed)) throw(exception("Некорректный ввод! Повторите попытку!"));
							cin.ignore();
						}
						switch (ed)
						{
						case 1:
						{
							string newTitle;
							cout << "Напишите новое название:\n\n";
							getline(cin, newTitle);
							f->title = newTitle;
							SaveToFile();
							cout << endl << "Запись изменена!" << endl << endl;
							int otv = 0;
							while (otv != 1 && otv != 2)
							{
								cout << endl << "Хотите отменить редактирование ? (1 - да | 2 - нет)" << endl;
								cin.clear();
								if (!(cin >> otv)) throw(exception("Некорректный ввод! Повторите попытку!"));
								cin.ignore();
							}
							switch (otv)
							{
							case 1: { f->title=tmp1; SaveToFile(); break; }

							case 2: { break; }
							}
							break;
						}
						case 2:
						{
							string newGenre;
							std::cout << "Напишите новый жанр:\n\n";
							getline(cin, newGenre);
							f->genre = newGenre;
							SaveToFile();
							cout << endl << "Запись изменена!" << endl << endl;
							int otv = 0;
							while (otv != 1 && otv != 2)
							{
								cout << endl << "Хотите отменить редактирование ? (1 - да | 2 - нет)" << endl;
								cin.clear();
								if (!(cin >> otv)) throw(exception("Некорректный ввод! Повторите попытку!"));
								cin.ignore();
							}
							switch (otv)
							{
							case 1: { f->genre=tmp2; SaveToFile(); break; }

							case 2: { break; }
							}
							break;
						}
						case 3:
						{
							float newScore;
							std::cout << "Напишите новый рейтинг:\n\n";
							cin.clear();
							if(!(cin >> newScore)) throw(exception("Некорректный ввод! Повторите попытку!"));
							cin.ignore();
							f->score = newScore;
							SaveToFile();
							cout << endl << "Запись изменена!" << endl << endl;
							int otv = 0;
							while (otv != 1 && otv != 2)
							{
								cout << endl << "Хотите отменить редактирование ? (1 - да | 2 - нет)" << endl;
								cin.clear();
								if (!(cin >> otv)) throw(exception("Некорректный ввод! Повторите попытку!"));
								cin.ignore();
							}
							switch (otv)
							{
							case 1: { f->score=tmp3; SaveToFile(); break; }

							case 2: { break; }
							}
							break;
						}
						case 4:
						{
							int newYear;
							std::cout << "Напишите новый год выпуска:\n\n";
							cin.clear();
							if(!(cin >> newYear)) throw(exception("Некорректный ввод! Повторите попытку!"));
							cin.ignore();
							f->year = newYear;
							SaveToFile();

							cout << endl << "Запись изменена!" << endl << endl;
							int otv = 0;
							while (otv != 1 && otv != 2)
							{
								cout << endl << "Хотите отменить редактирование ? (1 - да | 2 - нет)" << endl;
								cin.clear();
								if (!(cin >> otv)) throw(exception("Некорректный ввод! Повторите попытку!"));
								cin.ignore();
							}
							switch (otv)
							{
							case 1: { f->year=tmp4; SaveToFile(); break; }

							case 2: { break; }
							}
							break;


						}
						case 5: return;
						}

						int ans = 0;
						while (ans != 1 && ans != 2)
						{
							std::cout << "Вы хотите изменить что-то еще? (1 - да | 2 - нет)" << endl;
							cin.clear();
							if(!(cin >> ans)) throw(exception("Некорректный ввод! Повторите попытку!"));
							cin.ignore();
						}
						switch (ans)
						{
						case 1: {Flag = true; break; }
						case 2: {Flag = false; break; }
						}

					} while (Flag);
				}
			}
			break;
		}
		catch (const exception& ex)
		{
			cout << ex.what() << endl;
			cin.clear();
			cin.ignore(100, '\n');
		}
	}
}


void FilmCollection::Sort()
{
	while (true)
	{
		try
		{
			int choice = 0;
			while (choice != 1 && choice != 2 && choice != 3 && choice != 4)
			{
				cout << "Сортировать по:" << endl
					<< "1. Названию (в алфавитном порядке);" << endl
					<< "2. Рейтингу;" << endl
					<< "3. Году выпуска;" << endl
					<< "4. Вернуться в меню." << endl << endl;
				rewind(stdin);
				cin.clear();
				if(!(cin >> choice)) throw(exception("Некорректный ввод! Повторите попытку!"));
			}
			if (choice == 1)
			{
				vector<Film*> tmp = films;
				sort(tmp.begin(), tmp.end(), ComparisonForTitle);
				for (Film* f : tmp) f->Print();
			}
			if (choice == 2)
			{
				int v = 0;
				while (v != 1 && v != 2 && v != 3)
				{
					cout << "\nСортировать по:\n1. Возрастанию.\n" <<
						"2. Убыванию.\n3. Вернуться в меню.\n\n";
					if(!(cin >> v)) throw(exception("Некорректный ввод! Повторите попытку!"));
				}
				if (v == 1)
				{
					vector<Film*> tmp = films;
					sort(tmp.begin(), tmp.end(), ComparisonForScore1);
					for (Film* f : tmp) f->Print();
				}
				if (v == 2)
				{
					vector<Film*> tmp = films;
					sort(tmp.begin(), tmp.end(), ComparisonForScore2);
					for (Film* f : tmp) f->Print();
				}
				if (v == 3) return;

			}
			if (choice == 3)
			{
				int v = 0;
				while (v != 1 && v != 2 && v != 3)
				{
					cout << "\nСортировать по:\n1. Возрастанию.\n" <<
						"2. Убыванию.\n3. Вернуться в меню.\n\n";
					if(!(cin >> v)) throw(exception("Некорректный ввод! Повторите попытку!"));
				}
				if (v == 1)
				{
					vector<Film*> tmp = films;
					sort(tmp.begin(), tmp.end(), ComparisonForYear1);
					for (Film* f : tmp) f->Print();
				}
				if (v == 2)
				{
					vector<Film*> tmp = films;
					sort(tmp.begin(), tmp.end(), ComparisonForYear2);
					for (Film* f : tmp) f->Print();
				}
				if (v == 3) return;
			}
			if (choice == 4) return;
			break;
		}
		catch (const exception& ex)
		{
			cout << ex.what() << endl;
			cin.clear();
			cin.ignore(100, '\n');
		}
	}
}

void FilmCollection::ViewInfo()
{
	while (true)
	{
		try
		{
			PrintHeader();
			for (Film* f : films) f->Print();
			cout << "\nВведите ID, чтобы увидеть информацию о фильме. Введите '0' для выхода в меню.\n\n";
			rewind(stdin);
			int idd;
			if(!(cin >> idd)) throw(exception("Некорректный ввод! Повторите попытку!"));
			for (Film* f : films)
			{
				if (idd == 0) return;
				if (f->id == idd) f->PrintInfo();
			}
			break;
		}
		catch (const exception& ex)
		{
			cout << ex.what() << endl;
			cin.clear();
			cin.ignore(100, '\n');
		}
	}
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

#pragma region TopGenreFilms

vector<Film*> TopGenreFilms::GetTopGenre(string genre)
{
	vector<Film*> arr;
	for (Film* f : GetTop())
		if (!f->genre.compare(genre))
			arr.push_back(f);
	return arr;
}

#pragma endregion TopGenreFilms


void PrintMenu()
{
	cout << "\n\t[ М Е Н Ю ]\n";
	cout << "\n";
	cout << "\t1 - показать список фильмов\n";
	cout << "\t2 - показать историю просмотра\n";
	cout << "\t3 - показать рекомендации\n";
	cout << "\t4 - лучшие фильмы\n";
	cout << "\t5 - лучшие фильмы по жанру\n";
	cout << "\t6 - показать информацию о фильме\n";
	cout << "\t7 - добавить фильм\n";
	cout << "\t8 - удалить фильм\n";
	cout << "\t9 - редактировать запись\n";
	cout << "\t10 - поиск фильма\n";
	cout << "\t11 - посмотреть фильм\n";
	cout << "\t12 - просмотр записей с фильтрами\n";
	cout << "\t13 - найти похожие фильмы\n";
	cout << "\t14 - выйти\n";
}

int main()
{
	while (true)
	{
		try
		{

			setlocale(LC_ALL, "ru");

			FilmCollection* x = new FilmCollection();

			FilmCollection films;
			SimilarFilms similar;

			similar.LoadFromFile();

			int code;

			while (true)
			{
				system("cls");
				PrintMenu();
				if(!(cin >> code)) throw(exception("Некорректный ввод! Повторите попытку!"));
				getchar();
				fflush(stdin);
				system("cls");

				switch (code)
				{
				case 1:
				{
					cout << endl << "\t[ СПИСОК ВСЕХ ФИЛЬМОВ ]" << endl;
					PrintHeader();
					for (Film* f : x->GetCollection())
						f->Print();
					cout << "______________________________________________________________\n";
					break;
				}
				case 2:
				{
					cout << endl << "\t[ ИСТОРИЯ ПРОСМОТРА ]" << endl;
					PrintHeader();
					for (Film* f : x->GetViews())
						f->Print();
					cout << "______________________________________________________________\n";
					break;
				}
				case 3:
				{
					cout << endl << "\t[ РЕКОМЕНДАЦИИ ]" << endl;
					PrintHeader();
					for (Film* f : x->GetRecomendations())
						f->Print();
					cout << "______________________________________________________________\n";
					break;
				}
				case 4:
				{
					cout << endl << "\t[ ЛУЧШИЕ ФИЛЬМЫ ]" << endl;
					TopFilms* top = new TopFilms();
					PrintHeader();
					for (Film* f : top->GetTop())
						f->Print();
					cout << "______________________________________________________________\n";
					break;
				}
				case 5:
				{
					cout << endl << "\t[ ЛУЧШИЕ ФИЛЬМЫ ПО ЖАНРУ ]" << endl;
					cout << "Введите жанр, чтобы увидеть его лучшие фильмы:\n\n";
					string _genre;
					getline(cin, _genre);
					TopGenreFilms* top = new TopGenreFilms();
					PrintHeader();
					for (Film* f : top->GetTopGenre(_genre))
						f->Print();
					cout << "______________________________________________________________\n";
					break;
				}
				case 6:
				{
					cout << endl << "\t[ ИНФОРМАЦИЯ О ФИЛЬМЕ ]" << endl;
					x->ViewInfo();
					break;
				}
				case 7:
				{
					cout << endl << "\t[ ДОБАВЛЕНИЕ ФИЛЬМА ]" << endl;
					Film* tmp = new Film();
					cout << "Введите название: ";

					getline(cin, tmp->title);

					cout << "Введите жанр: ";
					cin >> tmp->genre;
					cout << "Введите рейтинг: ";
					if(!(cin >> tmp->score)) throw(exception("Некорректный ввод! Повторите попытку!"));
					if(tmp->score<0||tmp->score>10) throw(exception("Некорректный ввод! Повторите попытку!"));
					cout << "Введите год выпуска: ";
					if(!(cin >> tmp->year)) throw(exception("Некорректный ввод! Повторите попытку!"));
					tmp->info = "No data avaible";
					tmp->actors = "No data avaible";

					x->Add(tmp);
					break;
				}
				case 8:
				{
					cout << endl << "\t[ УДАЛЕНИЕ ФИЛЬМА ]" << endl;
					PrintHeader();
					for (Film* f : x->GetCollection()) f->Print();
					cout << "\n\nВведите название: ";
					string title;
					getline(cin, title);

					x->Delete(title);
					break;
				}
				case 9:
				{
					cout << endl << "\t[ РЕДАКТИРОВАНИЕ ФИЛЬМА ]" << endl;
					x->Edit();
					break;
				}
				case 10:
				{
					cout << endl << "\t[ ПОИСК ФИЛЬМА ]" << endl;
					x->Search();
					break;
				}
				case 11:
				{
					cout << endl << "\t[ ПОСМОТРЕТЬ ФИЛЬМ ]" << endl;
					int select = 0;
					while (select != 1 && select != 2 && select != 3)
					{
						cout << "Выберите: " << endl << endl
							<< "1. Найти и посмотреть фильм." << endl
							<< "2. Посмотреть рандомный фильм из рекомендаций." << endl
							<< "3. Вернуться в меню." << endl << endl
							<< "-> ";
						cin.clear();
						if(!(cin >> select)) throw(exception("Некорректный ввод! Повторите попытку!"));
						cin.ignore();
					}
					switch (select)
					{
					case 1:
					{
						x->ViewFilm();
						break;
					}
					case 2:
					{
						Film* film = x->GetRandomFilm();
						cout << "Фильм '" << film->title << "' был просмотрен!";
						x->ViewRandFilmFromRec(film);
						break;
					}
					default: break;
					}
					break;
				}
				case 12:
				{
					cout << endl << "\t[ ПРОСМОТР С СОРТИРОВКОЙ ]" << endl;
					x->Sort();
					break;
				}
				case 13:
				{
					cout << endl << "\t[ ПОИСК ПОХОЖЕГО ФИЛЬМА ]" << endl;
					PrintHeader();
					for (Film* f : x->GetCollection())
						f->Print();
					similar.CheckSimilarFilms(&films);
					break;
				}
				case 14: return 0;
				default:	break;
				}

				cout << "\npress any key for exit to menu\n";
				rewind(stdin);
				getchar();
			}
			break;
		}
		catch (Exception exc)
		{
			cout << endl << exc.message << endl << endl;
			system("pause");
		}
		catch (const exception& ex)
		{
			cout << ex.what() << endl;
			cin.clear();
			cin.ignore(100, '\n');
			system("pause");
		}
	}
	return 0;
}
