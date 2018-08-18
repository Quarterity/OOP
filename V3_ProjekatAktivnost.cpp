#include<iostream>
using namespace std;

class Uposlenik {

	int _sifra; //Automatski uvecavati za 1 za svakog novog uspolenika.
	char _ime[30];
	char _prezime[30];
	char* _radnoMjesto;
	static const char* _kompanija;
	static int _rbsifre;

public:
	Uposlenik() :_sifra(_rbsifre++)
	{
		_radnoMjesto = new char('\0');
	}

	//Implementirati potrebne konstruktor i destruktor funkcije
	Uposlenik(char* ime, char* prezime, char* radnoMjesto) :_sifra(_rbsifre++)
	{

		strncpy_s(_ime, ime, _TRUNCATE);
		strncpy_s(_prezime, prezime, _TRUNCATE);

		int vel = strlen(radnoMjesto) + 1;
		_radnoMjesto = new char[vel];
		strcpy_s(_radnoMjesto, vel, radnoMjesto);

	}
	Uposlenik(const Uposlenik &u) :_sifra(_rbsifre++)//TODO::ili _rbsifra??
	{
		strncpy_s(_ime, u._ime, _TRUNCATE);
		strncpy_s(_prezime, u._prezime, _TRUNCATE);

		int vel = strlen(u._radnoMjesto) + 1;
		_radnoMjesto = new char[vel];
		strcpy_s(_radnoMjesto, vel, u._radnoMjesto);


	}
	Uposlenik & operator=(const Uposlenik &u)//TODO::ili _rbsifra??
	{
		strncpy_s(_ime, u._ime, _TRUNCATE);
		strncpy_s(_prezime, u._prezime, _TRUNCATE);
		if (_radnoMjesto != nullptr)
		{
			delete[] _radnoMjesto; _radnoMjesto = nullptr;
		}
		int vel = strlen(u._radnoMjesto) + 1;
		_radnoMjesto = new char[vel];
		strcpy_s(_radnoMjesto, vel, u._radnoMjesto);
		_sifra = u._sifra;
		return *this;
	}

	~Uposlenik()
	{
		if (_radnoMjesto != nullptr)
		{
			delete[] _radnoMjesto;
			_radnoMjesto = nullptr;

		}
	}

	//Ostale funkcije dodati po potrebi
	int & getSifra() { return _sifra; }
	void setSifra(int a) { _sifra = a; }
	char* getIme() { return _ime; }
	void Info()const
	{
		cout << "Sifra: " << _sifra << endl;
		cout << "Ime i prezime: " << _ime << " " << _prezime << endl;
		cout << "Radno mjesto: " << _radnoMjesto << endl;

	}
	friend bool operator==(const Uposlenik &u, const Uposlenik &u2);
	friend ostream & operator<<(ostream & out, Uposlenik &u)
	{
		out <<"ime uposlenika:"<< u._ime<<endl<<"Prezime uposlenik" << u._prezime << "Kompanije:" << u._kompanija << endl <<endl<<"Radno mjesto:"<< u._radnoMjesto <<endl<<"sifra:"<< u._sifra<<endl;
		return out;
	}
};
int Uposlenik::_rbsifre = 0;
const char * Uposlenik::_kompanija = ("FIT");
bool operator==(const Uposlenik &u, const Uposlenik & u2)
{
	if (strcmp(u._ime, u2._ime) == 0)
	{
		return true;
	}
	else return false;
}

