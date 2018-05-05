#include <iostream>
using namespace std;


/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
6. ZA POTREBE TESTIRANJA, UNUTAR MAIN FUNKCIJE MOZETE DODAVATI NOVE TESTNE PODATKE
****************************************************************************/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

char * crt = "\n----------------------------------------------------\n";
class Datum {
	int * _dan, *_mjesec, *_godina;
public:
	Datum(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	Datum(const Datum & d)
	{
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);

	}

	Datum & operator=(const Datum & d)
	{
		if (&d!=this)
		{
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
		}
		return *this;

	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream & operator<<(ostream & COUT, Datum obj) {
		COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina;
		return COUT;
	}
};

template<class T, int velicina>
class FITArray {
	T _niz[velicina];
	int _trenutno;
	bool _dozvoliDupliciranje;
public:
	//OSTALE FUNKCIJE CLANICE DEFINISATI VAN TEMPLATE KLASE
	FITArray(bool dozvoliDupliciranje = true) { _dozvoliDupliciranje = dozvoliDupliciranje; _trenutno = 0; }
	FITArray(const FITArray &);
	int GetTrenutno() const { return _trenutno; }
	T * GetNiz() { return _niz; }
	void setDupliciranje(bool b) { _dozvoliDupliciranje = b; }
	bool getDupliciranje() { return _dozvoliDupliciranje ; }

