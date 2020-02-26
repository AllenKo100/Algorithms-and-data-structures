#include <iostream>
#include <stdexcept>

template <typename Tip>
class Lista
{
public:
    Lista() {};
    virtual ~Lista() {};
    virtual int brojElemenata() const=0;
    virtual Tip& trenutni()=0;
    virtual Tip trenutni() const=0;
    virtual bool prethodni()=0;
    virtual bool sljedeci()=0;
    virtual void pocetak()=0;
    virtual void kraj()=0;
    virtual void obrisi()=0;
    virtual void dodajIspred(const Tip& x)=0;
    virtual void dodajIza(const Tip& x)=0;
    virtual Tip& operator[](int n)=0;
};

template <typename Tip>
class NizLista : public Lista<Tip>
{
    int velicina=0;
    int kapacitet=10;
    int tekuci=0;
    Tip** niz;
public:
    NizLista()
    {
        velicina=0;
        tekuci=0;
        kapacitet=10;
        niz=new Tip*[10] {};
    }
    ~NizLista()
    {
        for(int i=0; i<velicina; i++) {
            delete niz[i];
        }
        delete[] niz;
    }
    NizLista(const NizLista<Tip>& niz2)
    {
        this->niz=new Tip*[niz2.kapacitet] {};
        for(int i=0; i<niz2.velicina; i++) {
            this->niz[i]=new Tip(*niz2.niz[i]);
        }
        this->velicina=niz2.velicina;
        this->kapacitet=niz2.kapacitet;
        this->tekuci=niz2.tekuci;
    }
    NizLista& operator=(const NizLista<Tip> &lista)
    {
        if(&lista==this) return *this;
        if(kapacitet<lista.kapacitet) {
            for(int i=0; i<kapacitet; i++) {
                delete niz[i];
            }
            delete[] niz;
            niz=new Tip*[lista.kapacitet] {};
            kapacitet=lista.kapacitet;
            tekuci=lista.tekuci;
        }
        for(int i=0; i<velicina; i++) {
            delete niz[i];
        }
        velicina=lista.velicina;
        for(int i=0; i<velicina; i++) {
            niz[i]=new Tip(*lista.niz[i]);
        }
        return *this;
    }
    int brojElemenata() const
    {
        return velicina;
    }
    Tip& trenutni()
    {
        if(velicina==0) throw std::logic_error("Prazna lista!");
        return *niz[tekuci];
    }
    Tip trenutni() const
    {
        if(velicina==0) throw std::logic_error("Prazna lista!");
        return *niz[tekuci];
    }
    bool prethodni()
    {
        if(velicina==0) return false;
        if(tekuci==0) return false;
        tekuci--;
        return true;;
    }
    bool sljedeci()
    {
        if(velicina==0) return false;
        if(tekuci==velicina-1) return false;
        tekuci++;
        return true;
    }
    void pocetak()
    {
        if(velicina==0) throw std::domain_error("Lista je prazna");
        tekuci=0;
    }
    void kraj()
    {
        if(velicina==0) tekuci=0;
        tekuci=velicina-1;
    }
    void obrisi()
    {
        if(velicina==0) return;
        if(tekuci==velicina-1 && tekuci!=0) {
            delete niz[tekuci];
            velicina--;
            niz[tekuci]=nullptr;
            tekuci--;
            return;
        }

        delete niz[tekuci];
        niz[tekuci]=nullptr;
        for(int i=tekuci; i<velicina; i++) {
            niz[i]=niz[i+1];
        }
        velicina--;
    }
    void dodajIspred(const Tip& x)
    {
        if(velicina==0) {
            niz[0]=new Tip(x);
            velicina++;
            return;
        }
        if(kapacitet<velicina+1) {
            Tip** pomocni=niz;
            niz=new Tip*[kapacitet+kapacitet] {};

            for(int i=0; i<velicina; i++) {
                niz[i]=new Tip(*pomocni[i]);
            }
            for(int i=0; i<velicina; i++) {
                delete pomocni[i];
            }
            delete[] pomocni;
            kapacitet=kapacitet+int(kapacitet/2);
        }
        if(tekuci==0) {
            for(int i=velicina-1; i<velicina; i++) {
                niz[i+1]=niz[i];
            }
            niz[0]=new Tip(x);
            velicina++;
            tekuci++;
            return;
        }
        for(int i=velicina-1; i>=tekuci; i--) {
            niz[i+1]=niz[i];
        }
        niz[tekuci]=new Tip(x);
        tekuci++;
        velicina++;
    }
    void dodajIza(const Tip& x)
    {
        if(velicina==0) {
            niz[0]=new Tip(x);
            velicina++;
            return;
        }
        if(kapacitet<velicina+1) {
            Tip** pomocni=niz;
            niz=new Tip*[kapacitet+int(kapacitet/2)] {};
            for(int i=0; i<velicina; i++) {
                niz[i]= new Tip(*pomocni[i]);
            }
            for(int i=0; i<velicina; i++) {
                delete pomocni[i];
            }
            delete[] pomocni;
            kapacitet=kapacitet+int(kapacitet/2);
        }
        for(int i=velicina-1; i>tekuci; i--) {
            niz[i+1]=niz[i];
        }
        niz[tekuci+1]=new Tip(x);
        velicina++;
    }
    Tip& operator[](int indeks)
    {
        if(indeks<0 || indeks>velicina-1) throw std::logic_error("Neispravan indeks");
        return *niz[indeks];
    }
    Tip operator[](int indeks) const
    {
        if(indeks<0 || indeks>velicina-1) throw std::logic_error("Neispravan indeks");
        return *niz[indeks];
    }
};