class Aktivnost {
	char* _naziv;
	bool _zavrsena;
	int* _trajanje; //Izrazeno u danima
	Uposlenik* _uposlenici[5];
public:
	//Implementirati potrebne konstruktor i destruktor funkcije
	Aktivnost() :_naziv(new char('\0')), _trajanje(nullptr), _uposlenici{ nullptr } { ; }
	Aktivnost(string  &naziv, const bool & zavrsena, const int *& trajanje,/*const*/ Uposlenik **& _uposlenici, int & brojUposlenika) :_zavrsena(zavrsena), _trajanje(new int(*trajanje))
	{
		int brojDodanih=0;
		bool postoji = false;
		_naziv = new char[naziv.size() + 1];
		strcpy_s(_naziv, naziv.size() + 1, naziv.c_str());
		for (size_t i = 0; i < brojUposlenika; i++)
		{
			if (i == 5)
			{
				break;
			}
			Uposlenik * u = dynamic_cast<Uposlenik *>(_uposlenici[i]);//TODO:provjeriti velicinu poslanog niza ili vrijednosti u njemu pa dodijeliti moguci broj objekata saljem niz velicine 2 i pokusavam ga staviti u niz velicine 5
			if (u != nullptr && _uposlenici[i] != nullptr)
			{
				for (size_t y = 0; y < i; y++)
				{
					if (this->_uposlenici[y]->getSifra() == _uposlenici[i]->getSifra())
					{
						postoji = true;
					}
				}
				if (!postoji)
				{

					this->_uposlenici[i] = (_uposlenici[i]);
					_uposlenici[i] = nullptr;
					brojDodanih++;
				}
			}
		}
		brojUposlenika -= brojDodanih;
	}
	Aktivnost(const Aktivnost & a) :_zavrsena(a._zavrsena)
	{
		_naziv = new char[strlen(a._naziv) + 1];

		strcpy_s(_naziv, strlen(a._naziv) + 1, a._naziv);
		for (size_t i = 0; i < 5; i++)
		{
			if (a._uposlenici[i] != nullptr)
			{
				_uposlenici[i] = new Uposlenik(*a._uposlenici[i]);
			}
		}
		if (a._trajanje != nullptr)
		{
			_trajanje = (new int(*a._trajanje));

		}
	}
	Aktivnost & operator=(const Aktivnost & a)//TODO:provjeriti ispravnost operator= za aktivnsot!
	{
		if (this != &a)
		{
			if (_naziv != nullptr)
			{
				delete[] _naziv;
			}
			_naziv = new char[strlen(a._naziv) + 1];
			strcpy_s(_naziv, strlen(a._naziv) + 1, a._naziv);

			for (size_t i = 0; i < 5; i++)
			{
				if (a._uposlenici[i] != nullptr)
				{

					_uposlenici[i] = (a._uposlenici[i]);//TODO:provjeriti treba li new uposlenik op.=
				}
			}
			_zavrsena = (a._zavrsena);
			if (a._trajanje != nullptr)//TODO:pronaci bolje rjesenje za slanje defaultne Aktivnosti u operator= tj problem za dodoieljivanje vrijednosti iz nealocirane vrijednosti
			{
				_trajanje = (new int(*a._trajanje));

			}
		}
		return *this;
	}
	~Aktivnost()
	{
		delete[] _naziv; _naziv = nullptr;
		delete _trajanje; _trajanje = nullptr;
	}

