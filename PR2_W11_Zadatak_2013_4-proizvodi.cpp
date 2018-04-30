#include<iostream>
#include<exception>
using namespace std;
char* crt = "\n----------------------------------------------------\n";

class NedozvoljenaOperacija : public exception {
	int linijaKoda;
public:
	//Konstruktor sa dva parametra (poruka i linija koda u kojoj je greška nastala)
	NedozvoljenaOperacija(char * poruka, int lin):exception(poruka)
	{
		linijaKoda = lin;
	}
	//Funkcija za ispis informacija o nastaloj grešci
	void ErrorInfo()
	{
		cout <<"Poruka:"<<exception::what()<<endl;
		cout <<"Linija:"<< linijaKoda<<endl;
	}
};

//KATALOŠKA PRODAJA
class Proizvod
{
	char _sifra[10];
	char* _naziv;
	double _cijena;
	double _popust;
	int _kolicina;//TODO:atribut kolicina??
public:
	double getCijena() { return _cijena - (_cijena*_popust / 100); }
	double getPopust() { return _popust; }
	void setKolicina() { _kolicina++; }
	char * getSifra() { return _sifra; }
	//Potrebne konstruktor i destruktor funkcije
	//U slučaju nedozvoljenih vrijednosti za cijenu i popust baciti izuzetak
	Proizvod(char* sifra="---", char* naziv="---", double cijena=0, double popust=0)
	{
		//Dopuniti dato rješenje
		if (strlen(sifra) > 10 ) throw NedozvoljenaOperacija("Sifra je prevelika", __LINE__);
		strncpy_s(_sifra,10, sifra,_TRUNCATE);
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		if (cijena < 0) throw NedozvoljenaOperacija("Cijena ne moze biti negativna", __LINE__);
		_cijena = cijena;
		if (popust < 0) throw NedozvoljenaOperacija("Popust ne moze biti negativan", __LINE__);
		_popust = popust;
	}
	Proizvod(const Proizvod &  p) 
	{
		//Dopuniti dato rješenje
		if (strlen(p._sifra) > 10) throw NedozvoljenaOperacija("Sifra je prevelika", __LINE__);
		strncpy_s(_sifra, 10, p._sifra, _TRUNCATE);
		int vel = strlen(p._naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, p._naziv);
		if (p._cijena < 0) throw NedozvoljenaOperacija("Cijena ne moze biti negativna", __LINE__);
		_cijena = p._cijena;
		if (p._popust < 0) throw NedozvoljenaOperacija("Popust ne moze biti negativan", __LINE__);
		_popust = p._popust;
	}
	void operator=(const Proizvod &p)
	{
		//Dopuniti dato rješenje
		if (strlen(p._sifra) > 10) throw NedozvoljenaOperacija("Sifra je prevelika", __LINE__);
		strncpy_s(_sifra, 10, p._sifra, _TRUNCATE);
		int vel = strlen(p._naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, p._naziv);
		if (p._cijena < 0) throw NedozvoljenaOperacija("Cijena ne moze biti negativna", __LINE__);
		_cijena = p._cijena;
		if (p._popust < 0) throw NedozvoljenaOperacija("Popust ne moze biti negativan", __LINE__);
		_popust = p._popust;
	}
	~Proizvod()
	{
		delete _naziv;
		_naziv = NULL;
	}
	//Funkciju Info
	virtual void Info() {
		cout << "Sifra:"<< _sifra << " Naziv:" << _naziv << " Cijena:" << _cijena << "KM Popust:" << _popust <<"%"<< endl;
	}
};

//Implementirati nasljeđivanje
class Odjeca:public Proizvod
{
	char* _velicine; //34, 36, 38 ili XS, S, M...
	char* _boje; //crna, bijela, siva 
public:
	//Potrebne konstruktor i destruktor funkcije	   
	Odjeca(Proizvod * proizvod = new Proizvod(), char* velicine="---", char* boje="---"):Proizvod::Proizvod(*proizvod)
	{
		if (strcmp(velicine, "---") == 0 || strlen(velicine) > 5) throw NedozvoljenaOperacija("Nepostojeca velicina", __LINE__);
		_velicine = new char[6];
		strncpy_s(_velicine,5,velicine, _TRUNCATE);
		if (strcmp(boje, "---") == 0 || strlen(boje) > 20) throw NedozvoljenaOperacija("Nepostojeca boja", __LINE__);
		_boje = new char[20];
		strncpy_s(_boje, 20, boje, _TRUNCATE);

	}
	Odjeca(Odjeca & o) :Proizvod(o)
	{
		if (strcmp(o._velicine, "---") == 0 || strlen(o._velicine) > 5) throw NedozvoljenaOperacija("Nepostojeca velicina", __LINE__);
		_velicine = new char[6];
		strncpy_s(_velicine, 5, o._velicine, _TRUNCATE);
		if (strcmp(o._boje, "---") == 0 || strlen(o._boje) > 20) throw NedozvoljenaOperacija("Nepostojeca boja", __LINE__);
		_boje = new char[20];
		strncpy_s(_boje, 20, o._boje, _TRUNCATE);
	}
	
