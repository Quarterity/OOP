#include<iostream>
using namespace std;
char * crt= { "\n-----------------------------------------------------------------\n"};
template<class T1, class T2, int max>
class FITKolekcija
{
	int* _trenutno;
	T1 _elementi1[max];
	T2* _elementi2[max];
public:
	//Potrebne konstruktor i destruktor funkcije
	//Kreirati konstruktor kopije za kolekciju
	FITKolekcija(): _trenutno(new int(0)) 
	{
		for (size_t i = 0; i < max; i++)
		{
			_elementi2[i] = nullptr;
		}
	}
	~FITKolekcija() {
		for (size_t i = 0; i < max; i++)
		{
			delete[] _elementi2[i];
			_elementi2[i] = nullptr;
		}
		delete _trenutno;
		_trenutno = nullptr;
	}
	FITKolekcija(FITKolekcija & desni)
	{
		if (*desni._trenutno!=0)
		{
		_trenutno = new int(*desni._trenutno);
		for (size_t i = 0; i <*desni._trenutno; i++)
		{
			_elementi1[i] = desni._elementi1[i];
			_elementi2[i] = new T2(*desni._elementi2[i]);
		}
		}
		for (size_t i = *_trenutno; i < max; i++)
		{
			_elementi2[i] = nullptr;
		}
		
	} 
	bool operator==(T1 desni)
	{
		for (size_t i = 0; i < *_trenutno; i++)
		{
			if ((strcmp(desni, _elementi1[i]) == 0))
			{
				return true;
			}
		}
		return false;
	}
	//Preklopiti operator() na način da omogući dodavanje novih elemenata u kolekciju
	//Pri tome spriječiti dodavanja duplih elemenata (odnosi se i na T1 i na T2)
	void operator()(T1 el1, T2  el2)
	{
		for (size_t i = 0; i < *_trenutno; i++)
		{
			if (el1==_elementi1[i] || el2==*_elementi2[i])
			{
				return;
			}
		}
		_elementi1[(*_trenutno)] = el1;
		_elementi2[*_trenutno] = new T2;
		*_elementi2[(*_trenutno)++] = el2;
	}
	
	//Preklopiti operator[] na način da vraća element tipa T1 na osnovu njegove lokacije u nizu
	T1 & operator[](int index)
	{
		if(index>=0 && index<*_trenutno)
		return _elementi1[index];
	}
	//Preklopiti operator-= na način da uklanja elemente iz kolekcije na osnovu rednog broja proslijeđenog kao parametar
	bool operator-=(int index)
	{
		for (size_t i = 0; i < *_trenutno; i++)
		{
			if (index== *_elementi2[i])
			{
				index = i;
			}
		}
				delete[] _elementi2[index];
				_elementi2[index] = nullptr;
				for (size_t j = index; j <= *_trenutno; j++)
				{
					_elementi1[j] = _elementi1[j + 1];
					//_elementi2[j] = _elementi2[j + 1];
					(*_trenutno)--;
				}
				return true;
	}
	int GetTrenutno()
	{
		return *_trenutno;
	}
	//Preklopiti operator za ispis. Implementaciju raditi izvan tijela klase.
	friend ostream & operator<< <>(ostream &cout, FITKolekcija &  obj);
};
template <class T1,class T2,int max>
ostream & operator<< (ostream & cout, FITKolekcija<T1,T2,max> & obj)
{
	if (*obj._trenutno!=0)
	{
for (size_t i = 0; i < max; i++)
	{
		if (obj._elementi2[i]!=nullptr)
		{
			cout << *obj._elementi2[i] << " - " << obj._elementi1[i] << endl;
		}	
	}
	cout <<"Trenutno el:"<< *obj._trenutno<<endl;
	}
	return cout;

}

class Anketa
{
	char* _pitanje;
	int _trajanje; //izraženo u danima
				   //Kolekcija treba da pohrani moguće odgovore na pitanja, sa pripadajućim rednim brojem
	FITKolekcija<char*, int, 8> _odgovori;
public:
	//Kreirati potrebne konstruktor i destruktor funkcije
	Anketa(char* pitanje="---", int trajanje=0)
	{
		int vel = strlen(pitanje) + 1;
		_pitanje = new char[vel];
		strcpy_s(_pitanje, vel, pitanje);
		_trajanje = trajanje;
	}
	~Anketa() {
		delete[] _pitanje;	
		_pitanje = nullptr;
		for (size_t i = 0; i <_odgovori.GetTrenutno() ; i++)
		{
			delete[] _odgovori[i];
			_odgovori[i] = nullptr;
		}
	}

	//Kreirati funkciju DodajOdgovor koja treba da doda novi odgovor za anketu.
	//Spriječiti dodavanje istog odgovora više puta.
	void DodajOdgovor(char * odgovor,int rb)
	{
		for (size_t i = 0; i < 8; i++)
		{
			if (_odgovori[i] != nullptr)
			{
				if (strcmp(_odgovori[i], odgovor) == 0)
				{
					return;
				}
			}
		}
		char* temp = new char[strlen(odgovor )+1];
		strcpy_s(temp, strlen(odgovor) + 1, odgovor);
		_odgovori(temp, rb);
	}

	//Kreirati funkciju UkloniOdgovor koja uklanja odgovor na osnovu njegovog rednog broja.
	void UkloniOdgovor(int rb)
	{
		_odgovori -= rb;
	}

	//Preklopiti operator++ na način da povećava trajanje ankete za jedan dan
	//Pri tome voditi računa da se implementiraju dvije verzije ovog operatora (objekat++; ++objekat)
	Anketa & operator++(int)
	{
		Anketa *temp=new Anketa(*this);
		_trajanje++;
		return *temp;

	}
	Anketa & operator++()
	{
		_trajanje++;
		return *this;
	}
	//Preklopiti operator za ispis
	friend ostream & operator<<(ostream & cout, Anketa & a)
	{
		cout << crt << "Pitanje:" << a._pitanje << endl << "Odgovori:" << endl << a._odgovori << endl << "Trajanje u danima:" << a._trajanje << crt;
		return cout;
	}
};

int main()
{
	FITKolekcija<char*,int,8> f;
	char * x ("ajdin");
	int xy = 1;
	f(x, xy);
	f[1];
	FITKolekcija<char*, int, 8> b=f;
	cout << b;
	f -= 1;
	cout << f;
	Anketa a("Gdje se nalazi FIT",3);
	cout << a;
	a.DodajOdgovor("U Mostaru",0); cout << a;
	a.DodajOdgovor("U Sarajevu",1);
	cout << a;
	a.UkloniOdgovor(1);
	cout << a++;
	cout << ++a;
	

	system("pause>fit");
}