	//Funkciju koja omogucava angazovanje uposlenika na projektnoj aktivnosti. Funkcija treba da vrati vrijednost tipa
	//bool za uspjesno, odnosno neuspjesno dodavanje uposlenika. Nije moguce dodati istog uposlenika vise puta. Nije moguce
	//angazovati uposlenike na vec zavrsenoj aktivnosti.
	bool angazujUposlenika(Uposlenik *& u)
	{
		for (size_t i = 0; i < 5; i++)
		{
			if (_uposlenici[i] != nullptr && &u != nullptr)//TODO:provjeravati "Uposlenik u" ??
			{
				if (*_uposlenici[i] == *u)
				{
					break;
				}
			}
			else if (_uposlenici[i] == nullptr)
			{
				_uposlenici[i] = (u);
				u = nullptr;
				return true;
			}
		}
		return false;
	}
	//Funkciju koja uklanja uposlenika na aktivnosti sa sifrom proslijedjenom kao parametar.
	bool ukloniUposlenika(int  sifra)
	{
		bool postoji = false;
		for (size_t i = 0; i < 5; i++)
		{
			if (_uposlenici[i] != nullptr)
			{

				if (_uposlenici[i]->getSifra() == sifra)
				{
					postoji = true;
					for (size_t j = i; j < 5; j++)
					{
						if (_uposlenici[j + 1] != nullptr)
						{
							/*delete _uposlenici[j];
							_uposlenici[j] = nullptr;*///TODO:provjeriti je li potrebno dealocirat posto u operator= vec brisemo dinamicke vrijednosti
							*_uposlenici[j] = *(_uposlenici[j + 1]);
						}
						else {
							delete _uposlenici[j];
							_uposlenici[j] = nullptr;
							break;
						}
					}
				}
			}
			else break;
		}
		return postoji;
	}
	//Funkciju koja uklanja sve uposlenike angazovane na aktivnosti.
	void ukloniUposlenike()
	{
		for (size_t i = 0; i < 5; i++)
		{
			if (_uposlenici[i] != nullptr)
			{
				delete _uposlenici[i];
				_uposlenici[i] = nullptr;
			}
			else return;
		}
	}
	bool operator==(const Aktivnost & a)const
	{
		return strcmp(a._naziv , _naziv)==0?true:false;
	}
	void setStanje(bool b) { _zavrsena = b; }
	char*& getNaziv() { return _naziv; }
	void dodajUposlenikaAktivnosti(Uposlenik *&u)
	{
		for (size_t i = 0; i < 5; i++)
		{
			if (_uposlenici[i]==nullptr)
			{
				_uposlenici[i] = u;
				u = nullptr;
				return;
			}
		}
	}
	Uposlenik **&& getUposlenici() {
		return static_cast<Uposlenik**>(_uposlenici); //TODO: provjeriti sto u ovom slucaju pravilno kopira staticki niz pokazivaca u dinamicki a kod fje 3najangazovanijaUposlenika ne kopira ispravno(u nullptr) na returnu
		/*Uposlenik ** u= new Uposlenik*[5]{nullptr};
		for (size_t i = 0; i < 5; i++)
		{
			if (_uposlenici[i] != nullptr)
			{
				u[i] = _uposlenici[i];
			}
		}
		return u;*/
	}
	bool nadjiUposlenika(int sifra)
	{
		for (size_t i = 0; i < 5; i++)
		{
			if (_uposlenici[i]!=nullptr)
			{
				if (_uposlenici[i]->getSifra()==sifra)
				{
					return true;
				}
			}
		}
		return false;
	}
	bool nadjiUposlenikaImenom(const char* ime)
	{
		for (size_t i = 0; i < 5; i++)
		{
			if (_uposlenici[i] != nullptr)
			{
				if (strcmp(_uposlenici[i]->getIme(),ime)==0)
				{
					return true;
				}
			}
		}
		return false;
	}
	int getTrajanje() { return *_trajanje; }
	bool getStanje() {
		return _zavrsena;
	}
	friend ostream & operator<<(ostream &out, const Aktivnost &a);
};
ostream & operator<<(ostream &out, const Aktivnost &a)
{
	out << "naziv Aktivnosti:" << a._naziv << endl << "trajanje:" << a._trajanje << endl << "stanje" << a._zavrsena << endl << "uposlenici:" << endl;
	for (size_t i = 0; i < 5; i++)
	{
		if (a._uposlenici[i]!=nullptr)
		{
			out << *a._uposlenici[i]<<endl;
		}
	}
	return out;
}

class Projekat {
	char* _naziv;
	char* _oblast;
	int _trajanje; //Izrazeno u mjesecima
	Aktivnost* _aktivnosti;
	int _brojAktivnosti;
public:

