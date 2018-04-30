#include <iostream>	
#include <string>	
#include <vector>	
#include <algorithm>
#include <sstream>
#include <thread>
#include<functional>
#include <iterator>
using namespace std;

template<class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max];
	T2* _elementi2[max];
	int _trenutno;
public:
	Kolekcija()
	{
		for (int i = 0; i < max; i++)
		{
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}

	Kolekcija(const Kolekcija& k)
	{
		for (int i = 0; i < max; i++)
		{
			if (i < k._trenutno)
			{
				_elementi1[i] = new T1(*k._elementi1[i]);
				_elementi2[i] = new T2(*k._elementi2[i]);
			}
			else
			{
				_elementi1[i] = nullptr;
				_elementi2[i] = nullptr;
			}
		}

		_trenutno = k._trenutno;

	}
	~Kolekcija()
	{
		for (int i = 0; i < _trenutno; i++)
		{
			delete _elementi1[i];
			_elementi1[i] = nullptr;

			delete _elementi2[i];
			_elementi2[i] = nullptr;
		}
	}

	void Dodaj(const T1& el1,const  T2& el2)
	{
		if (_trenutno == max)
			throw exception("Dostigli ste kapacitet kolekcije.");

		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno] = new T2(el2);

		_trenutno++;

	}

	T1& GetElement1(const int index)const 
	{
		if (index >= 0 && index < _trenutno)
			return *_elementi1[index];

		throw exception("Vrijednost indeksa nije validna.");
	}
	T2& GetElement2(const int index)const 
	{
		if (index >= 0 && index < _trenutno)
			return *_elementi2[index];

		throw exception("Vrijednost indeksa nije validna.");

	}

	int GetTrenutno()const { return _trenutno; }
	friend ostream & operator<<(ostream & out, Kolekcija &k)
	{
		for (size_t i = 0; i < k.GetTrenutno(); i++)
		{
			out << k.GetElement1(i);
			out << k.GetElement2(i);
		}
	}
};

class Uplata {
	char* _datum;
	double _iznos;
public:
	Uplata(const char* datum, double iznos)
	{
		int vel = strlen(datum) + 1;
		_datum = new char[vel];
		strcpy_s(_datum, vel, datum);

		_iznos = iznos;
	}
	Uplata(const Uplata &u)
	{
		int vel = strlen(u._datum) + 1;
		_datum = new char[vel];
		strcpy_s(_datum, vel, u._datum);
		_iznos = u._iznos;
	}
	Uplata & operator=(const Uplata &u)
	{
		int vel = strlen(u._datum) + 1;
		_datum = new char[vel];
		strcpy_s(_datum, vel, u._datum);
		_iznos = u._iznos;
		return *this;
	}
	~Uplata()
	{
		delete[] _datum;
		_datum = nullptr;
	}
	double getIznos() { return _iznos; }
	friend ostream& operator<<(ostream&, const Uplata&);
};

ostream& operator<<(ostream& cout, const Uplata& u)
{
	cout << "Datum uplate: " << u._datum << endl;
	cout << "Iznos: " << u._iznos << endl;

	return cout;
}

class Oprema
{
	string _naziv;
	string* _proizvodjac;
	double _cijena;
	int _dostupnoNaStanju;
public:
	//Konstruktor i destruktor
	Oprema(string naziv, string proizvodjac, double cijena,int dostupnoNaStanju)
	{
		_naziv = naziv;
		_proizvodjac = new string (proizvodjac);
		_cijena = cijena;
		_dostupnoNaStanju = dostupnoNaStanju;
	}
	Oprema(const Oprema &o)
	{
		_naziv = o._naziv;
		_proizvodjac = new string(*o._proizvodjac);
		_cijena = o._cijena;
		_dostupnoNaStanju = o._dostupnoNaStanju;
	}
	~Oprema() { delete _proizvodjac; }
	//Osnovne funkcije za rad sa klasom
	Oprema & operator=(const Oprema &o)
	{
		_naziv = o._naziv;
		_proizvodjac = new string;
		*_proizvodjac = *o._proizvodjac;
		_cijena = o._cijena;
		_dostupnoNaStanju = o._dostupnoNaStanju;
		return *this;
	}
	bool operator==(const Oprema &o)const
	{
		return o._naziv == _naziv;
	}
	bool operator<<(const Oprema & o) { return _cijena < o._cijena; }
	friend ostream & operator<<(ostream &out,const Oprema &o) 
	{
		out << o._naziv << endl << o._cijena << endl << o._dostupnoNaStanju << endl << *o._proizvodjac << endl;
		return out;
	}
};

