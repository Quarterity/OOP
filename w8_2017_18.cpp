#include<iostream>
using namespace std;
char * crt="\n===================================\n";
template<class T1, class T2>
class Kolekcija
{
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	Kolekcija()
	{
		_trenutno = 0;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}

	~Kolekcija()
	{
		delete[] _elementi1;
		_elementi1 = nullptr;

		delete[] _elementi2;
		_elementi2 = nullptr;
	}

	void DodajElemente(T1& el1, T2& el2)
	{
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];

		for (int i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;
		_trenutno++;
	}
	T1 & operator[](int index) { return _elementi1[index]; }
	T2 & getElement2(int index) { return _elementi2[index]; }
	int GetTrenutno() const { return _trenutno; }

	friend ostream& operator<<<>(ostream&, const Kolekcija&);
};
template<class T1, class T2>
ostream& operator<<<>(ostream& cout, const Kolekcija<T1, T2>& k)
{
	for (int i = 0; i < k._trenutno; i++)
	{
		cout << k._elementi1[i] << "\t"/* << k._elementi2[i]*/ << endl;
	}

	return cout;
}
//U programu implementirati nasljeđivanje između odgovarajućih klasa
//Klasu Osoba deklarisati kao apstraktnu.
class Osoba
{
	char* _ime;
	char* _prezime;
	char* _adresa;
	char _telefon[15];
public:
	char * getIme() { return _ime; }
	char * getPrezime() { return _prezime; }

	 Osoba(char* ime="---", char* prezime="---", char* adresa="---", char* telefon="---")
	{
		int vel = strlen(ime) + 1;
		_ime = new char[vel];
		strcpy_s(_ime, vel, ime);

		vel = strlen(prezime) + 1;
		_prezime = new char[vel];
		strcpy_s(_prezime, vel, prezime);

		vel = strlen(adresa) + 1;
		_adresa = new char[vel];
		strcpy_s(_adresa, vel, adresa);

		strcpy_s(_telefon, 15, telefon);
	}
	Osoba(Osoba &o)
	{
		int vel = strlen(o._ime) + 1;
		_ime = new char[vel];
		strcpy_s(_ime, vel, o._ime);

		vel = strlen(o._prezime) + 1;
		_prezime = new char[vel];
		strcpy_s(_prezime, vel, o._prezime);

		vel = strlen(o._adresa) + 1;
		_adresa = new char[vel];
		strcpy_s(_adresa, vel, o._adresa);

		strcpy_s(_telefon, 15, o._telefon);
	}
	virtual ~Osoba()
	{
		delete[] _ime;
		_ime = nullptr;
		delete[] _prezime;
		_prezime = nullptr;
		delete[] _adresa;
		_adresa = nullptr;
	}

	virtual void Info() = 0
	{
		cout << crt;
		cout << "Ime i prezime: " << _ime << " " << _prezime << endl;
		cout << "Adresa: " << _adresa << endl;
		cout << "Telefon: " << _telefon << endl;
	}
};

class Proizvod
{
	char* _naziv;
	char* _opis;
	double _cijena;
public:
	Proizvod(char* naziv, char* opis, double cijena)
	{
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);

		vel = strlen(opis) + 1;
		_opis = new char[vel];
		strcpy_s(_opis, vel, opis);

