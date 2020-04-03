#include<iostream>
#include <vector>
#include <iterator>
#include <mutex>
#include <thread>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/

const char* nedozvoljena_operacija = "Nedozvoljena operacija";
const char* not_set = "NOT_SET";

enum Sortiranje { ASC, DESC };
enum Predmet { MATEMATIKA, FIZIKA, HEMIJA, GEOGRAFIJA, NOT_SET };
enum Razred { I = 1, II, III, IV };

char* AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
template <class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int _trenutnoElemenata;
	bool _dozvoliDupliranje;
	Sortiranje _sortiranje;
public:
	Kolekcija(Sortiranje sortiranje = ASC, bool dozvoliDupliranje = true) {
		_trenutnoElemenata = 0;
		_sortiranje = sortiranje;
		_dozvoliDupliranje = dozvoliDupliranje;
	}
	~Kolekcija() {
		for (size_t i = 0; i < _trenutnoElemenata; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1& GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi1[lokacija];
	}
	T2& GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi2[lokacija];
	}
	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
	Kolekcija(const Kolekcija& obj) {
		_trenutnoElemenata = obj._trenutnoElemenata;
		_sortiranje = obj._sortiranje;
		_dozvoliDupliranje = obj._dozvoliDupliranje;
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			if (obj._elementi1[i] != nullptr && obj._elementi2[i] != nullptr) {
				_elementi1[i] = new T1(*obj._elementi1[i]);
				_elementi2[i] = new T2(*obj._elementi2[i]);
			}
			else {
				_elementi1[i] = nullptr;
				_elementi2[i] = nullptr;
			}
		}
	}
	void AddElement(const T1& ele1, const T2& ele2) {
		if (_trenutnoElemenata == max) {
			throw exception(nedozvoljena_operacija);
			return;
		}

		if (_dozvoliDupliranje == false) {
			for (size_t i = 0; i < _trenutnoElemenata; i++)
			{
				if (*_elementi1[i] == ele1 && *_elementi2[i] == ele2) {
					throw exception(nedozvoljena_operacija);
					return;
				}
			}
		}
		_elementi1[_trenutnoElemenata] = new T1(ele1);
		_elementi2[_trenutnoElemenata] = new T2(ele2);
		_trenutnoElemenata++;

		if (_sortiranje == 0) {

			for (size_t i = 0; i < _trenutnoElemenata - 1; i++)
			{
				if (*_elementi1[i] > * _elementi1[i + 1]) {
					T1 temp1 = *_elementi1[i];
					T2 temp2 = *_elementi2[i];
					*_elementi1[i] = *_elementi1[i + 1];
					*_elementi2[i] = *_elementi2[i + 1];
					*_elementi1[i + 1] = temp1;
					*_elementi2[i + 1] = temp2;

				}
			}
		}
		else {
			for (size_t i = 0; i < _trenutnoElemenata - 1; i++)
			{
				if (*_elementi1[i] < *_elementi1[i + 1]) {
					T1 temp1 = *_elementi1[i];
					T2 temp2 = *_elementi2[i];
					*_elementi1[i] = *_elementi1[i + 1];
					*_elementi2[i] = *_elementi2[i + 1];
					*_elementi1[i + 1] = temp1;
					*_elementi2[i + 1] = temp2;

				}
			}
		}
	}


};

class Aktivnost {
	shared_ptr<Razred> _razred;
	string _opis;
	int _ocjena;//1-5
public:
	Aktivnost(Razred razred = I, int ocjena = 0, string opis = not_set) {
		_ocjena = ocjena;
		_opis = opis;
		_razred = make_shared<Razred>(razred);
	}
	Aktivnost(const Aktivnost& obj) {
		_opis = obj._opis;
		_ocjena = obj._ocjena;
		_razred = make_shared<Razred>(*obj._razred);
	}
	int  GetOcjenu() const { return _ocjena; }
	string GetOpis() const { return _opis; }
	Razred GetRazred() const { return *_razred; }

	friend ostream& operator<<(ostream& COUT, const Aktivnost& obj) {
		COUT << *obj._razred << " " << obj._ocjena << " " << obj._opis << endl;
		return COUT;
	}
	bool operator==(const Aktivnost& a)
	{
		if (_opis == a._opis)
			return true;
		else return false;
	}
};

class Polaznik {
protected:
	char* _imePrezime;
	string _brojTelefona;
	bool ispravanBrojTelefona(string brojTelefona) {
		//+387(6X)XXX-XXX ili +387 6X XXX-XXX
		string provjera = "^(\\+387)((\\(6\\d\\))|(\\s6\\d\\s))(\\d{3}-\\d{3})$";
		return regex_match(brojTelefona, regex(provjera));
	}
public:
	Polaznik(string imePrezime, string brojTelefona) : _imePrezime(AlocirajNizKaraktera(imePrezime.c_str())) {
		_brojTelefona = brojTelefona;
	}
	Polaznik(const Polaznik& obj) {
		_imePrezime = AlocirajNizKaraktera(obj._imePrezime);
		_brojTelefona = obj._brojTelefona;
	}
	~Polaznik() { delete[] _imePrezime; }
	char* GetImePrezime() { return _imePrezime; }
	string GetTelefon() { return _imePrezime; }
	virtual void PredstaviSe() = 0;
};

