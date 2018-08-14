#include<iostream>

using namespace std;

class Vrijeme {
	int _sati;
	int _minute;
	int _sekunde;
public:

	//Konstruktor bez parametara
	Vrijeme() :_sati(0), _minute(0), _sekunde(0) { ; }
	//Konstruktor sa 3 parametra (sati, minute i sekunde)
	Vrijeme(const int & sati, const int & minute, const int & sekunde) :_sati(sati), _minute(minute), _sekunde(sekunde) { ; }
	//Konstruktor sa 2 parametra (sati i minute)
	Vrijeme(const int & sati, const int & minute) :_sati(sati), _minute(minute), _sekunde(0) { ; }

	//Funkciju Info sa neobaveznim parametrom tipa bool koji određuje da li će vrijeme biti ispisano u punom formatu
	//(hh:mm:ss) ili u skraćenom formatu (hh:mm). Default-no funkcija vrši ispis u punom formatu.
	void Info(const bool &puniFormat = true)
	{
		cout << _sati <<":"<< _minute;
		if (puniFormat) cout << ":" << _sekunde<<endl;
	}

	//Funkciju Add koja kao rezultat vraća novi objekat tipa vrijeme koji predstavlja 
	//sumu vremena poslijeđenog kao parametar i instance this.
	Vrijeme Add(const Vrijeme & v2)
	{
		return Vrijeme(v2._sati + _sati, v2._minute + _minute, v2._sekunde + _sekunde);
	}


	//Funkciju Add koja kao rezultat vraća novi objekat tipa vrijeme koji predstavlja 
	//sumu parametra izraženog u minutama i vremena instance this.
	Vrijeme Add(const int & minute)
	{
		return Vrijeme((minute / 60) + _sati, (minute % 60) + _minute,_sekunde);
	}

	//Funkciju CompareTo koja vraća vrijednost tipa int u zavisnosti od toga da li je vrijeme instance this prije, jednako ili poslije
	//vremena proslijeđenog kao parametar.
	//Moguće povratne vrijednosti:
	//-1 - vrijeme instance this nastupa prije vremena v2 
	// 0 - vrijeme instance this je jednako vremenu v2
	// 1 - vrijeme instance this nastupa nakon vremena v2

int CompareTo(const Vrijeme & v2)
{
	if ((_sekunde + _minute * 60 + _sati * 3600) < (v2._sekunde + v2._minute * 60 + v2._sati * 3600))
		return -1;
	else if ((_sekunde + _minute * 60 + _sati * 3600) > (v2._sekunde + v2._minute * 60 + v2._sati * 3600))
	{
		return 1;
	}
	return 0;

}
};

/*
Potrebno je kreirati aplikaciju za dijeljenje znanja putem razmjene članaka među korisnicima.

Izdvojene su sljedeće funkcionalnosti:
- Administrator sistema je zadužen za upravljanje korisničkim nalozima (Ime, Prezime, Email, DatumRegistracije, KorisnickoIme, Lozinka).
- Svaki članak (Naslov, Sadrzaj, Datum, Satnica, BrojPregleda) može sadržavati više tagova, ali pripada samo jednoj oblasti.
- Članke mogu dodavati svi registrovani korisnici.
- Korisnici aplikacije također mogu ostaviti komentar (Tekst, Datum, Popularnost) na određeni članak.
*/

/*
Na osnovu datih klasa implementirati opisane funkcionalnosti sistema. Po potrebi dopuniti klase novim atributima i/ili funkcijama.
*/