	//Implementirati potrebne konstruktor i destruktor funkcije.
	Projekat() :_naziv(new char('\0')), _oblast(new char('\0')), _aktivnosti(nullptr), _brojAktivnosti(0) { ; }
	Projekat(const char* naziv, const char *oblast, const int  trajanje, const Aktivnost * aktivnost, const int brojAktivnosti) :_trajanje(trajanje), _brojAktivnosti(brojAktivnosti)//TODO:popraviti konstruktor aktivnosti tako da prima sve po referenci
	{
		_naziv = new char[strlen(naziv) + 1];
		strcpy_s(_naziv, strlen(naziv) + 1, naziv);
		_oblast = new char[strlen(oblast) + 1];
		strcpy_s(_oblast, strlen(oblast) + 1, oblast);
		if (brojAktivnosti > 1)
		{
			_aktivnosti = new Aktivnost[brojAktivnosti];
			for (size_t i = 0; i < brojAktivnosti; i++)
			{
				_aktivnosti[i] = aktivnost[i];//TODO:ili new Aktivnost ako se poslani parametar brise odakle je poslan
			}
		}
		else
			_aktivnosti = new Aktivnost(*aktivnost);//TODO:provjeriti zasto se moze dodijeljivati alociranjem novog pokazivaca na aktivnost sa vrijednoscu poslanog a ne sa alociranjem novog niza
													//Rijesenje:problem je sto se bez if-a alocira Aktivnost kao niz a to nije pa kad se brise nastaje problem//TODO:razdjasniti alocrianje pokazivanja na niz objekata i dealociranje		
	}
	Projekat(const Projekat & p) :_trajanje(p._trajanje), _brojAktivnosti(p._brojAktivnosti)
	{
		_naziv = new char[strlen(p._naziv) + 1];
		strcpy_s(_naziv, strlen(p._naziv) + 1, p._naziv);
		_oblast = new char[strlen(p._oblast) + 1];
		strcpy_s(_oblast, strlen(p._oblast) + 1, p._oblast);
		if (p._brojAktivnosti > 1)
		{
			_aktivnosti = new Aktivnost[p._brojAktivnosti];
			for (size_t i = 0; i < p._brojAktivnosti; i++)
			{
				_aktivnosti[i] = (p._aktivnosti[i]);//TODO:operator= za aktivnosti
			}
		}
		else
			_aktivnosti = new Aktivnost(*p._aktivnosti);
	}
	~Projekat()
	{
		delete[] _naziv;
		delete[] _oblast;
		if (_brojAktivnosti > 1)
		{
			delete[] _aktivnosti;
		}
		else
			delete _aktivnosti;
		_naziv = nullptr;
		_oblast = nullptr;
		_aktivnosti = nullptr;
	}


	//Funkciju za dodavanje projektnih aktivnosti. Sprijeciti dodavanje vise aktivnosti sa istim nazivom.
	//Za potrebe poredjenja aktivnosti kreirati funkciju unutar klase Aktivnosti.
	bool dodajAktivnost(const Aktivnost &a)
	{
		//_aktivnosti[_brojAktivnosti] = a;
		//TODO:provjera valjanosti aktivnosti

		bool(*lambda)(Aktivnost *&, int, const Aktivnost &) = [](Aktivnost *& p, int brojAktivnosti, const  Aktivnost &a)->bool {
			for (size_t i = 0; i < brojAktivnosti; i++)
			{
				if (p[i] == a)
				{
					return true;
				}
			}
			return false;
		};
		bool postoji = lambda(_aktivnosti, _brojAktivnosti, a);

		if (postoji)
		{
			return false;
		}

		Aktivnost * temp = new Aktivnost[_brojAktivnosti + 1];//TODO: provjeriti je li brojAkt>1 pa zaobici petlju
		for (size_t i = 0; i < _brojAktivnosti; i++)
		{
			temp[i] = _aktivnosti[i];
		}
		temp[_brojAktivnosti] = a;
		if (_brojAktivnosti > 1)
		{
			delete[] _aktivnosti;
		}
		else
			delete _aktivnosti;
		_aktivnosti = temp;
		_brojAktivnosti++;
		temp = nullptr;
		return true;
	}

	//Funkciju koja oznacava aktivnost kao zavrsenu. Funkcija kao parametar prima naziv aktivnosti.
	void zavrsiAktivnost(const char* nazivAktivnosti)
	{
		for (size_t i = 0; i < _brojAktivnosti; i++)
		{
			if (strcmp(_aktivnosti[i].getNaziv(), nazivAktivnosti) == 0)
			{
				_aktivnosti[i].setStanje(true);
				return;
			}
		}
	}