class Ucenik
{
	string _imePrezime;
	int _godiste;
	bool _posjedujeOpremu;
	vector<Oprema>* _iznajmljenaOprema; //Trenutno iznajmljena oprema
	Kolekcija<Uplata, string, 100> _uplate; //Drugi parametar tipa string predstavlja svrhu uplate: trening ili oprema
public:
	Ucenik(string imePrezime, int godiste, bool posjedujeOpremu = false)
	{
		//Konstruktor dopuniti po potrebi
		_imePrezime = imePrezime;
		_godiste = godiste;
		_posjedujeOpremu = posjedujeOpremu;
		_iznajmljenaOprema = new vector<Oprema>;
	}
	Ucenik(const Ucenik &u)
	{
		_imePrezime = u._imePrezime;
		_godiste = u._godiste;
		_posjedujeOpremu = u._posjedujeOpremu;
		_iznajmljenaOprema = new vector<Oprema>;
		vector<Oprema>::iterator it;
		for (it = u._iznajmljenaOprema->begin(); it != u._iznajmljenaOprema->end(); it++)
		{
			_iznajmljenaOprema->push_back(*it);
		}
		for (size_t i = 0; i < u._uplate.GetTrenutno(); i++)
		{
			_uplate.Dodaj(u._uplate.GetElement1(i), u._uplate.GetElement2(i));
		}
	}
	Ucenik()
	{
		//Konstruktor dopuniti po potrebi
		_imePrezime = "imePrezime";
		_godiste = 1111;
		_posjedujeOpremu = false;
		_iznajmljenaOprema = new vector<Oprema>;
	}
	const Ucenik & operator=(const Ucenik &u)
	{
		_imePrezime = u._imePrezime;
		_godiste = u._godiste;
		_posjedujeOpremu = u._posjedujeOpremu;
		_iznajmljenaOprema = new vector<Oprema>;
		vector<Oprema>::iterator it;
		while (!_iznajmljenaOprema->empty())
		{
			_iznajmljenaOprema->pop_back();
		}
		for (it = u._iznajmljenaOprema->begin(); it != u._iznajmljenaOprema->end(); it++)
		{
			_iznajmljenaOprema->push_back(*it);
		}
		for (size_t i = 0; i < u._uplate.GetTrenutno(); i++)
		{
			_uplate.Dodaj(u._uplate.GetElement1(i), u._uplate.GetElement2(i));
		}
		return *this;
	}
	//Destruktor
	~Ucenik() {
		_iznajmljenaOprema->clear();
		delete _iznajmljenaOprema;
	}
	//Preklopljeni operator+= za evidenciju iznajmljene opreme.
	void operator+=(Oprema &o)
	{
		_iznajmljenaOprema->push_back(o);
	}
	bool operator<(const Ucenik &u) { return _godiste < u._godiste; }
	//Preklopljeni operator-= za uklanjanje opreme kada se vrati.
	double getUplate() {
		//return 100 * _uplate.GetTrenutno();	
		double suma = 0;
		for (size_t i = 0; i < _uplate.GetTrenutno(); i++)
		{
			suma += _uplate.GetElement1(i).getIznos();
		}
		return suma;
	
	}
	void operator-=(Oprema &o)
	{
		for (size_t i = 0; i < _iznajmljenaOprema->size(); i++)
		{
			if (*(_iznajmljenaOprema->begin()+i)==o)
			{
				_iznajmljenaOprema->erase(_iznajmljenaOprema->begin() + i);
			}
		}
	}
	bool operator==(Ucenik &u) { return u._imePrezime == _imePrezime; }
	//Funkciju za dodavanje uplata.
	void DodajUplatu(Uplata &u, string svrha)
	{
		_uplate.Dodaj(u, svrha);
	}
	friend ostream & operator<<(ostream & out,const Ucenik &u)
	{
		out << u._imePrezime << endl << u._godiste << endl << u._posjedujeOpremu;
		ostream_iterator <Oprema, char> out_it(cout,"\n");
		copy(u._iznajmljenaOprema->begin(), u._iznajmljenaOprema->end(),out_it);
		for (size_t i = 0; i < u._uplate.GetTrenutno(); i++)
		{
			out << u._uplate.GetElement1(i);
			out << u._uplate.GetElement2(i);
		}
		return out;
	}
	
};

