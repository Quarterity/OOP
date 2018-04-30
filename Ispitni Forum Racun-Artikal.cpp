#include<iostream>
#include <exception>
#include<string>
#include <vector>
#include <thread>
#include <functional>
#include <sstream>
using namespace std;

char * crt = "\n----------------------------------------------------\n";
class Izuzetak :public exception
{
	string _funkcija;
	string _poruka;
public:
	Izuzetak(const char * poruka, string funkcija) :exception(poruka), _funkcija(funkcija), _poruka(poruka) { ; }
	const char* what()
	{
		_poruka += _funkcija;
		return const_cast<char *>(_poruka.c_str());
	}
};
template<class T1, class T2, int max>
class Kolekcija {
	T1 * _elementi1[max];
	T2 * _elementi2[max];
	int * _trenutno;
public:
	Kolekcija() :_trenutno(new int(0))
	{
		for (size_t i = 0; i < max; i++)
		{
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}
	Kolekcija(const Kolekcija & k) :_trenutno(new int(*k._trenutno))
	{
		for (size_t i = 0; i < *k._trenutno; i++)
		{
			_elementi1[i] = new T1(*k._elementi1[i]);//TODO:* ???
			_elementi2[i] = new T2(*k._elementi2[i]);
		}
	}
	Kolekcija & operator=(const Kolekcija &k)
	{
		if (this != &k)
		{
			Clear();
			//while (max<=*k._trenutno)
			//{
			//	//ProsiriNiz();
			//}
			_trenutno = new int(*k._trenutno);
			for (size_t i = 0; i < *k._trenutno; i++)
			{
				_elementi1[i] = new T1(*k._elementi1[i]);
				_elementi2[i] = new T2(*k._elementi2[i]);
			}
		}
		return *this;
	}
	bool AddElement(T1 el1, T2 el2)
	{
		if (*_trenutno == max)
		{
			//ProsiriNiz();
		}
		_elementi1[*_trenutno] = new T1(el1);
		_elementi2[(*_trenutno)++] = new T2(el2);
		return true;
	}
	//void ProsiriNiz()//TODO: tacno prosirnenje??
	//{
	//	T1 * temp1[max + 1];
	//	T2 * temp2[max + 1];
	//	for (size_t i = 0; i < *_trenutno; i++)
	//	{
	//		temp1[i] = new T1(*_elementi1[i]);
	//		delete _elementi1[i]; _elementi1[i] = nullptr;
	//		temp2[i] = new T2(*_elementi2[i]);
	//		delete _elementi2[i]; _elementi2[i] = nullptr;
	//	}
	//	for (size_t i = 0; i < max; i++)
	//	{
	//		if (*_trenutno == max - 1)
	//		{
	//			_elementi1[i] = nullptr;
	//			_elementi2[i] = nullptr;
	//		}
	//		else {
	//			_elementi1[i] = new T1(*temp1[i]);
	//			delete temp1[i]; temp1[i] = nullptr;
	//			_elementi2[i] = new T2(*temp2[i]);
	//			delete temp2[i]; temp2[i] = nullptr;
	//		}
	//	}
	//}
	int GetTrenutno() { return *_trenutno; }
	T1& GetElement1(int i) { return *_elementi1[i]; }
	T2 & GetElement2(int i) { return *_elementi2[i]; }
	//1. potrebne funkcije	
	~Kolekcija()
	{
		Clear();
	}
	void Clear()
	{
		for (size_t i = 0; i < *_trenutno; i++)
		{
			delete _elementi1[i];
			delete _elementi2[i];//TODO:???
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		delete _trenutno;
		_trenutno = nullptr;
	}
	friend ostream & operator<<(ostream & out, Kolekcija & k)
	{
		out << crt;
		for (size_t i = 0; i < k.GetTrenutno(); i++)
		{
			out << endl << k.GetElement1(i) << "-";
			out << k.GetElement2(i) << endl;
		}
		out << crt;
		return out;
	}
};
class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum & d)
	{
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);

	}
	Datum & operator=(const Datum &d)
	{
		if (this != &d)
		{
			Clear();
			_dan = new int(*d._dan);
			_mjesec = new int(*d._mjesec);
			_godina = new int(*d._godina);
		}
		return *this;
	}
	~Datum() {
		Clear();
	}
	void Clear()
	{
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, const Datum &obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}
	//1. potrebne funkcije

};
class Osoba {
protected:
	char * _imePrezime;
	Datum * _datumRodjenja;
public:
	Osoba(char * imePrezime, Datum datum) {
		_imePrezime = new char[strlen(imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
		_datumRodjenja = new Datum(datum);
	}
	Osoba()
	{
		_imePrezime = new char(' ');
		_datumRodjenja = new Datum();
	}
	Osoba(const Osoba & obj) {
		_imePrezime = new char[strlen(obj._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
		_datumRodjenja = new Datum(*obj._datumRodjenja);
	}
	Osoba & operator=(const Osoba & o)
	{
		if (this != &o)
		{
			//if (o._datumRodjenja != nullptr || o._imePrezime != nullptr); //TODO:smije li se raditi??
			//{
			Clear();
			//}
			_imePrezime = new char[strlen(o._imePrezime) + 1];
			strcpy_s(_imePrezime, strlen(o._imePrezime) + 1, o._imePrezime);
			_datumRodjenja = new Datum(*o._datumRodjenja);
		}
		return *this;
	}
	virtual ~Osoba() { delete[]_imePrezime; delete _datumRodjenja; }
	void Clear()
	{
		delete[]_imePrezime; delete _datumRodjenja;
	}
	//1. potrebne funkcije
	friend ostream & operator<<(ostream & out, Osoba & o)
	{
		out << o._imePrezime;
		out << "-" << *o._datumRodjenja << endl;
		return out;
	}
};






class Kupac :public Osoba {
	//svaki kupac ima popust koji mu se zaracunava prilikom svake kupovine
	//pocetni popust je 0.10%
	//svaka kupovina u iznosu 100 - 500KM povecava iznos popusta za 0.15%
	//svaka kupovina u iznosu 500 - 1000KM povecava iznos popusta za 0.25%
	//svaka kupovina u iznosu vecem od 1000KM povecava iznos popusta za 0.30%
	float _odobreniPopust;
public:
	//1. potrebne funkcije
	Kupac(char * imePrezime, Datum datum, float popust = 0) :_odobreniPopust(popust), Osoba((imePrezime), datum) { ; }
	Kupac(const Kupac &k) :Osoba(k), _odobreniPopust(k._odobreniPopust) { ; }
	Kupac & operator=(const Kupac &k)
	{
		if (this != &k)
		{
			//Osoba::Clear(); //TODO: Greska bila 2 puta pozvan destruktor za isti objekt
			Osoba::operator=(static_cast<Osoba>(k));
			_odobreniPopust = k._odobreniPopust;
		}
		return *this;
	}
	friend ostream & operator<<(ostream & out, const Kupac & k)
	{
		out << static_cast<Osoba>(k);
		out << k._odobreniPopust;
		return out;
	}
};
class Artikal {
	string _barCode;
	char * _naziv;
	float _cijena;
public:
	Artikal(char * naziv, float cijena, string barCode) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_cijena = cijena;
		_barCode = barCode;
	}
	Artikal(Artikal &a)
	{
		_barCode = a._barCode;
		_cijena = a._cijena;
		_naziv = new char[strlen(a._naziv) + 1];
		strcpy_s(_naziv, strlen(a._naziv) + 1, a._naziv);
	}
	Artikal & operator=(const Artikal &a)
	{
		if (this != &a)
		{
			delete[] _naziv;
			_barCode = a._barCode;
			_cijena = a._cijena;
			_naziv = new char[strlen(a._naziv) + 1];
			strcpy_s(_naziv, strlen(a._naziv) + 1, a._naziv);
		}
		return *this;
	}
	float GetCijena()
	{
		return _cijena;
	}
	friend bool operator==(const Artikal & a1, const Artikal & a2) { return a1._cijena == a2._cijena && a1._barCode == a2._barCode && a1._naziv == a2._naziv; }
	~Artikal() { delete[]_naziv; _naziv = nullptr; }
	friend ostream& operator<< (ostream &COUT, const Artikal &obj) { COUT << obj._naziv << " " << obj._cijena << " KM" << endl; return COUT; }
	//1. potrebne funkcije

};
int brojacRacuna = 0;

string GenerisiSljedeciBrojRacuna()
{
	brojacRacuna++;
	char brRacuna[10];
	stringstream a;
	a <<brojacRacuna;
	brRacuna[0] = 'R';
	brRacuna[1] = 'N';
	int br = 0;
	int kopi = brojacRacuna;
	while (kopi != 0) {
		kopi /= 10;
		br++;
	}
	br = 8 - br;
	for (size_t i = 2; i < br + 2; i++)
	{
		brRacuna[i] = '0';
	}
	for (size_t i = 9; i >br + 1; i--)
	{
		int neki =brojacRacuna/10  ;
		brRacuna[i] = char(neki);
	}

	return brRacuna;
}

class Racun {
	//format broja racuna: RN6005491
	//osigurati automatsko generisanje broja racuna pocevsi od RN0000001
	char _brojRacuna[10];
	Datum _datumKreiranja;
	//prvi argument je Artikal, a drugi kolicina/broj artikala koju je kupac uzeo
	Kolekcija<Artikal *, int, 15> * _stavkeRacuna;
	Kupac * _kupac;

public:
	Racun(string niz, Datum datumKreiranja, Kupac *neki) :_datumKreiranja(datumKreiranja)
	{
		for (size_t i = 0; i < 10; i++)
		{
			_brojRacuna[i] = niz[i];
		}
		//strcpy_s(_b)
		_kupac = new Kupac(*neki);
		_stavkeRacuna = new Kolekcija<Artikal*, int, 15>;
	}
	Racun(const Racun & r) :_datumKreiranja(r._datumKreiranja)
	{
		for (size_t i = 0; i < 10; i++)
		{
			_brojRacuna[i] = r._brojRacuna[i];
		}
		_kupac = new Kupac(*r._kupac);
		_stavkeRacuna = new Kolekcija<Artikal*, int, 15>;
		for (size_t i = 0; i < r._stavkeRacuna->GetTrenutno(); i++)
		{
			_stavkeRacuna->AddElement(new Artikal(*r._stavkeRacuna->GetElement1(i)), r._stavkeRacuna->GetElement2(i));
		}
	}
	~Racun()
	{
		delete _kupac;
		for (size_t i = 0; i < _stavkeRacuna->GetTrenutno(); i++)
		{
			delete _stavkeRacuna->GetElement1(i);
		}
		delete _stavkeRacuna;
	}
	/*
	1. potrebne (de)konstruktor i druge funkcije
	2. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom. Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko korisnik vise puta pokusa dodati isti artikal.
	3. operator<< - ispisuje racun u formatu prikazanom na slici(nije obavezno da bude identican, ali je svakako pozeljno).Nakon ispisa svakog racuna, ukoliko su ispunjeni definisani uslovi, kupcu se uvecava popust.
	4. Sortiraj - sortira artikle (rastuci) po kolicini novca kojom ucestvuju u ukupnom iznosu racuna
	*/
	void DodajArtikal(Artikal a, int kolicina)
	{
		bool isti = false;
		for (size_t i = 0; i < _stavkeRacuna->GetTrenutno(); i++)
		{
			if (_stavkeRacuna->GetElement1(i) == &a)
			{
				_stavkeRacuna->GetElement2(i) += kolicina;
				isti = true;
			}

		}
		if (!isti)
		{
			_stavkeRacuna->AddElement(new Artikal(a), kolicina);
		}
	}
	friend ostream & operator<<(ostream & out, Racun & r)
	{
		for (size_t i = 0; i < 10; i++)
		{
			out << r._brojRacuna[i];
		}
		out << endl << r._datumKreiranja << endl;
		out << *r._kupac;
		//out << *r._stavkeRacuna<<endl;
		for (size_t i = 0; i < r._stavkeRacuna->GetTrenutno(); i++)
		{
			out << *r._stavkeRacuna->GetElement1(i);
			out << r._stavkeRacuna->GetElement2(i);
		}
		return out;
	}
	void Zamijeni(int prvi, int drugi)
	{
		Artikal *temp1 = new Artikal(*_stavkeRacuna->GetElement1(prvi));
		int temp2 = _stavkeRacuna->GetElement2(prvi);
		*_stavkeRacuna->GetElement1(prvi) = *_stavkeRacuna->GetElement1(drugi);
		_stavkeRacuna->GetElement2(prvi) = _stavkeRacuna->GetElement2(drugi);
		*_stavkeRacuna->GetElement1(drugi) = *temp1;
		_stavkeRacuna->GetElement2(drugi) = temp2;


	}
	void Sortiraj()
	{
		int *prethodniIznos = new int[_stavkeRacuna->GetTrenutno()];
		for (size_t i = 0; i < _stavkeRacuna->GetTrenutno(); i++)
		{
			float cijena = _stavkeRacuna->GetElement1(i)->GetCijena();
			prethodniIznos[i] = cijena*_stavkeRacuna->GetElement2(i);
			for (size_t j = i; j > 0; j--)
			{
				if (prethodniIznos[j]<prethodniIznos[j - 1])
				{
					Zamijeni(j, j - 1);
				}
			}
		}
	}
};

void main() {

	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_.TXT
	5. NAZIV I LISTA PARAMETARA FUNKCIJA MORAJU BITI IDENTICNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.
	6. NIJE POTREBNO BACATI IZUZETKE.
	****************************************************************************/
#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region TestiranjeKolekcije
	Kolekcija<int, int, 15> kolekcija1;
	int brojElemenata = 15;
	for (size_t i = 0; i < brojElemenata; i++)
		kolekcija1.AddElement(i, i);
	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
	cout << kolekcija1 << endl;
	Kolekcija<int, int, 15> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;
	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;
	Kolekcija<int, int, 15> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
#pragma endregion
#pragma region TestiranjeKupca
	Kupac jasmin("Jasmin Azemovic", Datum(15, 2, 1980));
	Kupac adel("Adel Handzic", Datum(15, 10, 1981));
	Kupac jasmin2 = jasmin;
	cout << jasmin << endl << jasmin2 << endl;
	jasmin = jasmin2;
	cout << jasmin << endl << jasmin2 << endl;
#pragma endregion

#pragma region TestiranjeArtikla
	Artikal sokCppyNarandza("Cappy narandza", 2.10, "699511236545");
	Artikal cokoladaMilkaMilkins("Milka Milkins", 2.55, "622445875463");
	Artikal jogurtMeggle("Meggle jogurt", 2.55, "155338774458");

	cout << sokCppyNarandza << cokoladaMilkaMilkins << jogurtMeggle << endl;
	sokCppyNarandza = cokoladaMilkaMilkins;
	cout << sokCppyNarandza << endl;
	if (sokCppyNarandza == cokoladaMilkaMilkins)
		cout << "Artikli su isti" << endl;
#pragma endregion
#pragma region TestiranjeRacuna
	/*GenerisiSljedeciBrojRacuna - globalna funkcija koja generise i vraca naredni broj racuna u prethodno definisanom formatu*/
	Racun racun1(GenerisiSljedeciBrojRacuna(), danas, &jasmin);
	racun1.DodajArtikal(cokoladaMilkaMilkins, 15);
	racun1.DodajArtikal(sokCppyNarandza, 20);
	racun1.DodajArtikal(jogurtMeggle, 20);

	cout << racun1 << endl;

	Racun racun2(GenerisiSljedeciBrojRacuna(), sutra, &jasmin);
	racun2.DodajArtikal(cokoladaMilkaMilkins, 15);
	racun2.DodajArtikal(sokCppyNarandza, 20);
	racun2.DodajArtikal(jogurtMeggle, 20);
	cout << racun2 << endl;

	Racun racun3(racun2);
	cout << racun3 << endl;
	racun3.Sortiraj();

#pragma endregion
	system("pause");
}