template <typename Tip>
struct Cvor {
    Tip x;
    Cvor* next;
};

template <typename Tip>
class JednostrukaLista : public Lista<Tip>
{
    int velicina=0;
    Cvor<Tip>* lista=nullptr;
    Cvor<Tip>* tekuci=nullptr;
    Cvor<Tip>* end=nullptr;

public:
    JednostrukaLista()
    {
        lista=nullptr;
        tekuci=nullptr;
        end=nullptr;
    }
    JednostrukaLista(const JednostrukaLista<Tip>& l)
    {
        velicina=l.velicina;
        if(l.lista) {
            Cvor<Tip>* klizni=l.lista, *temp;
            for(; klizni; klizni=klizni->next) {
                if(l.lista==klizni) {
                    lista=new Cvor<Tip> {klizni->x, nullptr};
                    tekuci=lista;
                    if(klizni==l.tekuci) temp=lista;
                } else {
                    if(klizni==l.tekuci) temp=tekuci;
                    tekuci->next=new Cvor<Tip> {klizni->x, nullptr};
                    tekuci=tekuci->next;
                    if(l.end==klizni) {
                        end=tekuci;
                    }
                }
            }
            velicina=l.velicina;
        } else {
            lista=nullptr;
            tekuci=nullptr;
            end=nullptr;
            velicina=0;
        }
    }
    JednostrukaLista& operator=(const JednostrukaLista<Tip> &l)
    {
        if(this==&l) return *this;
        while(tekuci) {
            tekuci=lista->next;
            delete lista;
            lista=tekuci;
        }
        end=nullptr;
        velicina=0;

        velicina=l.velicina;
        if(l.lista) {
            Cvor<Tip>* klizni=l.lista, *temp;
            for(; klizni; klizni=klizni->next) {
                if(l.lista==klizni) {
                    lista=new Cvor<Tip> {klizni->x, nullptr};
                    tekuci=lista;
                    if(klizni==l.tekuci) temp=lista;
                } else {
                    if(klizni==l.tekuci) temp=tekuci;
                    tekuci->next=new Cvor<Tip> {klizni->x, nullptr};
                    tekuci=tekuci->next;
                    if(l.end==klizni) {
                        end=tekuci;
                    }
                }
            }
            velicina=l.velicina;
        } else {
            lista=nullptr;
            tekuci=nullptr;
            end=nullptr;
            velicina=0;
        }
        return *this;
    }
    ~JednostrukaLista()
    {
        while(tekuci) {
            tekuci=lista->next;
            delete lista;
            lista=tekuci;
        }
        end=nullptr;
        velicina=0;
    }
    int brojElemenata() const
    {
        return velicina;
    }
    Tip trenutni() const
    {
        if(velicina==0) throw "Lista je prazna !";
        return tekuci->x;
    }
    Tip &trenutni()
    {
        if(velicina==0) throw "Lista je prazna !";
        return tekuci->x;
    }
    bool sljedeci()
    {
        if(velicina==0) throw "Lista je prazna !";
        if(tekuci->next==nullptr) return false;
        tekuci=tekuci->next;
        return true;
    }
    bool prethodni()
    {
        if(velicina==0) throw "Lista je prazna !";
        if(tekuci==lista) return false;
        Cvor<Tip>* klizni=lista;
        for(; klizni->next!=tekuci; klizni=klizni->next);
        tekuci=klizni;
        return true;
    }
    void pocetak()
    {
        if(velicina==0) throw "Lista je prazna !";
        tekuci=lista;
    }
    void kraj()
    {
        if(velicina==0) throw "Lista je prazna !";
        if(lista->next==nullptr) {
            tekuci=lista;
            return;
        }

        for(; tekuci->next != nullptr; tekuci=tekuci->next);
    }
    void obrisi()
    {
        if(velicina==0) throw "Lista je prazna !";
        if(tekuci==lista) {
            if(velicina==1) {
                lista=nullptr;
                delete tekuci;
                tekuci=nullptr;
                velicina--;
                end=nullptr;
            } else {
                lista=tekuci->next;
                delete tekuci;
                tekuci=lista;
                velicina--;
            }
            return;
        }
        Cvor<Tip> *klizni=lista;
        for(; klizni->next!=tekuci; klizni=klizni->next);
        klizni->next=tekuci->next;
        delete tekuci;
        tekuci=klizni->next;
        if(tekuci==nullptr) {
            tekuci=klizni;
            end=klizni;
        }
        velicina--;
    }
    void dodajIspred(const Tip& x)
    {
        if(velicina==0) {
            lista=new Cvor<Tip> {x, nullptr};
            tekuci=lista;
            velicina++;
        } else {
            if(tekuci==lista) {
                Cvor<Tip>* novi=new Cvor<Tip> {x,tekuci};
                lista=novi;
                velicina++;
            } else {
                Cvor<Tip>* klizni=lista;
                for(;;) {
                    if(klizni->next==tekuci) break;
                    klizni=klizni->next;
                }
                Cvor<Tip>* novi=new Cvor<Tip> {x,tekuci};
                klizni->next=novi;
                velicina++;
            }
        }
    }
    void dodajIza(const Tip& x)
    {
        Cvor<Tip>* novi=new Cvor<Tip> {x,nullptr};
        if(velicina==0) {
            lista=novi;
            tekuci=novi;
            end=novi;
        } else {
            novi->next=tekuci->next;
            tekuci->next=novi;
            if(tekuci==end) {
                end=tekuci->next;
            }
        }
        velicina++;
    }
    const Tip& operator[](int n) const
    {
        if(n>=velicina && n<0) throw "Neispravan indeks !";
        Cvor<Tip>* klizni=lista;
        for(int i=0; i<n; i++) {
            klizni=klizni->next;
        }
        return klizni->x;
    }
    Tip& operator[](int n)
    {
        if(n>=velicina && n<0) throw "Neispravan indeks !";
        Cvor<Tip>* klizni=lista;
        for(int i=0; i<n; i++) {
            klizni=klizni->next;
        }
        return klizni->x;
    }
};

