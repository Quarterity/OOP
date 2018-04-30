﻿#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <future>
using namespace std;
char * crt = "\n----------------------------------------------------------\n";

class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum &dat) {
		_dan = new int(*dat._dan);
		_mjesec = new int(*dat._mjesec);
		_godina = new int(*dat._godina);
	}
	int & getDan()
	{
		return *_dan;
	}
	int & getMj()
	{
		return *_mjesec;
	}int getGod()
	{
		return *_godina;
	}
	void setDan(int i)
	{
		*_dan=i;
	}
	void setMjesec(int i)
	{
		*_mjesec -= i;
	}
	void setGodina(int i)
	{
		*_godina -= i;
	}
	Datum & operator=(const Datum &dat) {
		if (this != &dat)
		{
			delete _dan;
			delete _mjesec;
			delete _godina;
			_dan =new int(*dat._dan);
			_mjesec =new int (*dat._mjesec);
			_godina =new int (*dat._godina);

		}
		return *this;
	}
	bool operator==(Datum & dat)
	{
		return *dat._dan == *_dan && *dat._mjesec == *_mjesec &&*dat._godina == *_godina;
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream &operator<<(ostream &COUT, const Datum &obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}

};
class Izuzetak : public exception {
	string _funkcija;
	string s;
public:

	Izuzetak(const char * poruka, string funkcija) : exception(poruka)
	{
		_funkcija = funkcija;
	}
	const char* what()
	{
		s = _funkcija + exception::what();
		return s.c_str();


		/*string poruka = const_cast <char *>(exception::what());
		poruka += "-funkcija:" + _funkcija+"\n";
		char * a= new char[poruka.size()+1];
		strcpy_s(a, poruka.size() + 1, poruka.c_str());
		return a;*/
	}
};

template <class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutnoElemenata;
	bool _omoguciDupliranjeElemenata;
public:
	Kolekcija(bool omoguciDupliranjeElemenata = false) {
		_trenutnoElemenata = 0;
		_omoguciDupliranjeElemenata = omoguciDupliranjeElemenata;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	Kolekcija(const Kolekcija &k)
	{
		_elementi1 = new T1[k._trenutnoElemenata ];
		_elementi2 = new T2[k._trenutnoElemenata ];
		_omoguciDupliranjeElemenata = k._omoguciDupliranjeElemenata;
		_trenutnoElemenata = k._trenutnoElemenata;
		for (size_t i = 0; i < k._trenutnoElemenata; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}
	Kolekcija & operator=(const Kolekcija &k)
	{
		if (this!=&k)
		{
			delete[] _elementi1;
			delete[] _elementi2;
			_trenutnoElemenata = k._trenutnoElemenata;
			_omoguciDupliranjeElemenata = k._omoguciDupliranjeElemenata;
			_elementi1 = new T1[k._trenutnoElemenata];
			_elementi2 = new T2[k._trenutnoElemenata];
			for (size_t i = 0; i < _trenutnoElemenata; i++)
			{
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];
			}
		}
		return *this;
	}
	~Kolekcija() {
		delete[]_elementi2; _elementi2 = nullptr;
		delete[]_elementi1; _elementi1 = nullptr;
		_trenutnoElemenata = 0;
	}

	bool AddElement(T1 el1, T2 el2)
	{
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			if (_omoguciDupliranjeElemenata == false && (_elementi1[i]==el1 || _elementi2[i]==el2) )
			{
				return false;
			}
		}
		T1 * temp1 = new T1[_trenutnoElemenata + 1];
		T2 * temp2 = new T2[_trenutnoElemenata + 1];
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;
		temp1[_trenutnoElemenata] = el1;
		temp2[_trenutnoElemenata++] = el2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		return true;
	}
	bool RemoveElement(T1 el1)
	{
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			if (el1 == _elementi1[i])
			{
				for (size_t j = i; j < _trenutnoElemenata - 1; j++)
				{
					_elementi1[j] = _elementi1[j + 1];
					_elementi2[j] = _elementi2[j + 1];
				}
				if (_omoguciDupliranjeElemenata == false)
				{
					_trenutnoElemenata--;
					return true;
				}
			}
		}
		if (_omoguciDupliranjeElemenata==false)
		{
			return false;
		}
		_trenutnoElemenata--;
		return true;
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	T1& GetElement1(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi1[lokacija];
	}

	T2& GetElement2(int lokacija) const
	{
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi2[lokacija];
	}

	friend ostream &operator<<(ostream &COUT, const Kolekcija &obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
		return COUT;
	}
};

