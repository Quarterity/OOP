#include <iostream>
#include <string>
#include <iomanip>
#include <thread>
#include <algorithm>
#include <regex>
#include <functional>
#include <vector>

using namespace std;



char *crt = "\n-----------------------------------------------\n";

enum Status { ZAHTJEV_ZAPRIMLJEN, ODUSTAO, ODBIJENA, IZDATA, ISTEKLA, PONISTENA };
enum Sortiranje { RASTUCI, OPADAJUCI };
enum Drzava { SAD, AUSTRALIJA, ENGLESKA };

class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum & dat)
	{
		_dan = new int(*dat._dan);
		_mjesec = new int(*dat._mjesec);
		_godina = new int(*dat._godina);

	}
	Datum &operator=(const Datum & dat)
	{
		this->~Datum();
		_dan = new int(*dat._dan);
		_mjesec = new int(*dat._mjesec);
		_godina = new int(*dat._godina);
		return *this;
	}
	bool operator <(Datum & d) { 
		return _godina < d._godina ? true : (_mjesec < d._mjesec ? true : _dan < d._dan );
	}
	bool operator >(Datum & d) {
		return _godina > d._godina ? true : (_mjesec > d._mjesec ? true : _dan > d._dan);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum & operator++() { *_dan+=1; return *this; }
	int GetGodina() { return *_godina; }
	bool operator==(const Datum &d)const { return *_dan==*d._dan && *_mjesec==*d._mjesec && *d._godina==*_godina; }
	friend ostream& operator<< (ostream &COUT, Datum &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
};

template<class T1, class T2, int max>
class FITKolekcija {
	T1 * _elementi1[max];
	T2 * _elementi2[max];
	int _trenutno;
public:
	FITKolekcija() {
		for (size_t i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}
	~FITKolekcija() {
		for (size_t i = 0; i < _trenutno; i++) {
			if (_elementi1[i] != NULL)
			{
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
			}
			
		}
	}
	void RemoveAll() {
		for (size_t i = 0; i < _trenutno; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}
	bool operator==(const FITKolekcija & f)const
	{
		if (_trenutno==f._trenutno)
		{
			for (size_t i = 0; i < f._trenutno; i++)
			{
				if (_elementi1[i]!=_elementi1[i])
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}
	FITKolekcija(const FITKolekcija & fit)
	{
		_trenutno = fit._trenutno;
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = new T1(*fit._elementi1[i]);
			_elementi2[i] = new T2(*fit._elementi2[i]);
		}
	}
	FITKolekcija& operator=(const FITKolekcija & fit)
	{
		if (!(fit == *this))
		{
			this->RemoveAll();
			_trenutno = fit._trenutno;
			for (size_t i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = new T1(*fit._elementi1[i]);
				_elementi2[i] = new T2(*fit._elementi2[i]);
			}
		}
		return *this;
	}
	int Dodaj(const T1 & el1, const T2 &el2)
	{
		
		for (size_t i = 0; i < _trenutno; i++)
		{
			if (el1==*_elementi1[i] || el2==*_elementi2[i])
				return i;
		}
		if (_trenutno < max)
		{
		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno++] = new T2(el2);
		}
		else
		{
			throw exception("Prekoracen max!");
		}
		return -1;
	}
	void zamijeni(int i, int j)
	{
		T1 temp1=*_elementi1[i];
		T2 temp2= *_elementi2[i];
		*_elementi1[i] = *_elementi1[j];
		*_elementi2[i] = *_elementi2[j];
		*_elementi1[j] = temp1;
		*_elementi2[j] = temp2;

	}
	void Sortiraj(Sortiranje filter) {
		if (filter==OPADAJUCI)
		{
			for (size_t i = 0; i < _trenutno-1; i++)
			{
				if (*_elementi1[i] < *_elementi1[i + 1])
				{
					zamijeni(i, i + 1);
					for (size_t j = i; j > 0; j--)
					{
						if (*_elementi1[j] > *_elementi1[j-1])
						{
							zamijeni(j - 1, j);
						}
						else break;
					}
				}
			}
		}
		else 
			for (size_t i = 0; i < _trenutno-1; i++)
			{
				if (*_elementi1[i] > *_elementi1[i + 1])
				{
					zamijeni(i, (i + 1));
					for (size_t j = i; j >0; j--)
					{
						if (*_elementi1[j]<*_elementi1[j-1])
						{
						zamijeni(j - 1, j);
						}
					}
				}
			}
	}
	T1 ** GetT1() { return _elementi1; }
	T2 ** GetT2() { return _elementi2; }
	int GetTrenutno()const { return _trenutno; }
	T1 & operator[](int i) { return *_elementi1[i]; }
	friend ostream& operator<< (ostream &COUT, FITKolekcija &obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << *obj._elementi1[i] << " " << *obj._elementi2[i] << endl;
		return COUT;
	}
};


class Viza {
protected:
	Drzava _drzava;
	Datum _vazenjeOD;
	Datum _vazenjeDO;
	char * _napomena;
	// string se odnosi na razlog promjene statusa, a moguće je samo 10 izmjena statusa
	FITKolekcija<Status, string, 10> _statusi;
public:
	Viza(Drzava drzava, Datum _OD, Datum _DO, char * napomena = nullptr) :_vazenjeDO(_DO), _vazenjeOD(_OD)
	{
		if (napomena != nullptr) {
			_napomena = new char[strlen(napomena) + 1];
			strcpy_s(_napomena, strlen(napomena) + 1, napomena);
		}
		else
			_napomena = nullptr;
		_drzava = drzava;
	}
	Viza(const Viza & v) :_vazenjeDO(v._vazenjeDO), _vazenjeOD(v._vazenjeOD)
	{
		if (v._napomena != nullptr) {
			_napomena = new char[strlen(v._napomena) + 1];
			strcpy_s(_napomena, strlen(v._napomena) + 1, v._napomena);
		}
		else
			_napomena = nullptr;
		_drzava = v._drzava;
		if (v._statusi.GetTrenutno() > 0)
		{
			_statusi = v._statusi;
		}

	}
	Viza & operator=(const Viza & v)
	{
		delete[] this->_napomena;
		this->_statusi.RemoveAll();
		_vazenjeDO = (v._vazenjeDO);
		_vazenjeOD = (v._vazenjeOD);
		if (v._napomena != nullptr) {
			_napomena = new char[strlen(v._napomena) + 1];
			strcpy_s(_napomena, strlen(v._napomena) + 1, v._napomena);
		}
		else
			_napomena = nullptr;
		_drzava = v._drzava;
		_statusi = v._statusi;
	}
	int getTrenutnoStatusa() { return _statusi.GetTrenutno(); }
	void SetStatus(Status &s, string a) {
		_statusi.Dodaj(s, a);
		if (_napomena != nullptr)
		{
			delete[] _napomena; _napomena = nullptr;
		}
		_napomena = new char[a.size() + 1];
		strcpy_s(_napomena, a.size() + 1, a.c_str());
		
	}
	Drzava & GetDrzava() { return _drzava; }
	Status & getZadnjiStatus() { 
		return _statusi[_statusi.GetTrenutno()-1]; 
	}
	~Viza() { delete[] _napomena; _napomena = nullptr; }
	bool PostojiVazecaZaPeriod(Viza & v)
	{
		if (v._vazenjeOD>_vazenjeOD && v._vazenjeDO<_vazenjeDO)
		{
			if (_statusi[_statusi.GetTrenutno()]==IZDATA)
			{
			return true;
			}
		}
		return false;
	}
	void SetNapomena(string &a)
	{
		if (_napomena!=nullptr)
		{
			delete[] _napomena; _napomena = nullptr;
		}
		_napomena = new char[a.size() + 1];
		strcpy_s(_napomena, a.size() + 1, a.c_str());

	}
	bool operator ==(const Viza &v)
	{
		return v._vazenjeOD == _vazenjeOD && v._vazenjeDO == _vazenjeDO;
	}
	bool PostojiPonistena() {
		if (_drzava==SAD)
		{
			for (size_t i = 0; i < _statusi.GetTrenutno(); i++)
			{
				if (_statusi[i]==PONISTENA)
				{
					return true;
				}
			}
		}
		return false;
	}
	
	Datum GetVazenjeOD() { return _vazenjeOD; }
	Datum GetVazenjeDO() { return _vazenjeDO; }
	char * GetNapomena() { return _napomena; }
	FITKolekcija<Status, string, 10> & GetStatuse() { return _statusi; }
	virtual void Info() = 0
	{
		cout << *this;
	}
	friend ostream & operator<<(ostream &out, Viza & t)
	{
		out << t._drzava << endl << t._napomena << endl << t._statusi << endl << t._vazenjeOD << endl << t._vazenjeDO << crt;
		return out;
	}
};

class PoslovnaViza :public Viza {
	string _kompanija; //naziv kompanije koja potvrđuje da posluje sa aplikantom
public:
	PoslovnaViza(Drzava drzava, Datum _OD, Datum _DO, string kompanija, char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_kompanija = kompanija;
	}
	PoslovnaViza(PoslovnaViza &p) : Viza(p) {
		_kompanija = p._kompanija;
	}
	PoslovnaViza & operator =(const PoslovnaViza &p) {
		Viza* pa = (Viza*)this;
		*pa=p;
		//Viza::operator=(p);
		_kompanija = p._kompanija;
	}
	friend ostream & operator << (ostream & COUT, PoslovnaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._kompanija << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;

	}
	virtual void Info() {
		cout << _drzava << " " << _vazenjeOD << " " << _vazenjeDO << " " << _kompanija << endl;
		cout << ((_napomena != nullptr) ?_napomena : "") << endl;
		cout << "STATUSI -> " <<_statusi << endl;
	}
};
class TuristickaViza : public Viza {
	string _adresa; //adresa hotele ili drugog smještaja na kojoj će aplikant boraviti
public:
	TuristickaViza(Drzava drzava, Datum _OD, Datum _DO, string adresa, char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_adresa = adresa;
	}
	friend ostream & operator << (ostream & COUT, TuristickaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._adresa << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;

	}
	virtual void Info()
	{ 
		Viza::Info();
		cout << _adresa;
	}
};

class Aplikant {
	char * _imePrezime;
	Datum * _datumRodjenja;
	char * _brojPasosa;
	vector<Viza *> _vize;
public:
	Aplikant(char * imePrezime, Datum datumRodjenja, char * brPasosa) {
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		vel = strlen(brPasosa) + 1;
		_brojPasosa = new char[vel];
		strcpy_s(_brojPasosa, vel, brPasosa);
		_datumRodjenja = new Datum(datumRodjenja);
	}
	Aplikant(const Aplikant &a) {
		int vel = strlen(a._imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, a._imePrezime);
		vel = strlen(a._brojPasosa) + 1;
		_brojPasosa = new char[vel];
		strcpy_s(_brojPasosa, vel, a._brojPasosa);
		_datumRodjenja = new Datum(*a._datumRodjenja);
		if (a._vize.size()>0)
		{
			_vize = a._vize;
		}
	}
	Aplikant& operator=(const Aplikant &a) {
		a.~Aplikant();
		int vel = strlen(a._imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, a._imePrezime);
		vel = strlen(a._brojPasosa) + 1;
		_brojPasosa = new char[vel];
		strcpy_s(_brojPasosa, vel, a._brojPasosa);
		_datumRodjenja = new Datum(*a._datumRodjenja);
		if (a._vize.size()>0)
		{
			_vize = a._vize;
		}
	}
	~Aplikant() {
		delete[]_imePrezime; _imePrezime = nullptr;
		delete[]_brojPasosa; _brojPasosa = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			delete _vize[i]; _vize[i] = nullptr;
		}
	}
	bool DodajStatus(Viza *v, Status s, string napomena="")
	{
		bool dodana = false;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (*_vize[i] == *v)
			{
				if (_vize[i]->getTrenutnoStatusa() > 0)
				{
					if ((int)_vize[i]->getZadnjiStatus() > (int)s )//TODO: ako se status dodaje samo na kraj kolekcije statusi.
					{
						return false;
					}
				}
				_vize[i]->SetStatus(s, napomena);
				
				dodana = true;
			}
		}
		if (dodana)
		{
			return true;
		}
		return false;
	}
	void ZamijeniRijec(string trazeniSadrzaj, string noviSadrzaj)
	{
		string pravilo = "\\b" + trazeniSadrzaj + "\\b";
		for (size_t i = 0; i < _vize.size(); i++)
		{
				string att=regex_replace(_vize[i]->GetNapomena(), regex(pravilo), noviSadrzaj);
				cout << att;
				_vize[i]->SetNapomena(att);
				
		}
	}
	bool DodajVizu(Viza *v)
	{
		if (_datumRodjenja->GetGodina()<10)
		{
			return false;
		}
		if (v->GetDrzava() == SAD)
		{
			for (size_t i = 0; i < _vize.size(); i++)
			{
				if (_vize[i]->PostojiPonistena())
				{
					return false;
				}

			}
		}
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (_vize[i]->PostojiVazecaZaPeriod(*v))
			{
				return false;
			}
		}
		PoslovnaViza* p=dynamic_cast<PoslovnaViza*>(v);
		if (p!=nullptr)
		{
			PoslovnaViza *nova = new PoslovnaViza(*p);
			_vize.push_back(nova);
		}
		return true;
	}
	vector<Viza*> GetVizeByDrzava(Drzava d, Status s)
	{
		vector<Viza*> noviVector;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (_vize[i]->GetDrzava()==d )
			{
				FITKolekcija<Status,string,10> novi = _vize[i]->GetStatuse();
				for (size_t j = 0; j < novi.GetTrenutno(); j++)
				{
					if (novi[j]==s)
					{
						PoslovnaViza* p = dynamic_cast<PoslovnaViza*>(_vize[i]);
						if (p != nullptr)
						{
							PoslovnaViza *nova = new PoslovnaViza(*p);
							noviVector.push_back(nova);
						}
					}
				}
			}
		}
		return noviVector;
	}
	void PrintajVizu(Viza * viza)
	{
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (*viza==*_vize[i])
			{
				_vize[i]->Info();
			}
		}
		this_thread::sleep_for(chrono::seconds(2));
	}
	friend ostream & operator << (ostream & COUT, Aplikant & obj) {
		COUT << obj._imePrezime << " " << *obj._datumRodjenja << " " << obj._brojPasosa << endl;
		COUT << "VIZE -> " << endl;
		for (size_t i = 0; i < obj._vize.size(); i++)
			COUT << *obj._vize[i] << endl;
		return COUT;
	}

	vector<Viza * > GetVize() { return _vize; }
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



	Datum danas(15, 2, 2017);
	Datum sutra(++danas); // inkrementiranjem se uvecava dan za 1
	Datum prekosutra;
	Datum dat15022017(15, 2, 2017), dat18082017(18, 8, 2017), dat11052018(11, 5, 2018), dat11122017(11, 12, 2017);


	prekosutra = danas;
	if (prekosutra == danas)
		cout << "Datumi su jednaki" << endl;
	cout << danas << endl << sutra << endl << prekosutra << crt;

	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int, 7> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//NA KOJI NAČIN ĆE SE VRŠITI SORTIRANJE PRVENSTVENO ZAVISI OD VRIJEDNOSTI PARAMETRA, A VRŠI SE NA OSNOVU VRIJEDNOSTI TIPA T1
	brojevi.Sortiraj(RASTUCI);
	cout << brojevi << crt;
	brojevi.Sortiraj(OPADAJUCI);
	cout << brojevi << crt;

	try
	{
		//U SLUČAJU PREKORAČENJA DEFINISANE VELIČINE KOLEKCIJE, POTREBNO JE BACITI IZUZETAK
		brojevi.Dodaj(5, 5);
	}
	catch (exception & err)
	{
		cout << "Error -> " << err.what() << endl;
	}

	Aplikant jasmin("Jasmin Azemovic", Datum(15, 5, 1979), "BS6985645");
	PoslovnaViza poslovna(SAD, dat15022017, dat18082017, "Microsoft");

	/*
	NOVA VIZA SE NE MOŽE DODATI UKOLIKO:
	-	JE APLIKANT MLAĐI OD 10 GODINA
	-	U TOM PERIODU (ODNOSI SE NA PERIOD VIZE OD - DO) NE POSTOJI VAŽEĆA (POJAM VAŽEĆA PODRAZUMIJEVA DA JE POSLJEDNJI STATUS VIZE BIO IZDATA TJ. 
		DA VIZA NIJE PONIŠTENA ILI ISTEKLA) VIZA ZA TU DRŽAVU, BEZ OBZIRA NA NJENU VRSTU.
	-	JE NEKA OD RANIJIH VIZA PONIŠTENA (OVO SE ODNOSI SAMO NA VIZE ZA DRŽAVU: SAD)
	*/
	if (jasmin.DodajVizu(&poslovna))
		cout << "Podaci o vizi uspješno zaprimljeni" << endl;
	/*
	STATUSI SE MOGU DODAVATI SAMO U REDOSLIJEDU U KOJEM SE NALAZE U ENUMERACIJI NPR.
	-	NIJE MOGUĆE ODBITI VIZU AKO PRETHODNO NIJE IMALA STATUS DA JE ZAHTJEV_ZAPRIMLJEN;
	-	NIJE MOGUĆE PONIŠTITI VIZU KOJA PRETHODNO NIJE IZDATA
	*/
	if (jasmin.DodajStatus(&poslovna, ZAHTJEV_ZAPRIMLJEN, "provjeriti navode sa predstavnicima kompanije Microsoft"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;


	if (jasmin.DodajStatus(&poslovna, PONISTENA, "predstavnicima kompanije Microsoft nemaju informaciju o posjeti navedenog aplikanta"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	TuristickaViza turisticka(ENGLESKA, dat18082017, dat11122017, "Whitehall Pl, London SW1A 2BD, UK");

	if (jasmin.DodajStatus(&turisticka, ZAHTJEV_ZAPRIMLJEN))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	if (jasmin.DodajStatus(&turisticka, IZDATA)) {
		cout << "Status vize uspijesno promijenjen" << endl;
	}
	else
	cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;
	
	//U ZASEBNOJ NITI URADITI PRINTANJE VIZE...KORISTITI METODU SLEEP_FOR
	thread noviT(&Aplikant::PrintajVizu, &jasmin, &(poslovna));
	noviT.join();
	
	//VRAĆA PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODREĐENU DRŽAVU, A KOJE SU IMALE ODREĐENI STATUS
	vector<Viza *> jasminVize = jasmin.GetVizeByDrzava(SAD, PONISTENA);

	//UNUTAR NAPOMENE MIJENJA SADRŽAJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
	jasmin.ZamijeniRijec("nemaju informaciju", "su upoznti sa");
	system("pause>fit");
}