		_cijena = cijena;
	}
	Proizvod(const Proizvod &p )
	{
		int vel = strlen(p._naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, p._naziv);

		vel = strlen(p._opis) + 1;
		_opis = new char[vel];
		strcpy_s(_opis, vel, p._opis);

		_cijena = p._cijena;
	}
	char*  getNaziv() { return _naziv; }
	~Proizvod()
	{
		delete[] _naziv;
		_naziv = nullptr;

		delete[] _opis;
		_opis = nullptr;
	}
	void setPrice(int price) { _cijena = price; }
	double GetPrice() { return _cijena; }
	friend ostream& operator<<(ostream&, const Proizvod&);

};
ostream& operator<<(ostream& cout, const Proizvod& p)
{
	cout << "Naziv: " << p._naziv << endl;
	cout << "Opis: " << p._opis << endl;
	cout << "Cijena: " << p._cijena << endl;

	return cout;
}
class Narudzba {
	char _datum[12]; //Format: dd.MM.yyyy.
	Kolekcija<Proizvod*, int> _proizvodi; //Podaci o naručenim proizvodima i količinama
public:
	//Potrebne osnovne funkcije za rad sa klasom
	Narudzba(char* datum = "01.01.1111") {
		strncpy_s(_datum, strlen(datum) + 1, datum, _TRUNCATE);
		for (size_t i = 0; i < _proizvodi.GetTrenutno(); i++)
			_proizvodi[i] = nullptr;
	}
	Narudzba(Narudzba &n) {
		strncpy_s(_datum, strlen(n._datum) + 1, n._datum, _TRUNCATE);
		for (size_t i = 0; i < n._proizvodi.GetTrenutno(); i++)
		{
			//TODO: Proizvod * tmp = new Proizvod(*n._proizvodi[i]);
	
			_proizvodi.DodajElemente(n._proizvodi[i], n._proizvodi.getElement2(i));
		}
			
	}
	void operator=(Narudzba &n) {
		strncpy_s(_datum, strlen(n._datum) + 1, n._datum, _TRUNCATE);
		for (size_t i = 0; i < n._proizvodi.GetTrenutno(); i++)
		{
			Proizvod * tmp = new Proizvod(*n._proizvodi[i]);
			_proizvodi.DodajElemente(tmp, n._proizvodi.getElement2(i));
		}

	}
	~Narudzba() {
		for (size_t i = 0; i < _proizvodi.GetTrenutno(); i++)
		{
			delete _proizvodi[i];
		}
	}
	int getMjesec()
	{
		int mjesec = 0;
		char temp[2];
		temp[0] = _datum[3];
		temp[1] = _datum[4];
		mjesec = temp[1] - '0';
		mjesec += 10 * (temp[0] - '0');
		return mjesec;
	}
	void DodajProizvod( Proizvod *& p)
	{
		int a = 1;
		for (size_t i = 0; i < _proizvodi.GetTrenutno(); i++)
		{
			if (strcmp(p->getNaziv(),_proizvodi[i]->getNaziv())==0)
			{
				_proizvodi.getElement2(i)++;
				return;
			}
		}
		Proizvod * tmp = new Proizvod(*p);
		_proizvodi.DodajElemente(tmp, a);
	}
	//Funkciju GetIznosNarudzbe koja vraća ukupan iznos narudžbe na osnovu
	//podataka o cijenama proizvoda i naručenim količinama. Funkcija prima neobavezni parametar popust
	//koji inicijalno ima vrijednost 0, što znači da ne postoji popust na postojeće cijene proizvoda.
	double GetIznosNarudzbe(double popust = 0)
	{
		double iznos=0;
		for (size_t i = 0; i < _proizvodi.GetTrenutno(); i++)
		{
			iznos += _proizvodi.getElement2(i)*_proizvodi[i]->GetPrice();
			if (popust!=0)
				iznos -= (iznos * popust) / 100;
		}
		return iznos;
	}

	//Funkciju za ispis svih vrijednosti atributa klase.
	friend ostream & operator<<(ostream & cout, Narudzba & obj)
	{
		cout <<crt<< obj._datum << endl;
		for (size_t i = 0; i < obj._proizvodi.GetTrenutno(); i++)
		{
			cout << *obj._proizvodi[i];
			cout<< "Kolicina:" << obj._proizvodi.getElement2(i) << endl;
			/*for (size_t j = 0; j < obj._proizvodi.getElement2(); j++)
			{
				cout << obj._proizvodi[i] << ":" << obj._proizvodi[i]->GetPrice() << " KM" << endl;
			}*/
		}
		cout <<"Ukupan iznos narudzbe:"<< obj.GetIznosNarudzbe()<<" KM"<<endl;
		return cout;
	}
};