class Dogadjaj
{
	Datum _datumOdrzavanja;
	Kolekcija<string, bool> * _obaveze; //cuva informaciju o obavezama koje je potrebno ispuniti prije samog dogadjaja, string se odnosi na opis, a bool na izvrsenje te obaveze (da li je zavrsena ili ne)

	char *_naziv;
	int _notificirajPrije; //oznacava broj dana prije samog dogadjaja kada ce krenuti notifikacija/podsjetnik
	bool _rekurzivnaNotifikacija; //ako je vrijednost true onda se korisnik notificira svaki dan do _datumaOdrzavanja dogadjaja, a pocevsi prije dogadjaja za _brojDanaZaNotifikaciju

public:
	Dogadjaj(Datum datumOdrzavanja, const char *naziv, int brojDana = 1,
		bool rekurzivnaNotifikacija = false) : _datumOdrzavanja(datumOdrzavanja)
	{
		_naziv = new char[strlen(naziv) + 1];
		strcpy_s(_naziv, strlen(naziv) + 1, naziv);

		_notificirajPrije = brojDana;
		_rekurzivnaNotifikacija = rekurzivnaNotifikacija;
		_obaveze = new Kolekcija<string,bool>;
	}

	Dogadjaj(const Dogadjaj &obj) : _datumOdrzavanja(obj._datumOdrzavanja)
	{
		_naziv = new char[strlen(obj._naziv) + 1];
		strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);

		_notificirajPrije = obj._notificirajPrije;
		_rekurzivnaNotifikacija = obj._rekurzivnaNotifikacija;
		_obaveze = new Kolekcija<string,bool>(*obj._obaveze);
	}
	~Dogadjaj()
	{
		delete[] _naziv;
		_naziv = nullptr;
		delete _obaveze;
		_obaveze = nullptr;
	}
	bool AddObavezu(string el1, bool el2=false)
	{
		if(_obaveze->AddElement(el1, el2))
			return true;
		return false;
	}
	char *GetNaziv() { return _naziv; }
	Kolekcija<string, bool> *GetObaveze() { return _obaveze; }
	Datum & GetDatum() { return _datumOdrzavanja; }
	bool getRekurzivna() { return _rekurzivnaNotifikacija; }
	int getNotificirajPrije() { return _notificirajPrije; }
};

class Student
{
	int _indeks;
	string _imePrezime;
	vector<Dogadjaj> _dogadjaji;
public:

	Student(int indeks, string imePrezime) : _indeks(indeks), _imePrezime(imePrezime) {}

	int GetIndeks() const { return _indeks; }

	vector<Dogadjaj>& GetDogadjaji() { return _dogadjaji; }

