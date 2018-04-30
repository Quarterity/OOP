#include<iostream>
#include<string>
#include<regex>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

//Koristiti klasu NedozvoljenaOperacija za bacanje izuzetaka u cijelom programu
class NedozvoljenaOperacija : public exception
{
	int _linijaKoda;
public:
	NedozvoljenaOperacija(const char* poruka, int linijaKoda) : exception(poruka)
	{
		_linijaKoda = linijaKoda;
	}

	void Info()
	{
		cout << "Greska u liniji koda " << _linijaKoda << ": " << exception::what() << endl;
	}
};
string crt = "\n---------------------------------------------\n";

template<class T1, class T2>
class FITKolekcija
{
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	void SetTrenutno(int i) { _trenutno = i; }
	//Konstruktor, destruktor i ostale funkcije potrebne za rad sa klasom
	FITKolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	~FITKolekcija() {
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	void RemoveAll() {
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	FITKolekcija(const FITKolekcija &f)
	{
		_elementi1 = new T1[f._trenutno];
		_elementi2 = new T2[f._trenutno];
		for (size_t i = 0; i < f._trenutno; i++)
		{
			_elementi1[i] = f._elementi1[i];
			_elementi2[i] = f._elementi2[i];
		}
		_trenutno = f._trenutno;
	}
	FITKolekcija & operator=(const FITKolekcija &f)
	{
		if (this != &f) {
			delete[] _elementi1;
			delete[] _elementi2;
			_elementi1 = new T1[f._trenutno];
			_elementi2 = new T2[f._trenutno];
			for (size_t i = 0; i < f._trenutno; i++)
			{
				_elementi1[i] = f._elementi1[i];
				_elementi2[i] = f._elementi2[i];
			}
			_trenutno = f._trenutno;
		}
		return *this;
	}
	//izmjena povratna vrijednost,kako ne bi ponovno provjeravali postoji li element 
	int	AddElement(const T1 & el1, const T2 & el2) {
		for (size_t i = 0; i < _trenutno; i++)
		{
			if (el1 == _elementi1[i] || el2 == _elementi2[i])
			{
				cout << "\nelement vec postoji\n";
				return i;
			}
		}
		T1 * temp1 = new T1[_trenutno + 1];
		T2 * temp2 = new T2[_trenutno + 1];
		for (size_t i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno++] = el2;

		return -1;

	}

	//Larisa: Nema potrebe da se ide kroz petlju da se pronađe index, može se odmah ići od njegove pozicije
	//i raditi zamjena elemenata.
	//U nizu su elementi tipa T1 i T2, a ne nizovi, pa dealokacija na ovaj način ne ide.
	//Ima smisla samo umanjiti kompletan niz za jedan element.
	bool RemoveElement(const int index)
	{
		if (index>=0 && index<_trenutno)
		{
			for (size_t j = index; j < _trenutno - 1; j++)
			{
				_elementi1[j] = _elementi1[j + 1];
				_elementi2[j] = _elementi2[j + 1];
			}
			_trenutno--;
		return true;
		//L1
		}
	return false;
}

int GetTrenutno() const { return _trenutno; }
T1& GetElement1(const int index) const {
	if (index<_trenutno && index >= 0) //Larisa: >=0 && umjesto || < _trenutno
	{//L2
		return _elementi1[index];
	}
	throw NedozvoljenaOperacija("Index was out of range", __LINE__);
}

//Larisa: Zašto porediti adrese prvih elemenata u nizu?
friend bool operator==(const FITKolekcija & fitk, const FITKolekcija & fitk2)
{
	if (fitk.GetTrenutno()!=fitk2.GetTrenutno())
	{
		return false;
	}
	/*for (size_t i = 0; i <fitk.GetTrenutno(); i++)
	{
		if (fitk.GetElement1(i)=fitk2.GetElement1(i) || fitk.GetElement2(i)!=fitk2.GetElement2(i))
		{
			return false;
		}
	}*/
	//L4
	return true;
}
T2& GetElement2(const int index) {
	if (index<_trenutno && index>=0) //Larisa: >=0 && < _trenutno
	{
		return _elementi2[index];
	}//L5
	throw NedozvoljenaOperacija("Index was out of range", __LINE__);
}
friend ostream& operator<< <>(ostream &out, const FITKolekcija<T1, T2> & f);

};
template <class T1, class T2>
ostream& operator<< <>(ostream &out, const FITKolekcija<T1, T2> & f)
{
	for (size_t i = 0; i < f._trenutno; i++)
	{
		out << f._elementi1[i];
		out << f._elementi2[i];
	}
	return out;
}


//Klasu proglasiti apstraktnom
class  Osoba
{
	char* _ime;
	char* _prezime;
	int _godiste;
public:
	string GetIme() const { return string(_ime) + " " + string(_prezime); }
	Osoba(const char* ime, const char* prezime, const int godiste)
	{
		int vel = strlen(ime) + 1;
		_ime = new char[vel];
		strcpy_s(_ime, vel, ime);

		vel = strlen(prezime) + 1;
		_prezime = new char[vel];
		strcpy_s(_prezime, vel, prezime);

		_godiste = godiste;
	}
	Osoba()
	{
		_ime = nullptr;
		_prezime = nullptr;
		_godiste = 0;
	}
	Osoba(const Osoba &o)
	{
		int vel = strlen(o._ime) + 1;
		_ime = new char[vel];
		strcpy_s(_ime, vel, o._ime);

		vel = strlen(o._prezime) + 1;
		_prezime = new char[vel];
		strcpy_s(_prezime, vel, o._prezime);

		_godiste = o._godiste;
	}

	//Larisa: Dealocirati prethodne vrijednosti u nizovima
	Osoba & operator=(const Osoba &o)
	{
		if (&o != this) {
			delete[] _ime;
			delete[] _prezime;//L6
			int vel = strlen(o._ime) + 1;
			_ime = new char[vel];
			strcpy_s(_ime, vel, o._ime);

			vel = strlen(o._prezime) + 1;
			_prezime = new char[vel];
			strcpy_s(_prezime, vel, o._prezime);

			_godiste = o._godiste;
		}
			return *this;
	}

	//Larisa: Dealocirati prethodne vrijednosti u nizovima

	void Add(const char* ime, const char* prezime, const int godiste)
	{
		delete[] _ime;
		delete[] _prezime;//L7
		int vel = strlen(ime) + 1;
		_ime = new char[vel];
		strcpy_s(_ime, vel, ime);
		vel = strlen(prezime) + 1;
		_prezime = new char[vel];
		strcpy_s(_prezime, vel, prezime);
		_godiste = godiste;
	}
	virtual ~Osoba()
	{
		delete[] _ime;
		_ime = nullptr;

		delete[] _prezime;
		_prezime = nullptr;
	}
	virtual void info() = 0 { cout << _ime << endl << _prezime << _godiste; }
	friend ostream & operator<<(ostream &out, const Osoba &o)
	{
		out << o._ime << endl << o._prezime << endl << o._godiste;
		return out;
	}
};

//Prepoznati nasljedjivanje izmedju odgovarajući klasa
//I ucesnici i clanovi komisije trebaju imati podatke za pristup sistemu (email i password)
class Ucesnik : public Osoba
{
	char* _email;
	string password; //Najmanje 5 karaktera, kombinacija brojeva i velikih slova
	string _obrazovnaInstitucija;
public:
	string getImeUcesnika() const { return Osoba::GetIme(); }
	//Potrebne funkcije za rad sa klasom
	Ucesnik(const char* ime, const char* prezime, const int godiste,
		const char * email, const string sifra, const string institucija) :Osoba("ime", "prezime", 1111)//L8
	{

		//string pravilo = ("(?=.{5,})(?=.*[A-Z])(?=.*[0-9])");
		//string pravilo2 = ("(?=.*[a - z])|(?=.*[.,\\[\\]\"#\$%&/\(\)\='\+\?\*-])");//Larisa: Nepotrebno?
		//if (!regex_search(sifra, regex(pravilo)) || regex_search(sifra, regex(pravilo2)))
		//{
		//	//Larisa: U ovom slučaju će se postaviti podaci o osobi, a neće o učesniku.
		//	//Trebalo je onda i za osobu postaviti defaultn-e vrijednosti, a ako sve bude validno onda dodijeliti
		//	//vrijednosti parametara.
		//	throw NedozvoljenaOperacija("Pogresna sifra!", __LINE__);
		//}
		string pravilo3 = "^[A-Z0-9]*$";
		if (!regex_match(sifra,regex(pravilo3)))
		{
			throw NedozvoljenaOperacija("Pogresna sifra!", __LINE__);
		}
		Osoba::Add(ime, prezime, godiste);
		_email = new char[strlen(email) + 1];
		strcpy_s(_email, strlen(email) + 1, email);
		password = sifra;
		_obrazovnaInstitucija = institucija;
	}
	friend bool operator==(const Ucesnik &u, const Ucesnik &u2) { return (strcmp(u._email, u2._email) == 0); }
	Ucesnik():Osoba()//L9
	{
		//Osoba *o = (Osoba*)this;
		//Larisa: Zašto ne pisati odmah this->Add...?
		//Još je bolje odmah pozvati u zaglavlju konstruktor klase Osoba i proslijediti vrijednosti
		//ili jednostavno iskoristiti default-ni konstruktor klase Osoba
		//o->Add("Ime", "Prezime", 1000);
		_email = new char[strlen("email") + 1];
		strcpy_s(_email, strlen("email") + 1, "email");
		password = "password";
		_obrazovnaInstitucija = "obrazovnaInstitucija";

	}
	Ucesnik(const Ucesnik &u):Osoba(u)//Larisa: Pozvati kons. kopije klase Osoba
	{
		//L10
		_email = new char[strlen(u._email) + 1];
		strcpy_s(_email, strlen(u._email) + 1, u._email);
		password = u.password;
		_obrazovnaInstitucija = u._obrazovnaInstitucija;
	}
	void operator=(const Ucesnik &u)
	{
		Osoba *o = (Osoba*)this;
		*o = u;
		delete[] _email;//L11
		//Larisa: Uraditi dealokaciju prethodno zauzete memorije.
		_email = new char[strlen(u._email) + 1];
		strcpy_s(_email, strlen(u._email) + 1, u._email);
		password = u.password;
		_obrazovnaInstitucija = u._obrazovnaInstitucija;

	}
	virtual ~Ucesnik()
	{
		delete[] _email;
		_email = nullptr;
	}
	virtual void info() {
		Osoba::info();
		cout << _email << endl << _obrazovnaInstitucija << endl << password << endl;
	}
	char * GetEmail() { return _email; }
	string GetPassword() { return password; }
	string GetInstitucija() { return _obrazovnaInstitucija; }
	friend ostream & operator<<(ostream & out, Ucesnik &u) {
		Osoba *p = dynamic_cast<Osoba*> (&u);
		if (p != NULL) out << *p;
		out << u._obrazovnaInstitucija << endl << u._email << endl;
		return out;
	}

};

class ClanKomisije :public Ucesnik // Svaki clan komisije ucestvuje u takmicenju sa povlastima i ulogom Clana komisije? 
{
	char* _pozicija; //Radno mjesto u firmi
	string _firma;
public:
	//Potrebne funkcije za rad sa klasom
	//Larisa: Generalno je poželjno da se od krajnjeg korisnika sakrije nasljeđivanje, odnosno da
	//konstruktori uvijek prime pojedinačne parametre koji se postavljaju za određeni objekat.
	ClanKomisije(const char* ime, const char* prezime, const int godiste,
		const char * email, const string sifra, const string institucija, const char* pozicija, const string firma) :Ucesnik::Ucesnik( ime, prezime,godiste,email,sifra, institucija)
	{//L12
		_pozicija = new char[strlen(pozicija) + 1];
		strcpy_s(_pozicija, strlen(pozicija) + 1, pozicija);
		_firma = firma;
	}
	ClanKomisije(const ClanKomisije & clan) :Ucesnik::Ucesnik(clan)
	{
		_pozicija = new char[strlen(clan._pozicija) + 1];
		strcpy_s(_pozicija, strlen(clan._pozicija) + 1, clan._pozicija);
		_firma = clan._firma;
	}
	void operator=(const ClanKomisije & clan)
	{
		Ucesnik *u = (Ucesnik*)this;
		*u = clan;
		_pozicija = new char[strlen(clan._pozicija) + 1];
		strcpy_s(_pozicija, strlen(clan._pozicija) + 1, clan._pozicija);
		_firma = clan._firma;
	}
	ClanKomisije() {//Larisa: Default-ni konstruktor se ne treba pozivati
		//L13
		_pozicija = new char[strlen("pozicija") + 1];
		strcpy_s(_pozicija, strlen("pozicija") + 1, "pozicija");
		_firma = "firma";
	}
	virtual ~ClanKomisije() { delete[] _pozicija; _pozicija = nullptr; }
	virtual void info() {
		Ucesnik::info();
		cout << _pozicija << endl << _firma << endl;
	}
	bool operator==(const ClanKomisije *clan) const
	{
		if (clan->GetIme() == GetIme())
		{
			return true;
		}
	}
	friend ostream & operator<<(ostream &out, const ClanKomisije &clan)
	{
		out << Ucesnik(clan);
		out << clan._firma << endl << clan._pozicija << endl;
		return out;
	}

};
bool ProvjeriDatum(string datum)
{
	//salih//string p = "[0-9]{2}\\.[0-9]{2}\\.[0-9]{4}";
	string pravilo = "(\\d{2})(.)+(\\d{2})(.)+(\\d{4})";
	if (regex_match(datum, regex(pravilo)))
	{
		return true;
	}
	return false;
}
enum Kategorija { GameChallenge, InnovationChallenge, MobileChallenge, SQLChallenge, CodingChallenge ,Default};

//Prijave koje ne ukljucuju izradu i predaju projekata (kategorije: SQLChallenge i CodingChallenge)
class Prijava {
	string _datum; //Podrzani format: dd.MM.yyyy
	Kategorija _kategorija;
	FITKolekcija<Ucesnik, bool> _tim; //Tim se sastoji od jednog ili vise ucesnika, drugi paramtetar oznacava predstavnika tima
public:
	bool operator==(const Prijava &p) const { return (p._tim == _tim && _kategorija == p._kategorija); }
	string getDatum() const { return _datum; }
	Kategorija getKategorija() const { return _kategorija; }
	int getBrojUcesnika()const { return (_tim.GetTrenutno()); }
	//virtual ~Prijava() { ; } //Larisa: Suvišno navoditi ako ne treba implementacija.
	string GetUcesnik(const int index) const {
		if (index > _tim.GetTrenutno() || index<0) throw NedozvoljenaOperacija("Pogresan index", __LINE__);//Larisa: || <0
			//Larisa: Nepotrebna petlja, samo odmah vratiti vrijednosti na poziciji index 
			 return _tim.GetElement1(index).getImeUcesnika();
			 //L14
	}
	//Konstruktor
	//Larisa: Konstruktori, desktruktor i operator= se uvijek prvi implementiraju u smislu redoslijeda
	//Larisa: Možda bi dobro bilo dodati jednu kategoriju koja je dafult, a nema konkretno značenje
	Prijava() { _datum = "01.01.1111."; _kategorija = Default; _tim.AddElement(Ucesnik(), false); }//L15
	Prijava(const string datum, const Kategorija &k) :_kategorija(k) {
		if (ProvjeriDatum(datum))
		{
			_datum = (datum);
		}
		else throw NedozvoljenaOperacija("pogresan format datuma", __LINE__);
	}
	//Prijava(const Prijava & p)//Larisa: Ne treba, ugrađeni je ok.
	//{
	//	_datum = p._datum;
	//	_kategorija = p._kategorija;
	//	_tim = p._tim;
	//}
	//void operator=(Prijava & p)//Larisa: Ne treba, ugrađeni je ok.
	//{
	//	_datum = p._datum;
	//	_kategorija = p._kategorija;
	//	_tim = p._tim;
	//}
	//Funkciju za dodavanje clanova tima uz postivanje ogranicenja maksimalno dozvoljenog broja clanova u timu.
	//SQLChallenge i CodingChallenge: 2
	//GameChallenge i MobileChallenge: 3
	//InnovationChallenge: 4
	//Onemoguciti dodavanje istog ucesnika vise puta. Tim moze imati najvise jednog predstavnika.
	void DodajUcesnika(const Ucesnik &u, const bool predstavnik)
	{
		switch (_kategorija)
		{
		case GameChallenge:
		case MobileChallenge:
			if (_tim.GetTrenutno() == 2) throw NedozvoljenaOperacija("Prekoracen broj clanova!", __LINE__); break;
		case SQLChallenge:
		case CodingChallenge:
			if (_tim.GetTrenutno() == 3) throw NedozvoljenaOperacija("Prekoracen broj clanova!", __LINE__); break;
		case InnovationChallenge:
			if (_tim.GetTrenutno() == 4) throw NedozvoljenaOperacija("Prekoracen broj clanova!", __LINE__); break;
		default:
			throw NedozvoljenaOperacija("Pogresna kategorija!", __LINE__);
		}
		if (_tim.AddElement(u, predstavnik) != -1) throw NedozvoljenaOperacija("Ucesnik vec postoji!", __LINE__);
	}
	//Funkciju za promjenu predstavnika tima.
	void PromijeniPredstavnika(const Ucesnik &u)
	{
		for (size_t i = 0; i < _tim.GetTrenutno(); i++)
		{
			if (_tim.GetElement2(i)) _tim.GetElement2(i) = false;
			if (u == _tim.GetElement1(i))	_tim.GetElement2(i) = true;
		}
	}
	virtual void info() {
		cout << _datum << endl << _kategorija << endl << _tim;
	}
	friend ostream & operator<<(ostream &out, const Prijava &p) {
		out << p._datum << endl << p._kategorija << endl << p._tim << endl;
		return out;
	}
};

//Prijave koje ukljucuju izradu projekata (kategorije: GameChallenge, InnovationChallenge i MobileChallenge)
class PrijavaProjekta : public Prijava {
	char* _nazivProjekta;
	string _opisProjekta;
	bool _finalizirano; //Atribut oznacava da li je projekat zavrsen i predan u predvidjenom roku
	bool _prezentacija; //Projekti koji se prezentuju i ocjenjuju
public:
	bool getFinalizirano() { return _finalizirano; }
	bool getPrezentovano() { return _prezentacija; }
	void definisiPrijavu(bool fin, bool prez) { _finalizirano = fin; _prezentacija = prez; }
	//Konstruktor i destruktor
	PrijavaProjekta(const string datum, const Kategorija &k, const char * nazivProjekta, const string opisProjekta) :Prijava(datum, k)
	{
		_nazivProjekta = new char[strlen(nazivProjekta) + 1];
		strcpy_s(_nazivProjekta, strlen(nazivProjekta) + 1, nazivProjekta);
		_opisProjekta = opisProjekta;
		_finalizirano = false; //Larisa: Ne treba slati u konstruktoru, jer se prijava tek kreira (false), const bool finalizirano, const bool prezentacija
		_prezentacija = false; //Larisa: Ne treba slati u konstruktoru, jer se prijava tek kreira (false)
		//L16
	}
	PrijavaProjekta() :Prijava("1.1.111", GameChallenge)
	{
		_nazivProjekta = new char[strlen("nazivProjekta") + 1];
		strcpy_s(_nazivProjekta, strlen("nazivProjekta") + 1, "nazivProjekta");
		_opisProjekta = "opisProjekta";
		_finalizirano = false;
		_prezentacija = false;
	}
	~PrijavaProjekta() { delete[] _nazivProjekta; }
	//Ostale potrebne funkcije za rad sa klasom
	PrijavaProjekta(const PrijavaProjekta &pp) :Prijava(pp)
	{
		_nazivProjekta = new char[strlen(pp._nazivProjekta) + 1];
		strcpy_s(_nazivProjekta, strlen(pp._nazivProjekta) + 1, pp._nazivProjekta);
		_opisProjekta = pp._opisProjekta;
		_finalizirano = pp._finalizirano;
		_prezentacija = pp._prezentacija;
	}
	void operator=(const PrijavaProjekta &pp)
	{
		//Prijava::Prijava(pp);//Larisa: Značenje? Konstruktori se ne mogu pozvati nego u momentu nastanka objekta.
		//L17
		_nazivProjekta = new char[strlen(pp._nazivProjekta) + 1];
		strcpy_s(_nazivProjekta, strlen(pp._nazivProjekta) + 1, pp._nazivProjekta);
		_opisProjekta = pp._opisProjekta;
		_finalizirano = pp._finalizirano;
		_prezentacija = pp._prezentacija;
	}
	bool operator==(const PrijavaProjekta &pp) { return pp._nazivProjekta == _nazivProjekta; }
	virtual void info()
	{
		Prijava::info();
		cout << _nazivProjekta << endl << _opisProjekta << endl;
		if (_finalizirano) cout << "Finalizirano!" << endl;
		if (_prezentacija) cout << "Prezentovano" << endl;
	}
	friend ostream & operator<<(ostream &out, const PrijavaProjekta &p) {
		out << Prijava(p);
		out << p._nazivProjekta << endl << p._opisProjekta << endl << p._finalizirano << endl << p._prezentacija << endl;

	}

};


int DatumUBroj(string datum)
{
	// 12.12.2018
	// 01.01.2019

	//20180000 + 1200 + 12 ==> 20181212
	//20190000 + 100  + 1  ==> 20190101
	string pDatum = datum;

	char a = datum[0];
	int dani = 10 * (atoi(&a));
	a = datum[1];
	dani += (atoi(&a));
	//dani *= 365;

	a = datum[3];
	int mjesec = 10 * (atoi(&a));
	a = datum[4];
	mjesec += (atoi(&a));
	mjesec *= 100;

	a = datum[6];
	int godina= 1000 * (atoi(&a));
	a = datum[7];
	godina += 100*(atoi(&a));
	a = datum[8];
	godina += 10 * (atoi(&a));
	a = datum[9];
	godina += (atoi(&a));
	godina *= 10000;

	//char cDays[3];
	//strncpy_s(cDays, pDatum.c_str(), _TRUNCATE);

	//char cMopnths[3];
	//strncpy_s(cMopnths, pDatum.c_str() + 3, _TRUNCATE);

	//char cYears[5];
	//strncpy_s(cYears, pDatum.c_str() + 6, _TRUNCATE);

	//int dani2= (atoi(cDays) + atoi(cMopnths) * 100 + atoi(cYears) * 10000);

	//cout << crt << dani+mjesec+godina << crt<< dani2 ;


	return dani;
};
class FITCodingChallenge
{
	int godina;
	string _prijaveDo; //Podrzani format unosa: dd.MM.yyyy (koristiti regex)
	vector<Prijava*> _prijave;
	FITKolekcija<ClanKomisije, Kategorija> _komisija;
	FITKolekcija<Prijava*, FITKolekcija<ClanKomisije*, double>> _ocjene;
public:
	//Konstruktor
	FITCodingChallenge(const int pgodina, const string prijaveDo) : godina(pgodina) {
		if (ProvjeriDatum(prijaveDo))
		{
			_prijaveDo = (prijaveDo);
		}
		else throw NedozvoljenaOperacija("pogresan format datuma za fitcc", __LINE__);

	}
	~FITCodingChallenge()
	{
		for (size_t i = 0; i < _prijave.size(); i++)
		{
			delete _prijave[i];
		}
		_prijave.clear();
		for (size_t i = 0; i < _ocjene.GetTrenutno(); i++)
		{
			delete _ocjene.GetElement1(i);
			for (size_t j = 0; j < _ocjene.GetElement2(i).GetTrenutno(); j++)
			{
				delete _ocjene.GetElement2(i).GetElement1(j);
			}
			//_ocjene.GetElement2(i).RemoveAll();
		}
		//_ocjene.RemoveAll(); //Larisa: U desktruktoru ovo ne treba pisati, jer će se pozvati destruktor i za samu kolekciju
		//L18
		//_komisija.RemoveAll();
	}
	//Operator += za dodavanje novih prijava. Isti ucesnik ne moze imati vise prijava u sklopu iste kategorije.
	//Postovati ogranicenje roka za prijave (_prijaveDo).
	//Voditi racuna o alokaciji memorije za prijave (ne zadrzavati adresu proslijedjenog objekta)
	void operator+=(Prijava & prijava)
	{
		for (size_t i = 0; i < _prijave.size(); i++)
			for (size_t j = 0; j < prijava.getBrojUcesnika(); j++)
				if (_prijave[i]->getKategorija() == prijava.getKategorija())
				{
					for (size_t y = 0; y < _prijave[i]->getBrojUcesnika(); y++)
					{
						if (_prijave[i]->GetUcesnik(y) == prijava.GetUcesnik(j))
							throw NedozvoljenaOperacija(" Isti ucesnik ne moze imati vise prijava u sklopu iste kategorije", __LINE__);
					}
				}
		if (DatumUBroj(prijava.getDatum())<DatumUBroj(_prijaveDo))
			throw NedozvoljenaOperacija(" Prijava zakasnila", __LINE__);
		Prijava * nova = new Prijava(prijava);
		_prijave.push_back(nova);
	}
	//Funkciju koja odredjenu prijavu oznacava kao finaliziranu i odabranu za prezentaciju ili ne.
	void DefinisiPrijavu(const int indexPrijave, const bool finalizirana, const bool prezentacijska) {
		for (size_t i = 0; i < _prijave.size(); i++)
		{
			if (i == indexPrijave)
			{
				PrijavaProjekta * p = dynamic_cast<PrijavaProjekta *> (_prijave[i]);
				if (p != NULL)
					p->definisiPrijavu(finalizirana, prezentacijska);
			}
		}
	}
	//Operator () za dodavanje clanova komisije po kategorijama. Sprijeciti dodavanje istih clanova u sklopu jedne kategorije.
	void operator()(const ClanKomisije &clan, const Kategorija  &kategorija)
	{
		if (_komisija.AddElement(clan, kategorija) != -1)
			throw NedozvoljenaOperacija("Clan ne moze biti ponovo dodan!", __LINE__);
	}

	//Funkciju za ispis svih prijava po kategorijama. Funkcija neobavezno prima parametar kategoriju.
	//Ispis treba da ukljuci i podatke o ucesnicima, odnosno clanovima tima.
	//Ukoliko kategorija nije proslijedjena, podaci treba da budu grupisani po kategorijama.
	void ispisPrijavaKategorijama(const Kategorija & kategorija = Default) // =Kategorija() za defaultnu kategoriju
	{
		for (size_t i = 0; i < _prijave.size(); i++)
			if (kategorija == _prijave[i]->getKategorija())
			{
				_prijave[i]->info();
				if (i == _prijave.size() - 1) return;
			}
		for (size_t j = 0; j < 5; j++)
			for (size_t i = 0; i < _prijave.size(); i++)
				if (_prijave[i]->getKategorija() == Default)
					_prijave[i]->info();
	}

	//Funkciju za ocjenu projekata (radova) od strane clanova komisije (kolekcija _ocjene).
	//Kolekcija sadrzi samo pokazivace (bez alokacije nove memorije) na postojece prijave i vec definisane clanove komisije.
	//Voditi racuna da se ne mogu ocijeniti projekti koji nisu finalizirani i nisu odabrani za prezentaciju, niti da ocjenu mogu dati clanovi komisije 
	//koji nisu pridruzeni odgovarajucoj kategoriji prijave. Svaki clan komisije daje ocjenu za svaki projekat, odnosno rad, u
	//kategoriji gdje je dodijeljen. Ocjene se krecu u rasponu od 1 do 10 za projekte, a za radove u kategoriji SQL ili CodingChallenge
	//u rasponu osvojenih bodova od 0 do 100. Pretpostaviti da su svi ucesnici prijavljeni u kategorijama SQL i CodingChallenge predali radove.
	void OcjeniProjekte(const int indexPrijave, const int indexKomisije, double Ocjena)
	{
		for (size_t i = 0; i < _prijave.size(); i++)
		{
			if (i == indexPrijave)
			{
				PrijavaProjekta * projekat = dynamic_cast<PrijavaProjekta*> (_prijave[i]);
				if (projekat != NULL)
					if (projekat->getPrezentovano() && projekat->getFinalizirano())
						for (size_t j = 0; j < _komisija.GetTrenutno(); j++)
							if (_komisija.GetElement2(i) == projekat->getKategorija() && j == indexKomisije)
							{
								FITKolekcija<ClanKomisije*, double> OcjenaKomisije;
								//if (projekat->getKategorija()==SQLChallenge || projekat->getKategorija() == CodingChallenge) Ocjena *= 10;

								//Larisa: Član komisije nije alociran prije dodavanja nego je samo proslijeđena adresa, 
								//a u destruktoru je urađena dealokacija. Treba uskladiti.
								ClanKomisije * p = new ClanKomisije(_komisija.GetElement1(j));
								OcjenaKomisije.AddElement(p, Ocjena);
								_ocjene.AddElement(_prijave[i], OcjenaKomisije);// _prijave[i] ili projekat ? Larisa: _prijave[i] jer se čuva pokazivač na osnovnu klasu
								return;
							}
			}
		}

		//throw NedozvoljenaOperacija("Neuspjesno ocjenjivanje projekta!", __LINE__);
	}
	//Funkcija koja na osnovu imena i prezimena clana komisije ispisuje podatke o njegovim ocjenama uključujući
	//prijave na koje se one odnose.
	void OcjeneClanaKomisije(const string ime, const  string prezime)
	{
		string imeClana = ime + " " + prezime;
		for (size_t i = 0; i < _ocjene.GetTrenutno(); i++)
			for (size_t j = 0; j < _ocjene.GetElement2(i).GetTrenutno(); j++)
				if (imeClana == _ocjene.GetElement2(i).GetElement1(j)->GetIme())
				{
					cout << _ocjene.GetElement1(i);
					_ocjene.GetElement1(i)->info();
					cout << "\nOcjena:" << _ocjene.GetElement2(i).GetElement2(j) << endl;
				}
	}


	//Funkcija koja na osnovu imena i prezimena ucesnika ispisuje podatke o njegovim prijavama na takmicenju, zajedno sa 
	//ocjenama, ukoliko su one definisane za prijavu/e datog ucenika.
	void OcjeneUcesnika(const string ime, const string prezime)
	{
		string imeUcesnika = ime + " " + prezime;
		for (size_t i = 0; i < _ocjene.GetTrenutno(); i++)
			for (size_t j = 0; j < _ocjene.GetElement1(i)->getBrojUcesnika(); j++)
				if (_ocjene.GetElement1(i)->GetUcesnik(j) == imeUcesnika)
				{
					_ocjene.GetElement1(i)->info();
					cout << "\nOcjena:" << _ocjene.GetElement2(i).GetElement2(j) << endl;
				}
	}

	//Funkciju za rangiranje kandidata na osnovu kategorije proslijedjene kao parametar.
	//Funkcija treba da na osnovu ocjena clanova komisije ispise rang liste timova zajedno sa prosjecnim ocjenama njihovih projekata (radova).
	//Dakle, konacna rang lista ukljucuje nazive timova (u zavisnosti od kategorije i naziv projekta), te prosjecnu ocjenu.
	void RangirajKandidate(const Kategorija &kategorija)
	{
		FITKolekcija<string, double> ProsjecneOcjeneProjekata;
		for (size_t i = 0; i < _ocjene.GetTrenutno(); i++)
			for (size_t j = 0; j < _ocjene.GetElement1(i)->getBrojUcesnika(); j++)
				if (_ocjene.GetElement1(j)->getKategorija() == kategorija)
				{
					/*cout << _ocjene.GetElement1(i)->GetUcesnik(j)<<endl;
					ostream_iterator<double, char> out(cout, "\n");
					sort(_ocjene.GetElement2(i).GetElement2(0), _ocjene.GetElement2(i).GetElement2(_ocjene.GetElement2(i).GetTrenutno()));
					copy(_ocjene.GetElement2(i).GetElement2(0), _ocjene.GetElement2(i).GetElement2(_ocjene.GetElement2(i).GetTrenutno()), out);*/
					double prosjecnaOcjena = 0;
					for (size_t y = 0; y < _ocjene.GetElement2(j).GetTrenutno(); y++)
						prosjecnaOcjena += _ocjene.GetElement2(i).GetElement2(j);
					prosjecnaOcjena /= (int)_ocjene.GetElement2(i).GetTrenutno();
					ProsjecneOcjeneProjekata.AddElement(_ocjene.GetElement1(i)->GetUcesnik(j), prosjecnaOcjena); //TODO: uraditi sa vector-om
				}
		/*ostream_iterator<double, char> out(cout, "\n");
		sort(ProsjecneOcjeneProjekata.GetElement2(0), ProsjecneOcjeneProjekata.GetElement2(ProsjecneOcjeneProjekata.GetTrenutno()));
		cout << ProsjecneOcjeneProjekata << endl;
		copy(ProsjecneOcjeneProjekata.GetElement2(0), ProsjecneOcjeneProjekata.GetElement2(ProsjecneOcjeneProjekata.GetTrenutno()), out); */
		cout << ProsjecneOcjeneProjekata;
	}



	//Funkciju Info koja ispisuje sve podatke o takmicenju: prijave i clanove komisije po kategorijama, projekte i 
	void Info() {
		cout << godina << endl;
		for (size_t i = 0; i < _prijave.size(); i++)
		{
			cout << *_prijave[i];
		}
		cout << _komisija;
		cout << _ocjene;
	}

	friend ostream & operator<<(ostream &out, const FITCodingChallenge &fitcc) {
		out << fitcc.godina << endl;
		/*ostream_iterator<Prijava *, char> Cout(cout, "\n");
		copy(_prijave.begin(), _prijave.end(), Cout);*/
		for (size_t i = 0; i < fitcc._prijave.size(); i++)
		{
			out << *fitcc._prijave[i];
		}
		cout << crt;
		out << fitcc._komisija << crt << fitcc._ocjene << endl;
		return out;
	}
};

int main()
{
	//Kreirati objekat klase FITCodingChallenge, te demonstrirati sve implementirane funkcionalnosti.
	try {
		Ucesnik u("ajdin", "ljubuncic", 1997, "ajdinlj@mail.com", "AJIN123", "FIT");
		Ucesnik u1(u);
		u1.info();
		Ucesnik u2("niko", "nikic", 1977, "niko@mail.com", "NIKIN123", "FIT");
		Prijava p("12.12.2016", GameChallenge);
		p.DodajUcesnika(u, true);
		p.DodajUcesnika(u2, false);
		p.PromijeniPredstavnika(u2);
		p.info();
		Prijava prijava1("21.05.2018", GameChallenge);
		PrijavaProjekta prijava2("20.04.2018", GameChallenge, "Nullpointers", "game-learning igra");
		prijava2.DodajUcesnika(u2, true);
		ClanKomisije clan("ajdin", "ljubuncic", 1997, "ajdinlj@mail.com", "AJIN123", "FIT", "asistent", "fit");
		cout << clan;
		FITCodingChallenge fitcc(2018, "01.01.2017");
		fitcc += prijava1;
		fitcc += prijava2;
		fitcc(clan, GameChallenge);
		fitcc.DefinisiPrijavu(0, true, true);
		fitcc.ispisPrijavaKategorijama();
		fitcc.OcjeneClanaKomisije("ajdin", "ljubuncic");
		fitcc.RangirajKandidate(GameChallenge);
		fitcc.OcjeneUcesnika("niko", "nikic");
		fitcc.OcjeniProjekte(1, 0, 10);
		cout << crt;
		fitcc.Info();
		cout << crt << fitcc;



	}
	catch (NedozvoljenaOperacija n) {
		n.Info();
	}


	//TODO: provjezbat polimorfizam i dodat klasu za vjezbu visestukog nasljeđivanja
	system("pause");

	return 0;
}

