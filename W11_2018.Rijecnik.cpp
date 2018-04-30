#include<iostream>
#include<vector>
#include<string>
#include <regex>
#include<algorithm>
using namespace std;

class Term //Pojam
{
	char* _key;
	char* _value;

	void SetDefaultValues() {
		_key = new char[strlen("bijela") + 1];
		strcpy_s(_key, 6, "white");
		_value = new char[strlen("white") + 1];
		strcpy_s(_value, 7, "white");
	}
public:
	//Konstruktor
	//Atributi _key i _value ne smiju sadržavati brojeve, prazne prostore niti specijalne znakove: @, #, $.
	//U slučaju da vrijednosti proslijeđenih parametara ne zadovolje postavljene uslove,
	//postaviti default-ne vrijednosti atributima klase i baciti izuzetak.
	Term(const char * key, const char * value)
	{
		string pravilo = "(?=[0-9]|\\s|[@#$])";
		if (regex_search(key, regex(pravilo)))
		{
			SetDefaultValues();
			throw exception("greska:key");
		}if (regex_search(value, regex(pravilo)))
		{
			SetDefaultValues();
			throw exception("greska:value");
		}
		_key = new char[strlen(key) + 1];
		strcpy_s(_key, strlen(key) + 1, key);

		_value = new char[strlen(value) + 1];
		strcpy_s(_value, strlen(value) + 1, value);
	}

	//Larisa: const prije Term, a ne nakon
	//L1
	Term(const Term  &t)
	{
		//Larisa: U konstruktoru kopije je suvišno raditi provjeru validnosti podataka
		//jer je objekat t koji je primljen kao parametar već morao proći istu provjeru prilikom nastanka
		//L2
		_key = new char[strlen(t._key) + 1];
		strcpy_s(_key, strlen(t._key) + 1, t._key);

		_value = new char[strlen(t._value) + 1];
		strcpy_s(_value, strlen(t._value) + 1, t._value);
	}
	string GetKey() { return string(_key); }
	void operator=(const Term &t) //Larisa: const prvo //L3
	{
		//Larisa: Također nepotrebno raditi validnost, isti razlog kao i za kons. kopije
		//L4

		//Larisa: Nije urađena dealokacija prethodnih vrijednosti
		delete[] _key;
		delete[] _value;
		//L5
		_key = new char[strlen(t._key) + 1];
		strcpy_s(_key, strlen(t._key) + 1, t._key);

		_value = new char[strlen(t._value) + 1];
		strcpy_s(_value, strlen(t._value) + 1, t._value);
	}
	//Destruktor
	~Term() {
		delete[] _key;
		delete[] _value;
	}
	char* GetValue() { return _value; }
	//Operator za ispis
	friend ostream & operator<<(ostream & cout, const Term &t)
	{
		cout << t._key << "-" << t._value << endl;
		return cout;
	}
	//Operatori poređenja == i != (koristiti samo vrijednost atributa _key)
	bool operator<(const Term &t)
	{
		//Larisa: Ovo se moglo kraće napisati u jednoj liniji: return string(t._key) < string(t._value);
		//TODO:key<value??
		return	string(_key) < string(t._key);
		//L6
	}
	bool operator==(Term &t)
	{
		return (strcmp(t._key, _key) == 0);
	}
	bool operator!=(Term &t)
	{
		//Larisa: Obzirom da je provjera jedna linija koda, za program je bolje da je ona urađena ovdje ponovo
		//umjesto da se poziva operator== i negira njegova povratna vrijednost.
		return strcmp(t._key, _key) == 0;
		//L7
	}
};

