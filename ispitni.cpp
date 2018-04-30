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
	Datum(int d=1, int m=1, int g=1111) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	Datum(Datum & d) {
		_dan =new int(*d._dan);
		_mjesec =new int(*d._mjesec);
		_godina =new int(*d._godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum & operator=(Datum &d)
	{
		*_dan = *d._dan;
		*_mjesec =* d._mjesec;
		*_godina = *d._godina;
		return *this;
	}
	friend ostream & operator<<(ostream & COUT, Datum & obj) {
		COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina<<endl;
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
	//FITArray(T*,bool);
	int GetTrenutno() { return _trenutno; }
	T * GetNiz() { return _niz; }
    bool operator+=(T );
	friend ostream & operator<<<>(ostream & cout, FITArray & obj);
	bool operator-=(T);
	FITArray operator()(int min, int max);
	T & operator[](int index) { return _niz[index]; }
};


template <class T,int velicina>
bool FITArray<T,velicina>::operator+=(T novi)
{	
	if(!_dozvoliDupliciranje)
	for (size_t i = 0; i < _trenutno; i++)
	{
		if (novi==_niz[i])
		{
			return false;
		}
	}
	_niz[_trenutno++] = novi;
	return true;
}

template<class T,int velicina>
bool FITArray<T, velicina>::operator-=(T item)
{
	for (size_t i = 0; i < _trenutno; i++)
		if (_niz[i] == item) 
		{
			for (size_t j = i; j < _trenutno-1 ; j++)
				_niz[j] = _niz[j + 1];
			_trenutno--;
			return true;
		}
	return false;
}
template <class T,int velicina>
ostream & operator<<<>(ostream & cout, FITArray<T, velicina> & obj)
{
	for (size_t i = 0; i < obj._trenutno; i++)
	{
		cout  << " "<< obj._niz[i];
	}
	cout << endl << "Trenutno clanova:" << obj._trenutno<<endl;
	return cout;
}
template <class T,int velicina>
FITArray<T,velicina>  FITArray<T, velicina>::operator()(int min, int max)
{
	FITArray<T, velicina> tempNiz;
	T temp[velicina];
	int velTemp = 0;
	if(min<_trenutno)
	for (size_t i = min; i < _trenutno; i++,velTemp++)
	{
		temp[velTemp] = _niz[i];
		if (i==max)
			break;
	}
	for (size_t i = 0; i < velTemp; i++)
		tempNiz._niz[i] = temp[i];
	tempNiz._trenutno = velTemp;
	
	return tempNiz ;
}



class PolozeniPredmet {
	char * _naziv;
	int _ocjena;
	Datum _datumPolaganja;
public:
	int getOcjena() { return _ocjena; }
	PolozeniPredmet(Datum datumPolaganja = { 1,1,111 }, char* naziv = "---", int ocjena = 5) :_datumPolaganja(datumPolaganja)
	{
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_ocjena = ocjena;

	}
	PolozeniPredmet(PolozeniPredmet & pOriginal) :_datumPolaganja(pOriginal._datumPolaganja)
	{
		int vel = strlen(pOriginal._naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, pOriginal._naziv);
		_ocjena = pOriginal._ocjena;
	}
	~PolozeniPredmet() { delete[] _naziv; _naziv = nullptr; }
	friend ostream & operator<<(ostream & COUT, PolozeniPredmet & obj) {
		COUT << obj._naziv << "(" << obj._ocjena << ")" << obj._datumPolaganja << endl;
		return COUT;
	}
	bool operator==(PolozeniPredmet & desni)
	{
		return desni._naziv == _naziv;
	}
	void operator=(PolozeniPredmet & desni)
	{
		if (this != &desni) {
			int vel = strlen(desni._naziv) + 1;
			delete[] _naziv;
			_naziv = new char[vel];
			strcpy_s(_naziv, vel, desni._naziv);
			_ocjena = desni._ocjena;
			_datumPolaganja = desni._datumPolaganja;
		}
	}
};

class Student {
	static int _indeksCounter; //sluzi za odredjivanje narednog broja indeksa koji ce biti dodijeljen studentu, pocevsi od 160000
	const int _indeks;
	char * _imePrezime;
	FITArray<PolozeniPredmet, 40> _polozeniPredmeti;
public:
	~Student() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	Student(char* imePrezime="---") :_indeks(_indeksCounter++)
	{
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
	}
	bool DodajPredmet(PolozeniPredmet & predmet)
	{
		_polozeniPredmeti+=(predmet);
		return true;
	}
	void operator=(Student & desni)
	{
		if (this != &desni) {
			delete[] _imePrezime;
			int vel = strlen(desni._imePrezime) + 1;
			_imePrezime = new char[vel];
			strcpy_s(_imePrezime, vel, desni._imePrezime);
			for (size_t i = 0; i < desni._polozeniPredmeti.GetTrenutno(); i++)
			{
				_polozeniPredmeti += desni._polozeniPredmeti[i];
			}
		}
		
	}
	int GetProsjek()
	{
		int ukupno = 0;
		for (size_t i = 0; i < _polozeniPredmeti.GetTrenutno(); i++)
		{
			ukupno += _polozeniPredmeti[i].getOcjena();
		}
		return ukupno / _polozeniPredmeti.GetTrenutno();
	}

	friend ostream & operator<<(ostream & COUT, Student & obj) {
		COUT << obj._indeks << " " << obj._imePrezime << endl << obj._polozeniPredmeti;
		return COUT;
	}
};
int Student::_indeksCounter = 160000;

void main() {
	const int max = 20;

	Datum jucer(20, 11, 2017), danas(21, 11, 2017), sutra(22, 11, 2017);
	cout << jucer << danas << sutra << endl;
	jucer = danas;
	cout << jucer << danas << sutra << endl;

	//parametar tipa bool odredjuje da li je u nizu dozvoljeno dupliciranje elemenata
	FITArray<int, max> nizIntegera(false);

	for (size_t i = 0; i < max - 1; i++)
		nizIntegera += i;//dodaje novog clana niza

	if (!(nizIntegera += 6))//pokusavamo dodati dupli clan niza
		cout << "Element nije dodan u niz" << endl;

	cout << crt << nizIntegera << crt; //ispisuje sve clanove niza

	nizIntegera -= 9; //uklanja clan niza sa vrijednoscu 9
	nizIntegera -= 17;

	FITArray<int, max> noviNizIntegera(nizIntegera);

	cout << crt << noviNizIntegera << crt;

	/*parametri odredjuju lokacije (indekse u nizu) elemenata OD - DO koje je potrebno vratiti. 
	u slucaju da u nizu ne postoji trazeni broj elemenata funkcija treba da vrati sve element od lokacije OD pa do posljednje dostupnog elementa */
	cout << "Clanovi niza od lokacije 2 do lokacije 5 su -> " << noviNizIntegera(2, 5) << crt;

	Student adel("Adel Handzic"), jasmin("Jasmin Azemovic");

	PolozeniPredmet prII(jucer, "PRII", 8), prIII(sutra, "PRIII", 7);

	adel.DodajPredmet(prII);
	adel.DodajPredmet(prIII);
	cout << adel << endl;
	////vraca prosjecnu ocjenu studenta
	cout << "Prosjecna ocjena -> " << adel.GetProsjek() << crt;

	jasmin = adel;
	cout << jasmin << endl;
	cout << "Prosjecna ocjena -> " << jasmin.GetProsjek() << crt;

	system("pause>fit");
}