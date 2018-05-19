#include<iostream>

using namespace std;

class Vrijeme {
	int _sati;
	int _minute;
	int _sekunde;
public:

	//Konstruktor bez parametara
	Vrijeme()
	{
		_sati = 0;
		_minute = 0;
		_sekunde = 0;
	}
	//Konstruktor sa 3 parametra (sati, minute i sekunde)
	Vrijeme(int sati, int minute, int sekunde)
	{
		if ((sati <= 24 && sati > 0) && (minute < 61 && minute>0) && (sekunde <= 60 && sekunde > 0))
		{
			_sati = sati;
			_minute = minute;
			_sekunde = sekunde;
		}
		else
			throw exception("Vrijeme je netacno!");
	}
	//Konstruktor sa 2 parametra (sati i minute)
	Vrijeme(int sati, int minute)
	{
		if ((sati <= 24 && sati > 0) && (minute < 61 && minute>0))
		{
			_sati = sati;
			_minute = minute;
			_sekunde = 0;
		}
		else
			throw exception("Vrijeme je netacno!");
	}
	Vrijeme(Vrijeme & v)
	{
		if ((v._sati <= 24 && v._sati >= 0) && (v._minute < 61 && v._minute>=0) && (v._sekunde <= 60 && v._sekunde >= 0))
		{
			_sati = v._sati;
			_minute = v._minute;
			_sekunde = v._sekunde;
		}
		else
			throw exception("Vrijeme je netacno!");
	}
	//Funkciju Info sa neobaveznim parametrom tipa bool koji određuje da li će vrijeme biti ispisano u punom formatu
	//(hh:mm:ss) ili u skraćenom formatu (hh:mm). Default-no funkcija vrši ispis u punom formatu.
	void Info(bool format = true)
	{
		cout << _sati << ":" << _minute;
		if (format)
		{
			cout <<":"<< _sekunde;
		}
		cout << endl;
	}

	//Funkciju Add koja kao rezultat vraća novi objekat tipa vrijeme koji predstavlja 
	//sumu vremena poslijeđenog kao parametar i instance this.
	Vrijeme & Add(Vrijeme & proslijedjeno)
	{
		_sekunde += proslijedjeno._sekunde;
		if (_sekunde>60)
		{
			_sekunde -= 60;
			_minute += 1;
		}
		_minute += proslijedjeno._minute;
		if (_minute>60)
		{
			_minute -= 60;
			_sati += 1;
		}
		_sati += proslijedjeno._sati;
		if (_sati>24)
		{
			_sati = 24;
			cout << "Dodani broj sati premasuje realnu vrijednost"<<endl;
		}
		return *this;
	}

	//Funkciju Add koja kao rezultat vraća novi objekat tipa vrijeme koji predstavlja 
	//sumu parametra izraženog u minutama i vremena instance this.
	Vrijeme & Add(int minute)
	{
		if (minute / 60>24)
		{
			throw exception("Greska u dodavanju minuta");
		}
		_sati += minute / 60;
		_minute += minute % 60;
		return *this;
	}