enum TipTreninga { Pocetnicki, Rekreativni, Profesionalni };
class SkiTrener
{
	string _imePrezime;
	bool _certificiran;
	char _spol;
	TipTreninga  _tipTreninga;
public:
	//Osnovne funkcije za rad sa klasom
	SkiTrener(string imePrezime, bool certificiran, char spol, TipTreninga tipTreninga)
	{
		_imePrezime = imePrezime;
		_certificiran = certificiran;
		_spol = spol;
		_tipTreninga = tipTreninga;
	}
	SkiTrener(const SkiTrener & s)
	{
		_imePrezime = s._imePrezime;
		_certificiran = s._certificiran;
		_spol = s._spol;
		_tipTreninga = s._tipTreninga;
	}
	SkiTrener &  operator=(const SkiTrener & s)
	{
		_imePrezime = s._imePrezime;
		_certificiran = s._certificiran;
		_spol = s._spol;
		_tipTreninga = s._tipTreninga;
		return *this;
	}
	SkiTrener()
	{
		_imePrezime = "imePrezime";
		_certificiran = false;
		_spol = 'M';
		_tipTreninga = Pocetnicki;
	}
	bool operator==(SkiTrener &t) { return (t._imePrezime == _imePrezime); }
	bool operator<(const SkiTrener & s) { return _tipTreninga < s._tipTreninga; }
	friend ostream& operator<<(ostream & out, SkiTrener &s)
	{
		out << s._imePrezime <<endl<< s._certificiran <<endl<< s._spol <<endl<< s._tipTreninga << endl;
		return out;
	}
	TipTreninga & getTip() { return _tipTreninga; }
};

class Trening
{
	string* _datum;
	SkiTrener* _trener;
	vector<Ucenik*> _ucenici;
	int _brojCasova;
public:
	//Konstruktor i destruktor
	Trening(string datum, SkiTrener trener, int brojCasova)
	{
		_datum = new string(datum);
		_trener = new SkiTrener(trener);
		_brojCasova = brojCasova;
	}
	Trening(const Trening & trening)
	{
		_datum = new string(*trening._datum);
		_trener = new SkiTrener(*trening._trener);
		_brojCasova = trening._brojCasova;
		//TODO:uraditi iteratorom
		/*vector<Ucenik*>::iterator it;
		for (*it = *trening._ucenici.begin(); *it !=*trening._ucenici.end(); it++)
		{
			_ucenici.push_back(*it);
		}*/
		
		for (size_t i = 0; i < trening._ucenici.size(); i++)
		{
			_ucenici.push_back(trening._ucenici[i]);
		}
	}
	bool operator !=(const Trening & t) {
		return _datum != t._datum;
	}
	const Trening & operator=(const Trening & trening)
	{
		if (*this != trening)
		{
			_datum = new string(*trening._datum);
			_trener = new SkiTrener(*trening._trener);
			_brojCasova = trening._brojCasova;
			while (!_ucenici.empty())
			{
				_ucenici.pop_back();
			}
			for (size_t i = 0; i < trening._ucenici.size(); i++)
			{
				_ucenici.push_back(trening._ucenici[i]);
			}
		}
	return *this;
	}
	Trening()
	{
		_datum = new string("1.1.1111");
		_trener = new SkiTrener(SkiTrener("Niko",false,'M',Pocetnicki));
		_brojCasova = 0;
	}
	//Napomena: Atributi _trener i _ucenici čuvaju samo adrese objekata koji su pohranjeni u glavnoj klasi SkiKurs.
	~Trening()
	{
		delete _datum;
		delete _trener;
		//TODO:provjeriti!
		/*for (size_t i = 0; i < _ucenici.size(); i++)
		{
			delete _ucenici[i];
		}*/
		_ucenici.clear();
	}
	void ispisTreninga()
	{
		cout << *_datum;
		cout << *_trener;
		for (size_t i = 0; i < _ucenici.size(); i++)
		{
			cout << *_ucenici[i];
		}
		cout <<endl<< _brojCasova;
	}
	bool operator<(const Trening & t) const 
	{ 
		return _ucenici.size()<t._ucenici.size(); 
	}
	int getBrojCasova() { return _brojCasova; }
	TipTreninga & GetTipTreningaTreneru() { return _trener->getTip(); }
	//Operator += za dodavanje ucenika na trening.
	void operator +=(Ucenik &u)
	{
		_ucenici.push_back(&u);
	}
	bool TrenerNaTreningu(SkiTrener &s) { return s == *_trener; }
	bool operator==(Trening & t) { return(t._trener==_trener && t._ucenici==_ucenici && t._datum==_datum); }
	//Operator za ispis.
	friend ostream& operator<<(ostream &out,Trening &t)
	{
		out << *t._datum << endl;
		out << t._brojCasova << endl;
		out << *t._trener;
		for (size_t i = 0; i < t._ucenici.size(); i++)
		{
			out << *t._ucenici[i];
		}
		return out;
	}
	int UcenikNaTreningu(Ucenik & uc)
	{
		for (size_t i = 0; i < _ucenici.size(); i++)
		{
			if (*_ucenici[i]==uc)
			{
				return i;
			}
		}
		return -1;
	}
};