	~Odjeca()
	{
		delete[] _velicine;
		_velicine = NULL;
		delete[] _boje;
		_boje = NULL;
	}

	//Funkciju Info
	void Info() {
		cout << crt << "\t\tOdjeca" << crt;
		Proizvod::Info();
		cout<<"Boja:"<<this->_boje<<" Velicina:"<<_velicine<<crt;
	}
};


class Katalog
{
	char* _naslov;
	//Datum
	char* _vrijediDo;
	Proizvod* _proizvodi[100];
	int _trenutnoProizvoda;
public:
	//Potrebne konstruktor i destruktor funkcije
	Katalog(char * naslov,char* vrijediDo) {
		_naslov = new char[strlen(naslov) + 1];
		strcpy_s(_naslov, strlen(naslov) + 1, naslov);
		_vrijediDo = new char[strlen(vrijediDo) + 1];
		strcpy_s(_vrijediDo, strlen(vrijediDo) + 1, vrijediDo);
		_trenutnoProizvoda = 0;
	}
	Katalog(Katalog &k) {
		_naslov = new char[strlen(k._naslov) + 1];
		strcpy_s(_naslov, strlen(k._naslov) + 1, k._naslov);
		_vrijediDo = new char[strlen(k._vrijediDo) + 1];
		strcpy_s(_vrijediDo, strlen(k._vrijediDo) + 1, k._vrijediDo);
		_trenutnoProizvoda = k._trenutnoProizvoda;
	}
	~Katalog()
	{
		delete[] _naslov;
		_naslov = NULL;

		delete _vrijediDo;
		_vrijediDo = NULL;

		for (int i = 0; i < _trenutnoProizvoda; i++)
		{
			delete _proizvodi[i];
			_proizvodi[i] = NULL;
		}
	}

	//Kreirati funkciju za dodavanje proizvoda u katalog i pri tome provjeriti da li se radi
	//o objektu izvedene ili osnovne klase (rezervisati memoriju za odgovarajući tip).
	//U slučaju prekoračenja baciti izuzetak
	//Onemogućiti dodavanje identičnih proizvoda u jedan katalog
	//Proizvode razlikovati po šifri

	void DodajProizvod(Proizvod &p)
	{
		if (_trenutnoProizvoda>100)//TODO: uhvatiti max broja proizvoda??
		{
			throw NedozvoljenaOperacija("Max broj proizvoda je prekoracen!", __LINE__);
		}
		for (size_t i = 0; i < _trenutnoProizvoda; i++)
		{
			if (strcmp(_proizvodi[i]->getSifra(),p.getSifra())==0)
			{
				throw NedozvoljenaOperacija("Greska! sifra proizvoda vec postoji!",__LINE__);
			}
		}
		Odjeca* odjeca = dynamic_cast<Odjeca*>(&p);
		if (odjeca != nullptr)
		{
			_proizvodi[_trenutnoProizvoda++] = new Odjeca(odjeca);
		}
		else {
			_proizvodi[_trenutnoProizvoda++] = new Proizvod(p);
		}
		
	}
	//Funkciju za uklanjanje proizvoda iz kataloga na osnovu sifre
	//Ukoliko se ne pronađe traženi proizvod baciti izuzetak
	void UkloniProizvod(char * sifra) {
		for (size_t i = 0; i < _trenutnoProizvoda; i++)
		{
			if (strcmp(_proizvodi[i]->getSifra(),sifra) == 0)
			{
				for (size_t j = i; j < _trenutnoProizvoda-1; j++)
				{
					delete _proizvodi[j];
					Odjeca* odjeca = dynamic_cast<Odjeca*>(_proizvodi[i]);
					if (odjeca != nullptr)
					{
						_proizvodi[_trenutnoProizvoda++] = new Odjeca(odjeca);
					}
					else
					_proizvodi[j]=new Proizvod(*_proizvodi[j + 1]);
				}
				_trenutnoProizvoda--;
				return;
			}	
		}
		throw NedozvoljenaOperacija("Greska! sifra proizvoda vec postoji!", __LINE__);
	}
	//Preklopiti operator za ispis kataloške ponude
	friend ostream & operator<<(ostream & cout, Katalog &k)
	{
		cout << k._naslov<<endl;
		for (size_t i = 0; i <k._trenutnoProizvoda; i++)
		{
			k._proizvodi[i]->Info();
		}
		cout << endl << k._vrijediDo << endl;
		return cout;
	}
};