class Dictionary //Riječnik pojmova
{
	string _title;
	int _publicationYear;
	//Ključne riječi bosanskog jezika u atribut _key
	//Odgovarajući prevod na engleskom jeziku u atribut _value
	vector<Term> _terms;
public:
	//Konstruktor
	//Naslov riječnika treba da počinje velikim slovom. U suprotnom baciti izuzetak i 
	//postaviti default-ne vrijednosti atributa klase.
	Dictionary(string title, int year)
	{
		string pravilo = "(^[A-Z])";
		if (!(regex_search(title, regex(pravilo))))
		{
			SetDefaultValues();
			throw exception("Greska! Naslov mora biti veliko slovo!");
		}
		_title = title;
		_publicationYear = year;
	}
	void SetDefaultValues()
	{
		_title = "Naslov";
		_publicationYear = 1111;
	}
	//Operator () za dodavanje novih pojmova u riječnik (u paru key-value).
	//Spriječiti dodavanje istih vrijednosti u kolekciju (baciti izuzetak).
	//Pojmovi u svakom momentu treba da budu sortirani abecedno (koristiti funkciju sort iz biblioteke algorithm).
	void operator()(const char* key, const char* value) {
		vector<Term>::iterator it;
		Term temp(key, value);
		for (it = _terms.begin(); it != _terms.end(); it++)
		{
			if (*it == temp)
			{
				throw exception("Greska! Vrijednosti se ponavljaju!");
			}
		}
		_terms.push_back(temp);
		if (_terms.size()>1)
			sort(_terms.begin(), _terms.end());
	}
	friend ostream & operator<< (ostream &out, const Dictionary & d)
	{
		out << d._title << endl << d._publicationYear << endl;
		ostream_iterator<Term> out_it(cout, "\n");
		copy(d._terms.begin(), d._terms.end(), out_it);

		return out;
	}
	//Operator -= za uklanjanje pojma iz riječnika na osnovu atributa _key.
	void operator-=(const char* key) {
		//vector<Term>::iterator it;
		Term temp(key, "---");
		for (int i = 0; i<_terms.size(); i++)
		{
			if (_terms[i] == temp)
			{
				_terms.erase(_terms.begin() + i);
			}
		}

	}

	//Funkciju koja vraća trenutni broj pojmova u riječniku.
	int NumberOfTerms()
	{
		return _terms.size();
	}

	//operator[] koji vraća vrijednost na osnovu ključne riječi (povratna vrijednost je engleska riječ-value, 
	//na osnovu bosanske-key)
	char* operator[](const char * key)
	{
		Term temp(key, "---");
		for (size_t i = 0; i < _terms.size(); i++)
		{
			if (_terms[i] == temp)
				return _terms[i].GetValue();
		}
	}


	//Funkciju koja vraća kolekciju ključeva (bosanskih riječi) generisanih slučajnim odabirom.
	//Generisanje slučajnog broja između min i max: int randNum = rand()%(max-min + 1) + min;
	//Funkcija kao parametar prima broj riječi koje je potrebno generisati.
	vector<string> RandomKeys(int quantity)
	{
		if (quantity>_terms.size())
		{
			throw exception("Greska! Prekoracen broj pojmova!");
		}
		//Larisa: Može i kao char* samo paziti da se na mjestu poziva ne uradi dealokacija ovih vraćenih ključeva
		//Time bi se riječnik učinio neupotrebljivim. Općenito je ovo ok ako smo mi korisnici svojih klasa, međutim
		//da je u pitanju rješenje koje se isporučuje, onda bi ovakva metoda učinila našu klasu potencijalno neupotrebljivom
		//jer javna metoda vraća stvarne adrese memorijskih lokacija.
		int randNum = 0, postoji = -1;
		vector<string> temp;
		for (size_t i = 0; i < quantity; i++)
		{
			do {
				postoji = randNum;
				randNum = rand() % (_terms.size());
			} while (postoji == randNum);
			temp.push_back(_terms[randNum].GetKey());
		}
		return temp;
		//L8
	}

};

