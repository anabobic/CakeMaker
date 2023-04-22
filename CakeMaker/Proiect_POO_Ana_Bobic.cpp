#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <Windows.h>

using namespace std;

class Reteta {

private:
    string nume;
    int time;

public:
    Reteta() {}
    Reteta(string nume, int time)
    {
        this->nume = nume;
        this->time = time;
    }

    string get_nume()
    {
        return this->nume;
    }

    int get_time()
    {
        return this->time;
    }

};

class Prajitura {
private:
    string nume;
    Reteta reteta;
public:

    Prajitura(string nume, Reteta reteta)
    {
        this->nume = nume;
        this->reteta = reteta;
    }

    void afisare_p() {
        cout << this->nume << endl;
    }

    string get_nume()
    {
        return this->nume;
    }

    Reteta get_reteta()
    {
        return this->reteta;
    }
    virtual void afisare() {
        this->afisare_p();
    }
    virtual bool este_pentru_diabetici() {
        return false;
    }
};



class NormalPrajitura : public Prajitura {
public:
    NormalPrajitura(string nume, Reteta reteta) : Prajitura(nume, reteta) { }
    void afisare() override {
        this->afisare_p();
    }
};

class DiabeticiPrajitura : public Prajitura {
private:
    int zahar;
public:
    DiabeticiPrajitura(string name, Reteta reteta, int zahar) : Prajitura(name, reteta)
    {
        this->zahar = zahar;
    }
    int get_zahar()
    {
        return this->zahar;
    }
    void afisare() override {
        this->afisare_p();
        cout << "Cantitate de zahar: " << this->zahar << endl;
    }
    virtual bool este_pentru_diabetici()override {
        return true;
    }
};

class PrajiMaker {
public:

    Prajitura* prepar_prajitura(Reteta reteta)
    {

        Sleep(reteta.get_time() * 1000);
        Prajitura* praji;
        praji = new Prajitura(reteta.get_nume(), reteta);

        return praji;
    }

};
class Depozit {
private:
    list<Prajitura*> prajituri_depozit;
    int capacitate_maxima;
    int capacitate_curenta;

public:
    Depozit(int maxim)
    {
        this->capacitate_maxima = maxim;
        this->capacitate_curenta = 0;
    }

    void Add_Praji(Prajitura* praji)
    {
        if (capacitate_curenta < capacitate_maxima) {
            prajituri_depozit.push_back(praji);
            capacitate_curenta++;
        }
        else {
            cout << "Depozitul este plin." << endl;
        }
    }
    bool exista_in_depozit(string nume)
    {
        for (auto it = prajituri_depozit.begin(); it != prajituri_depozit.end(); it++) {
            Prajitura* iterator = *it;
            if (iterator->get_nume() == nume)
            {
                return true;
            }
        }
        return false;
    }

    void Stergere_praji(string nume)
    {
        for (auto it = prajituri_depozit.begin(); it != prajituri_depozit.end(); it++) {
            if ((*it)->get_nume() == nume) {
                prajituri_depozit.erase(it);
                capacitate_curenta--;
                return;
            }
        }

    }

    void Afisare_praji()
    {
        for (auto it = prajituri_depozit.begin(); it != prajituri_depozit.end(); it++) {
            cout << (*it)->get_nume() << " - " << (*it)->get_reteta().get_time() << " minute." << endl;
        }
        cout << endl;
    }

    int get_capacitate_maxima()
    {
        return capacitate_maxima;
    }

    int get_capacitate_curenta()
    {
        return capacitate_curenta;
    }

    list<Prajitura*> get_prajituri_depozit()
    {
        return this->prajituri_depozit;
    }

    int get_time_depozit(Prajitura* it)
    {
        return it->get_reteta().get_time();
    }

};

class Panou_control {
private:
    Depozit* depozit;
    PrajiMaker* maker;
    list<Prajitura*> prajituri;

public:
    Panou_control(Depozit* depozit, PrajiMaker* maker)
    {
        this->depozit = depozit;
        this->maker = maker;
    }
    void initializare_depozit();

    void Add_Praji(Prajitura* praji) {
        prajituri.push_back(praji);
    }

    void AfisarePrajituri()
    {
        cout << "Lista de prajiturile disponibile: " << endl;
        depozit->Afisare_praji();
    }