	//Funkciju koja treba da postojecoj aktivnosti dodijeli novog uposlenika.
	void dodajUposlenikaAktivnostiProjekta(Uposlenik *&u)
	{
		_aktivnosti->dodajUposlenikaAktivnosti(u);
	}

	//Funkciju koja treba da vrati sve uposlenike angazovane na odredjenoj aktivnosti. Ulazni parametar je naziv aktivnosti.
	Uposlenik **&& vratiUposlenike(const char* nazivAktivnosti)
	{
		for (size_t i = 0; i < _brojAktivnosti; i++)
		{
			if (strcmp(_aktivnosti[i].getNaziv(), nazivAktivnosti) == 0)
			{
				return _aktivnosti[i].getUposlenici();
			}
		}
	}

	//Funkciju koja uklanja odredjenog uposlenika na aktivnosti. Ulazni parametri funkcije su naziv aktivnosti i sifra uposlenika.
	void ukloniUposlenikaAktivnostiProjekta(const char* nazivAktivnosti, int sifraUposlenika)
	{
		for (size_t i = 0; i < _brojAktivnosti; i++)
		{
			if (strcmp(_aktivnosti[i].getNaziv(), nazivAktivnosti) == 0)
			{
				_aktivnosti[i].ukloniUposlenika(sifraUposlenika);
			}
		}
	}

	//Funkciju koja uklanja sve uposlenike angazovane na aktivnosti sa nazivom proslijedjenim kao parametar.
	void ukloniSveUposlenikeAktivnostiProjekta(const char* nazivAktivnosti)
	{
		for (size_t i = 0; i < _brojAktivnosti; i++)
		{
			if (strcmp(_aktivnosti[i].getNaziv(), nazivAktivnosti) == 0)
			{
				_aktivnosti[i].ukloniUposlenike();
			}
		}
	}

	//Funkciju koja pronalazi uposlenika sa najvecim brojem angazmana na aktivnostima.
	 Uposlenik & najangazovanijiUposlenik()
	{
		int najangazovanijiUposlenik = 0;
		Uposlenik * uposlenikSaNajviseAngazmana = nullptr;

		for (size_t i = 0; i < _brojAktivnosti; i++)
		{
			Uposlenik **u = _aktivnosti[i].getUposlenici();
			for (size_t j = 0; j < 5; j++)
			{
				if (u[j] != nullptr)
				{
					int brojacAngazovanjaUposlenika = 0;
					for (size_t y = 0; y < _brojAktivnosti; y++)
					{
						if (_aktivnosti[y].nadjiUposlenikaImenom(u[j]->getIme()))//TODO:provjeravati imenom uposlenika posto je sifra unikatna u cijelom programu za svakog uposlenika u aktivnosti
						{
							brojacAngazovanjaUposlenika++;
						}
					}
					if (brojacAngazovanjaUposlenika > najangazovanijiUposlenik)
					{
						najangazovanijiUposlenik = brojacAngazovanjaUposlenika;
						uposlenikSaNajviseAngazmana = u[j];
					}
				}
				else break;
			}
		}
		return *uposlenikSaNajviseAngazmana;
	}