class Ucenik : public Polaznik {
	Kolekcija<Predmet, Aktivnost, 16>* _aktivnosti;
public:
	Ucenik(string imePrezime, string brojTelefona) :Polaznik(imePrezime, brojTelefona) {
		_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>();
	}
	Ucenik(const Ucenik& original) :Polaznik(original)
	{
		_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>(*original._aktivnosti);

	}
	~Ucenik() { delete _aktivnosti; _aktivnosti = nullptr; }
	Kolekcija<Predmet, Aktivnost, 16>& GetAktivnosti() { return *_aktivnosti; };
	Kolekcija<Predmet, Aktivnost, 16>* GetAktivnosti2() { return _aktivnosti; };
	friend ostream& operator<<(ostream& COUT, Ucenik& n)
	{
		COUT << "PODACI O UCENIKU I NJEGOVIM AKTIVNOSTIMA....IMPLEMENTIRATI" << endl;
		COUT << "Ime i prezime" << n._imePrezime << endl;
		COUT << "Broj telefona" << n._brojTelefona << endl;
		if (n._aktivnosti != nullptr) {
			COUT << *n._aktivnosti;
		}
		else {
			COUT << "Nema aktivnosti" << endl;
		}
		return COUT;
	}
	void PredstaviSe() {
		cout << *this << endl;
	}

};
class Skola {
	char* _naziv;
	vector<Ucenik> _ucenici;
public:
	Skola(const char* naziv = nullptr) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	~Skola() {
		delete[] _naziv; _naziv = nullptr;
	}
	char* GetNaziv()const { return _naziv; }
	vector<Ucenik>& GetUcenici() { return _ucenici; };

	void operator()(string imePrezime, string brojTelefona) {
		Ucenik temp(imePrezime, brojTelefona);
		vector<Ucenik>::iterator it = _ucenici.begin();
		while (it != _ucenici.end()) {
			if (it->GetImePrezime() == imePrezime || it->GetTelefon() == brojTelefona) {
				throw exception("Ucenik sa tim imenom/brojem telefona vec postoji");
				return;
			}
			it++;
		}
		_ucenici.push_back(temp);
	}

	friend ostream& operator<<(ostream& COUT, Skola& obj) {
		COUT << "Skola: " << obj._naziv << endl;
		COUT << "Ucenici: " << endl;
		for (size_t i = 0; i < obj._ucenici.size(); i++)
			COUT << obj._ucenici[i] << endl;
		return COUT;
	}
	/*na nivou svakog razreda se mogu evidentirati maksimalno 4 aktivnosti, a takodjer, na nivou razreda se ne smiju ponavljati
	aktivnosti iz istog predmeta*/
	/*u slucaju da je ucenik uspjesno (ocjenom vecom od 1) realizovao aktivnosti na nivou odredjenog razreda, te posjeduje validan
	broj telefona,
	u okviru zasebnog thread-a se salje SMS sa sadrzajem "Uspjesno ste okoncali aktivnosti u okviru X razreda sa
	prosjecnom ocjenom X.X"*/
	/*bool DodajAktivnost(string imePrezime, Predmet predmet, Aktivnost a){
		for (vector<Ucenik>::iterator i = _ucenici.begin(); i != _ucenici.end(); i++)
		{
			for (size_t j = 0; j < i->GetAktivnosti().GetTrenutno(); j++)
			{
				if (i->GetAktivnosti().GetElement2(j).GetRazred() == a.GetRazred()) {
					if (i->GetAktivnosti().GetTrenutno() > 3) return false;
					if (i->GetAktivnosti().GetElement1(j) == predmet) return false;
					i->GetAktivnosti().AddElement(predmet, a);
					return true;
				}
			}			
		}
	}*/
	bool DodajAktivnost(string imePrezime, Predmet predmet, Aktivnost nekaAktivnost) {
		int brojac = 0;
		for (int i = 0; i < _ucenici.size(); i++) {
			if (!strcmp(imePrezime.c_str(), _ucenici[i].GetImePrezime())) {
				for (int j = 0; j < _ucenici[i].GetAktivnosti().GetTrenutno(); j++) {
					if (_ucenici[i].GetAktivnosti().GetElement2(j).GetRazred() == nekaAktivnost.GetRazred()) {
						brojac++;
						if (_ucenici[i].GetAktivnosti().GetElement1(j) == predmet) {
							return false;

						}
					}
				}
			}
		}
		if (brojac > 4) {
			return false;
		}
		for (int i = 0; i < _ucenici.size(); i++) {
			if (!strcmp(imePrezime.c_str(), _ucenici[i].GetImePrezime())) {
				_ucenici[i].GetAktivnosti().AddElement(predmet, nekaAktivnost);
				return true;
			}
		}
	}