class SkiKurs {
	string _periodOdrzavanja;
	vector<SkiTrener> _treneri;
	vector<Ucenik> _polaznici;
	vector<Trening> _treninzi;
public:
	//Konstruktor
	SkiKurs() { _periodOdrzavanja = "10.10.2010-20.10.2010"; }
	SkiKurs(string periodOdrzavanja) :_periodOdrzavanja(periodOdrzavanja) { ; }
	SkiKurs(SkiKurs & s) :_periodOdrzavanja(s._periodOdrzavanja),_treneri(s._treneri),_polaznici(s._polaznici),_treninzi(s._treninzi) { ; }
	//Funkciju za dodavanje trenera (bez dupliranja elemenata).
	void DodajTrenera(SkiTrener & t) {
		for (size_t i = 0; i < _treneri.size(); i++)
			if (_treneri[i]==t)	throw exception("Trener vec postoji!");
		_treneri.push_back(t);
	}
	//Funkciju za dodavanje polaznika (bez dupliranja elemenata).
	void DodajPolaznika(Ucenik & p)
	{
		vector<Ucenik>::iterator it;
		for (it =_polaznici.begin() ; it !=_polaznici.end() ; it++)
		{
			if (*it==p)
			{
				throw exception("Polaznik vec postoji!");
			}
		}
		_polaznici.push_back(p);
	}
	//Funkciju za dodavanje odrzanih treninga.
	void DodajTrening(Trening & t)
	{
		/*for (size_t i = 0; i < _treninzi.size(); i++)
			if (_treninzi[i]==t)throw exception("Trening vec postoji!");*/
		_treninzi.push_back(t);
	}

	//Funkciju za dodavanje uplate uceniku za odredjeni trening (dakle, funkcija prima 3 parametra: uplata, ucenik, trening).
	//Prvenstveno je potrebno provjeriti da li je ucenik uopste evidentiran na datom treningu.
	//Cijena jednog casa zavisi od tipa treninga na kojem je ucenik bio:
	//Pocetnicki - 10KM
	//Rekreativni - 15KM
	//Profesionalni - 20KM
	//Uplata treba da bude tacnog iznosa proracunatog na osnovu prethodne kategorizacije i broja casova odrzanog treninga.
	//U svim slucajevima kada je neko od ogranicenja naruseno, baciti izuzetak.
	double tipTreningaUiznos(TipTreninga & t)
	{
		switch (t)
		{
		case Pocetnicki: return 10;
		case Rekreativni:return 15;
		case Profesionalni:return 20;
		}
		return 0;
	}
	void DodajUplatuUceniku( Uplata & up,Ucenik & uc,Trening & t)
	{
		if (t.UcenikNaTreningu(uc)==-1)
		{
			throw exception("Greska! Ucenik ne postoji!");
		}
		if ((tipTreningaUiznos(t.GetTipTreningaTreneru()) * t.getBrojCasova())!=up.getIznos())
		{
			throw exception("Greska! Uplata nije validna!");
		}
		uc.DodajUplatu(up, "trening");
	}


