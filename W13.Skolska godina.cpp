#include<iostream>
using namespace std;
#include <string>
#include <regex>
#include <algorithm>
#include<thread>
#include <functional>

template<class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutnoElemenata;
public:
	Kolekcija()
	{
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutnoElemenata = 0;
	}
	void operator=(Kolekcija& k)
	{
		_elementi1 = new T1[k._trenutnoElemenata];
		_elementi2 = new T2[k._trenutnoElemenata];

		for (size_t i = 0; i < k._trenutnoElemenata; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}

		_trenutnoElemenata = k._trenutnoElemenata;
	}
	Kolekcija(const Kolekcija& k)
	{
		_elementi1 = new T1[k._trenutnoElemenata];
		_elementi2 = new T2[k._trenutnoElemenata];

		for (size_t i = 0; i < k._trenutnoElemenata; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}

		_trenutnoElemenata = k._trenutnoElemenata;
	}

	Kolekcija& operator=(const Kolekcija& k)
	{
		if (this != &k)
		{
			delete[] _elementi1;
			delete[] _elementi2;

			_elementi1 = new T1[k._trenutnoElemenata];
			_elementi2 = new T2[k._trenutnoElemenata];

			for (size_t i = 0; i < k._trenutnoElemenata; i++)
			{
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];
			}

			_trenutnoElemenata = k._trenutnoElemenata;
		}

		return *this;
	}

	~Kolekcija()
	{
		delete[] _elementi1;
		_elementi1 = nullptr;

		delete[] _elementi2;
		_elementi2 = nullptr;
	}

	/*1. AddElement :: omogucava dodavanje novog elementa u kolekciju.
	Osigurati automatsko prosiranje kolekcije prilikom dodavanja svakog novog elementa, te onemoguciti ponavljanje elemenata.
	Ukoliko se pokusa dodati dupli element funkcija vraca index pronadjenog duplog elementa, u suprotnom vraca vrijednost -1.*/

	int AddElement(T1& el1, T2& el2)
	{
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			if (_elementi1[i] == el1 && _elementi2[i] == el2)
				return i;
		}

		T1* temp1 = new T1[_trenutnoElemenata + 1];
		T2* temp2 = new T2[_trenutnoElemenata + 1];

		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[_trenutnoElemenata] = el1;
		_elementi2[_trenutnoElemenata] = el2;
		_trenutnoElemenata++;

		return -1;
	}

	/*2. RemoveElement :: na osnovu parametra tipa T1 uklanja elemente iz kolekcije i smanjuje velicinu niza.
	Ukoliko je potrebno, prilikom uklanjanja elementa izvrsiti dealokaciju. Prilikom uklanjanja elemenata ocuvati redoslijed
	njihovog dodavanja.*/

	bool RemoveElement(T1& el)
	{
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			if (_elementi1[i] == el)
			{
				T1* temp1 = new T1[_trenutnoElemenata - 1];
				T2* temp2 = new T2[_trenutnoElemenata - 1];

				int poz = 0;
				for (size_t j = 0; j < _trenutnoElemenata; j++)
				{
					if (j != i)
					{
						temp1[poz] = _elementi1[j];
						temp2[poz++] = _elementi2[j];
					}
				}

				delete[] _elementi1;
				delete[] _elementi2;

				_elementi1 = temp1;
				_elementi2 = temp2;
				_trenutnoElemenata--;

				return true;

			}
		}

		return false;
	}

	void RemoveAll()
	{
		delete[] _elementi1;
		_elementi1 = nullptr;

		delete[] _elementi2;
		_elementi2 = nullptr;

		_trenutnoElemenata = 0;
	}

	int GetTrenutno()const { return _trenutnoElemenata; }

	T1& GetElement1(int index)
	{
		if (index < 0 || index >= _trenutnoElemenata)
		{
			throw exception("Index was out of range.");
		}

		return _elementi1[index];
	}

	T2& GetElement2(int index)
	{
		if (index < 0 || index >= _trenutnoElemenata)
		{
			throw exception("Index was out of range.");
		}

		return _elementi2[index];
	}

	friend ostream& operator<<<>(ostream&, const Kolekcija&);
};