//Osmisliti i implementirati klasu Competitor (Takmičar) na način da se upotrebom objekta klase Dictionary
//svakom takmičaru dodijele određena pitanja (riječi bosanskog jezika) - koristiti implementiranu funkciju klase Dictionary.
//Svakom pitanju je potrebno pridružiti odgovor takmičara (kolekcija _odgovori).
//Obavezno implementirati funkciju koja vrši kalkulaciju osvojenih bodova (1 tačan odgovor = 1 bod).
//Po potrebi dopuniti klasu dodatnim atributima.
class Competitor //Takmičar
{
	string _ime;
	string _prezime;
	string _username; //ime.prezime
	string _password; //minimalno 6 karaktera, kombinacija velikih i malih slova, 
					  //te najmanje jedan specijalni karakter iz liste: #%$@?! 
	vector<string> _pitanja;
	vector<string> _odgovori;
public:
	Competitor(string ime, string prezime, string password)
	{
		//Larisa: Posljednji dio pravila nije ispravan, jer navodi da prije specijalnog karaktera
		//mora biti neki drugi znak, što nije navedeno.
		//TODO:provjerit regex
		string pravilo = "(?=.{6,})(?=.*[a-zA-Z](?=.+[#%\\&@?!]))";
		//L9(*[a-zA-Z])??
		if (!regex_search(password, regex(pravilo)))
		{
			SetDefaultValues();
			throw exception("Sifra nije validna!");
		}
		_ime = ime;
		_prezime = prezime;
		_username = ime + '.' + prezime;
		_password = password;
	}
	void IspisiPitanja()
	{
		cout << "\nPitanja:" << endl;
		for (size_t i = 0; i < _pitanja.size(); i++)
		{
			cout << i << "." << _pitanja[i] << endl;
		}
	}
	void SetDefaultValues()
	{
		_ime = "ime";
		_prezime = "prezime";
		_username = "ime.prezime";
		_password = "password";
	}
	void AddQuestion(Dictionary &d, int NumOfQuestions)
	{
		vector<string> s = (d.RandomKeys(NumOfQuestions));
		for (size_t i = 0; i < NumOfQuestions; i++)
		{

			string pitanje = s[i];
			_pitanja.push_back(pitanje);
		}
		//_pitanja=(d.RandomKeys(NumOfQuestions));
	}
	void AddAnswer(string & odgovor)
	{
		_odgovori.push_back(odgovor);
	}
	int Calculate(Dictionary &d)
	{
		int SumOfPoints = 0;
		for (size_t i = 0; i < _pitanja.size(); i++)
		{
			const char* cstr = _pitanja[i].c_str();
			//char* pitanje = new char[strlen(cstr) + 1];//Larisa: Ko će dealocirati ovo pitanje? Riješiti bez alokacije nove memorije
			//strcpy_s(pitanje, strlen(cstr) + 1, cstr);
			//L10
			if (d[cstr] == _odgovori[i])
			{
				SumOfPoints++;
			}
		}
		return SumOfPoints;
	}
};

int main()
{
	//Kreirati riječnik pojmova.
	//Kreirati niz objekata tipa Competitor i simulirati takmičenje u poznavanju riječi engleskog jezika.
	//Pronaći najboljeg među takmičarima (najveći broj bodova).
	Term probni("plava", "blue");
	cout << probni;
	try {
		Term probni2("plava", "blue");
		if (probni == probni2)
			cout << "isti";
		if (probni != probni2)
		{
			cout << "nisu isti";
		}
	}
	catch (exception ex) {
		cout << ex.what();
	}try {
		Dictionary bosanski("Rijecnik", 1990);
		bosanski("plava", "blue");
		bosanski("crna", "black");
		bosanski("crvena", "red");
		bosanski("roza", "pink");
		//bosanski("plava", "blue");
		bosanski("bijela", "white");
		cout << bosanski;
		bosanski -= "plava";
		cout << bosanski.NumberOfTerms();
		cout << bosanski["bijela"];
		vector<string> ispis = bosanski.RandomKeys(3);
		ostream_iterator<string> out(cout, "\n");
		copy(ispis.begin(), ispis.end(), out);
		Competitor takmicar("Ajdin", "Ljubuncic", "Ajdin!123");
		takmicar.AddQuestion(bosanski, 3);
		takmicar.IspisiPitanja();
		string odgovor;
		for (size_t i = 0; i < 3; i++)
		{
			getline(cin, odgovor);
			takmicar.AddAnswer(odgovor);
		}
		cout << "Osvojenih Bodova:" << takmicar.Calculate(bosanski);
	}
	catch (exception ex) {
		cout << ex.what();
	}
	system("pause>fit");
	return 0;
}

