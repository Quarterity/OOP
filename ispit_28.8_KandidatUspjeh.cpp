#include <iostream>
#include <vector>
#include <sstream>
#include <thread>
using namespace std;

#pragma warning(disable:4996)
const char *crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
char * nizRazreda[4] { "PRVI", "DRUGI", "TRECI", "CETVRTI" };

const float minimalan_prosjek = 4.5;

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int * _trenutno;
public:
	Kolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = new int(0);
	}
	Kolekcija(const Kolekcija &k) :_elementi1(new T1[*k._trenutno]), _elementi2(new T2[*k._trenutno])
	{
		_trenutno = new int(*k._trenutno);
		for (size_t i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}

	}
	Kolekcija & operator=(const Kolekcija &k)
	{
		if (this != &k)
		{

			delete[]_elementi1;
			delete[]_elementi2;
			delete _trenutno;
			_elementi1 = (new T1[*k._trenutno]);
			_elementi2 = (new T2[*k._trenutno]);
			_trenutno = (new int(*k._trenutno));
			for (size_t i = 0; i < *_trenutno; i++)
			{
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];
			}
		}
		return *this;
	}
	~Kolekcija() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}
	bool AddElement(T1 el1, T2 el2)
	{
		T1* temp1 = new T1[(*_trenutno) + 1];
		T2* temp2 = new T2[(*_trenutno) + 1];
		for (size_t i = 0; i < *_trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[*_trenutno] = el1;
		temp2[(*_trenutno)++] = el2;
		delete[]_elementi1;
		delete[]_elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		temp1 = nullptr;
		temp2 = nullptr;
		return true;
	}
	void SortOpadajuciByT2()
	{
		for (size_t i = 0; i < *_trenutno; i++)
		{
			if (_elementi2[i]<_elementi2[i + 1])
			{
				for (size_t j = i + 1; j > 0; j--)
				{
					if (_elementi2[j]>_elementi1[j - 1])
					{
						swap(_elementi1[j], _elementi1[j - 1]);
						swap(_elementi2[j], _elementi2[j - 1]);
					}
				}
			}
		}
	}
	T1& getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() { return *_trenutno; }
	friend ostream& operator<< (ostream &COUT, Kolekcija &obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
};

class DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	DatumVrijeme(const DatumVrijeme &d) :_dan(new int(*d._dan)), _mjesec(new int(*d._mjesec)), _godina(new int(*d._godina)), _sati(new int(*d._sati)), _minuti(new int(*d._minuti)) { ; }
	DatumVrijeme& operator=(const DatumVrijeme &d)
	{
		if (this != &d)
		{
			delete _dan;
			delete _mjesec;
			delete _godina;
			delete _sati;
			delete _minuti;

			_dan = (new int(*d._dan));
			_mjesec = (new int(*d._mjesec));
			_godina = (new int(*d._godina));
			_sati = (new int(*d._sati));
			_minuti = (new int(*d._minuti));
		}
		return *this;
	}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	char * ToCharArray()
	{
		stringstream s;
		s << *_dan << '.' << *_mjesec << '.' << *_godina << ' ' << *_sati << ':' << *_minuti;
		return AlocirajNizKaraktera(s.str().c_str());
	}
	friend bool operator==(DatumVrijeme & d1, DatumVrijeme &d2)
	{
		if (*d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina && *d1._sati == *d2._sati && *d1._minuti == *d2._minuti)
		{
			return true;
		}
		return false;
	}
	friend ostream& operator<< (ostream &COUT, DatumVrijeme &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
		return COUT;
	}
};

