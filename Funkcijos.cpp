#include "Mylib.h"

int uzkl_iv = 0;
int uzkl_paz = 0;
int uzkl_spausd = 0;
int uzkl_vykd = 0;

void testuoti() {
	if (uzkl_vykd != 1) {		//jei vartotojas nera pasirinkes testi programos su tomis paciomis parinktimis
		menu({ "ivesti duomenis patiems", "ivesti duomenis is failo", "generuoti duomenis atsitiktinai" }, uzkl_iv);
		menu({ "skaiciuoti galutini pazymi pagal vidurki", "skaiciuoti galutini pazymi pagal mediana" }, uzkl_paz);
		menu({ "rezultatus isvesti i faila", "rezultatus isvesti i konsole" }, uzkl_spausd);
	}

	Timer t;
	vector<studentas> grupe;
	string pavad;
	ifstream sar;
	vector<string> failai;
	int uzkl_fail = -1;
	studentas temp;
	switch (uzkl_iv) {
	case 1:		//Ranka daromas studentu duomenu ivedimas
		do {
			pildConsole(temp);
			grupe.push_back(temp);
			menu({ "ivesti dar vieno studento duomenis", "baigti ivedima" }, uzkl_iv);
		} while (uzkl_iv != 2);
		break;
	case 2:		//Duomenu ivedimas is failo
		cout << endl;
		cout << "Iveskite norimo failo nr: " << endl;
		system("dir /b *.txt >sar.txt");
		sar.open("sar.txt");
		while (!sar.eof()) {
			if (sar >> pavad && pavad != "vargsai.txt" && pavad != "kieti.txt" && pavad != "sar.txt") failai.push_back(pavad);
		}
		menu(failai, uzkl_fail);
		pavad = failai.at(--uzkl_fail);
		sar.close();
		system("del sar.txt");
		cout << endl;

		do {
			t.reset();
			try {
				pildFailas(grupe, pavad);
			}
			catch (std::runtime_error& e) {
				cout << e.what();
			}
		} while (grupe.empty());

		//pildFailas(grupe, pavad);
		break;
	case 3:		//Atsitiktinai daromas studentu duomenu ivedimas
		pildRandom(grupe);
		break;
	default:
		break;
	}

	spausdinti(grupe);
	string line1(40, '_'), line2(16, '_');
	cout << "|" << line1 << "|" << line2 << "|" << endl;
	if (uzkl_iv == 2 && uzkl_spausd == 1) spausdLaikas("Visas programos vykdymo laikas:", t.elapsed());
	cout << "|" << line1 << "|" << line2 << "|" << endl;
}

//naujo duomenu failo generavimas
void generuoti() {
	int n;
	int m;
	string pavad;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 10);

	intUzklausa("irasu skaicius", n);
	intUzklausa("pazymiu skaicius", m);
	pavad = "stud" + to_string(n) + ".txt";
	ofstream rf(pavad);

	Timer t;
	rf << left << setw(16) << "Vardas" << setw(16) << "Pavarde" << right;
	for (int i = 1; i <= m; i++) rf << setw(5) << "ND" + to_string(i);
	rf << setw(12) << "Egzaminas\n";
	for (int i = 1; i <= n; i++) {
		rf << left << setw(16) << "Vardas" + to_string(i) << setw(16) << "Pavarde" + to_string(i) << right;
		for (int j = 0; j < m; j++) rf << setw(5) << dist(mt);
		rf << setw(8) << dist(mt) << "\n";
	}
	rf.close();
	cout << "Failas pavadinimu " << pavad << " sugeneruotas per " << t.elapsed() << "s" << endl;
}

void menu(vector<string> tekstas, int& uzkl) {
	bool geraIvestis = false;
	int n = 1;
	cout << endl << "Galimos komandos:" << endl;
	for (auto& i : tekstas) {
		cout << n << " - " << i << endl;
		n++;
	}
	do {
		cout << "Iveskite norimos komandos numeri: ";
		cin >> uzkl;
		for (int i = 1; i <= tekstas.size(); i++)
			if (uzkl == i) {
				geraIvestis = true;
				break;
			}
		if (!cin) {
			cin.clear();
		}
		cin.ignore(10000, '\n');
	} while (!geraIvestis);
}

void intUzklausa(string kint_pavad, int& n) {
	while (true) {
		try {
			cout << "Koks yra " << kint_pavad << "? ";
			cin >> n;
			if (!cin || n < 1) {
				throw std::runtime_error("Bloga ivestis, " + kint_pavad + " turi buti naturalusis skaicius. \n");
			}
			else break;
		}
		catch (std::runtime_error& e) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << e.what();
		}
	}
	cin.ignore(10000, '\n');
}

//Galutinio pazymio skaiciavimas
double pazym(studentas temp, double x) {
	return 0.4 * x + 0.6 * temp.egz;
}

//Pazymiu medianos skaiciavimas
double med(studentas temp) {
	sort(temp.paz.begin(), temp.paz.end());
	int n = temp.paz.size();
	if (n % 2 != 0) return double(temp.paz.at(size_t(n) / 2));
	else return double(temp.paz.at((size_t(n) - 1) / 2) + temp.paz.at(size_t(n) / 2)) / 2;
}


bool palygVard(studentas& t1, studentas& t2) {
	return (t1.vardas < t2.vardas) ? true : (t1.pavarde < t2.pavarde);
}

bool palygPaz(studentas& t1, studentas& t2) {
	return t1.galPaz < t2.galPaz;
}

bool palygGal(const studentas& stud, const double& x) {
	return stud.galPaz < x;
}

std::ostream& operator<< (std::ostream& out, studentas& a) {
	out << left << setw(15) << a.vardas << setw(15) << a.pavarde << fixed << setprecision(2) << a.galPaz; 
	return out;
}