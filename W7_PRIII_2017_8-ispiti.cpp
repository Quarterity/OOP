#include<iostream>
using namespace std;

class Ispit
{
protected:
	char* _predmet;
	char _datumOdrzavanja[12]; //
	char _vrijemeOdrzavanja[6]; // početak ispita
	int _trajanje; //trajanje ispita u minutama
	int _maxBrojBodova;
	double _rezultat; //procentualni rezultat ispita
public:

	Ispit(char* predmet="---", char* datumOdrzavanja="---", char* vrijemeOdrzavanja="---", int trajanje =0,int maxBodova=0,double rezultat=0)
	{
		int vel = strlen(predmet) + 1;
		_predmet = new char[vel];
		strcpy_s(_predmet, vel, predmet);

		strncpy_s(_datumOdrzavanja, datumOdrzavanja, _TRUNCATE);
		strncpy_s(_vrijemeOdrzavanja, vrijemeOdrzavanja, _TRUNCATE);

		_trajanje = trajanje;
		_maxBrojBodova = maxBodova;
		_rezultat = rezultat;

	}
	Ispit(Ispit & i)
	{
		int vel = strlen(i._predmet) + 1;
		_predmet = new char[vel];
		strcpy_s(_predmet, vel, i._predmet);

		strncpy_s(_datumOdrzavanja, i._datumOdrzavanja, _TRUNCATE);
		strncpy_s(_vrijemeOdrzavanja, i._vrijemeOdrzavanja, _TRUNCATE);

		_trajanje = i._trajanje;
		_maxBrojBodova = i._maxBrojBodova;
		_rezultat = i._rezultat;

	}
	void SetAll(Ispit & i)
	{
		int vel = strlen(i._predmet) + 1;
		_predmet = new char[vel];
		strcpy_s(_predmet, vel, i._predmet);

		strncpy_s(_datumOdrzavanja, i._datumOdrzavanja, _TRUNCATE);
		strncpy_s(_vrijemeOdrzavanja, i._vrijemeOdrzavanja, _TRUNCATE);

		_trajanje = i._trajanje;
		_maxBrojBodova = i._maxBrojBodova;
		_rezultat = i._rezultat;

	}

	void operator=(Ispit & i)
	{
		int vel = strlen(i._predmet) + 1;
		_predmet = new char[vel];
		strcpy_s(_predmet, vel, i._predmet);

		strncpy_s(_datumOdrzavanja, i._datumOdrzavanja, _TRUNCATE);
		strncpy_s(_vrijemeOdrzavanja, i._vrijemeOdrzavanja, _TRUNCATE);

		_trajanje = i._trajanje;
		_maxBrojBodova = i._maxBrojBodova;
		_rezultat = i._rezultat;

	}

	~Ispit()
	{
		delete[] _predmet;
		_predmet = NULL;
	}
	//TODO:Preklopiti ispis za Ispit!
	friend ostream & operator<<(ostream & cout, Ispit &ispit)
	{
		cout <<endl<< ispit._predmet <<endl<<"Datum:";
		for (size_t i = 0; i < 10; i++)
			cout << ispit._datumOdrzavanja[i];
		cout << " - ";
		for (size_t i = 0; i < 6; i++)
			cout<<ispit._vrijemeOdrzavanja[i];
		cout <<endl<<"Trajanje:"<< ispit._trajanje << endl <<"Max bodova:"<< ispit._maxBrojBodova << endl <<"rezultat:"<< ispit._rezultat<<endl;
		return cout;
	}
};

template <class T1, class T2, int max>
class Kolekcija
{
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutno;
public:
	//Implementirati potrebne funkcije za rad sa kolekcijom
	Kolekcija();
	Kolekcija(Kolekcija &);
	friend ostream & operator<<<>(ostream & cout, Kolekcija &k);
	int GetTrenutno();
	void SetTrenutno(int temp) { _trenutno = temp; }
	T1& GetPrviNiz(); 
	T2& GetDrugiNiz();
	T1& operator[](int i) {
		if (i >= 0 || i < _trenutno)
		{
			return _elementi1[i];
		}
	}

	T2& GetT2(int i) { return _elementi2[i]; }
	void DodajElemente(T1 &, T2 &);

};
template <class T1, class T2, int max>
void Kolekcija<T1, T2, max>::DodajElemente(T1 & el1, T2 & el2)
{
	if (_trenutno<max)
	{
	_elementi1[_trenutno] = el1;
	_elementi2[_trenutno++] = el2;
	}
	else throw exception("Greska! Niz je dosegao max");
}
template <class T1,class T2,int max>
Kolekcija<T1,T2,max>::Kolekcija() { _trenutno = 0; }
template <class T1,class T2,int max>
Kolekcija<T1,T2,max>::Kolekcija(Kolekcija<T1, T2,max> & k) {
	for (size_t i = 0; i < k._trenutno; i++)
	{
		_elementi1[i] = k._elementi1[i];
		_elementi2[i] = k._elementi2[i];
	}
	_trenutno = k._trenutno();
}
template <class T1,class T2,int max>
 ostream & operator<<<>(ostream & cout, Kolekcija<T1,T2,max> &k) {
	for (size_t i = 0; i < k._trenutno; i++)
	{
		cout << k._elementi1[i] << " - " << k._elementi2[i];
	}
	cout << endl;
	return cout;
}
template <class T1,class T2,int max>
int Kolekcija<T1, T2, max>::GetTrenutno() { return _trenutno; }
template <class T1, class T2, int max>
T1& Kolekcija<T1,T2,max>::GetPrviNiz() { return *_elementi1; }
template <class T1, class T2, int max>
T2& Kolekcija<T1, T2, max>::GetDrugiNiz() { return *_elementi2; }