class Predmet {
	char * _naziv;
	int _ocjena;
	string _napomena;
public:
	Predmet(const char * naziv, int ocjena, string napomena = "") :_ocjena(ocjena), _napomena(napomena) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}
	Predmet(const Predmet &p) :_naziv(new char[strlen(p._naziv) + 1]), _ocjena(p._ocjena), _napomena(p._napomena)
	{
		strcpy_s(_naziv, strlen(p._naziv) + 1, p._naziv);
	}
	Predmet & operator=(const Predmet &p)
	{
		if (this != &p)
		{
			delete[] _naziv;
			_naziv = (new char[strlen(p._naziv) + 1]);
			_ocjena = (p._ocjena);
			_napomena = (p._napomena);
			strcpy_s(_naziv, strlen(p._naziv) + 1, p._naziv);
		}
		return *this;
	}
	friend ostream& operator<< (ostream &COUT, Predmet &obj) {
		COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
		return COUT;
	}
	char * GetNaziv() { return _naziv; }
	int getOcjena() { return _ocjena; }
	string GetNapomena() { return _napomena; }
	void DodajNapomenu(string napomena) {
		_napomena += " " + napomena;
	}
};
struct Uspjeh {
	eRazred _razred;
	//datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Kolekcija<Predmet*, DatumVrijeme> * _predmeti;
public:
	Uspjeh(eRazred razred) :_razred(razred), _predmeti(new  Kolekcija<Predmet*, DatumVrijeme>()) {
	}
	Uspjeh(const Uspjeh & u) :_razred(u._razred), _predmeti(new Kolekcija<Predmet*, DatumVrijeme>())
	{
		*_predmeti = *u._predmeti;
	}
	~Uspjeh() {
		delete _predmeti; _predmeti = nullptr;
	}
	int brojPredmeta() { return _predmeti->getTrenutno(); }
	bool dodajPredmet(Predmet *p, DatumVrijeme & d)
	{
		for (size_t i = 0; i < _predmeti->getTrenutno(); i++)
		{
			if (strcmp(_predmeti->getElement1(i)->GetNaziv() , p->GetNaziv())==0)
			{
				return false;
			}
		}
		return _predmeti->AddElement(p, d);
	}
	DatumVrijeme & getDatum(int i) { return _predmeti->getElement2(i); }
	eRazred getRazred() { return _razred; }
	Kolekcija<Predmet*, DatumVrijeme> * GetPredmeti() { return _predmeti; }
	friend ostream& operator<< (ostream &COUT, const Uspjeh &obj) {
		COUT << obj._razred  << endl;
		for (size_t i = 0; i < obj._predmeti->getTrenutno(); i++)
		{
			cout << *obj._predmeti->getElement1(i);
			cout << obj._predmeti->getElement2(i);
		}
		return COUT;
	}
};
void PosaljiPoruku(Kolekcija<Predmet*,DatumVrijeme>   predmeti)
{
	int sumaOcjena = 0;
	for (size_t i = 0; i < predmeti.getTrenutno(); i++)
	{
		sumaOcjena += predmeti.getElement1(i)->getOcjena();
	}
	if (((float)sumaOcjena/(float)predmeti.getTrenutno())>minimalan_prosjek)
	{
		cout << "svaka cast za uspjeh " << (float)sumaOcjena / (float)predmeti.getTrenutno()<<endl;
	}

}
void PosaljiMail(eRazred e)
{
	cout << "Uspjesno ste evidentirali 6 predmeta za razred "<<nizRazreda[e+1]<<endl;
}
class Kandidat {
	char * _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh *> _uspjeh;
public:
	Kandidat(const char * imePrezime, string emailAdresa, string brojTelefona) :_emailAdresa(emailAdresa), _brojTelefona(brojTelefona) {
		_imePrezime = AlocirajNizKaraktera(imePrezime);
	}
	~Kandidat() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _uspjeh.size(); i++) {
			delete _uspjeh[i];
			_uspjeh[i] = nullptr;
		}
	}
	/*
	uspjeh(tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	tom prilikom onemoguciti :
	-dodavanje vise od 6 predmeta za jedan razred
	-dodavanje istoimenih predmeta na nivou jednog razreda,
	-dodavanje vise predmeta u kratkom vremenskom periodu(na nivou jednog razreda, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje minut).
	razredi(predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom(npr.prvo se moze dodati uspjeh za II
	razred, pa onda za I razred i sl.).Funkcija vraca true ili false u zavisnosti od(ne)uspjesnost izvrsenja
	*/
	bool AddPredmet(eRazred e, Predmet & p, DatumVrijeme & d)
	{
		if (_uspjeh.size()>0)
		{
			int sumaPredmeta = 0;
			for (size_t i = 0; i < _uspjeh.size(); i++)
			{
				if (_uspjeh[i]->getRazred() == e)
				{
					sumaPredmeta += _uspjeh[i]->brojPredmeta();
				}
			}
			if (sumaPredmeta>=6)
			{
				return false;
			}
		}
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			for (size_t j = 0; j < _uspjeh[i]->GetPredmeti()->getTrenutno(); j++)
				if (_uspjeh[i]->getRazred() == e && d == _uspjeh[i]->getDatum(j))
				{
					return false;
				}
		}
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			if (_uspjeh[i]->getRazred() == e)
			{

				bool b = _uspjeh[i]->dodajPredmet(new Predmet(p), d);
				if (_uspjeh[i]->brojPredmeta()==6)
				{
					thread mojThread(PosaljiMail,_uspjeh[i]->getRazred());
					mojThread.join();
					Kolekcija<Predmet*, DatumVrijeme> * p = _uspjeh[i]->GetPredmeti();
					thread mojThread2(PosaljiPoruku,*p);
					mojThread2.join();

				}
				return b;
			}
		}

		Uspjeh * u = new Uspjeh(e);
		if (!u->dodajPredmet(new Predmet(p), d))
			return false;
		_uspjeh.push_back(u);
		return true;
	}
	Uspjeh * operator()(eRazred e)
	{
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			if (_uspjeh[i]->getRazred())
			{
				return _uspjeh[i];

			}
		}
	}
	friend ostream& operator<< (ostream &COUT, Kandidat &obj) {
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
		for (size_t i = 0; i < obj._uspjeh.size(); i++)
			COUT << *obj._uspjeh[i] << endl;
		return COUT;
	}
	vector<Uspjeh *> GetUspjeh() { return _uspjeh; }

};