	bool operator+=(const T & element)
	{
		if (_dozvoliDupliciranje == false)
		{
			for (size_t i = 0; i < _trenutno; i++)
			{
				if (element == _niz[i])
				{
					return false;
				}
			}
		}
		_niz[_trenutno++] = element;
		return 1;
	}
	bool operator-=(int element)
	{
		bool brisi = false;
		for (size_t i = 0; i < _trenutno; i++)
		{
			if (element == _niz[i])
			{
				brisi = true;
				_trenutno--;//?

			}
			if (brisi == true)
			{
				_niz[i] = _niz[i + 1];
			}
		}
		if (brisi == true)
		{
			return true;
		}
		else
			return false;
	}
	friend ostream & operator<<(ostream &out, FITArray & obj)
	{
		int trenutno = obj.GetTrenutno();
		T* niz = obj.GetNiz();
		for (size_t i = 0; i < trenutno; i++)
		{
			out<<niz[i];
		}
		return out;
	}
	FITArray operator()(int,int );
};

template<class T,int velicina>
FITArray<T, velicina>::FITArray<T, velicina>(const FITArray<T, velicina> & f)
{
	this->_trenutno = f._trenutno;
	for (size_t i = 0; i < f._trenutno; i++)
	{
		this->_niz[i] = f._niz[i];
	}
	this->_dozvoliDupliciranje = f._dozvoliDupliciranje;

};
template<class T, int velicina>
FITArray<T, velicina>  FITArray<T,velicina>::operator()(int a, int b)
{
	FITArray<T, velicina> novi;
	novi._trenutno=0;
	novi._dozvoliDupliciranje = this->_dozvoliDupliciranje;
	for (size_t i = a; i < this->_trenutno; i++)
	{
		novi._niz[novi._trenutno++] = this->_niz[i];
		if (i==b)
		{
			break;
		}
	}
	return novi;
};

class PolozeniPredmet {
	char * _naziv;
	int _ocjena;
	Datum _datumPolaganja;
public:
	PolozeniPredmet(Datum & dat= Datum(1,1,1111),char * naziv=new char(' '),int ocjena=0):_datumPolaganja(dat),_ocjena(ocjena)
	{
		_naziv = new char[strlen(naziv) + 1];
		strcpy_s(_naziv, strlen(naziv) + 1, naziv);
	}
	PolozeniPredmet(PolozeniPredmet & p):_datumPolaganja(p._datumPolaganja)
	{
			_naziv = new char[strlen(p._naziv) + 1];
			strcpy_s(_naziv, strlen(p._naziv) + 1, p._naziv);
			_ocjena = p._ocjena;
	}
	bool operator==( const PolozeniPredmet p) const
	{
		if (p._naziv==_naziv)
		{
			return true;
		}
	}
	int GetOcjena() { return _ocjena; }
	PolozeniPredmet & operator=(const PolozeniPredmet & p)
	{

		if (this != &p)
		{
			delete[] _naziv;
			_naziv = new char[strlen(p._naziv) + 1];
			strcpy_s(_naziv, strlen(p._naziv) + 1, p._naziv);
			_ocjena = p._ocjena;
			_datumPolaganja = p._datumPolaganja;
		}
		return *this;
	}
	~PolozeniPredmet() { delete[] _naziv; _naziv = nullptr; }
	friend ostream & operator<<(ostream & COUT, PolozeniPredmet & obj) {
		COUT << obj._naziv << "(" << obj._ocjena << ")" << obj._datumPolaganja << endl;
		return COUT;
	}
};

class Student {
	static int _indeksCounter; //sluzi za odredjivanje narednog broja indeksa koji ce biti dodijeljen studentu, pocevsi od 160000
	const int _indeks;
	char * _imePrezime;
	FITArray<PolozeniPredmet, 40> _polozeniPredmeti;
public:
	Student(string imePrezime=""):_indeks(_indeksCounter++)
	{
		_imePrezime = new char[imePrezime.size() + 1];
		strcpy_s(_imePrezime, imePrezime.size() + 1,(imePrezime).c_str());
		_polozeniPredmeti.setDupliciranje(false);
	}
	Student(Student & s):_indeks(s._indeks)
	{
		_imePrezime = new char[strlen(s._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(s._imePrezime) + 1, s._imePrezime);
		_polozeniPredmeti.setDupliciranje(s._polozeniPredmeti.getDupliciranje());
		for (size_t i = 0; i < s._polozeniPredmeti.GetTrenutno(); i++)
		{
			_polozeniPredmeti+=((s._polozeniPredmeti.GetNiz())[i]);
		}
	}
	Student &  operator=(Student & s)
	{	
		if (this!=&s)
		{
			delete[] _imePrezime;
			_imePrezime = new char[strlen(s._imePrezime) + 1];
			strcpy_s(_imePrezime, strlen(s._imePrezime) + 1, s._imePrezime);
			_polozeniPredmeti.setDupliciranje(s._polozeniPredmeti.getDupliciranje());
			for (size_t i = 0; i < s._polozeniPredmeti.GetTrenutno(); i++)
				{
					_polozeniPredmeti+=(s._polozeniPredmeti.GetNiz())[i];
				}
		}
		return *this;
	}
	~Student() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	double GetProsjek()
	{
		double ukupno=0;
		for (size_t i = 0; i < _polozeniPredmeti.GetTrenutno(); i++)
		{
			ukupno += (_polozeniPredmeti.GetNiz())[i].GetOcjena();
		}
		return ukupno / _polozeniPredmeti.GetTrenutno();
	}
	void DodajPredmet(PolozeniPredmet & p)
	{
		_polozeniPredmeti+=(p);
	}

	int GetIndeks() { return _indeksCounter; }
	friend ostream & operator<<(ostream & COUT, Student & obj) {
		COUT << obj._indeks << " " << obj._imePrezime << endl << obj._polozeniPredmeti;
		return COUT;
	}
};
int Student::_indeksCounter = 160000;

void main() {
	const int max = 20;
	int a = 0;
	Datum jucer(20, 11, 2017), danas(21, 11, 2017), sutra(22, 11, 2017);
	cout << jucer << danas << sutra << endl;
	jucer = danas;
	cout << jucer << danas << sutra << endl;

	//parametar tipa bool odredjuje da li je u nizu dozvoljeno dupliciranje elemenata
	FITArray<int, max> nizIntegera(false);

	for (size_t i = 0; i < max - 1; i++)
		nizIntegera+=i;//dodaje novog clana niza

	if (!(nizIntegera += int(6)))//pokusavamo dodati dupli clan niza
		cout << "Element nije dodan u niz" << endl;

	cout << crt << nizIntegera << crt; //ispisuje sve clanove niza

	nizIntegera -= 9; //uklanja clan niza sa vrijednoscu 9
	nizIntegera -= 17;
	FITArray<int, max> noviNizIntegera(nizIntegera);

	cout << crt << noviNizIntegera << crt;

	/*parametri odredjuju lokacije (indekse u nizu) elemenata OD - DO koje je potrebno vratiti. u slucaju da u nizu ne postoji
	trazeni broj elemenata funkcija treba da vrati sve element od lokacije OD pa do posljednje dostupnog elementa */
	cout << "Clanovi niza od lokacije 2 do lokacije 5 su -> " << noviNizIntegera(2, 5) << crt;
	Student adel("Adel Handzic"), jasmin("Jasmin Azemovic");
	cout << jasmin<<crt;
	PolozeniPredmet prII(jucer, "PRII", 8), prIII(sutra, "PRIII", 7);

	adel.DodajPredmet(prII);
	adel.DodajPredmet(prIII);
	cout << adel << endl;
	////vraca prosjecnu ocjenu studenta
	cout << "Prosjecna ocjena -> " << adel.GetProsjek() << crt;

	jasmin = adel;
	cout << jasmin << endl;
	cout << "Prosjecna ocjena -> " << jasmin.GetProsjek() << crt;
	system("pause>FIT");
}