class PismeniIspit: public Ispit
{
	Kolekcija<char*, int, 50>* _pitanja; //kolekcija pitanja sa brojem bodova
	Kolekcija<char*, int, 50> _odgovori; //kolekcija odgovora sa brojem osvojenih bodova
	int _brTacnihOdgovora;
	int _brOsvojenihBodova;
public:

	//Konstruktor i destruktor funkcije
	PismeniIspit(Ispit *);
	//Konstruktor kopije
	PismeniIspit(PismeniIspit &ispit);
	//Operator() za dodavanje pitanja u kolekciju uz pripadajući broj bodova.
	//Ispit ne smije sadržavati ista pitanja. Broj bodova na jednom pitanju ne smije biti negativan.
	void operator()(char* pitanje, int Maxbodovi);
	//Operator+= za dodavanje odgovora na pitanje. Odgovori se dodaju u redoslijedu dodavanja pitanja.
	//Broj osvojenih bodova se inicijalno postavlja na 0. Moguće je dodati i prazan tekst kao odgovor.
	void operator+=(char *odgovor );
	//Funkciju za ocjenu tačnog odgovora koja kao parametar prima redni broj pitanja u kolekciji i tačan odgovor. 
	//Funkcija treba da pronađe odgovor na traženo pitanje, te da taj odgovor uporedi sa tačnim odgovorom i postavi
	//osvojene bodove. Student može osvojiti 0 ili maksimalan broj bodova na pitanju. U skladu sa rezultatom
	//provjere postaviti vrijednosti odgovarajućih atributa.
	void ProvjeriOdgovor(int rb, char* tacanOdgovor);
	void Info();
};
void PismeniIspit::Info() {
	cout << Ispit(*this);
	cout << "Pismeni:" <<endl<<"Pitanje:"<< *_pitanja;
	cout <<"Odgovor:"<< _odgovori;
	cout <<"Tacnih odgovora:"<< _brTacnihOdgovora<<endl<<"Osvojenih bodova:"<<_brOsvojenihBodova<<endl;
}
PismeniIspit::PismeniIspit(Ispit * ispit = new Ispit ("---","---","---",0)) :Ispit::Ispit(*ispit) {
	_pitanja = new Kolekcija<char*, int, 50>;
	_brOsvojenihBodova = 0;
	_brTacnihOdgovora = 0; }

//Konstruktor kopije
PismeniIspit::PismeniIspit(PismeniIspit &ispit) :Ispit::Ispit(ispit) {
	_pitanja = new Kolekcija<char*, int, 50>;
	for (size_t i = 0; i <ispit._pitanja->GetTrenutno() ; i++)
	{
		//TODO:Operator [] koristiti zadano.
		(*_pitanja)[i] = ispit._pitanja->operator[](i);
		(*_pitanja).GetT2(i) = ispit._pitanja->GetT2(i);
	}
	_pitanja->SetTrenutno(ispit._pitanja->GetTrenutno());
	for (size_t i = 0; i < ispit._odgovori.GetTrenutno(); i++)
	{
		_odgovori[i] = ispit._odgovori[i];
		_odgovori.GetT2(i) = ispit._odgovori.GetT2(i);
	}
	_odgovori.SetTrenutno(ispit._odgovori.GetTrenutno());
	_brOsvojenihBodova = ispit._brOsvojenihBodova;
	_brTacnihOdgovora = ispit._brTacnihOdgovora;

}
//Operator() za dodavanje pitanja u kolekciju uz pripadajući broj bodova.
//Ispit ne smije sadržavati ista pitanja. Broj bodova na jednom pitanju ne smije biti negativan.
void PismeniIspit::operator()(char* pitanje, int Maxbodovi)
{
	for (size_t i = 0; i < _pitanja->GetTrenutno(); i++)
		if (strcmp((*_pitanja)[i],pitanje)==0)
			return;
	if (Maxbodovi>=0)
	{
		_pitanja->DodajElemente(pitanje, Maxbodovi);
	}
}
//Operator+= za dodavanje odgovora na pitanje. Odgovori se dodaju u redoslijedu dodavanja pitanja.
//Broj osvojenih bodova se inicijalno postavlja na 0. Moguće je dodati i prazan tekst kao odgovor.
void PismeniIspit::operator+=(char *odgovor)
{
	//TODO:napraviti fju DodajElement u kolekciji.
	int a = 0;
	_odgovori.DodajElemente(odgovor,a);
}
//Funkciju za ocjenu tačnog odgovora koja kao parametar prima redni broj pitanja u kolekciji i tačan odgovor. 
//Funkcija treba da pronađe odgovor na traženo pitanje, te da taj odgovor uporedi sa tačnim odgovorom i postavi
//osvojene bodove. Student može osvojiti 0 ili maksimalan broj bodova na pitanju. U skladu sa rezultatom
//provjere postaviti vrijednosti odgovarajućih atributa.
void PismeniIspit::ProvjeriOdgovor(int rb, char* tacanOdgovor)
{
	if (rb<_odgovori.GetTrenutno())
		{
			if (strcmp(_odgovori[rb],tacanOdgovor)==0)
			{
				_odgovori.GetT2(rb) = (*_pitanja).GetT2(rb);
				_brTacnihOdgovora++;
				_brOsvojenihBodova += (*_pitanja).GetT2(rb);
			}
			else _odgovori.GetT2(rb) = 0;
		}
	

}