void Test1()                        //NizLista: testiranje nekih jednostavnih metoda tipa brojElemenata, dodajIspred, trenutni, te redoslijed clanova u listi
{
    std::cout << "TEST1:" << std::endl;
    NizLista<int> lista1;
    std::cout << "Tremutni broj elemenata, treba ispisati: 0, a nas ispis je: " << lista1.brojElemenata() << std::endl;
    lista1.dodajIspred(10);
    lista1.dodajIspred(20);
    lista1.dodajIspred(30);
    std::cout << "Trenutni element, treba ispisat: 10, a nas ispis je: " << lista1.trenutni() << std::endl;
    std::cout << "Broj elemenata nakon dodavanja: 3, a nas ispis je: " << lista1.brojElemenata() << std::endl;

    std::cout << "Trenutni elementi u listi su: 20 30 10, a nas ispis je: ";
    for(int i=0; i<lista1.brojElemenata(); i++) {
        std::cout << lista1[i] << " ";
    }
}

void Test2()                        //NizLista: testiranje dodajIza, sljedeci, te konstruktora i operatora dodjele
{
    std::cout << "TEST2:" << std::endl;
    NizLista<int> lista1;
    for(int i=1; i<=10; i++) {
        lista1.dodajIza(i);
        lista1.sljedeci();
    }
    std::cout << "Ispis bi trebao biti: 1 2 3 4 5 6 7 8 9 10, a nas ispis je: ";
    for(int i=0; i<10; i++) {
        std::cout << lista1[i] << " ";
    }
    std::cout << std::endl;

    NizLista<int> lista2=lista1;
    std::cout << "Ispis bi trebao biti: 1 2 3 4 5 6 7 8 9 10, a nas ispis nakon operatora dodjele je: ";
    for(int i=0; i<10; i++) {
        std::cout << lista2[i] << " ";
    }
    std::cout << std::endl;
}