void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/

	DatumVrijeme temp,
		datum19062018_1015(19, 6, 2018, 10, 15),
		//datum19062018_10152(19, 6, 2018, 12, 15),
		//datum19062018_10153(19, 6, 2018, 13, 15),
		//datum19062018_10154(19, 6, 2018, 14, 15),
		//datum19062018_10155(19, 6, 2018, 15, 15),
		//datum19062018_10156(19, 6, 2018, 16, 15),
		//datum19062018_10157(19, 6, 2018, 17, 15),
		datum20062018_1115(20, 6, 2018, 11, 15),
		datum30062018_1215(30, 6, 2018, 12, 15),
		datum05072018_1231(5, 7, 2018, 12, 31),
		datum20062018_1115_Copy(datum20062018_1115);
	//funkcija ToCharArray vraca datum i vrijeme kao char *
	cout << datum19062018_1015.ToCharArray() << endl;//treba ispisati: 19.6.2018 10:15
	temp = datum05072018_1231;
	cout << temp.ToCharArray() << endl;//treba ispisati: 5.7.2018 12:31
	cout << datum20062018_1115_Copy.ToCharArray() << endl;//treba ispisati: 20.6.2018 11:15

	const int kolekcijaTestSize = 10;
	Kolekcija<int, int> kolekcija1;
	for (size_t i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i + 1, i * i);
	cout << kolekcija1 << endl;

	Kolekcija<int, int> kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;
	Kolekcija<int, int> kolekcija3;
	kolekcija3 = kolekcija1;
	kolekcija3.SortOpadajuciByT2(); //od najvece prema najmanjoj vrijednosti
	cout << kolekcija3 << crt;

	//napomena se moze dodati i prilikom kreiranja objekta
	Predmet Matematika("Matematika", 5, "Ucesce na takmicenju"),
		Fizika("Fizika", 5),
		Hemija("Hemija", 2),
		//Hemija2("Hemija2", 5),
		//Hemija3("Hemija3", 5),
		//Hemija4("Hemija4", 5),
		//Hemija5("Hemija5", 6),
		//Hemija6("Hemija6", 6),
		Engleski("Engleski", 5);
	Fizika.DodajNapomenu("Pohvala za ostvareni uspjeh"); // dodaje novu napomenu zadrzavajuci ranije dodane
	cout << Matematika << endl;

	Kandidat jasmin("Jasmin Azemovic", "jasmin@gmail.com", "033 281 172");
	Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");

	/*
	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	tom prilikom onemoguciti:
	- dodavanje vise od 6 predmeta za jedan razred
	- dodavanje istoimenih predmeta na nivou jednog razreda,
	- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje minut).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II
	razred, pa onda za I razred i sl.). Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	if (jasmin.AddPredmet(DRUGI, Fizika, datum20062018_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Hemija, datum30062018_1215))
		cout << "Predmet uspjesno dodan!" << crt;
	//if (jasmin.AddPredmet(DRUGI, Hemija2, datum19062018_10152))
	//	cout << "Predmet uspjesno dodan!" << crt;
	//if (jasmin.AddPredmet(DRUGI, Hemija3, datum19062018_10153))
	//	cout << "Predmet uspjesno dodan!" << crt;
	//if (jasmin.AddPredmet(DRUGI, Hemija4, datum19062018_10154))
	//	cout << "Predmet uspjesno dodan!" << crt;
	//if (jasmin.AddPredmet(DRUGI, Hemija5, datum19062018_10155))
	//	cout << "Predmet uspjesno dodan!" << crt;
	//if (jasmin.AddPredmet(DRUGI, Hemija6, datum19062018_10156))//ne treba dodat
	//	cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Engleski, datum19062018_1015))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Matematika, datum20062018_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Matematiku jer je vec dodana u prvom razredu
	if (jasmin.AddPredmet(PRVI, Matematika, datum05072018_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Fiziku jer nije prosao minut od dodavanja posljednjeg predmeta
	if (jasmin.AddPredmet(PRVI, Fizika, datum05072018_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	/*pretpostavka je da na nivou jednog razreda kandidati imaju 6 predmeta.
	upravo zbog toga, nakon evidentiranja 6 predmeta na nivou jednog razreda kandidatu se salje mail
	sa porukom: "evidentirali ste uspjeh za X razred". ukoliko je prosjek na nivou tog razreda veci od minimalan_prosjek
	kandidatu se salje SMS sa porukom: "svaka cast za uspjeh 4.X". slanje poruka i emailova implemenitrati koristeci
	zasebne thread-ove.
	*/

	Uspjeh * u = jasmin(PRVI);//vraca uspjeh kandidata ostvaren u prvom razredu
	if (u != nullptr)
		cout << *u << endl;


	system("pause>0");
}