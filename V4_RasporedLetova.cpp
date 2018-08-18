#include<iostream>

using namespace std;

class Vrijeme
{
	int _sati;
	int _minute;
	int _sekunde;
public:
	Vrijeme() :_sati(0), _minute(0), _sekunde(0) { ; }
	Vrijeme(int sati, int minute, int sekunde = 0) {
		_sati = sati;
		_minute = minute;
		_sekunde = sekunde;
	}
	Vrijeme(const Vrijeme &v) :_sati(v._sati),_minute(v._minute),_sekunde(v._sekunde) { ; }
	/*Vrijeme & operator=(Vrijeme &v)
	{
		if (this!=&v)
		{
			_sati = v._sati;
			_minute = v._minute;
			_sekunde = v._sekunde;
		}
		return *this;
	}*/
	Vrijeme & povecajMinutama(int dodatneMinute)
	{
		_sati += dodatneMinute / 60;
		_minute += dodatneMinute % 60;
		return *this;
	}
	//Preklopiti operator << za ispis ojekata tipa Vrijeme
	friend ostream & operator<<(ostream & out,  Vrijeme & v)
	{
		out << v._sati << ":" << v._minute << ":" << v._sekunde << endl;
		return out;
	}
	friend bool operator<(Vrijeme &v1, Vrijeme &v2)
	{
		int prvoVrijemeSekunde = v1._sekunde + v1._minute * 60 + v1._sati * 3600;
		int drugoVrijemeSekunde = v2._sekunde + v2._minute * 60 + v2._sati * 3600;
		if (prvoVrijemeSekunde<drugoVrijemeSekunde)
		{
			return true;
		}
		else return false;
	}
	friend bool operator>(Vrijeme &v1, Vrijeme &v2)
	{
		return v1 < v2 ? false : true;
	}
};

class Let
{
	static const  int _oznakaSize = 10;
	char _oznaka[_oznakaSize];
	char* _odrediste;
	int _brIzlazneKapije;
	Vrijeme _vrijemePolijetanja; //sati i minute
	int _trajanje; //izraženo u minutama
	int _kasnjenje; //izraženo u minutama

public:
	//Potrebne konstruktor i destruktor funkcije
	Let() :_odrediste(new char('\0')), _brIzlazneKapije(-1), _trajanje(0), _kasnjenje(0) { ; }//TODO:provjeriti treba li brizlKapije ikasnjenej biti 0
	Let(char * oznaka, char * odrediste, int brIzlazneKapije, Vrijeme & vrijemePolijetanja, int trajanje, int kasnjenje)
		:_brIzlazneKapije(brIzlazneKapije), _vrijemePolijetanja(vrijemePolijetanja), _trajanje(trajanje), _kasnjenje(kasnjenje)
	{
		strncpy_s(_oznaka, oznaka, _oznakaSize);
		int v = strlen(odrediste) + 1;
		_odrediste = new char[v];
		strcpy_s(_odrediste, v, odrediste);
	}
	Let(const Let &l)
		:_brIzlazneKapije(l._brIzlazneKapije), _vrijemePolijetanja(l._vrijemePolijetanja), _trajanje(l._trajanje), _kasnjenje(l._kasnjenje)
	{
		strncpy_s(_oznaka, l._oznaka, _oznakaSize);
		int v = strlen(l._odrediste) + 1;
		_odrediste = new char[v];
		strcpy_s(_odrediste, v, l._odrediste);
	}
	/*Let & operator=(const Let &l)
	{
		if (this!=&l)
		{

		_brIzlazneKapije=(l._brIzlazneKapije);
		_vrijemePolijetanja = (l._vrijemePolijetanja);
		_trajanje = (l._trajanje);
		_kasnjenje = (l._kasnjenje);
		strncpy_s(_oznaka, l._oznaka, _oznakaSize);
		if (_odrediste != nullptr)
		{
			delete[] _odrediste;
		}
		int v = strlen(l._odrediste) + 1;
		_odrediste = new char[v];
		strcpy_s(_odrediste, v, l._odrediste);
		
		}
		return *this;
	}*/
	~Let() {
		delete[] _odrediste;
		_odrediste = nullptr;
	}
	Vrijeme & getVrijemePolijetanja() {
		return _vrijemePolijetanja;
	}
	void setVrijemePolijetanja(Vrijeme v) { _vrijemePolijetanja = v; }//TODO:provjeti oerator= za Vrijeme
																	  //Funkciju sa jednim parametrom koja postavlja informaciju o eventualnom kašnjenju na vrijednost zadanu parametrom.
	void postaviKasnjenje(int kasnjenje) { _kasnjenje = kasnjenje; }//TODO:ili poveavati kasnjenje za parametar

