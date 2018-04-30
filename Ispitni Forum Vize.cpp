#include<iostream>
#include<string>

#include<thread>
#include<mutex>
#include<regex>
using namespace std;



char *crt = "\n-----------------------------------------------\n";
mutex mojMutex;
enum Status { ZAHTJEV_ZAPRIMLJEN, ODUSTAO, ODBIJENA, IZDATA, ISTEKLA, PONISTENA };
enum Sortiranje { RASTUCI, OPADAJUCI };
enum Drzava { SAD, AUSTRALIJA, ENGLESKA };
char* eDrzava[3]{ "SAD", "AUSTRALIJA"," ENGLESKA" };

class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum(const Datum & d)  :_dan(new int(*d._dan)), _mjesec(new int(*d._mjesec)), _godina(new int(*d._godina)) { ; }
	Datum & operator=(const Datum & d)
	{
		if (this!=&d)
		{
			delete _dan;
			delete _mjesec;
			delete _godina;
			_dan = new int(*d._dan);
			_mjesec = new int(*d._mjesec);
			_godina = new int(*d._godina);
		}
		return *this;
	}
	friend bool operator<(const Datum & d1, const Datum & d2)
	{
		if (*d1._godina > *d2._godina)//ako je druga godina veca vrati false
			return false;
		else if (*d1._godina==*d2._godina)//ako je druga godina jednaka prvoj provjeri mjesec
		{
			if (*d1._mjesec>*d2._mjesec)//ako je drugi mjesec veci vrati false
				return false;
			else if (*d1._mjesec == *d1._mjesec)// ako je drugi mjesec jednak prvom provjeri dane
			{
				if (*d1._dan>=*d2._dan)//ako je drugi dan veci ili jednak prvom nece biti manji pa vrati false
					return false;
				return true;// dan je manji pa je i datum
			}
			return true;// mjesec je manji pa je i datum
		}
		return true;// godina je manja pa je i datum
	}
	Datum & operator++() { (*_dan)++; return *this; }
	friend bool operator==(const Datum & d1,const Datum & d2)   {
		if (*d1._godina==*d2._godina && *d1._mjesec == *d2._mjesec &&*d1._dan == *d2._dan)
		{
			return true;
		}
		return false;
	}
	friend ostream& operator<< (ostream &COUT, Datum &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
	int GetGodina() { return *_godina; }
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
		for (size_t i = 0; i < max; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	void Dodaj(const T1 & el1,const T2 & el2)
	{
		for (size_t i = 0; i < _trenutno; i++)
		{
			if (el1==*_elementi1[i] && el2==*_elementi2[i])
			{
				throw exception("Greska! Pokusavate dodati iste elemente!");
			}
		}
		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno++] = new T2(el2);
	}
	
	void Zamijeni(int prvi, int drugi)
	{
		T1 temp1;
		T2 temp2;
		temp1 = *_elementi1[prvi];
		temp2 = *_elementi2[prvi];
		*_elementi1[prvi] = *_elementi1[drugi];
		*_elementi2[prvi] = *_elementi2[drugi];
		*_elementi1[drugi] = temp1;
		*_elementi2[drugi] = temp2;
	}

	void Sortiraj(Sortiranje filter=RASTUCI)
	{
		if (filter==RASTUCI)
		{
			for (size_t i = 0; i < _trenutno-1; i++)
			{
				if (*_elementi1[i]>*_elementi1[i+1])
				{
					Zamijeni(i, i + 1);
					for (size_t j = i; j >0; j--)
					{
						if (*_elementi1[j]<*_elementi1[j-1])
						{
							Zamijeni(j, j - 1);
						}
					}
				}
			}
		}
		//438172965
		//987654321
		else if (filter = OPADAJUCI)
		{
			for (size_t i = 0; i < _trenutno-1; i++)
			{
				if (*_elementi1[i]<*_elementi1[i+1])
				{
					Zamijeni(i, i + 1);
					for (size_t j = i; j >0 ; j--)
					{
						if (*_elementi1[j]>*_elementi1[j-1])
						{
							Zamijeni(j, j - 1);
						}
					}
				}

			}
		}
	}
	T1 ** GetT1() { return _elementi1; }
	T2 ** GetT2() { return _elementi2; }
	int GetTrenutno() { return _trenutno; }

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
	
	~Viza() { delete[] _napomena; _napomena = nullptr; }
	void setStatusi(Status status, string razlog)
	{
		_statusi.Dodaj(status, razlog);
	}
	friend bool operator==(const Viza & v1, const Viza &v2) { return (v1._drzava == v2._drzava && v1._vazenjeOD == v2._vazenjeOD && v1._vazenjeDO == v2._vazenjeDO); }
	Datum GetVazenjeOD() const { return _vazenjeOD; }
	Datum GetVazenjeDO() const { return _vazenjeDO; }
	char * GetNapomena() { return _napomena; }
	Drzava & GetDrzava() const { return const_cast<Drzava &>(_drzava); }
	FITKolekcija<Status, string, 10> & GetStatuse() const { return const_cast<FITKolekcija<Status, string, 10> &>(_statusi); }
	virtual void Info() = 0 {
		mojMutex.lock();
		cout <<crt<< eDrzava[_drzava] << endl;
		cout << _vazenjeOD << " - " << _vazenjeDO << endl;
		if (_napomena!=NULL)
		{
		cout << _napomena;
		}
		cout << _statusi<<crt;
		mojMutex.unlock();
	}
	friend ostream& operator<< (ostream &cout, Viza &obj) {
		cout <<crt<< eDrzava[obj._drzava] << endl;
		cout << obj._vazenjeOD << " - " << obj._vazenjeDO << endl;
		if (obj._napomena != NULL)
			cout << obj._napomena;
		cout << obj._statusi<<crt;
		return cout;
	}
};