class PrakticniIspit:public Ispit
{
	Kolekcija<char*, char*, 3> _alati; //potrebni alati i njihove verzije, npr. alat "Visual Studio", verzija "2013"
	char* _zadatak; //tekst zadatka
	bool _zadovoljeno; //true ako je student zadovoljio na ispitu, u suprotnom false
public:
	//Na osnovu prethodne implementacije klase PismeniIspit osmisliti i implementirati
	//potrebne funkcije za rad sa klasom PrakticniIspit.
	PrakticniIspit(Ispit * ispit = new Ispit ("---","---","---",0) ,char* zadatak="---"):Ispit::Ispit(*ispit),_zadovoljeno(false) {
		_zadatak = new char[strlen(zadatak) + 1];
		strcpy_s(_zadatak, strlen(zadatak) + 1, zadatak);
	}
	PrakticniIspit(PrakticniIspit &pi):Ispit(pi) {
		_zadatak = new char[strlen(pi._zadatak) + 1];
		strcpy_s(_zadatak, strlen(pi._zadatak) + 1, pi._zadatak);
		_zadovoljeno = pi._zadovoljeno;
		//_alati.SetTrenutno(pi._alati.GetTrenutno());
		for (size_t i = 0; i < pi._alati.GetTrenutno(); i++)
		{
			_alati.DodajElemente(pi._alati[i], pi._alati.GetT2(i));
		}
	}
	void operator=(PrakticniIspit &pi){
		Ispit::SetAll(pi);
		_zadatak = new char[strlen(pi._zadatak) + 1];
		strcpy_s(_zadatak, strlen(pi._zadatak) + 1, pi._zadatak);
		_zadovoljeno = pi._zadovoljeno;
		//_alati.SetTrenutno(pi._alati.GetTrenutno());
		for (size_t i = 0; i < pi._alati.GetTrenutno(); i++)
		{
			_alati.DodajElemente(pi._alati[i], pi._alati.GetT2(i));
		}
	}
	void DodajAlat(char* alat, char* verzija)
	{
		if (_alati.GetTrenutno()>3)
		{
			return;
		}
		_alati.DodajElemente(alat, verzija);
	}
	 ~PrakticniIspit() {
		 delete[ ] _zadatak;
	}

	void PregledajPrakticni( bool zadovoljeno = false)
	{
		if (strcmp(_zadatak,"---")!=0)
		{
			_zadovoljeno = zadovoljeno;
		}
	}
	void Info()
	{
		cout << Ispit(*this);
		cout <<endl<< "Prakticni:" << _zadatak<<endl;
		cout << _alati;
		if (!_zadovoljeno)
			cout << "Nije ";
		cout << "Zadovoljio"<<endl;
	}
};

int main()
{
	//Testirati sve dostupne funkcionalnosti putem datih kolekcija.
	//Drugi parametar kolekcije predstavlja broj indeksa studenta.
	Ispit ispit("Programiranje", "21.2.2017", "12:00", 120);
	PismeniIspit pismeni(&ispit);
	pismeni("Gdje se nalazi FIT", 3);
	pismeni += "U mostaru";
	pismeni.ProvjeriOdgovor(0, "U mostaru");
	PrakticniIspit prakticni(&ispit, "Napraviti program");
	prakticni.DodajAlat("Visual Studio", "Verzija 2017");
	prakticni.PregledajPrakticni(true);
	prakticni.Info();
	pismeni.Info();
	cout << ispit;
	Kolekcija<PismeniIspit, char*, 100> _pismeniIspiti;
	char* naziv = new char[strlen("programiranje") + 1];
	_pismeniIspiti.DodajElemente(pismeni, naziv);
	 strcpy_s(naziv, strlen("programiranje") + 1, "Programiranje");
	 Kolekcija<PrakticniIspit, char*, 100> _prakticniIspiti;
	_prakticniIspiti.DodajElemente(prakticni, naziv);
	PrakticniIspit pr(prakticni);
	cout << _prakticniIspiti;
	system("pause>0");
	return 0;
}