	pair<Polaznik*, float> GetNajboljegUcenika() {
		Polaznik* najpolaznik = nullptr;
		float prosjek = 0;
		for (vector<Ucenik>::iterator i = _ucenici.begin(); i != _ucenici.end(); i++)
		{
			float prosjekTrenutno = 0;
			for (size_t j = 0; j < i->GetAktivnosti().GetTrenutno(); j++)
			{
				prosjekTrenutno += i->GetAktivnosti().GetElement2(j).GetOcjenu();
			}
			prosjekTrenutno /= i->GetAktivnosti().GetTrenutno();
			if (prosjekTrenutno < prosjek) {
				prosjekTrenutno = prosjek;
				najpolaznik = i._Ptr;
			}
		}
		return pair<Polaznik*, float>(najpolaznik, prosjek);
	}
	bool SpasiUFajl(string fajl, bool dodaj = false)
	{
		if (dodaj)
		{
			ofstream upis(fajl, ios::app);
			upis << "Skola: " << _naziv << endl << "Ucenici: " << endl;
			for (vector<Ucenik>::iterator i = _ucenici.begin(); i != _ucenici.end(); i++)
			{
				upis << *i << endl;
			}
			upis.close();
			ifstream ispis(fajl);
			char znak;
			while (ispis.get(znak))
				cout << znak;
			cout << endl;
			ispis.close();
			return true;
		}
		else
		{
			ofstream upis("Gimnazija.txt", ios::out);
			upis << "Skola: " << _naziv << endl << "Ucenici: " << endl;
			for (vector<Ucenik>::iterator i = _ucenici.begin(); i != _ucenici.end(); i++)
			{
				upis << *i << endl;
			}
			upis.close();
			ifstream ispis("Gimnazija.txt");
			char znak;
			while (ispis.get(znak))
				cout << znak;
			cout << endl;
			ispis.close();
			return true;
		}
		return false;
	}
};

int main() {
	Kolekcija<int, int, 10> kolekcija1(DESC, false);
	try {
		kolekcija1.AddElement(1, 2);
		//dupliranje elemenata nije dozvoljeno
		kolekcija1.AddElement(1, 2);

	}
	catch (exception & ex) {
		cout << ex.what();
	}

	/*nakon svakog dodavanja, elemente sortirati prema T1 i vrijednosti atributa _sortiranje*/
	for (size_t i = 1; i < kolekcija1.GetMax() - 1; i++)
		kolekcija1.AddElement(rand(), rand());

	cout << kolekcija1 << endl;

	try {
		//prekoracen maksimalan broj elemenata
		kolekcija1.AddElement(rand(), rand());
	}
	catch (exception & ex) {
		cout << ex.what();
	}
	cout << kolekcija1 << endl;

	Kolekcija<int, int, 10> kolekcija2(kolekcija1);
	cout << kolekcija2 << endl;

	Skola gimnazijaMostar("GIMNAZIJA MOSTAR");
	//dodaje novog ucenika u skolu
	gimnazijaMostar("Jasmin Azemovic", "+387(61)111-222");
	gimnazijaMostar("Adel Handzic", "+387(61)333-444");

	/*
	koristeci regex, osigurati sljedeci format za broj telefona: +387(6X)XXX-XXX ili +387 6X XXX-XXX
	onemoguciti pojavljivanje samo jedne zagrade, a ukoliko format nije adekvatna koristiti vrijednost not_set
	*/
	gimnazijaMostar("Telefon NotValidFormat", "387 61)333-444");

	try
	{
		/*onemoguciti dodavanje ucenika sa istim imenom i prezimenom ili brojem telefona*/
		gimnazijaMostar("Adel Handzic", "+387(61)333-444");
	}
	catch (exception & ex)
	{
		cout << ex.what() << endl;
	}

	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Priprema za takmicenje iz Matematije koje se odrzava u Konjicu 07.02.2019")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (!gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Aktivnosti iz matematike")))
		cout << "Aktivnost nije uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", HEMIJA, Aktivnost(I, 5, "Priprema otopina za vjezbe iz predmeta Hemija")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", FIZIKA, Aktivnost(I, 4, "Analiza stepena apsorpcije materijala ")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", GEOGRAFIJA, Aktivnost(I, 4, "Izrada 5 reljefa Mostara")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Adel Handzic", MATEMATIKA, Aktivnost(I, 5, "Izrada skripte na temu integralni racun")))
		cout << "Aktivnost uspjesno dodana" << endl;

	//ispisuje sve ucenike i njihove aktivnosti
	cout << gimnazijaMostar << endl;


	pair<Polaznik*, float> par = gimnazijaMostar.GetNajboljegUcenika();
	if (par.first != nullptr)
		cout << "Najbolji ucenik je " << par.first->GetImePrezime() << " sa prosjekom " << par.second << endl;
	else
		cout << "Nema aktivnosti";

	/*U fajl (npr. Gimnazija.txt) upisati podatke (podatke upisati kao obicni tekst) o skoli i svim ucenicima.
	Nakon upisa, potrebno je ispisati sadrzaj fajla. Parametar tipa bool oznacava da li ce ranije dodani sadrzaj fajla prethodno biti pobrisan*/

	if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt"))
		cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;
	if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt", true))
		cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;

	cin.get();
	system("pause>0");
	return 0;
}