//DODATNO
//Sve funkcije implementirati van klase
//Baciti izuzetak po potrebi
template<class T>
class Stack
{
	int _size;
	int _top;
	T* _elements;
public:
	//Konstruktor postavlja početne vrijednosti i alocira dinamički niz elemenata tipa T
	Stack(int);
	~Stack();
	int getTop() { return _top; }

	//Provjeravaju stanje na stack-u, prazan ili popunjen
	//Iskoristiti prilikom implementacije funkcija Push i Pop
	bool IsEmpty()const;
	bool IsFull()const;

	//Dodaje novi element na stack
	void Push(const T& element);

	//Uzima posljednji element sa stack-a
	T Pop();


	//Vraća broj elemenata na stack-u
	int GetSize()const;

	//Vraća element sa stack-a na poziciji indeks (pozicije počinju od 1)
	T& operator[](int indeks)const;

	//Operator za ispis elemenata na stack-u
	friend ostream& operator<<<>(ostream &cout, Stack &s);
};
template <class T>
ostream & operator<<<>(ostream & cout, Stack<T> &s)
{
	//TODO: petlja!
	cout << s._elements;
	return cout;
}
template<class T>
Stack<T>::Stack(int size = 5) : _top(0) {	_size = size;_elements = new T[_size];}
template<class T>
Stack<T>::~Stack() { delete[] _elements; }
template<class T>
bool Stack<T>::IsEmpty() const { return _top==0; }
template <class T>
bool Stack<T>::IsFull() const { return _top == _size; }
template <class T>
void Stack<T>::Push(const T& element) {
	if (IsFull())
	{
		throw NedozvoljenaOperacija("Stek je pun", __LINE__);
	}
	_elements[_top++]=(element);
}
template <class T>
T Stack<T>::Pop() {
	if (IsEmpty())
	{
		throw NedozvoljenaOperacija("Stek je prazan", __LINE__);
	}
	T temp = _elements[_top];
	//delete _elements[_top];
	_top--;
	return temp;
}
template<class T>
int Stack<T>::GetSize() const { return _size; }
template <class T>
T& Stack<T>::operator[](int index)const { return _elements[index]; }

class Uposlenik
{
	char* _imePrezime;
	char* _radnoMjesto;
public:
	char * getRadno() { return _radnoMjesto; }
	//Potrebne konstruktor i destruktor funkcije
	Uposlenik(char* imePrezime, char* radnoMjesto)
	{
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);

		vel = strlen(radnoMjesto) + 1;
		_radnoMjesto = new char[vel];
		strcpy_s(_radnoMjesto, vel, radnoMjesto);
	}
	Uposlenik(Uposlenik & u)
	{
		int vel = strlen(u._imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, u._imePrezime);

		vel = strlen(u._radnoMjesto) + 1;
		_radnoMjesto = new char[vel];
		strcpy_s(_radnoMjesto, vel, u._radnoMjesto);
	}

	~Uposlenik()
	{
		delete[] _imePrezime;
		_imePrezime = NULL;
		delete[] _radnoMjesto;
		_radnoMjesto = NULL;
	}
	void Info()
	{
		cout <<crt<< _imePrezime << endl << _radnoMjesto << endl;
	}
};

class Narudzba
{
	int _brojNarudzbe;
	char* _datumIsporuke;
	Uposlenik* _uposlenik;
	Stack<Proizvod>* _proizvodi;
	int _brojProizvoda;
public:
	//Potrebne konstruktor i destruktor funkcije
	//Narudžbu može izdati samo uposlenik sa radnim mjestom "Prodavač". U suprotnom baciti izuzetak.
	Narudzba(int brojNarudzbe, char * datumIsporuke, Uposlenik & uposlenik)
	{
		_brojNarudzbe = brojNarudzbe;
		_datumIsporuke = new char[strlen(datumIsporuke) + 1];
		strcpy_s(_datumIsporuke, strlen(datumIsporuke) + 1, datumIsporuke);
		if (strcmp(uposlenik.getRadno(), "Prodavac") == 0)
		{
			_uposlenik = new Uposlenik(uposlenik);
		}
		else throw NedozvoljenaOperacija("Uposlenik nije \"Prodavač\"! ", __LINE__);
		_proizvodi = new Stack<Proizvod>(10);
	}