class Klijent: public Osoba
{
	int _id;
	char* _email;
	Kolekcija<Narudzba, double> _narudzbe; //Podaci o narudžbama sa ukupnim utrošenim iznosom u KM
public:
	//Potrebne konstruktor i destruktor funkcije.
	Klijent(char* ime = "---", char* prezime = "---", char* adresa = "---", char* telefon = "---", int id = 0, char* email = "---") : Osoba::Osoba( ime, prezime , adresa , telefon )
	{
		_id = id;
		_email = new char[strlen(email) + 1];
		strcpy_s(_email, strlen(email) + 1, email);
	}
	char* getMailFirme() {
		int velicinaNovog = 0;
		char* MailFirme=nullptr;
		for (size_t i = 0,y=0; i < strlen(_email); i++)
		{
			if (_email[i]=='@')
			{
				velicinaNovog += strlen(_email)+1;
				MailFirme = new char[velicinaNovog + 1];
				for (size_t j = i; j < strlen(_email); j++)
				{
					MailFirme[y++] = _email[j];
				}
				MailFirme[y] = '\0';
			}
			else
			velicinaNovog--;
		}
		return MailFirme;
	}
	Klijent(Klijent &k) : Osoba::Osoba(k)
	{
		_id = k._id;
		_email = new char[strlen(k._email) + 1];
		strcpy_s(_email, strlen(k._email) + 1, k._email);
		for (size_t i = 0; i < k._narudzbe.GetTrenutno(); i++)
		{
			EvidentirajNarudzbu(k._narudzbe[i]);
		}
	}
	virtual ~Klijent()
	{
		delete[] _email; _email = nullptr;
	}
	//Funkciju za evidenciju nove narudžbe.
	void EvidentirajNarudzbu(Narudzba & nova,double popust=0)
	{
		Narudzba *temp = new Narudzba(nova);
		double iznos = nova.GetIznosNarudzbe(popust);
		_narudzbe.DodajElemente(*temp,iznos);
	}
	Kolekcija<Narudzba, double> & getNarudzbe() { return _narudzbe; }
	//Funkciju koja vraća logičku vrijednost true ukoliko je klijent za odabrani mjesec u godini imao
	//potrošnju veću od zadate. Prototip funkcije: bool IsPotrosnjaVeca(int mjesec, double potrosnja);
	//Za pronalazak potrosnje po mjesecima koristiti podatak o datumu narudžbe. 
	bool IsPotrosnjaVeca(int mjesec, double potrosnja) {
		for (size_t i = 0; i < _narudzbe.GetTrenutno(); i++)
		{
			if (mjesec==_narudzbe[i].getMjesec())
			{
				return potrosnja < this->_narudzbe.getElement2(i);
			}
		}
		return false;
	}

	//Funkciju za izmjenu e-mail adrese.
	void izmjeniEmail(char* email)
	{
		_email = new char[strlen(email) + 1];
		strcpy_s(_email, strlen(email) + 1, email);
	}

	//Funkciju za ispis svih vrijednosti atributa klase.
	virtual void Info() {
		Osoba::Info();
		cout << _id << endl;
		cout << _email<<endl;
		for (size_t i = 0; i < _narudzbe.GetTrenutno(); i++)
		{
			cout << _narudzbe;
		}
	}
};

class VIPKlijent:public Klijent
{
	char* _parkingOznaka;
	double _popust;
public:
	//Potrebne konstruktor i destruktor funkcije
	VIPKlijent(Klijent & k, char* parkingOznaka = "---", double popust = 0) : Klijent::Klijent(k),_popust(popust)
	{
		_parkingOznaka = new char[strlen(parkingOznaka) + 1];
		strcpy_s(_parkingOznaka, strlen(parkingOznaka) + 1, parkingOznaka);
	}
	VIPKlijent(VIPKlijent &v ) : Klijent::Klijent(v), _popust(v._popust)
	{
		_parkingOznaka = new char[strlen(v._parkingOznaka) + 1];
		strcpy_s(_parkingOznaka, strlen(v._parkingOznaka) + 1, v._parkingOznaka);
	}
	virtual ~VIPKlijent() { delete[] _parkingOznaka; }
	//Funkciju za evidenciju nove narudžbe sa zaračunatim popustom na sve proizvode.
	void EvidentirajNarudzbu(Narudzba & nova)
	{
		Klijent::EvidentirajNarudzbu(nova,_popust);
	}