	//Funkciju koja pronalazi ski trenera sa najvecim brojem odrzanih casova i ispisuje njegov ostvareni profit.
	double NajKorisnijiTrener() {
		double profit = 0;
		double najveciProfit = -1;
		int NajvecibrCasova = -1;

		for (size_t i = 0; i < _treneri.size(); i++)
		{
			int brojCasova = 0;
			for (size_t j = 0; j < _treninzi.size(); j++)
			{
				if (_treninzi[j].TrenerNaTreningu(_treneri[i]))
				{
					brojCasova += _treninzi[i].getBrojCasova();
					profit += _treninzi[i].getBrojCasova()*tipTreningaUiznos(_treninzi[i].GetTipTreningaTreneru());
				}
			}
			if (brojCasova>NajvecibrCasova)
			{
				NajvecibrCasova = brojCasova;
			}
			if (profit>najveciProfit)
			{
				najveciProfit = profit;
			}
		}
		return najveciProfit;
	}

	//Funkciju koja pronalazi ucenika od kojeg je ostvarena najveca zarada, ukljucujuci uplate za iznajmljenu opremu.
	Ucenik & NajvecaZaradaOdUcenika()
	{
		Ucenik *naj=nullptr;
		int najUplata=0;
		for (size_t i = 0; i <_polaznici.size(); i++)
		{
			if (_polaznici[i].getUplate()>najUplata)
			{
				najUplata = _polaznici[i].getUplate();
				naj = &_polaznici[i];
			}
		}
		return *naj;
	}


	//Funkciju koja sortira podatke o kompletnom kursu. Podaci trebaju biti sortirani na sljedeci nacin: 
	//- treneri prema kategoriji
	//- treninzi prema broju ucenika koji su prisustvovali
	//- polaznici prema godistu.
	//Ovu funkciju je potrebno pozvati kreiranjem posebnog thread-a.
	void SortirajKurs()
	{
		sort(_treneri.begin(),_treneri.end());
		sort(_treninzi.begin(), _treninzi.end());
		sort(_polaznici.begin(), _polaznici.end());
	}

	//Funkciju koja vrsi ispis svih podataka po kategorijama i sortirano u skladu sa zahtijevima
	void ispis()
	{
		SortirajKurs();
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < _treninzi.size(); j++)
			{
				if (i==_treninzi[j].GetTipTreningaTreneru())
				{
					_treninzi[j].ispisTreninga();
				}
			}
		}
	}


};


int main() {
	try {
		SkiKurs s;
		Ucenik p("Ajdin Ljubuncic", 1997);
		Ucenik p2("Ahmo Ljubuncic", 1991);
		Ucenik p3("Amila Ljubuncic", 1995);

		cout << p;
		Oprema o("Kaciga", "NIKE", 5, 1);
		Oprema o1("Hlace", "NIKE", 50, 1);
		s.DodajPolaznika(p);
		p += o;
		p += o1;
		p -= o;
		SkiTrener t("Larisa Tipura", true, 'z', Pocetnicki);
		SkiTrener t2("Denis Music", false, 'M', Profesionalni);
		SkiTrener t3("Jasmin Azemovic", true, 'm', Rekreativni);
		s.DodajTrenera(t2);
		s.DodajTrenera(t3);
		s.DodajPolaznika(p2);
		s.DodajPolaznika(p3);
		s.DodajTrenera(t);
		Trening trening("21.1.2017", t, 5);
		Trening trening2("21.2.2017", t2, 5);
		Trening trening3("21.3.2017", t3, 5);

		trening += p;
		trening += p2;

		cout << trening;
		trening.ispisTreninga();
		s.DodajTrening(trening);
		
		//trening3 += p3;
		//trening3 += p;

		trening2 += p2;
		Uplata u("1.1.2017", 50);
		Uplata u1("2.1.2017", 100);

		p.DodajUplatu(u, "oprema");
		s.DodajTrening(trening3);
		s.DodajTrening(trening2);

		s.DodajUplatuUceniku(u, p, trening);
		s.DodajUplatuUceniku(u1, p2, trening2);
		cout << s.NajKorisnijiTrener();
		s.NajvecaZaradaOdUcenika();
		thread novi(&SkiKurs::SortirajKurs, &s);
		novi.join();
		//s.SortirajKurs();
		s.ispis();
	}
	catch (exception &ex) {
		cout << ex.what();
	}

	system("pause>FIT");
	return 0;
}