	bool AddDogadjaj(const Dogadjaj &d)
	{
		_dogadjaji.push_back(d);
		return true;
	}
	string getIme() { return _imePrezime; }
	friend ostream &operator<<(ostream &COUT, const Student &obj)
	{
		COUT << obj._imePrezime << " (" << obj._indeks << ")" << endl;
		return COUT;
	}
};
string sadrzaj = "\nDogadjaj Ispit iz PRIII je zakazan za 3 dana, a do sada ste obavili 56 % obaveza vezanih za ovaj dogadjaj.Neispunjene obaveze su :\n1.Preraditi ispitne zadatke\n2.Samostalno vjezbati\nPredlazemo Vam da ispunite i ostale planirane obaveze.\nFIT Reminder";
class DLWMSReminder
{
	vector<Student> _reminiderList;
public:
	bool AddStudent(const Student &s)
	{
		_reminiderList.push_back(s);
		return true;
	}
	bool OznaciObavezuKaoZavrsenu(int index,string naziv,string opis)
	{
		for (size_t i = 0; i < _reminiderList.size(); i++)
		{
			if (_reminiderList[i].GetIndeks() == index)
			{
				for (size_t j = 0; j < _reminiderList[i].GetDogadjaji().size(); j++)
				{
					if (_reminiderList[i].GetDogadjaji()[j].GetNaziv() == naziv)
					{
						for (size_t y = 0; y < _reminiderList[i].GetDogadjaji()[j].GetObaveze()->GetTrenutno(); y++)
						{
							if (_reminiderList[i].GetDogadjaji()[j].GetObaveze()->GetElement1(y)==opis && _reminiderList[i].GetDogadjaji()[j].GetObaveze()->GetElement2(y)==false)
							{
								_reminiderList[i].GetDogadjaji()[j].GetObaveze()->GetElement2(y) = true;
							}
						}
					}
				}

			}
		}
		return true;

	}
	int DanaMjesecno[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int PosaljiNotifikacije( Datum & d)
	{
		int br = 0;
		
		thread t([&]() {

			for (size_t i = 0; i <_reminiderList.size(); i++)
			{
				for (size_t j = 0; j < _reminiderList[i].GetDogadjaji().size(); j++)
				{
					Datum datumNotif = _reminiderList[i].GetDogadjaji()[j].GetDatum();
					int NotifPrije = _reminiderList[i].GetDogadjaji()[j].getNotificirajPrije();
					cout << datumNotif << "-" << NotifPrije << "=" << datumNotif.getDan() - NotifPrije << datumNotif.getMj() << datumNotif.getGod() << endl;
					if (datumNotif.getMj() == 0) // Ako je prvi mjesec umanji godinu
					{
						datumNotif.setGodina(1);
						datumNotif.getMj() = (13);
					}
					if (datumNotif.getDan() - NotifPrije <= 0) // ako je nulti dan ili manje umanji mjesec i postavi dan
					{
						datumNotif.setMjesec(1);
						if ((datumNotif.getGod() % 4 == 0 && datumNotif.getGod() % 100 != 0) || datumNotif.getGod() % 400 == 0)
						{
							datumNotif.setDan(DanaMjesecno[datumNotif.getMj()] + 1 + (datumNotif.getDan() - NotifPrije));
						}
						else
							datumNotif.setDan(DanaMjesecno[datumNotif.getMj()] + (datumNotif.getDan() - NotifPrije));
					}
					else
					datumNotif.getDan() -= NotifPrije;
					if (_reminiderList[i].GetDogadjaji()[j].getRekurzivna() == true)
					{
						for (size_t z = 0; z < NotifPrije; z++)
						{
							cout << "Postovani, " << _reminiderList[i].getIme() << endl << sadrzaj << endl;
							br++;
						}
						break;
					}
				}
			}

		});
		t.join();

		return br;
	}
};

void main() {

	/**************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_ParcijalniII.TXT
	5. NAZIVI FUNKCIJA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI PO ŽELJI.
	****************************************************************************/

#pragma region Datum

	Datum danas(28, 1, 2018), sutra(29, 1, 2018);
	Datum datumIspitaPRIII(30, 1, 2018), datumIspitBPII(31, 1, 2018);
	Datum prekosutra(danas);
	prekosutra = danas;
	cout << danas << endl
		<< sutra << endl
		<< prekosutra << crt;

#pragma endregion

#pragma region Kolekcija

	/*
	AddElement :: omogucava dodavanje novog elementa u kolekciju. Ukoliko je moguce, osigurati automatsko prosiranje kolekcije prilikom dodavanja svakog novog elementa, te onemoguciti ponavljanje elemenata
	RemoveElement :: na osnovu parametra tipa T1 uklanja elemente iz kolekcije i ukoliko je moguce smanjuje velicinu niza/kolekcije. Prilikom uklanjanja elemenata ocuvati redoslijed njihovog dodavanja
	*/

	const int brElemenata = 10;
	Kolekcija<int, float> kolekcija1(true);
	for (size_t i = 0; i < brElemenata; i++)
		if (!kolekcija1.AddElement(i, i + (0.6 * i)))
			cout << "Elementi " << i << " i " << i + (0.6 * i) << " nisu dodati u kolekciju" << endl;



	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
	cout << kolekcija1 << endl;

	kolekcija1.RemoveElement(1);

	Kolekcija<int, float> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;

	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;

	Kolekcija<int, float> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;

#pragma endregion

#pragma region Dogadjaj

	Dogadjaj ispitPRIII(datumIspitaPRIII, "Ispit iz PRIII", 5, true),
		ispitBPII(datumIspitBPII, "Ispit iz BPII", 7, true);
	/*po vlasitom izboru definisati listu zabranjenih rijeci koje ce onemoguciti dodavanje odredjene obaveze. Prilikom provjere koristiti regex*/
	if (ispitPRIII.AddObavezu("Preraditi pdf materijale"))cout << "Obaveza dodana!" << endl;
	//onemoguciti dupliranje obaveza
	if (!ispitPRIII.AddObavezu("Preraditi pdf materijale"))cout << "Obaveza nije dodana!" << endl;
	if (ispitPRIII.AddObavezu("Pregledati video materijale"))cout << "Obaveza dodana!" << endl;
	if (ispitPRIII.AddObavezu("Preraditi ispitne zadatke"))cout << "Obaveza dodana!" << endl;
	if (ispitPRIII.AddObavezu("Samostalno vjezbati"))cout << "Obaveza dodana!" << endl;

	if (ispitBPII.AddObavezu("Preraditi knjigu SQL za 24 h"))cout << "Obaveza dodana!" << endl;
	if (ispitBPII.AddObavezu("Pregledati video materijale"))cout << "Obaveza dodana!" << endl;
	if (ispitBPII.AddObavezu("Napraviti bazu za konkretnu aplikaciju"))cout << "Obaveza dodana!" << endl;

	Student jasmin(150051, "Jasmin Azemovic"), adel(160061, "Adel Handzic");

	if (jasmin.AddDogadjaj(ispitPRIII) && jasmin.AddDogadjaj(ispitBPII))
		cout << "Dogadjaj uspjesno dodan!" << endl;

	if (adel.AddDogadjaj(ispitPRIII) && adel.AddDogadjaj(ispitBPII))
		cout << "Dogadjaj uspjesno dodan!" << endl;
	//onemoguciti dupliranje dogadjaja
	if (!adel.AddDogadjaj(ispitPRIII))
		cout << "Dogadjaj nije uspjesno dodan!" << endl;

	DLWMSReminder reminder;

	try
	{
		reminder.AddStudent(jasmin);
		reminder.AddStudent(adel);
		//u slucaju dupliranja studenata funkcija baca izuzetak tipa Izuzetak
		reminder.AddStudent(jasmin);
		kolekcija3.GetElement2(50);

	}
	catch (exception &err)
	{
		//ispisati sve informacije o nastalom izuzetku
		Izuzetak * i = dynamic_cast<Izuzetak *>(&err);
		if (i!=NULL)
		{
			cout<<i->what();
		}
	}

	//da bi bila oznacena kao zavrsena, obaveza mora postojati i mora biti oznacena kao nezavrsena (false)
	if (reminder.OznaciObavezuKaoZavrsenu(150051, "Ispit iz PRIII", "Pregledati video materijale"))
		cout << "Obaveza oznacena kao zavrsena" << endl;

	/*metodi PosaljiNotifikacije se salje trenutni datum na osnovu cega ona pretrazuje sve studente koje treba podsjetiti/notoficirati o dogadjajima koji se priblizavaju.
	Koristeci multithread-ing, svim studentima se salju notifikacije sa sljedecim sadrzajem:
	-------------------------------------------------------------------------
	Postovani Jasmin Azemovic,
	Dogadjaj Ispit iz PRIII je zakazan za 3 dana, a do sada ste obavili 56% obaveza vezanih za ovaj dogadjaj. Neispunjene obaveze su:
	1.Preraditi ispitne zadatke
	2.Samostalno vjezbati
	Predlazemo Vam da ispunite i ostale planirane obaveze.
	FIT Reminder
	-------------------------------------------------------------------------
	Dakle, notifikacije ce biti poslane svim studentima koji su dodali dogadjaj za 30.01.2018. godine i oznacili da zele da budu podsjecani ponovo/rekurzivno najmanje 2 dana prije samog dogadjaja (podaci se odnose na konkretan dogadjaj: Ispit iz PRIII)

	*/
	int poslato = 0;
	future<int> novi = async(&DLWMSReminder::PosaljiNotifikacije,reminder, danas);
	//funkcija vraca broj poslatih podsjetnika/notifikacija
	poslato = novi.get();//  reminder.PosaljiNotifikacije(danas);
	cout << "Za " << danas << " poslato ukupno " << poslato << " podsjetnika!" << endl;
	future <int> novi2 = async(&DLWMSReminder::PosaljiNotifikacije, reminder, sutra);
	poslato = novi2.get();
	cout << "Za " << sutra << " poslato ukupno " << poslato << " podsjetnika!" << endl;

#pragma endregion

	system("pause");
}