	//Funkciju CompareTo koja vraća vrijednost tipa int u zavisnosti od toga da li je vrijeme instance this prije, jednako ili poslije
	//vremena proslijeđenog kao parametar.
	//Moguće povratne vrijednosti:
	//-1 - vrijeme instance this nastupa prije vremena v2
	// 0 - vrijeme instance this je jednako vremenu v2
	// 1 - vrijeme instance this nastupa nakon vremena v2
	int CompareTo(Vrijeme & proslijedjeno)
	{

		int prvo = _sati * 10000 + _minute * 100 + _sekunde;
		int drugo = proslijedjeno._sati * 10000 + proslijedjeno._minute * 100 + proslijedjeno._sekunde;
		if (prvo <drugo)
		{
			return -1;
		}
		else if (prvo==drugo)
		{
			return 0;
		}
		return 1;
		
		/*if (proslijedjeno._sati==_sati)
		{
			if (proslijedjeno._minute==_minute)
			{
				if (proslijedjeno._sekunde==_sekunde)
				{
					return 0;
				}
				else if(proslijedjeno._sekunde>_sekunde)
				{
					return 1;
				}
				else
				{
					return -1;
				}
			}
			else if(proslijedjeno._minute>_minute)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else if(proslijedjeno._sati>_sati)
		{
			return 1;
		}
		else
		{
			return -1;
		}*/
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
	Datum(int dan = 1,int mjesec=1,int godina=1900)
	{
		_dan = dan;
		_mjesec = mjesec;
		_godina = godina;
	}
	Datum(Datum & d) :_dan(d._dan), _mjesec(d._mjesec), _godina(d._godina) { ; }
	//Funkciju Info
	void Info()
	{
		cout << _dan << "." << _mjesec << "." << _godina << endl;
	}
};

enum TipKorisnika { Administrator, Guest };
class Korisnik
{
	char _ime[30];
	char _korisnickoIme[20];
	char* _lozinka;
	TipKorisnika _tipKorisnika; //Administrator, Guest
public:
	//Potrebne konstruktor i destruktor funkcije
	Korisnik()
	{
		_ime[0] = '/0';
		_korisnickoIme[0] = '/0';
		_lozinka = new char(' ');
		_tipKorisnika = Guest;
	}
	Korisnik(char ime[30], char korisnickoIme[30], char* lozinka, TipKorisnika tipKorisnika):_tipKorisnika(tipKorisnika)
	{
		strcpy_s(_ime, strlen(ime) + 1, ime);
		strcpy_s(_korisnickoIme, strlen(korisnickoIme) + 1, korisnickoIme);
		_lozinka = new char[strlen(lozinka) + 1];
		strcpy_s(_lozinka, strlen(lozinka) + 1, lozinka);
	}
	//Funkciju za promjenu korisničke lozinke
	void promijeniLozinku(char *lozinka) {
		if (_lozinka!=nullptr)
		{
			delete[] _lozinka;
			_lozinka = nullptr;
		}
		_lozinka = new char[strlen(lozinka) + 1];
		strcpy_s(_lozinka, strlen(lozinka) + 1, lozinka);
	}
	//Funkciju Info
	void Info()
	{
		cout << _ime << endl << _korisnickoIme << _tipKorisnika << endl;
		for (size_t i = 0; i < strlen(_lozinka); i++)
		{
			cout << '*';
		}
		cout << endl;
	}
	char* getKorisnicko()
	{
		return _korisnickoIme;
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
	Komentar(char * tekst = new char(' '), Datum datum = Datum(), Vrijeme satnica = Vrijeme(), int popularnost = 0,Korisnik * autor=new Korisnik())
		:_datum(datum),_satnica(satnica),_autor(new Korisnik(*autor)),_popularnost(popularnost)
	{
		_tekst = new char[strlen(tekst) + 1];
		strcpy_s(_tekst, strlen(tekst)+1, tekst);
	}
	//Funkciju za izmjenu popularnosti komentara
	void izmjeniPopularnost(int popularnost ) { _popularnost = popularnost; }
	//Funkciju Info 
	void Info()
	{
		_datum.Info();
		_satnica.Info();
		_autor->Info();
		cout << *_tekst<<endl;
		cout << _popularnost<<endl;
	}
};
char * defaultArrayof10[10] = {nullptr};
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
public:
	Clanak(char * naslov = new char(' '), char * sadrzaj = new char(' '), Datum datum = Datum(), Vrijeme satnica = Vrijeme(), int brojPregleda = 0,
		char * tagovi[10] = defaultArrayof10, char oblast[100] = {" "}, Korisnik * autor = new Korisnik(), Komentar * komentari = new Komentar())
		:_datum(datum),_satnica(satnica),_autor(new Korisnik(*autor)),_brojPregleda(brojPregleda)
	{
		strcpy_s(_naslov, strlen(naslov) + 1, naslov);
		_sadrzaj = new char[strlen(naslov)];
		strcpy_s(_sadrzaj, strlen(sadrzaj) + 1, sadrzaj);
		for (size_t i = 0; i < 10; i++)
		{
			if (tagovi[i] == nullptr)
			{
				_tagovi[i] = nullptr;
			}
			else {
				_tagovi[i] = new char[strlen(tagovi[i]) + 1];
				strcpy_s(_tagovi[i], strlen(tagovi[i]) + 1, tagovi[i]);
			}
		}
		strcpy_s(_oblast, strlen(oblast) + 1, oblast);
		_komentari = new Komentar(*komentari);
	}
	//Funkciju za dodavanje tagova za članak
	void dodajTagove(char * tag)
	{
		for (size_t i = 0; i < 10; i++)
		{
			if (_tagovi[i]==nullptr)
			{
				_tagovi[i] = new char[strlen(tag) + 1];
				strcpy_s(_tagovi[i], strlen(tag) + 1, tag);
				break;
			}
		}
	}
	/*Funkciju za promjenu sadržaja i vremena promjene članka. Kao verifikaciju dozvole potrebno je proslijediti korisničko ime autora članka.
	Samo kreator članka ima permisije da mijenja njegov sadržaj.
	*/
	void izmjeniClanak(char * sadrzaj,Vrijeme & satnica,char* korisnickoIme)
	{
		if (strcmp(korisnickoIme,_autor->getKorisnicko())==0)
		{
			delete [] _sadrzaj;
			_sadrzaj = nullptr;
			strcpy_s(_sadrzaj, strlen(sadrzaj) + 1, sadrzaj);
			_satnica=(satnica);

		}
	}

	//Funkciju za dodavanje komentara
	void dodajkomentar(Komentar & komentar)
	{
		if (_komentari != nullptr)
		{
			delete[] _komentari;
		}
		_komentari = new Komentar(komentar);

	}
	//Funkciju Info
	void Info()
	{
		_datum.Info();
		_satnica.Info();
		_autor->Info();
		cout << _naslov << endl << _sadrzaj << endl << _tagovi << endl << _brojPregleda <<endl<<_oblast<< endl;
		_komentari->Info();
	}
};

int main()
{
	//Kreirati testni program koji demonstrira implementirane funckionalnosti
	Vrijeme satnica;
	Vrijeme satnica2(02,01,03);
	satnica.Add(56);
	satnica.Add(satnica2);
	cout<<"poredjenje:"<<satnica.CompareTo(satnica2);
	satnica.Info();
	satnica2.Info();
	Datum datum;
	Datum datum2(datum);
	datum.Info();
	Korisnik korisnik;
	Korisnik korisnik2("denis","music","denis123",Administrator);
	korisnik.promijeniLozinku("nekanova");
	korisnik.Info();
	Komentar komentar;
	Komentar komentar2("tekst", Datum(2, 3, 2002), satnica2, 2, &korisnik2);
	komentar.izmjeniPopularnost(4);
	komentar.Info();
	char * tagovi[10] = {nullptr};
	Clanak clanak("naslov", "neki sdrzaj clanka", datum, satnica, 3, tagovi, ("neka oblast"), &korisnik2, &Komentar("komentar na clanak",datum2,satnica2,2,&korisnik));
	Clanak clanak2;
	clanak.dodajkomentar(komentar);
	clanak.dodajTagove("nekitag");
	clanak.izmjeniClanak("noovi sadrzaj", satnica, "guest");
	system("pause>FIT");
	return 0;
}