class PoslovnaViza :public Viza {
	string _kompanija; //naziv kompanije koja potvrđuje da posluje sa aplikantom
public:
	PoslovnaViza(Drzava drzava, Datum _OD, Datum _DO, string kompanija, char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_kompanija = kompanija;
	}
	PoslovnaViza(const PoslovnaViza &p) :Viza(p), _kompanija(p._kompanija) { ; }
	friend ostream & operator << (ostream & COUT, PoslovnaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._kompanija << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;

	}
	virtual void Info() { Viza::Info(); cout<<_kompanija<<endl; }
	//TODO: Uraditi ispis viza ostream
};
class TuristickaViza : public Viza {
	string _adresa; //adresa hotele ili drugog smještaja na kojoj će aplikant boraviti
public:
	TuristickaViza(Drzava drzava, Datum _OD, Datum _DO, string adresa, char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_adresa = adresa;
	}
	TuristickaViza(const TuristickaViza & t) :Viza(t), _adresa(t._adresa) { ; }
	friend ostream & operator << (ostream & COUT, TuristickaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._adresa << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;

	}
	virtual void Info() { Viza::Info(); cout << _adresa << endl; }
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
	~Aplikant() {
		delete[]_imePrezime; _imePrezime = nullptr;
		delete[]_brojPasosa; _brojPasosa = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			delete _vize[i]; _vize[i] = nullptr;
		}
	}
	friend ostream & operator << (ostream & COUT, Aplikant & obj) {
		COUT << obj._imePrezime << " " << *obj._datumRodjenja << " " << obj._brojPasosa << endl;
		COUT << "VIZE -> " << endl;
		for (size_t i = 0; i < obj._vize.size(); i++)
			COUT << *obj._vize[i] << endl;
		return COUT;
	}
	//VRAĆA PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODREĐENU DRŽAVU, A KOJE SU IMALE ODREĐENI STATUS
	vector<Viza *> GetVizeByDrzava(Drzava drzava, Status status)
	{
		vector<Viza *> trazeneVize;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (_vize[i]->GetDrzava()==drzava)
			{
				for (int j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++)
				{
					if (*_vize[i]->GetStatuse().GetT1()[j]==status)
					{
						trazeneVize.push_back(_vize[i]);
					}
				}
			}
		}
		return trazeneVize;
	}
	bool DodajVizu(const Viza* viza)
	{
		if (viza->GetVazenjeOD().GetGodina()-10<_datumRodjenja->GetGodina())
		{
			//throw exception("Greska! Aplikant nema starosnu dob za vizu u trazenom periodu!");
			return false;
		}
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if ( _vize[i]->GetDrzava()==viza->GetDrzava() && _vize[i]->GetVazenjeOD()<viza->GetVazenjeOD()	 //ako postoji viza da tu drzavu
														  && viza->GetVazenjeDO()<_vize[i]->GetVazenjeDO() ) // u tom periodu provjeri jel ona vazeca
			{
				if (*_vize[i]->GetStatuse().GetT1()[_vize[i]->GetStatuse().GetTrenutno()]==IZDATA)
				{
					//throw exception("Greska! Viza za tu drzavu vec postoji!");
					return false;
				}
			}
		}
		if (viza->GetDrzava()==SAD)
			for (size_t i = 0; i < _vize.size(); i++)
			{
				//if (_vize[i]->GetDrzava() == SAD)
					for (size_t j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++)
					{
						if (*_vize[i]->GetStatuse().GetT1()[i] == PONISTENA)
							//throw exception("Greska! Aplikant vec ima ponistenu vizu!");
							return false;
					}	
			}
		PoslovnaViza * poslovna = dynamic_cast<PoslovnaViza*>(const_cast<Viza *>(viza));
		if (poslovna!=NULL)
		{
			_vize.push_back(new PoslovnaViza(*poslovna));
			return true;
		}
		TuristickaViza * turisticka = dynamic_cast<TuristickaViza*>(const_cast<Viza *>(viza));
		if (turisticka!=NULL)
		{
			_vize.push_back(new TuristickaViza(*turisticka));
			return true;
		}
		return false;
	}
	//U ZASEBNOJ NITI URADITI PRINTANJE VIZE...KORISTITI METODU SLEEP_FOR(_vize[i])
	void PrintajVizu(const Viza * viza)
	{
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (*viza == *_vize[i])
			{
				this_thread::sleep_for(chrono::seconds(2));
				thread zasebnaNit(&Viza::Info, (_vize[i]));
				zasebnaNit.join();
			}
		}
	}

	bool DodajStatus(Viza * viza,Status statusVize, string opis="")
	{
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (*_vize[i]==*viza)
			{		
				if (statusVize==IZDATA && *_vize[i]->GetStatuse().GetT1()[_vize[i]->GetStatuse().GetTrenutno()-1]!= ZAHTJEV_ZAPRIMLJEN)
					return false;
				else if ((statusVize==PONISTENA || statusVize== ISTEKLA) && *_vize[i]->GetStatuse().GetT1()[_vize[i]->GetStatuse().GetTrenutno()-1] > IZDATA)
					return false;
				else if ((statusVize==ODBIJENA || statusVize==ODUSTAO) && *_vize[i]->GetStatuse().GetT1()[_vize[i]->GetStatuse().GetTrenutno()-1] !=ZAHTJEV_ZAPRIMLJEN )
					return false;
				_vize[i]->setStatusi(statusVize, opis);
				return true;
			}
		}
		return false;
	}
	////UNUTAR NAPOMENE MIJENJA SADRŽAJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
	void ZamijeniRijec(string trazeni, string novi)
	{
		for (size_t i = 0; i < _vize.size(); i++)
		{
			for (size_t j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++)
			{
				if (regex_search(*_vize[i]->GetStatuse().GetT2()[j],regex(trazeni)))
				{
					*_vize[i]->GetStatuse().GetT2()[j]=regex_replace(*_vize[i]->GetStatuse().GetT2()[j], regex(trazeni), novi);
				}
			}
		}
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
	//brojevi.Dodaj(v98, v196);
	//brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//NA KOJI NAČIN ĆE SE VRŠITI SORTIRANJE PRVENSTVENO ZAVISI OD VRIJEDNOSTI PARAMETRA, A VRŠI SE NA OSNOVU VRIJEDNOSTI TIPA T1
	brojevi.Sortiraj(RASTUCI);
	cout << brojevi << crt;
	brojevi.Sortiraj(OPADAJUCI);
	cout << brojevi << crt;

	try
	{
		//U SLUČAJU PREKORAČENJA DEFINISANE VELIČINE KOLEKCIJE, POTREBNO JE BACITI IZUZETAK
		brojevi.Dodaj(v13, v196);
	}
	catch (exception & err)
	{
		cout << "Error -> " << err.what() << endl;
	}

	Aplikant jasmin("Jasmin Azemovic", Datum(15, 5, 1979), "BS6985645");
	PoslovnaViza poslovna(SAD, dat15022017, dat18082017, "Microsoft");

	/*DodajVizu
	NOVA VIZA SE NE MOŽE DODATI UKOLIKO:
	-	JE APLIKANT MLAĐI OD 10 GODINA
	-	U TOM PERIODU (ODNOSI SE NA PERIOD VIZE OD - DO) NE POSTOJI VAŽEĆA (POJAM VAŽEĆA PODRAZUMIJEVA DA JE POSLJEDNJI STATUS VIZE BIO IZDATA TJ. DA VIZA NIJE PONIŠTENA ILI ISTEKLA) VIZA ZA TU DRŽAVU, BEZ OBZIRA NA NJENU VRSTU.
	-	JE NEKA OD RANIJIH VIZA PONIŠTENA (OVO SE ODNOSI SAMO NA VIZE ZA DRŽAVU: SAD)
	*/
	if (jasmin.DodajVizu(&poslovna))
		cout << "Podaci o vizi uspješno zaprimljeni" << endl;
	/*jasmin.DodajStatus(&poslovna, ZAHTJEV_ZAPRIMLJEN, "provjeriti navode sa predstavnicima kompanije Microsoft")
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
	if (jasmin.DodajVizu(&turisticka))
		cout << "Podaci o vizi uspješno zaprimljeni" << endl;
	if (jasmin.DodajStatus(&turisticka, ZAHTJEV_ZAPRIMLJEN))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	if (jasmin.DodajStatus(&turisticka, IZDATA)) {
		cout << "Status vize uspijesno promijenjen" << endl;
		//U ZASEBNOJ NITI URADITI PRINTANJE VIZE...KORISTITI METODU SLEEP_FOR
		jasmin.PrintajVizu(&turisticka);
		jasmin.PrintajVizu(&poslovna);

	}
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;
	//VRAĆA PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODREĐENU DRŽAVU, A KOJE SU IMALE ODREĐENI STATUS
	vector<Viza *> jasminVize = jasmin.GetVizeByDrzava(SAD, ZAHTJEV_ZAPRIMLJEN);
	for (size_t i = 0; i < jasminVize.size(); i++)
	{
		cout << *jasminVize[i];
	}
	////UNUTAR NAPOMENE MIJENJA SADRŽAJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
	jasmin.ZamijeniRijec("nemaju informaciju", "su upoznti sa");
	cout << jasmin;
	system("pause>FIT");
}