	//Funkciju koja pronalazi 3 uposlenika koja su angazovana na najvecem broju nezavrsenih aktivnosti.
	 Uposlenik **& najangazovanija3UposlenikaNezavrsenih()//TODO: provjeriti ispravnost nakon sto se porede uposlenici imenima posto je ovako svaki angazovan jednom
	{
		int najangazovanijiUposlenik1 = 0;
		int najangazovanijiUposlenik2 = 0;
		int najangazovanijiUposlenik3 = 0;

		Uposlenik ** uposleniciSaNajviseAngazmana = new Uposlenik*[3]{nullptr};//TODO:(provjeriti!ispravno kopiralo *[] u ** kod fje getUposlenike)ne raditi sa statickim nizom od 3 pokazivaca posto ce tokom return kopiranjem iz statickog u dinamicki izgubiti null vrijednosti ako ne bude dovoljno uposlenika pa se oni kasnije nece moci ispisati
		for (size_t i = 0; i < _brojAktivnosti; i++)
		{
			if (_aktivnosti[i].getStanje() == true)
			{

				Uposlenik **u = _aktivnosti[i].getUposlenici();
				for (size_t j = 0; j < 5; j++)
				{
					if (u[j] != nullptr)
					{
						int brojacAngazovanjaUposlenika = 0;
						for (size_t y = 0; y < _brojAktivnosti; y++)
						{
							if (_aktivnosti[y].nadjiUposlenikaImenom(u[j]->getIme()))//TODO:provjeravati imenom uposlenika posto je sifra unikatna u cijelom programu za svakog uposlenika u aktivnosti
							{
								brojacAngazovanjaUposlenika++;
							}
						}
						if (brojacAngazovanjaUposlenika > najangazovanijiUposlenik3)
						{
							if (brojacAngazovanjaUposlenika > najangazovanijiUposlenik2)
							{
								if (brojacAngazovanjaUposlenika > najangazovanijiUposlenik1)
								{
									/*najangazovanijiUposlenik3 = najangazovanijiUposlenik2;
									uposleniciSaNajviseAngazmana[2] = uposleniciSaNajviseAngazmana[1];

									najangazovanijiUposlenik2 = najangazovanijiUposlenik1;
									uposleniciSaNajviseAngazmana[1] = uposleniciSaNajviseAngazmana[0];*/

									najangazovanijiUposlenik1 = brojacAngazovanjaUposlenika;
									uposleniciSaNajviseAngazmana[0] = u[j];
								}
								else {
									/*najangazovanijiUposlenik3 = najangazovanijiUposlenik2;
									uposleniciSaNajviseAngazmana[2] = uposleniciSaNajviseAngazmana[1];*/

									najangazovanijiUposlenik2 = brojacAngazovanjaUposlenika;
									uposleniciSaNajviseAngazmana[1] = u[j];
								}
							}
							else
							{
								najangazovanijiUposlenik3 = brojacAngazovanjaUposlenika;
								uposleniciSaNajviseAngazmana[2] = u[j];
							}
						}
					}
					else break;
				}
			}
		}
		
		return  (uposleniciSaNajviseAngazmana);
	}

	//Funkciju koja ispisuje sve zavrsene aktivnosti na projektu.
	void zavrseneAktivnostiProjekta()
	{
		for (size_t i = 0; i < _brojAktivnosti; i++)
		{
			if (_aktivnosti[i].getStanje()==true)
			{
				cout<<_aktivnosti[i];
			}
		}
	}
	int getBrojAktivnosti() { return _brojAktivnosti; }
	Aktivnost *& getAktivnosti() { return _aktivnosti; }
};


/*Kreirati globalnu funkciju koja pronalazi najuspjesnijeg usposlenika na projektima proslijedjenim kao parametar.
Uposlenik se smatra najuspjesnijim ako je angazovan na najvecem broju zavrsenih aktivnosti. Ako vise uposlenika
ima isti broj zavrsenih aktivnosti, uspjesnijim se smatra onaj cije su aktivnosti trajale duze.
*/
Uposlenik & najUspjesnijiUposlenikProjekta(Projekat &p)//TODO:provjeriti ispravnost nakon pretrage angazovanja uposlenika u aktivnosti po imenu a ne sifri jer su sifre unikatne
{
	int najangazovanijiUposlenik = 0;
	Uposlenik * uposlenikSaNajviseAngazmana = nullptr;
	Aktivnost * a = p.getAktivnosti();
	Aktivnost * aktivnostNajangazovanijeg = nullptr;
	for (size_t i = 0; i <p.getBrojAktivnosti(); i++)
	{
		if (a[i].getStanje()==true)
		{
			Uposlenik** u = a[i].getUposlenici();
			for (size_t j = 0; j < 5; j++)
			{
				if (u[j]!=nullptr)
				{
					int brojacAngazovanjaUposlenika=0;
					for (size_t y = 0; y < p.getBrojAktivnosti(); y++)
					{
						if (a[y].nadjiUposlenikaImenom(u[j]->getIme()))
						{
							brojacAngazovanjaUposlenika++;
						}
					}
					if (brojacAngazovanjaUposlenika >= najangazovanijiUposlenik)
					{
						if (brojacAngazovanjaUposlenika == najangazovanijiUposlenik)
						{
							if (a[i].getTrajanje()<aktivnostNajangazovanijeg->getTrajanje())
							{
								break;
							}
						}
						najangazovanijiUposlenik = brojacAngazovanjaUposlenika;
						uposlenikSaNajviseAngazmana = u[j];
						aktivnostNajangazovanijeg = &a[i];
					}

				}
				else break;

			}


		}

	}
	return *uposlenikSaNajviseAngazmana;
}