void Test3()                        //NizLista: testiranje metode obrisi, te metode trenutni nakon primjene obrisi, pocetak i kraj
{
    std::cout << "TEST3:" << std::endl;
    NizLista<int> lista1;
    for(int i=1; i<=10; i++) {
        lista1.dodajIza(i);
    }
    std::cout << "Broj elemenata je: 10, a nas ispis je: " << lista1.brojElemenata() << std::endl;
    lista1.obrisi();
    std::cout << "Broj elemenata je: 9, a nas ispis je: " << lista1.brojElemenata() << std::endl;
    lista1.obrisi();
    lista1.obrisi();
    lista1.obrisi();
    std::cout << "Broj elemenata je: 6, a nas ispis je: " << lista1.brojElemenata() << std::endl;
    lista1.kraj();
    lista1.obrisi();
    std::cout << "Trenutni element je: 3, a nas ispis je: " << lista1.trenutni();
    std::cout << std::endl;
    lista1.pocetak();
    std::cout << "Trenutni element je: 7, a nas ispis je: " << lista1.trenutni() << std::endl;
}

void Test4()                        //NizLista: testiranje prethodni i sljedeci na vrijednost koju vracaju
{
    std::cout << "TEST4:" << std::endl;
    NizLista<int> lista1;
    for(int i=1; i<=10; i++) {
        lista1.dodajIza(i);
    }
    lista1.pocetak();
    bool test=lista1.prethodni();
    if(test==false) std::cout << "TACNO! Prvi element je trenutni tako da funkcija prethodni vraca false" << std::endl;
    lista1.kraj();
    test=lista1.sljedeci();
    if(test==false) std::cout << "TACNO! Posljednji element je trenutni tako da funkcija prethodni vraca false" << std::endl;
}