template<class T1, class T2>
ostream& operator<<<>(ostream& cout, const Kolekcija<T1, T2>& k)
{
	for (size_t i = 0; i < k._trenutnoElemenata; i++)
	{
		cout << k._elementi1[i] << "\n" << k._elementi2[i] << "\n\n";
	}

	return cout;
}

class Student {
	int _brojIndeksa;
	string _ime;
	string _prezime;
	string _korisnickomIme;
	string _lozinka;
	/*1. potrebne funkcije
	NAPOMENE:
	2. lozinka mora postovati sljedeca pravila:
	2.a: sadrzi kombinaciju slova, brojeva i nekog od specijalnih znakova
	2.b: ne smije sadrzavati niti ime niti prezime
	2.c: sadrzi minimalno 6 znakova
	postivanje navedenih pravila se mora osigurati prije kreiranje objekta tipa Student*/
public:
	int getBrIndexa() { return _brojIndeksa; }
	Student(int brIndeksa, string ime, string prezime, string userName, string lozinka)
	{
		string Provjera = "(?=.{6,})(?=.*[a-zA-Z])(?=.*[0-9])";
		string Provjera2 = "(?:(" + ime + "|" + prezime + "))";
		if (!regex_search(lozinka, regex(Provjera)) || regex_search(lozinka, regex(Provjera2)))
		{
			SetDefaultValues();
			throw exception("Sifra nije validna");
		}
		_brojIndeksa = brIndeksa;
		_ime = ime;
		_prezime = prezime;
		_korisnickomIme = userName;
		_lozinka = lozinka;
	}
	Student(Student &s)
	{
		_brojIndeksa = s._brojIndeksa;
		_ime = s._ime;
		_prezime = s._prezime;
		_korisnickomIme = s._korisnickomIme;
		_lozinka = s._lozinka;
	}
	Student()
	{
		_brojIndeksa = 0;
		_ime = "ime";
		_prezime = "prezime";
		_korisnickomIme = "userName";
		_lozinka = "lozinka";
	}
	friend bool operator==(const Student &s, const Student & s2)
	{
		return (s._brojIndeksa == s2._brojIndeksa);
	}
	void SetDefaultValues()
	{
		_brojIndeksa = 0;
		_ime = "ime";
		_prezime = "prezime";
		_korisnickomIme = "userName";
		_lozinka = "lozinka";
	}
	friend ostream & operator<<(ostream & out, Student &s)
	{
		out << s._ime << endl << s._prezime << endl << s._korisnickomIme << endl << s._brojIndeksa;
		return out;
	}
};

class Ispit {
protected:
	string _predmet;
	char * _opis;//npr. PR2::Integralni::30.01.2015
				 /*tip string u kolekciji sluzi kao napomena vezana za ponasanje studenta na ispitu gdje ce se
				 evidentirati utisci dezurnog nastavnika npr. kasnio, dosao bez indeksa, izbacen zbog prepisivanja*/
	Kolekcija<Student, string>  * _listaStudenata;
public:
	string getPredmet() { return _predmet; }
	bool Prisutan(int index)//L7
	{
		for (size_t i = 0; i < _listaStudenata->GetTrenutno(); i++)
			if (_listaStudenata->GetElement1(i).getBrIndexa() == index)
				return true;
		return false;
	}
	Ispit(string predmet, const char* opis)
	{
		_predmet = predmet;
		_opis = new char[strlen(opis) + 1];
		strcpy_s(_opis, strlen(opis) + 1, opis);
		_listaStudenata = new Kolekcija<Student, string>;
	}
	Ispit()
	{
		_predmet = "predmet";
		_opis = new char[strlen("opis") + 1];
		strcpy_s(_opis, strlen("opis") + 1, "opis");
		_listaStudenata = new Kolekcija<Student, string>;
	}
	Ispit(const Ispit &i)
	{
		_predmet = i._predmet;
		_opis = new char[strlen(i._opis) + 1];
		strcpy_s(_opis, strlen(i._opis) + 1, i._opis);
		_listaStudenata = new Kolekcija<Student, string>(*i._listaStudenata);
	}
	void operator=(const Ispit &i) //TODO:Larisa: Novi kompajler ovo ne prepoznaje kao validan operator=, treba const. //L1?
	{
		_predmet = i._predmet;
		_opis = new char[strlen(i._opis) + 1];
		strcpy_s(_opis, strlen(i._opis) + 1, i._opis);
		_listaStudenata = new Kolekcija<Student, string>(*i._listaStudenata);
	}
	~Ispit()
	{
		delete[] _opis;
		delete _listaStudenata;
	}

