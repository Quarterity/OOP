#include<iostream>
using namespace std;

//Implementirati nasljeđivanje između odgovarajućih klasa
class Klijent
{
	char* _imePrezime;
	char* _adresa;
	char _JMBG[14];
	int _godiste;
public:
	//Konstruktor sa 4 parametra na osnovu kojih će se postaviti svi atributi klase
	Klijent(char* imePrezime="---", char* adresa="---", char* jmbg="---", int godiste=0)
	{
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);

		vel = strlen(adresa) + 1;
		_adresa = new char[vel];
		strcpy_s(_adresa, vel, adresa);

		strncpy_s(_JMBG, jmbg, 14);

		_godiste = godiste;

	}
	Klijent(Klijent &k)
	{
		int vel = strlen(k._imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, k._imePrezime);

		vel = strlen(k._adresa) + 1;
		_adresa = new char[vel];
		strcpy_s(_adresa, vel, k._adresa);

		strncpy_s(_JMBG, k._JMBG, 14);

		_godiste = k._godiste;

	}


	//Destruktor
	~Klijent()
	{
		delete[] _imePrezime;
		_imePrezime = NULL;
		delete[] _adresa;
		_adresa = NULL;
	}
	friend ostream & operator<<(ostream &cout,Klijent &k) {
		cout << k._imePrezime << endl << k._JMBG << endl << k._adresa << endl << k._godiste << endl;
		return cout;
	}
};

class BankovniRacun
{
protected:
	char* brojRacuna;
	double stanje;
	Klijent* vlasnik;
public:
	//Kontruktor sa 3 parametra koji postavlja sve atribute klase
	BankovniRacun(Klijent * vlasnik = new Klijent(), char* brRacuna = "---", double stanje = 0): vlasnik(vlasnik)
	{
		brojRacuna = new char[strlen(brRacuna) + 1];
		strcpy_s(brojRacuna, strlen(brRacuna) + 1, brRacuna);
		this->stanje = stanje;
	}
	BankovniRacun(BankovniRacun &b) : vlasnik(b.vlasnik)
	{
		brojRacuna = new char[strlen(b.brojRacuna) + 1];
		strcpy_s(brojRacuna, strlen(b.brojRacuna) + 1, b.brojRacuna);
		this->stanje = b.stanje;
	}
	//Destruktor
	~BankovniRacun() {	delete[] brojRacuna;}
	//Funkciju pomoću koje se vrši uplata novca na račun
	void Uplati(int uplata) {	stanje += uplata; }
	//Funkciju koja vrši isplatu i kao povratnu vrijednost vraća preostali iznos na računu
	int Isplati(int isplata) { stanje -= isplata; return stanje; }
	//Funkciju Ispis za informacije o svim atributima klase
	virtual void Ispis() {	cout << *vlasnik<<"Broj Racuna:" << brojRacuna<<"\nStanje:"<<stanje<<endl;}
};

class StedniRacun: protected BankovniRacun
{
	double kamatnaStopa;
	//Broj mjeseci
	 int* periodOrocenja;
public:
	//Konstruktor za postavljanje vrijednosti svih atributa klase
	StedniRacun(BankovniRacun *b = new BankovniRacun(), int period=0) :BankovniRacun::BankovniRacun(*b)
	{
		periodOrocenja = new int;
		*periodOrocenja = period;
	}
	StedniRacun(StedniRacun &stedni) :BankovniRacun::BankovniRacun(stedni)
	{
		kamatnaStopa = stedni.kamatnaStopa;
		periodOrocenja = new int;
		*periodOrocenja = *stedni.periodOrocenja;
	}
	~StedniRacun() { delete periodOrocenja; }
	//Na osnovu perioda oročenja postaviti odgovarajuću kamatnu stopu
	//Period oročenja - kamatna stopa
	//manje od 1 god - 4.5%
	//2 god - 5.5%
	//preko 3 god - 6.5%
	void OdrediKamatnuStopu()
	{
		switch (*periodOrocenja)
		{
		case 1:kamatnaStopa = 4.5; break;
		case 2:kamatnaStopa = 5.5; break;
		default:if(*periodOrocenja>3)kamatnaStopa = 6.5;break;
		}
	}
	double Isplati(double isplata ) { return BankovniRacun::Isplati(isplata); }
	void Uplati(double uplata) { BankovniRacun::Uplati(uplata); }
	//Funkciju ObracunajKamate koja uvećava iznos na računu na osnovu kamatne stope
	void ObracunajKamate()
	{
		BankovniRacun::Uplati(BankovniRacun::stanje*kamatnaStopa / 100);
	}
	//Funkciju Ispis
	virtual void Ispis() {
		BankovniRacun::Ispis();
		cout << "Period Orocenja:" << *periodOrocenja<<"\nKamatna Stopa:"<<kamatnaStopa<<endl;
	}
};

class DevizniRacun: protected BankovniRacun
{
public:
	//Konstruktor za postavljanje vrijednosti svih atributa klase
	DevizniRacun(BankovniRacun &b) :BankovniRacun::BankovniRacun(b){ ; }
	DevizniRacun(DevizniRacun &d) :BankovniRacun::BankovniRacun(d) { ; }
	//Implementaciju funkcija uraditi samo za valute Eur i kn
	//1Eur=1.96KM
	//1KM=3.86kn
	int Konvertuj(char * valuta,double uplata)
	{
		if (strcmp(valuta, "Eur") == 0)
			uplata *= 1.96;
		else if (strcmp(valuta, "Kn") == 0)
			uplata *= 3.86;
		return uplata;
	}
	//Uplata na račun se vrši u određenoj valuti. Potrebno je promijeniti stanje računa (koje je uvijek u KM-ovima).
	//Konvertovati iznos iz proslijeđene valute u KM, a zatim promijeniti stanje.
	void UplatiNaRacun(char * valuta, double uplata)
	{
		BankovniRacun::Uplati(Konvertuj(valuta, uplata));
	}

	//Također prilikom isplate provjeriti valutu, a zatim korisniku umanjiti stanje na računu za iznos
	//u KM-ovim, prilagođen proslijeđenoj valuti.
	//Iz funkcije vratiti trenutni iznos također u valuti koju sadrži parametar
	double IsplatiSaRacuna(char * valuta, double isplata)
	{
		return	BankovniRacun::Isplati(Konvertuj(valuta, isplata));
	}
	 void Ispis() { BankovniRacun::Ispis(); }
};


void main()
{
	//Na datim klasama demonstrirati koncepciju polimorfizma
	Klijent ajdin("Ajdin", "Bugojno", "12345678123", 1997);
	BankovniRacun b(&ajdin, "123456", 0);
	BankovniRacun c(b);
	StedniRacun stedni(&c, 2);
	DevizniRacun d(b);
	DevizniRacun e(d);
	d.Ispis();
	d.UplatiNaRacun("KM", 100);
	d.UplatiNaRacun("Eur", 100);
	d.IsplatiSaRacuna("kn", 4 * 3.96);
	d.Ispis();
	cout << "-------\n";
	stedni.Uplati(100);
	stedni.Ispis();
	stedni.OdrediKamatnuStopu();
	stedni.ObracunajKamate();
	stedni.Isplati(50);
	stedni.Ispis();
}