class Datum
{
	int _dan;
	int _mjesec;
	int _godina;
public:
	//Potrebne konstruktor i destruktor funkcije
	Datum(const int &dan =1, const int & mjesec =1, const int & godina = 1900)
	{
		if (dan != 1 || mjesec != 1 || godina != 1900)
		{
			if (!ispravanDatum(dan, mjesec, godina))
			{
				_dan = 1;
				_mjesec = 1;
				_godina = 1900;
				return;
			}
		}
		_dan = (dan);
		_mjesec = (mjesec);
		_godina = (godina);
	}
	bool ispravanDatum(const int & dan, const int & mjesec, const int & godina) {
		bool prestupna = godina % 4 == 0 && (godina % 100 != 0 || godina % 400 == 0);
		int daniMjeseca[12] = { 31,28 + prestupna,31,30,31,30,31,31,30,31,30,31 };
		if (godina > 2100 || godina<1900 || mjesec < 1 || mjesec>12 || dan>daniMjeseca[mjesec] || dan < 1)
		{
			return false;
		}
		return true;
	}
	Datum(const Datum &d)
	{
		*this = Datum(d._dan, d._mjesec, d._godina);
	}
	//Funkciju Info
	void Info()
	{
		cout << _dan << "." << _mjesec << "." << _godina << endl;
	}
};
#include <string>
enum TipKorisnika { Administrator, Guest };
class Korisnik
{
	char _ime[30];
	char * _prezime;
	string _email;
	Datum _datumRegistracije;
	char _korisnickoIme[20];
	char* _lozinka;
	TipKorisnika _tipKorisnika; //Administrator, Guest
public:
	//Potrebne konstruktor i destruktor funkcije
	Korisnik() :_prezime(nullptr), _lozinka(nullptr), _tipKorisnika(Guest){ ; }
	Korisnik(const char * ime,char prezime[],string &email,const Datum &d,char *& korisnickoIme,char * lozinka,TipKorisnika korisnik):
		_email( email),_datumRegistracije(d), 	_tipKorisnika ( korisnik)
	{
		strncpy_s(_ime, ime, 30);
		_prezime = new char[strlen(prezime) + 1];
		strcpy_s(_prezime, strlen(prezime) + 1, prezime);
		strncpy_s(_korisnickoIme,korisnickoIme,20);
		_lozinka = new char[strlen(lozinka) + 1];
		strcpy_s(_lozinka, strlen(lozinka) + 1, lozinka);
	}

	Korisnik(const Korisnik &k):
		_email(k._email),  _datumRegistracije(k._datumRegistracije),  _tipKorisnika(k._tipKorisnika)
	{ 
		strncpy_s(_ime, k._ime, 30);
		_prezime = new char[strlen(k._prezime) + 1];
		strcpy_s(_prezime, strlen(k._prezime) + 1, k._prezime);
		strncpy_s(_korisnickoIme, k._korisnickoIme, 20);
		_lozinka = new char[strlen(k._lozinka) + 1];
		strcpy_s(_lozinka, strlen(k._lozinka) + 1, k._lozinka);
	}
	~Korisnik()
	{
		delete[] _prezime;
		_prezime = nullptr;
		delete[] _lozinka;
		_lozinka = nullptr;
	}
	char * getKorisnickoIme() { return _korisnickoIme; }
	//Funkciju za promjenu korisničke lozinke
	void promijeniLozinku(const char * novaLozinka)
	{
		delete[] _lozinka;
		_lozinka = new char[strlen(novaLozinka) + 1];
		strcpy_s(_lozinka, strlen(novaLozinka) + 1, novaLozinka);
	}

	//Funkciju Info
	void Info()
	{
		cout <<endl<< _ime << endl << _prezime << endl << _email << endl;
		_datumRegistracije.Info();
		cout<< _korisnickoIme <<endl<< _lozinka << endl << _tipKorisnika << endl;
	}
};

class Komentar
{
	char* _tekst;
	Datum _datum;
	Vrijeme _satnica;
	int _popularnost;
	Korisnik* _autor;
public:
	//Potrebne konstruktor i destruktor funkcije
	Komentar() :_tekst(nullptr), _autor(nullptr), _popularnost(0) { ; }
	Komentar(const char* tekst, const Datum & datum, const Vrijeme & satnica, const int & popularnost, const Korisnik * autor)
		:_datum(datum), _satnica(satnica), _popularnost(popularnost), _autor(new Korisnik(*autor))
	{
		_tekst = new char[strlen(tekst) + 1];
		strcpy_s(_tekst, strlen(tekst) + 1, tekst);
	}
	Komentar(const Komentar & k)
		:_datum(k._datum), _satnica(k._satnica), _popularnost(k._popularnost), _autor(new Korisnik(*k._autor))
	{
		_tekst = new char[strlen(k._tekst) + 1];
		strcpy_s(_tekst, strlen(k._tekst) + 1, k._tekst);
	}
	Komentar operator=( const Komentar & k)
	{
		if (this!=&k)
		{
		if (_tekst != nullptr )	delete[] _tekst;
		if(_autor !=nullptr) delete _autor;
		_datum=(k._datum);
		_satnica=(k._satnica);
		_popularnost=(k._popularnost);
		_autor=(new Korisnik(*k._autor));
		_tekst = new char[strlen(k._tekst) + 1];
		strcpy_s(_tekst, strlen(k._tekst) + 1, k._tekst);
		}
		return *this;

	}
	~Komentar()
	{
		delete[] _tekst;
		delete _autor;
	}