	//TODO:Larisa: Iako je ovo trik da se postigne željeni efekat, općenito nije dobra praksa
	//jer se operator< ne ponaša na očekivani način.
	bool operator<(const Ispit &i)
	{
		return (_listaStudenata->GetTrenutno() < i._listaStudenata->GetTrenutno());//L2

	}
	friend bool operator>(const Ispit& i1, const Ispit& i2)
	{
		return i1._listaStudenata->GetTrenutno() > i2._listaStudenata->GetTrenutno();
	}
	/*1. AddStudent :: dodaje podatke o studentu koji je pristupio ispitu uz odgovarajucu napomenu (napomena nije obavezna).
	Studentu nije dozvoljeno da dva puta pristupi istom ispitu, a u slucaju da pokusa pristupiti automatski se dodaje adekvatna napomena
	(npr. ponovo pokusao pristupiti). */
	void AddStudent(Student &s, string napomena = " ")
	{
		//TODO:Larisa: Metoda AddElement je već vraćala poziciju, mogao se samo unutar nje uslov provjere
		//prilagoditi da vraća dupli u slučaju da je u bilo kojoj kolekciji pronađen (&& umjesto ||).
		//Ovo rješenje nije do kraja ispravno jer se student ipak ponovo dodaje na kraju metode 
		//umjesto da mu se samo promijeni napomena.
		//TODO:unutar nje uslov provjere prilagoditi da vraća dupli u slučaju da je u bilo kojoj kolekciji pronađen (&& umjesto ||) ??
		if (int a=_listaStudenata->AddElement(s, napomena) != -1)
		{
			_listaStudenata->GetElement2(a)= "ponovo pokusao pristupiti";
			throw exception("Student ne moze ponovo pristupiti!");
			//L3
		}

	}
	/*2. Remove - Uklanja podatke o svim studentima koji su vise puta pokusali pristupiti ispitu.*/
	void Remove()
	{
		string napomena = "ponovo pokusao pristupiti";
		for (size_t i = 0; i < _listaStudenata->GetTrenutno(); i++)
		{
			//TODO:Larisa: Nepotrebno korišten regex.
			if (_listaStudenata->GetElement2(i)== napomena)
				_listaStudenata->RemoveElement(_listaStudenata->GetElement1(i));
			//L4 ? 
		}
	}
	Kolekcija<Student, string> GetLista() { return *_listaStudenata; }
};

class SkolskaGodina {
	string _skolskaGodina;//npr.2014/2015
	vector<Ispit> _odrzaniIspiti;

	/*1. AddIspit::dodate podatke o odrzanom ispitu
	2. RemoveIspit::uklanja podatke o odrzanom ispitu, ali samo pod uslovom da tom ispitu nije pristupio niti jedan student
	3. GetBrojIzlazakaByPredmet::vraca podatak o tome koliko je puta trazeni student izasao na ispit iz odredjenog predmeta u
	toku skolske godine

	4. Sortira sve podatke o ispitima silazno prema broju studenata koji su im pristupali. Prilikom testiranja metodu pozvati u
	zasebnom thread-u.
	5. operator<<::ispisuje sve dostupne podatke o odrzanim ispitima i studentima koji su im pristupili.*/
public:
	SkolskaGodina(string godina) { _skolskaGodina = godina; }
	void AddIspit(const Ispit &i)
	{
		_odrzaniIspiti.push_back(i);
	}