	/*Preklopiti unarni operator "!" putem kojeg je mogu?e saznati da li odgovaraju?i let kasni ili ne (vratiti logi?ku
	vrijednost "true" u slu?aju kašnjenja, u suprotnom vratiti "false").*/
	bool operator!() { return _kasnjenje > 0 ? true : false; }
	//Funkciju koja kao rezultat vra?a trajanje leta.
	int getTrajanjeLeta() { return _trajanje; }
	/*Preklopiti operator + na na?in da omogu?ava sabiranje objekata tipa "Let" i cijelog broja, pri
	?emu se kao rezultat dobiva novi objekat tipa "Let", u kojem je vrijeme polijetanja pomjereno
	unaprijed za iznos odre?en drugim sabirkom (ra?unato u minutama).
	Tako?er preklopiti i operator "+=" koji osigurava da izraz oblika "X += Y uvijek ima isto zna?enje kao i izraz
	"X = X + Y" */
	friend Let  operator+(Let &l, int dodatneMinute) {//TODO: provjeriti je li isto op + i op+= 
		Let a(l);
		a.setVrijemePolijetanja(a._vrijemePolijetanja.povecajMinutama(dodatneMinute));
		return  a;
	}
	Let & operator+=(int dodatneMinute)
	{
		this->setVrijemePolijetanja(this->_vrijemePolijetanja.povecajMinutama(dodatneMinute));
		return *this;
	}

	/* Funkciju kojom je mogu?e saznati o?ekivano vrijeme polijetanja kada se ura?una iznos kašnjenja
	u odnosu na predvi?eno vrijeme polijetanja (iskoristiti preklopljeni operator +). */
	Vrijeme  getOcekivanoVrijemePolijetanja()
	{
		return	(*this + _kasnjenje).getVrijemePolijetanja();
	}
	//Funkciju koja vra?a o?ekivano vrijeme slijetanja
	Vrijeme  getVrijemeSlijetanja()
	{
		return Vrijeme(this->getOcekivanoVrijemePolijetanja()).povecajMinutama(_trajanje);//TODO: provjeriti treba li sa kanjenjem ili samo trajanje
	}
	/*Preklopiti operator "++" na na?in da pomijera vrijeme polaska za jedan sat unaprijed.
	Potrebno je podržati i prefiksnu i postfiksnu verziju ovog operatora.*/
	Vrijeme & operator++()
	{
		return _vrijemePolijetanja.povecajMinutama(60);
	}
	Vrijeme  operator++(int)
	{
		Vrijeme temp = _vrijemePolijetanja;
		_vrijemePolijetanja.povecajMinutama(60);
		return temp;
	}
	/*Preklopiti relacione operatore "<" i ">" koji ispituju koji let nastupa ranije, odnosno kasnije.
	Operator "<" vra?a logi?ku vrijednost "true" ukoliko polijetanje leta sa lijeve strane nastupa
	prije polijetanje leta sa desne strane, a u suprotnom vra?a logi?ku vrijednost "false".
	Analogno vrijedi za operator ">". Prilikom upore?ivanja treba uzeti u obzir i o?ekivano vrijeme kašnjenja,
	a ne samo planirano vrijeme polijetanja.*/
	friend bool operator<(Let & l1, Let & l2)
	{
		return (l1.getOcekivanoVrijemePolijetanja() < l2.getOcekivanoVrijemePolijetanja()) ? true : false;
	}
	friend bool operator>(Let & l1, Let & l2)
	{
		return (l1.getOcekivanoVrijemePolijetanja() > l2.getOcekivanoVrijemePolijetanja()) ? true : false;
	}
	/*Preklopiti operator "<<" koji treba da podrži ispis objekata tipa "Let" na ekran. U slu?aju da
	se radi o polijetanju bez kašnjenja, ispis bi trebao da izgleda kako slijedi:

	JFK 156 Atalanta    12:50   19:30   5

	Podaci predstavljaju redom: oznaku leta, naziv odredišta, vrijeme polijetanja, o?ekivano
	vrijeme slijetanja i broj izlazne kapije.

	U slu?aju da se radi o letu koji kasni, ispis bi trebao izgledati kako slijedi:

	ZGK 932 Zagreb    15:50 (Planirano 15:30, Kasni 20 min)*/