	//Funkciju za izmjenu popularnosti komentara
	void izmjeniPopularnostKomentara(const int & popularnost) { _popularnost=(popularnost); }


	//Funkciju Info
	void Info()
	{
		cout << _tekst << endl;
		_datum.Info();
		_satnica.Info();
		_autor->Info();
		cout << _popularnost<<endl;
	}
};
//-Svaki članak(Naslov, Sadrzaj, Datum, Satnica, BrojPregleda) može sadržavati više tagova, ali pripada samo jednoj oblasti.

class Clanak
{
	char _naslov[150];
	char* _sadrzaj;
	Datum _datum;
	Vrijeme _satnica;
	int _brojPregleda;
	char* _tagovi[10];
	char _oblast[100];
	Korisnik* _autor;
	Komentar* _komentari;
	int brojKomentara;
public:
	Clanak() 
		: _sadrzaj(nullptr), _brojPregleda(0), _autor(nullptr), _komentari(nullptr),brojKomentara(0)
	{
		for each (auto tag in _tagovi)
		{
			_tagovi[(int)tag] = nullptr;
		}
	}
	Clanak(string naslov,const char sadrzaj[],Datum & datum,Vrijeme satnica,int brojPregleda,const char *tagovi[10],char oblast[100],const Korisnik & autor,Komentar * komentari)
		: _datum(datum),_satnica(satnica),_brojPregleda(brojPregleda)
	{
		strncpy_s(_naslov, naslov.c_str(), 150);
		_sadrzaj = new char[strlen(sadrzaj) + 1];
		strcpy_s(_sadrzaj, strlen(sadrzaj) + 1, sadrzaj);
		for (size_t i = 0; i < 10; i++)
		{
			if (tagovi[i] == nullptr) break;
			else {
				_tagovi[i] = new char[strlen(tagovi[i]) + 1];
				strcpy_s(_tagovi[i], strlen(tagovi[i]) + 1, tagovi[i]);
			}
		}
		strncpy_s(_oblast, oblast, 100);
		_autor = new Korisnik(autor);
		_komentari = new Komentar(*komentari);
		brojKomentara++;
	}
	Clanak(const Clanak & c)
		: _datum(c._datum), _satnica(c._satnica), _brojPregleda(c._brojPregleda), brojKomentara (c.brojKomentara)
	{
		strncpy_s(_naslov, (c._naslov), 150);
		_sadrzaj = new char[strlen(c._sadrzaj) + 1];
		strcpy_s(_sadrzaj, strlen(c._sadrzaj) + 1, c._sadrzaj);
		for (size_t i = 0; i < 10; i++)
		{
			if (c._tagovi[i] == nullptr)
			{
				break;
			}
			_tagovi[i] = c._tagovi[i];
		}
		strncpy_s(_oblast, c._oblast, 100);
		_autor = new Korisnik(*c._autor);
		_komentari = new Komentar(*c._komentari);
	}
	~Clanak() {
		delete[]_sadrzaj;
		if (brojKomentara > 1)
		{
			delete[] _komentari;
		}
		else
			delete _komentari;
		delete _autor;
		for (size_t i = 0; i < 10; i++)
		{
			delete[] _tagovi[i];
		}
	} 
	//Funkciju za dodavanje tagova za članak
	void dodajTagClanku(const char * tag)
	{
		for (size_t i = 0; i < 10; i++)
		{
			if (_tagovi[i]==nullptr)
			{
				_tagovi[i] = new char[strlen(tag) + 1];
				strcpy_s(_tagovi[i], strlen(tag) + 1, tag);
				return;
			}
		}
	}
	/*Funkciju za promjenu sadržaja i vremena promjene članka. Kao verifikaciju dozvole potrebno je proslijediti korisničko ime autora članka.
	Samo kreator članka ima permisije da mijenja njegov sadržaj.
	*/
	bool izmjeniSadrzajClanka(const char*& noviSadrzaj,Vrijeme & satnica,const char *& korisnickoIme )
	{
		if (strcmp(korisnickoIme,_autor->getKorisnickoIme())==0)
		{
			delete[] _sadrzaj;
			_sadrzaj = new char[strlen(noviSadrzaj) + 1];
			strcpy_s(_sadrzaj, strlen(noviSadrzaj) + 1, noviSadrzaj);
			_satnica=satnica;//TODO:operator=
			return true;
		}
		return false;
	}