	//Funkciju koja posljednjoj narudžbi klijenta dodaje gratis proizvod proslijeđen kao parametar.

	void AddGratis(Proizvod *& gratis)
	{
		double a = 1;
		gratis->setPrice(0);
		if (Klijent::getNarudzbe().GetTrenutno()==0)
		{
			Narudzba nova;
			nova.DodajProizvod(gratis);
			double iznos=nova.GetIznosNarudzbe();
			Klijent::getNarudzbe().DodajElemente(nova, iznos);
		}
		Klijent::getNarudzbe()[Klijent::getNarudzbe().GetTrenutno()-1].DodajProizvod(gratis);
	}

	//Funkciju za ispis svih vrijednosti atributa klase
	virtual void Info() {
		Klijent::Info();
		cout <<"parking:"<< _parkingOznaka<<endl;
		cout << "popust:" << _popust;
	}

};

//Funkcija treba da pronađe sve one klijente koji su za odabrani mjesec ostvarili potrošnju veću
//od zadate. Ispisati njihove podatke i vratiti ukupan broj pronađenih klijenata.
int PronadjiKlijentePoPotrosnji(Osoba* niz[], int max)
{
	int brKlijenata = 0;
	for (size_t i = 0; i < max; i++)
	{
		Klijent * klijent = dynamic_cast<Klijent*>(niz[i]);
		if (klijent!=NULL)
		{
			for (size_t j = 1; j < 12; j++)
			{
				if (klijent->IsPotrosnjaVeca(j,3))
				{
					brKlijenata++;
					break;
				}
			}

		}
	}
	return brKlijenata;

}

//Pronaći sve VIP klijente, te im dodati gratis proizvod proslijeđen kao parametar.
void UkljuciGratisProizvodVIPKlijentima(Osoba* niz[], int max, const Proizvod& p)
{
	for (size_t i = 0; i < max; i++)
	{
		VIPKlijent * klijent = dynamic_cast<VIPKlijent*>(niz[i]);
		if (klijent != NULL)
		{
			Proizvod * temp = new Proizvod(p);
			klijent->AddGratis(temp);

		}
	}
}
char* formirajImeFirme(char* mail)
{
	char* firma = nullptr;
	firma = new char[strlen(mail) + 1];
	strcpy_s(firma, strlen(mail) + 1, mail);
	for (size_t i = strlen(firma); i >0 ; i--)
	{
		if (firma[i]=='.')
		{
			strcat_s(firma, strlen(firma) + 3, "ba");
			break;
		}
		firma[i] = '\0';
	}
	return firma;
}
//Svim klijentima postaviti mail adresu u formatu: ime.prezime@firma.ba.
void GenerisiMailAdreseKlijentima(Osoba* niz[], int max)
{
	for (size_t i = 0; i < max; i++)
	{
		Klijent * klijent = dynamic_cast<Klijent*>(niz[i]);
		if (klijent != NULL)
		{
			char* noviMail=nullptr;
			char*ime = new char[strlen(niz[i]->getIme()) + 1];
			strcpy_s(ime, strlen(niz[i]->getIme()) + 1, niz[i]->getIme());
			char* prezime = new char[strlen(niz[i]->getPrezime()) + 1];
			strcpy_s(prezime, strlen(niz[i]->getPrezime()) + 1, niz[i]->getPrezime());
			char* firma (formirajImeFirme(klijent->getMailFirme()));

			noviMail = new char[strlen(ime) + strlen(prezime) + strlen(firma) + 2];
			strcpy_s(noviMail, strlen(ime) + strlen(prezime) + strlen(firma) + 3, (niz[i]->getIme()));
			strcat_s(noviMail, strlen(ime) + strlen(prezime) + strlen(firma) + 3, ".\0");
			strcat_s(noviMail, strlen(ime) + strlen(prezime) + strlen(firma) + 3, (niz[i]->getPrezime()));
			strcat_s(noviMail, strlen(ime) + strlen(prezime) + strlen(firma) + 3, firma);
			
			klijent->izmjeniEmail(noviMail);
		}
	}
}