	friend ostream & operator<<(ostream &out, Let &l)
	{
		out << l._oznaka << " " << l._odrediste << "  ";
		 (l._kasnjenje == 0)? 
			out << l._vrijemePolijetanja : out<<l.getOcekivanoVrijemePolijetanja() << "(planirano " << l._vrijemePolijetanja << ", kasni" << l._kasnjenje << " min) ";
		out << l.getVrijemeSlijetanja() << " " << l._brIzlazneKapije << endl;
		return out;
	}
	char * getOznaka() { return _oznaka; }
};


class RasporedLetova
{
	int _brojRegistrovanihLetova;
	const int _maksimalanBrojLetova;
	Let** _letovi;

public:
	RasporedLetova(int maksimalanBrojLetova) : _maksimalanBrojLetova(maksimalanBrojLetova),
		_letovi(new Let*[_maksimalanBrojLetova] {nullptr}), _brojRegistrovanihLetova(0) {
		;
	}

	/*Preklopiti operator "+=" na na?in da registruje novi let u raspored. Raspored letova u svakom momentu treba biti sortiran
	prema vremenu polijetanja. Voditi ra?una o maksimalnom broju letova.
	Za potrebe pore?enja vremena polaska letova preklopiti odgovaraju?e operatore u klasi Vrijeme.*/
	void operator+=(Let *&l)
	{
		if (_brojRegistrovanihLetova <= _maksimalanBrojLetova)
		{
			for (size_t i = 0; i < _brojRegistrovanihLetova + 1; i++)
			{
				if (_letovi[i] == nullptr)
				{
					_letovi[i] = l; _brojRegistrovanihLetova++;
					return;
				}
				if (l->getVrijemePolijetanja() < _letovi[i]->getVrijemePolijetanja())
				{
					for (size_t j = _brojRegistrovanihLetova; j > i; j --)
					{
						_letovi[j] = _letovi[j - 1];//TODO:preklopiti op= za letove
					}
					_letovi[i] = l;
					l = nullptr;
					_brojRegistrovanihLetova++;
					return;
				}

			}
		}
	}


	/*Preklopiti operator "-=" na na?in da ukljanja registrovani let sa oznakom definisanom putem desnog operanda.
	Pri tome sa?uvati redoslijed prethodno registrovanih letova. */
	void operator-=(char* oznaka)
	{
		for (size_t i = 0; i < _brojRegistrovanihLetova; i++)
		{
			if (strcmp(_letovi[i]->getOznaka(), oznaka) == 0)
			{
				for (size_t j = i; j < _brojRegistrovanihLetova; j++)
				{
					_letovi[j] = _letovi[j + 1];
				}
				delete _letovi[_brojRegistrovanihLetova];
				_letovi[_brojRegistrovanihLetova--] = nullptr;
				break;
			}
		}
	}

	//Preklopiti operator "[]" na na?in da vra?a objekat tipa Let na osnovu proslije?ene pozicije u rasporedu (pozicije kre?u od 1).
	Let & operator[](int pozicija)
	{
		if (pozicija>0 && pozicija<_brojRegistrovanihLetova)
		{
			return *_letovi[pozicija];
		}
		cout << "Trazeni let ne postoji, a zadnji postojeci let je:" << endl;
		return *_letovi[_brojRegistrovanihLetova-1];//TODO:posto je navedeno da se vrati objekat vraca se defaultni iako bi bolje bilo vratiti nullptr;
	}				//TODO: preklpiti op<< za let da moze ispisivati i defaultni let

	/*Preklopiti operator "()" na na?in da letu sa definisanom oznakom pomjeri vrijeme polijetanja za vrijednost drugog parametra izraženog
	u minutama. */
	void operator()(char * oznaka, int minutePomjeranja)
	{
		for (size_t i = 0; i < _brojRegistrovanihLetova; i++)
		{
			if (strcmp(_letovi[i]->getOznaka(), oznaka) == 0)
			{
				_letovi[i]->setVrijemePolijetanja(_letovi[i]->getVrijemePolijetanja().povecajMinutama(minutePomjeranja));//TODO:provjeriti je li porebna set fja
			}
		}
	}