    void umple_depozit(string nume)
    {
        int time;
        if (depozit->get_capacitate_curenta() < depozit->get_capacitate_maxima()) {
            if (depozit->get_capacitate_curenta() != depozit->get_capacitate_maxima()) {
                for (auto it = prajituri.begin(); it != prajituri.end(); it++)
                    if ((*it)->get_nume() == nume)
                        time = depozit->get_time_depozit(*it);

                cout << "Asteptati ca depozitul sa se umple." << endl;

                while (depozit->get_capacitate_curenta() < depozit->get_capacitate_maxima()) {
                    for (auto it = prajituri.begin(); it != prajituri.end(); it++) {
                        if ((*it)->get_nume() == nume) {
                            // Cream prajitura
                            Prajitura* praji = maker->prepar_prajitura((*it)->get_reteta());

                            depozit->Add_Praji(praji);
                            return;
                        }
                    }
                    cout << "Nu s-a putut gasi o reteta pentru ultima prajitura comandata." << endl;

                }

                cout << "Depozitul a fost umplut cu ultima prajitura comandata." << endl;
            }
        }
    }
    void Comanda(string nume)
    {

        if (depozit->get_capacitate_curenta() == 0)
            umple_depozit(nume);

        if (depozit->exista_in_depozit(nume) == true) {
            depozit->Stergere_praji(nume);
            cout << "Prajitura " << nume << " a fost servita." << endl;
            if (depozit->get_capacitate_curenta() == 0)
                umple_depozit(nume);
            return;
        }

        if (depozit->get_capacitate_curenta() == depozit->get_capacitate_maxima()) cout << "Depozitul este la capacitate maxima cu alte prajituri, nu se pot prepara prajituri deocamdata, comandati prajituri din depozit. " << endl;
        else
            for (auto it = prajituri.begin(); it != prajituri.end(); it++) {
                if ((*it)->get_nume() == nume) {
                    cout << "Va rugam asteptati " << (*it)->get_reteta().get_time() << " minute pana se prepara prajitura. " << endl;
                    Prajitura* praji = maker->prepar_prajitura((*it)->get_reteta());
                    cout << "Prajitura " << nume << " a fost preparata si servita." << endl;
                    return;
                }
            }
        cout << "Prajitura " << nume << " nu se gaseste printre retete." << endl;
    }


};

void Panou_control::initializare_depozit() {
    for (auto it = prajituri.begin(); it != prajituri.end() && depozit->get_capacitate_curenta() != depozit->get_capacitate_maxima(); it++) {
        depozit->Add_Praji(*it);
    }
}

int main()
{

    list<Prajitura*> retete;
    string nume_prajitura;
    int maxim;
    string nume;
    int time;
    int opt;
    bool tip;
    int zahar;

    ifstream fin("retete.txt");

    fin >> maxim;
    Depozit* depozit_prajituri = new Depozit(maxim);
    PrajiMaker* praji_maker = new PrajiMaker();
    Panou_control* cp = new Panou_control(depozit_prajituri, praji_maker);

    while (fin >> nume >> time) {

        Reteta r(nume, time);

        fin >> tip;
        if (tip == 0)
        {
            NormalPrajitura* c = new NormalPrajitura(nume, r);
            retete.push_back(c);
            cp->Add_Praji(c);

        }
        else {
            fin >> zahar;
            DiabeticiPrajitura* dc = new DiabeticiPrajitura(nume, r, zahar);
            retete.push_back(dc);
            cp->Add_Praji(dc);
        }
    }

    cp->initializare_depozit();

    do
    {
        cout << "\n1. Afisare retete" << endl;
        cout << "2. Comanda prajitura" << endl;
        cout << "3. Afisare depozit" << endl;
        cout << "4. Iesire" << endl;

        cout << "Introduceti optiunea dorita: ";
        cin >> opt;
        try {

            switch (opt)
            {
            case 1:

                for (auto it = retete.begin(); it != retete.end(); it++) {
                    (*it)->afisare();
                }
                break;

            case 2:
                cout << "Introduceti numele prajiturii dorite: ";
                cin >> nume_prajitura;
                cp->Comanda(nume_prajitura);
                break;


            case 3:
                cp->umple_depozit(nume_prajitura);
                cp->AfisarePrajituri();
                break;

            case 4: exit(0);

            default:
                throw exception("Comanda introdusa este incorecta!");
                break;
            }
        }
        catch (exception e) {
            cout << e.what() << endl;
            ofstream log_file;
            log_file << e.what() << endl;
            log_file.close();
        }

    } while (1);
    fin.close();
    system("pause");

    return 0;
}