//Omogućiti pretragu klijenata po imenu ili prezimenu (ispisati informacije
//o svim klijentima koji sadrže uneseni tekst u dijelu imena ili prezimena.
void PronadjiKlijentePoImenuPrezimenu(Osoba* niz[], int max)
{
	char* trazeniText = "ali";
	for (size_t i = 0; i < max; i++)
	{
		Klijent * klijent = dynamic_cast<Klijent*>(niz[i]);
		if (klijent != NULL)
		{
			if (strstr((klijent->getIme()),trazeniText) || strstr(klijent->getPrezime(), trazeniText))
			{
				//TODO: ili ispisati niz nakon for-a.
				klijent->Info();
			}

		}
	}

}

int main()
{
	//Testirati sve dostupne funkcionalnosti uz obaveznu provjeru globalnih funkcija.
	//VIP klijentima sve narudžbe kreirati uz zaračunati popust pozivom odgovarajuće funkcije.

	//Proizvod* p = new Proizvod("Cokolada", "Super slatka sa ljesnjakom", 2.3);
	//Proizvod* p1 = new Proizvod("Sokic", "Prirodni sok od narandze", 10);

	//Narudzba* n = new Narudzba("14.01.2017.");
	//n->DodajProizvod(*&p);
	//n->DodajProizvod(*&p);
	////Osoba * op=new Osoba("Niko", "Nikic", "Adresa b.b.", "066555777");

	//Osoba* k = new Klijent("Niko", "Nikic", "Adresa b.b.", "066555777", 1, "niko.nikic@edu.fit.ba");

	//k->Info();

	//Narudzba * nrd = new Narudzba(*n);
	//cout << *nrd;
	//cout << "\n===================================\n";
	//cout << "Klijent obicni" << endl;
	//cout << "\n===================================\n";

	//Klijent* klijent = dynamic_cast<Klijent*>(k);

	//if (klijent != nullptr)
	//{
	//	klijent->EvidentirajNarudzbu(*n, 0);
	//	klijent->Info();

	//	if (klijent->IsPotrosnjaVeca(1, 30))
	//		cout << "Potrosnja jeste veca od 30 za mjesec januar!" << endl;
	//}

	//cout << "\n===================================\n";


	//Osoba * o = new VIPKlijent(*klijent, "A12C", 20);
	//VIPKlijent* vipKlijent = dynamic_cast<VIPKlijent*>(o);

	//cout << "\n===================================\n";
	//cout << "Klijent VIP" << endl;
	//cout << "\n===================================\n";
	//vipKlijent->Info();
	//if (vipKlijent != nullptr) {
	//	vipKlijent->EvidentirajNarudzbu(*n);
	//	vipKlijent->AddGratis(*&p1);
	//	vipKlijent->Info();
	//}
	//cout << "\n===================================\n";
	//o->Info();
	//const int max = 3;
	//Osoba *niz[max];
	//niz[0]=klijent;
	//niz[1] =vipKlijent;
	//niz[2] = new Klijent("Denis", "Music", "Mostarska b.b.", "065777666");
	//cout<<endl<<"Broj klijenata koji su potrosili vise od 3 KM je:"<<PronadjiKlijentePoPotrosnji(niz, 3);
	//system("pause>fit");
	//UkljuciGratisProizvodVIPKlijentima(niz, max, *p);
	//for (size_t i = 0; i < max; i++)
	//{
	//	niz[i]->Info();
	//}
	//return 0;

	 Proizvod* p = new Proizvod("Cokolada", "Super slatka sa ljesnjakom", 2.3);
	Proizvod* p1 = new Proizvod("Sokic", "Prirodni sok od narandze", 10);

	Narudzba* n = new Narudzba("14.01.2017.");
	n->DodajProizvod(*&p);
	n->DodajProizvod(*&p);


	Osoba* k = new Klijent("Salih", "Agic", "Wien", "00434439732958", 1, "student.agic@edu.fit.ba");

	k->Info();

	cout << "\n===================================\n";
	cout << "Klijent obicni" << endl;
	cout << "\n===================================\n";

	Klijent* klijent = dynamic_cast<Klijent*>(k);


	if (klijent != nullptr)
	{
		klijent->EvidentirajNarudzbu(*n, 0);
		klijent->EvidentirajNarudzbu(*n, 0);
		klijent->Info();

		if (klijent->IsPotrosnjaVeca(1, 30))
			cout << "Potrosnja jeste veca od 30 za mjesec januar!" << endl;
	}

	cout << "\n===================================\n";


	Osoba* o = new VIPKlijent(*klijent, "A12C", 20);

	VIPKlijent* vipKlijent = dynamic_cast<VIPKlijent*>(o);

	cout << "\n===================================\n";
	cout << "Klijent VIP" << endl;
	cout << "\n===================================\n";
	if (vipKlijent != nullptr) {
		vipKlijent->EvidentirajNarudzbu(*n);
		vipKlijent->AddGratis(*&p1);

		vipKlijent->Info();
	}
	cout << "\n===================================\n";

	o->Info();


	cout << "\n==================================================================\n";
	cout << "Testiranje polimorfizma u globalnim funkcijama" << endl;
	cout << "\n==================================================================\n";

	Klijent* tmp_kl = nullptr;
	VIPKlijent* tmp_vipkl = nullptr;
	Proizvod* pro = new Proizvod("Cokolada", "Super slatka sa ljesnjakom", 2.5);
	Proizvod* gratispro = new Proizvod("Sok", "Super sok", 1);
	Narudzba* nar = new Narudzba("06.12.2017.");
	Narudzba* gratisnar = new Narudzba("06.12.2017.");

	nar->DodajProizvod(*&pro);
	gratisnar->DodajProizvod(*&gratispro);

	Osoba** niz = new Osoba*[10];

	for (size_t i = 0; i < 10; i++)
	{
		if (i < 5)
		{
			niz[i] = new Klijent("Salih", "Agic", "Wien", "00434439732958", 1, "student.agic@hotmail.com");
			tmp_kl = dynamic_cast<Klijent*>(niz[i]);
			if (tmp_kl != nullptr) {
				tmp_kl->EvidentirajNarudzbu(*gratisnar);
			}
		}
		else
		{
			niz[i] = new VIPKlijent(*klijent, "A12C", 20);
			tmp_vipkl = dynamic_cast<VIPKlijent*>(niz[i]);
			if (tmp_vipkl != nullptr) {
				tmp_vipkl->EvidentirajNarudzbu(*nar);
				tmp_vipkl->AddGratis(*&gratispro);
			}
		}
	}

	cout << "Broj klijenata sa potrosnjom za mjesec januar vecom od 1.5KM : " << PronadjiKlijentePoPotrosnji(niz, 10);

	cout << "\n==================================================================\n";

	GenerisiMailAdreseKlijentima(niz, 10);

	for (size_t i = 0; i < 10; i++)
	{
		niz[i]->Info();
	}


	cout << "\n==================================================================\n";

	PronadjiKlijentePoImenuPrezimenu(niz, 10);

	cout << "\n==================================================================\n";

	//dealociraj
	delete p;
	delete p1;
	delete n;
	delete k;
	delete o;
	delete pro;
	delete gratispro;
	delete nar;
	delete gratisnar;
	for (size_t i = 0; i < 10; i++)
		delete niz[i];
	delete[] niz;

	cin.get();


}