	//Funkciju koja daje prosje?no trajanje svih pohranjenih letova.
	float prosjecnoTrajanjeRegistrovanihLetova()
	{
		int sumaTrajanja = 0;
		for (size_t i = 0; i < _brojRegistrovanihLetova; i++)
		{
			sumaTrajanja += _letovi[i]->getTrajanjeLeta();
		}
		return sumaTrajanja / _brojRegistrovanihLetova;//TODO: cast to float
	}

	/*Preklopiti operator "<<" koji ispisuje kompletan spisak svih letova, sortiran po o?ekivanim vremenima polazaka.
	Za potrebe pore?enja vremena polaska letova preklopiti odgovaraju?e operatore u klasi Vrijeme. */
	friend ostream & operator<<(ostream & out, RasporedLetova &r)
	{
		out << "Broj reg letova je:" << r._brojRegistrovanihLetova << endl;
		for (size_t i = 0; i < r._brojRegistrovanihLetova; i++)
		{
			out << *r._letovi[i];
		}
		return out;
	}
};

int main()
{
	Vrijeme vrijemeDef;
	Vrijeme vrijemePar(1, 1, 1);//TODO:kons kopije za vrijeme
	vrijemePar.povecajMinutama(122);
	if (vrijemeDef>vrijemePar)
	{
		cout << vrijemeDef << endl;
	}
	else if (vrijemeDef<vrijemePar) cout << vrijemePar << endl;

	Let letDef;
	Let letPar("JFK 156", "Atalanta", 5, Vrijeme(12, 50), 440, 0);
	Let * pokLetPar = new Let("ZGK 932", "Zagreb", 9, Vrijeme(15, 30), 320, 20);
	Let letKop(*pokLetPar);
	cout << letKop.getVrijemePolijetanja();
	letKop.postaviKasnjenje(22);
	if (!letKop)
	{
		cout << "let Kopije:Kasni" << endl;
	}
	else cout << "letkop ne kasni" << endl;
	cout << "Trajanje:" << letKop.getTrajanjeLeta();
	cout << "prije dodavanja vremena:" << letKop.getVrijemePolijetanja();
	cout<< " a poslije:" << (letKop + 2).getVrijemePolijetanja() << endl;
	cout << "nakon prvog dodavanja vremena:" << letKop.getVrijemePolijetanja();
	cout << " a poslije op+= :" << (letKop += 2).getVrijemePolijetanja() << endl;
	cout << "ocekivano vrijeme bez kasnjenja" << letKop.getVrijemePolijetanja() << endl;
	cout << "ocekivano vrijeme sa kasnjenjem" << letKop.getOcekivanoVrijemePolijetanja() << endl;
	cout << "Vrijeme slijetanja sa kasnjenjem:" << letKop.getVrijemeSlijetanja();
	cout << "\nvrijeme poslaska: " << letKop.getVrijemePolijetanja();
	cout<< " pomaknuto prefiksom vrijeme polaska za 1 je:" << ++letKop << endl;
	cout << "a sa postfiksom:" << letKop++ << " i nakon njega" << letKop.getVrijemePolijetanja();
	if (letDef<letKop)
	{
		cout << "letdef je manji od letkop" << endl;
	}
	else if (letDef>letKop)cout << "veci je letDEF" << endl;
	cout << "\n ispis leta letkop" << endl << letKop << endl;

	RasporedLetova raspored(10);
	raspored += pokLetPar;
	Let * pokLetPar2 = new Let("TGK 132", "Tuzla", 10, Vrijeme(12, 30), 325, 20);
	Let * pokLetPar3 = new Let("SGK 232", "Sarajevo", 29, Vrijeme(5, 10), 122, 0);
	Let * pokLetPar4 = new Let("MGK 532", "Mostar", 6, Vrijeme(9, 20), 5000, 1);
	raspored += pokLetPar2;
	raspored += pokLetPar3;
	raspored += pokLetPar4;
	raspored -= "TGK 132";
	cout << "Mostarski" << raspored[3] << endl;
	raspored("MGK 532", 202);
	cout << "prosjecno svih trajanja" << raspored.prosjecnoTrajanjeRegistrovanihLetova() << endl;
	cout << raspored;





	system("pause>null");
	return 0; cout << "a";
	return 0;
}