	//Kreirati funkciju za dodavanje proizvoda narudžbe
	//TODO::Ukoliko proizvod više nije dostupan baciti izuzetak.
	//Dozvoliti dodavanje istog proizvoda više puta (po potrebi dopuniti atribute odgovarajuće klase)
	void DodajProizvodNarudzbi(Proizvod &p)
	{
		//TODO: provjera sifri!
		/*for (size_t i = 0; i < _brojProizvoda; i++)
		{
			for (size_t j = 0; j < _proizvodi[i].getTop(); j++)
			{
				if (strcmp(p.getSifra(), (_proizvodi[i][j].getSifra())) == 0)
				{
					_proizvodi[i][j].setKolicina();
					return;
				}
			}

		}*/
		try
		{
			_proizvodi->Push(p);
			_brojProizvoda++;
		}
		catch(NedozvoljenaOperacija &a)
		{
			a.ErrorInfo();
		}
	}
	//Funkciju za uklanjanje proizvoda iz narudžbe
	void UkloniProizvod() {
		try {
			_proizvodi->Pop();
			_brojProizvoda--;
		}
		catch(NedozvoljenaOperacija &a){
			a.ErrorInfo();
		}
	}

	//Dodati funkciju za proračun ukupne cijene narudžbe
	//Voditi računa o mogućnosti dodavanja više istih proizvoda
	//Uračunati popust u slučaju da je definisan
	int ProracunNarudzbe()
	{
		int ukupno=0;
		/*for (size_t i = 0; i < _brojProizvoda; i++)
		{*/
		for (int j = 0; j < _proizvodi->getTop(); j++)
		{
			ukupno += _proizvodi->operator[](j).getCijena();
		}
		//}
		return ukupno;
	}
	//Preklopiti operator za ispis podataka o narudžbi zajedno sa cijenom
	friend ostream & operator<<(ostream & cout, Narudzba &n)
	{
		cout << n._brojNarudzbe << endl << n._datumIsporuke << endl ;
		n._uposlenik->Info();
		for (size_t i = 0; i < n._proizvodi->getTop(); i++)
		{
			//TODO: j?
			 n._proizvodi->operator[](i).Info();
		}
		cout << n.ProracunNarudzbe()<<" KM"<<endl;
		return cout;
	}
};


void main()
{
	//Dodati minimalno 5 proizvoda u katalog
	//Ispisati ponudu kupcu
	//Omogućiti kreiranje narudžbe na osnovu izbora kupca

	try
	{
		Proizvod proizvod("12345678911", "cokolada", -5, -2);
		proizvod.Info();
		

	}
	catch (NedozvoljenaOperacija &a)
	{
		a.ErrorInfo();
	}
	try
	{
		Proizvod proizvod2("1234567891", "cokolada", 5, 2);
		proizvod2.Info();
	}
	catch (NedozvoljenaOperacija &a)
	{
		a.ErrorInfo();
	}
	try {
		Proizvod proizvod3("abd253dbb4", "Maica", 20, 5);
		proizvod3.Info();
		Odjeca o(&proizvod3, "XXL", "Plava");
		o.Info();
	}
	catch (NedozvoljenaOperacija &a)
	{
		a.ErrorInfo();
	}
	Proizvod p1("sdasd", "coksa", 5, 3);
	Proizvod p2("sdas111d", "Hlace", 50, 13);
	Proizvod p3("sdas22d", "Mandarina", 15);
	Proizvod p4("sd44asd", "sok", 4, 3);
	Proizvod p5("35asd", "Jakna", 100, 60);

	Katalog k("Bingo", "21.2.2019");
	k.DodajProizvod(p1);
	k.DodajProizvod(p2);
	k.DodajProizvod(p3);
	k.DodajProizvod(p4);
	k.DodajProizvod(p5);
	k.UkloniProizvod("35asd");
	cout <<crt<< k << endl<<crt;
	Uposlenik u1("Ajdin", "Prodavac");
	Uposlenik u2("Denis", "Predavac");
	Uposlenik u3("Larisa", "Asistent");
	
	Narudzba n(001, "12.22.2012", u1);
	n.DodajProizvodNarudzbi(p1);
	n.DodajProizvodNarudzbi(p2);
	n.DodajProizvodNarudzbi(p3);
	n.DodajProizvodNarudzbi(p4);
	n.DodajProizvodNarudzbi(p5);
	n.UkloniProizvod();
	n.ProracunNarudzbe();
	try {
		Narudzba n2(5, "11.11.2112", u3);
		n2.DodajProizvodNarudzbi(p1);
		n2.DodajProizvodNarudzbi(p2);
		n2.UkloniProizvod();
	}
	catch(NedozvoljenaOperacija &err){
		err.ErrorInfo();
	}
	cout << crt << n << crt ;


	system("pause>fit");
}