	//Funkciju za dodavanje komentara
	void dodajKomentar(const Komentar & kom)
	{

		Komentar * temp = new Komentar[brojKomentara + 1];
		for (size_t i = 0; i < brojKomentara; i++)
		{
			temp[i] = (_komentari[i]);
		}
		if (brojKomentara > 1)
		{
			delete[] _komentari;
		}
		else
			delete   _komentari;
		temp[brojKomentara++] = (kom);
		_komentari = temp;
		temp = nullptr;

		//Komentar * temp = new Komentar[brojKomentara + 1];
		//for (size_t i = 0; i < brojKomentara; i++)
		//{
		//	temp[i]=(_komentari[i]);
		//}
		//if (brojKomentara > 1)
		//{
		//	delete[] _komentari;
		//}
		//else
		//	delete  _komentari;
		//temp[brojKomentara++] = (kom);
		//_komentari = new Komentar[brojKomentara];
		//for (size_t i = 0; i < brojKomentara; i++)
		//{
		//	_komentari[i] = *new Komentar(temp[i]);//new nepotreban,alocira novi mem prostor za komentar sa vrijednosti iz temp[i] pa vrijednost iz tog mem prostora dodjeljuje komentari[i] i vodi do memory leak jer se ne dealocira.
		//}
		//delete [] temp;

	}
	//Funkciju Info
	void Info()
	{
		cout << _naslov << endl << _sadrzaj << endl;
		_datum.Info();
		_satnica.Info();
		cout << _brojPregleda << endl;
		for (size_t i = 0; i < 10; i++)
		{
			if (_tagovi[i]!=nullptr)
			{
				cout << _tagovi[i]<<endl;
			}
			else break;
		}
		cout << _oblast << endl;
		_autor->Info();
		for (size_t i = 0; i < brojKomentara; i++)
		{
			cout << "------------------------------";
			_komentari[i].Info();
			
		}
	}

};

int main()
{
	//Kreirati testni program koji demonstrira implementirane funckionalnosti

	Vrijeme v1;
	Vrijeme v2(11, 12, 1);
	Vrijeme v3(11, 12);
	v1.Info();
	v2.Info(false);
	Vrijeme V(v2.Add(v3));
	Vrijeme V2(v2.Add(128));
	cout<<V2.CompareTo(V);
	Datum d(1,2,2000);
	Datum d2(0, 2, 2000);
	Datum d3;
	Datum d4(d);
	Korisnik k;
	char p[15] = "ljubuncic";
	char * pp = new char[strlen("quarter") + 1];
	strcpy_s(pp, strlen("quarter") + 1, "quarter");
	Korisnik k2("Ajdin",p, string("mail"),d,(pp),"ajdin123", Guest);
	Korisnik k3(k2);
	k3.promijeniLozinku("nekaDr");
	k3.Info();
	Komentar kom;
	Komentar kom2("neki tekst", d2, v2, 1,&k2);
	Komentar kom3(kom2);
	kom3.izmjeniPopularnostKomentara(4);
	kom3.Info();
	Clanak c;
	const char * t[10] = { nullptr };
	Clanak c2("naslov clanka", "sadrzaj clanka", d2, v2, 5, t, "neka oblast", k2, &kom3);
	Clanak c3(c2);
	c3.dodajTagClanku("neki tag");
	c3.dodajTagClanku("neki tag2");
	/*char * ppo = new char[strlen("novi sadrzaj") + 1];
	strcpy_s(pp, strlen("novi sadrzaj") + 1, "novi sadrzaj");*/
	char *ppoo= "novi sadrzaj";
	const char *ppooo = k2.getKorisnickoIme();
	c3.izmjeniSadrzajClanka(const_cast<const char *&>(ppoo), V,ppooo);//(const char*&)ppo
	Komentar kom4("nadsadasd", d2, v2, 1, &k2);
	c3.dodajKomentar(kom4);
	Komentar kom5("fghgfhfghf", d2, v2, 1, &k2);
	c3.dodajKomentar(kom5);
	c3.Info();
	system("pause>FIT");
	return 0;
}