void Test5()                        //JednostrukaLista: testiranje nekih jednsotavnih metoda tipa brojElemenata, dodajIspred, trenutni, te redoslijed clanova u listi
{
    std::cout << "TEST5:" << std::endl;
    JednostrukaLista<int> lista1;
    std::cout << "Trenutni broj elemenata, treba ispisati: 0, a nas ispis je: " << lista1.brojElemenata() << std::endl;
    lista1.dodajIspred(10);
    lista1.dodajIspred(20);
    lista1.dodajIspred(30);
    std::cout << "Trenutni element, treba ispisat: 10, a nas ispis je: " << lista1.trenutni() << std::endl;
    std::cout << "Broj elemenata nakon dodavanja: 3, a nas ispis je: " << lista1.brojElemenata() << std::endl;

    std::cout << "Trenutni elementi u listi su: 20 30 10, a nas ispis je: ";
    for(int i=0; i<lista1.brojElemenata(); i++) {
        std::cout << lista1[i] << " ";
    }
}

void Test6()                        //JednostrukaLista: testiranje dodajIza, sljedeci, te konstruktora i operatora dodjele
{
    std::cout << "TEST6:" << std::endl;
    JednostrukaLista<int> lista1;
    for(int i=1; i<=10; i++) {
        lista1.dodajIza(i);
        lista1.sljedeci();
    }
    std::cout << "Ispis bi trebao biti: 1 2 3 4 5 6 7 8 9 10, a nas ispis je: ";
    for(int i=0; i<10; i++) {
        std::cout << lista1[i] << " ";
    }
    std::cout << std::endl;

    JednostrukaLista<int> lista2=lista1;
    std::cout << "Ispis bi trebao biti: 1 2 3 4 5 6 7 8 9 10, a nas ispis nakon operatora dodjele je: ";
    for(int i=0; i<10; i++) {
        std::cout << lista2[i] << " ";
    }
    std::cout << std::endl;
}

void Test7()                        //JednostrukaLista: testiranje metode obrisi, te metode trenutni nakon primjene obrisi, pocetak i kraj
{
    std::cout << "TEST7:" << std::endl;
    JednostrukaLista<int> lista1;
    for(int i=1; i<=6; i++) {
        lista1.dodajIza(i);
    }
    std::cout << "Broj elemenata je: 6, a nas ispis je: " << lista1.brojElemenata() << std::endl;
    lista1.obrisi();
    std::cout << "Broj elemenata je: 5, a nas ispis je: " << lista1.brojElemenata() << std::endl;
    lista1.obrisi();
    lista1.obrisi();
    lista1.obrisi();
    std::cout << "Broj elemenata je: 2, a nas ispis je: " << lista1.brojElemenata() << std::endl;
    lista1.kraj();
    lista1.obrisi();
    std::cout << "Trenutni element je: 3, a nas ispis je: " << lista1.trenutni();
    std::cout << std::endl;
    lista1.kraj();
    std::cout << "Trenutni element je: 3, a nas ispis je: " << lista1.trenutni() << std::endl;
}

void Test8()                        //JednostrukaLista: testiranje prethodni i sljedeci na vrijednost koju vracaju
{
    std::cout << "TEST8:" << std::endl;
    JednostrukaLista<int> lista1;
    for(int i=1; i<=10; i++) {
        lista1.dodajIza(i);
    }
    lista1.pocetak();
    bool test=lista1.prethodni();
    if(test==false) std::cout << "TACNO! Prvi element je trenutni tako da funkcija prethodni vraca false" << std::endl;
    lista1.kraj();
    test=lista1.sljedeci();
    if(test==false) std::cout << "TACNO! Posljednji element je trenutni tako da funkcija prethodni vraca false" << std::endl;
}

int main()
{
    std::cout << "U nastavku slijedi nekoliko testova za metode klasa NizLista i JednostrukaLista: " << std::endl;
    //testovi za NizLista
    Test1();
    std::cout << std::endl;
    Test2();
    std::cout << std::endl;
    Test3();
    std::cout << std::endl;
    Test4();
    std::cout << std::endl;
    //testovi za JednostrukaLista
    Test5();
    std::cout << std::endl;
    Test6();
    std::cout << std::endl;
    Test7();
    std::cout << std::endl;
    Test8();
    std::cout << std::endl;
    return 0;
}