	//TODO:Larisa: Ne uklanja se svaki ispit nego konkretan, onaj koji se proslijedi kao parametar.
	void RemoveIspit(int index)
	{
		for (size_t i = 0; i < _odrzaniIspiti.size(); i++)
		{
			if (_odrzaniIspiti[i].GetLista().GetTrenutno() == 0 && i == index)
			{
				_odrzaniIspiti.erase(_odrzaniIspiti.begin() + i);
				//L6
			}
		}
	}

	//TODO:Larisa: Ovdje je potrebno proslijediti i predmet za koji se broje izlasci.
	//Dodati jednu pomoćnu metodu u klasi Ispit koja provjerava da li je određeni student bio na ispitu.
	//Rješenje će biti jasnije i bolje napisano. Funkciji je dovoljno proslijediti broj indeksa studenta, uz naziv predmeta.

	int GetBrojIzlazakaByPredmet(int index, string predmet)
	{
		int izlasci = 0;
		for (size_t i = 0; i < _odrzaniIspiti.size(); i++)
		{
			if (_odrzaniIspiti[i].Prisutan(index) && _odrzaniIspiti[i].getPredmet() == predmet)//L7
			{
				izlasci++;
			}
		}
		return izlasci;
	}
	void Sortiraj()
	{
		//sort(_odrzaniIspiti.begin(), _odrzaniIspiti.end());
		sort(_odrzaniIspiti.begin(), _odrzaniIspiti.end(), std::greater<>());
	}
	friend ostream & operator<<(ostream & out, SkolskaGodina &s)
	{
		out << "Skolska godina:" << s._skolskaGodina << endl;
		for (size_t i = 0; i < s._odrzaniIspiti.size(); i++)
		{
			//TODO:Larisa: Zašto ne iskoristiti preklopljeni operator za ispis kolekcije?
			out << s._odrzaniIspiti[i].GetLista();
			//L8
		}
		return out;
	}
};

int main() {
	//izbjegavajte koristenje vlastitog imena i prezimena
	//provjeriti validnost izvrsenja svih dostupnih funkcionalnosti na objektu tipa SkolskaGodina	
	/*try {
	Student ajdin(160080, "ajdin", "ljubuncic", "ajdinlj", "ajdn!123");
	Student niko(161080, "niko", "nikic", "nitkonik", "idn!123");
	Student larisa(160030, "larisa", "dedovic", "lar", "lar?123");
	Ispit ispit1("Pr", "opis");
	Ispit ispit2("Dm", "neki opis");
	Ispit ispit3("ASP", "");
	ispit1.AddStudent(ajdin, "nije prisutvovao predmetu");
	ispit2.AddStudent(niko, "prisustvovao predmetu");
	ispit3.AddStudent(larisa);
	ispit3.AddStudent(ajdin);
	ispit2.AddStudent(larisa, "prisustvovala predmetu");

	SkolskaGodina s("2015");
	SkolskaGodina s2("2012");
	SkolskaGodina s3("2017");
	s.AddIspit(ispit1);
	s.AddIspit(ispit2);
	s.AddIspit(ispit3);
	s2.AddIspit(ispit2);
	s.GetBrojIzlazakaByPredmet(ajdin);
	s.GetBrojIzlazakaByPredmet(larisa);
	cout << s << endl;
	s.Sortiraj();
	cout << s;
	}
	catch (exception ex)
	{
	cout << ex.what();
	}*/

	try
	{
		Student s1(1, "Larisa", "Dedovic", "lara", "Lara%2");
		Student s2(2, "Denis", "Music", "denis", "FIT%25");

		Ispit i1("Programiranje III", "PRIII::Integralni::30.01.2017");
		Ispit i2("Razvoj softvera I", "RSI::Integralni::15.01.2017");


		i1.AddStudent(s1);
		i1.AddStudent(s2);

		i2.AddStudent(s1);

		SkolskaGodina sk2017("2017/2018");
		sk2017.AddIspit(i1);
		sk2017.AddIspit(i2);

		sk2017.Sortiraj();
		thread sortThread(&SkolskaGodina::Sortiraj, &(sk2017));
		sortThread.join();

		cout << sk2017 << endl;
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}



	system("pause");
	return 0;
}