int main()
{
	//Testirati sve dostupne funkcionalnosti
	Uposlenik u1;
	Uposlenik u2("ajdin", "ljubuncic", "student");
	Uposlenik u3(u2);
	int brojUposlenika(2);
	Uposlenik ** p = new Uposlenik*[brojUposlenika];
	p[0] = new Uposlenik(u2);
	p[1] = new Uposlenik(u3);
	u3.Info();
	const int * neki = new int(2);
	Aktivnost a1;
	Aktivnost a2(string("druga aktinvost"), true, neki, p/*const_cast<const Uposlenik ** &>(p)*/, brojUposlenika);//TODO:provjeriti slanje parametra u fju koja prima pokazivac na nesto po referenci
	Aktivnost a3(a2);
	Uposlenik *noviUposlenik = new Uposlenik("novi", "uposlenik", "radnik");
	Uposlenik *noviUposlenik2 = new Uposlenik("novi2", "uposlenik", "radnik");
	a2.angazujUposlenika(noviUposlenik);
	a2.angazujUposlenika(noviUposlenik2);
	a2.ukloniUposlenika(3);
	a2.ukloniUposlenike();

	Aktivnost *pAkt = new Aktivnost[2];
	pAkt[0] = a1;
	pAkt[1] = a3;
	Projekat p1;
	Projekat p2("neki naziv", "neka oblast", 2, pAkt, 2);
	Projekat p3(p2);

	p3.dodajAktivnost(a2);
	int brup = 2;
	Uposlenik ** pokUposlenika = new Uposlenik*[brup];
	pokUposlenika[0] = new Uposlenik(u2);
	pokUposlenika[1] = new Uposlenik("ajdinnn","asdas","dasda");
	Aktivnost najnovija(string("treca"), false, neki, pokUposlenika, brup);
	p3.dodajAktivnost(najnovija);
	//2
	p3.zavrsiAktivnost("treca");
	//3
	Uposlenik *uposlenik10 = new Uposlenik("nestonesto", "prez", "pozicija");
	p3.dodajUposlenikaAktivnostiProjekta(uposlenik10);
	//4
	Uposlenik **u=	p3.vratiUposlenike("treca");
	for (size_t i = 0; i < 5; i++)
	{
		if (u[i]!=nullptr)
		{
			cout << *u[i];
		}
		else break;
	}
	//7
	cout<<"\n-----------\nNajangazovaniji uposlenik:\n"<<p3.najangazovanijiUposlenik();
	//8
	Uposlenik **u200= p3.najangazovanija3UposlenikaNezavrsenih();
	for (size_t i = 0; i < 3; i++)
	{
		if ((u200[i]) != nullptr)
		{
			cout << *u200[i];
		}
		else
			break;
	}
	//9
	p3.zavrseneAktivnostiProjekta();
	
	//10
	cout<<najUspjesnijiUposlenikProjekta(p3);
	
	//5
	p3.ukloniUposlenikaAktivnostiProjekta("treca", 9);
	//6
	p3.ukloniSveUposlenikeAktivnostiProjekta("treca");


	system("PAUSE>FIT");
	return 0;
}

