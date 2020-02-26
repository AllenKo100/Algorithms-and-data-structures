#include <iostream>

template <typename Tip>
class Lista
{
public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const=0;
    virtual Tip& trenutni()=0;
    virtual Tip trenutni() const=0;
    virtual bool prethodni()=0;
    virtual bool sljedeci()=0;
    virtual void pocetak()=0;
    virtual void kraj()=0;
    virtual void obrisi()=0;
    virtual void dodajIspred(const Tip& el)=0;
    virtual void dodajIza(const Tip& el)=0;
    virtual Tip& operator[](int n)=0;
};

template <typename Tip>
struct Cvor {
    Tip x;
    Cvor* sljedeci;
    Cvor* prethodni;
};

template <typename Tip>
class DvostrukaLista : public Lista<Tip>
{
    int velicina=0;
    Cvor<Tip>* lista=nullptr;
    Cvor<Tip>* tekuci=nullptr;
    Cvor<Tip>* zadnji=nullptr;
public:
    DvostrukaLista() {};
    ~DvostrukaLista()
    {
        while(tekuci) {
            lista->prethodni=nullptr;
            tekuci=lista->sljedeci;
            delete lista;
            lista=tekuci;
        }
        velicina=0;
    }
    DvostrukaLista(const DvostrukaLista<Tip>& dlista)
    {
        velicina=dlista.velicina;
        if(dlista.lista) {
            Cvor<Tip>* pomocni=dlista.lista;
            Cvor<Tip>* tmp;

            for(; pomocni; pomocni=pomocni->sljedeci) {
                if(dlista.lista==pomocni) {
                    lista=new Cvor<Tip> {pomocni->x,nullptr,nullptr};
                    tekuci=lista;
                    tmp=tekuci;
                    if(dlista.zadnji==pomocni) zadnji=tekuci;
                } else {
                    tekuci->sljedeci=new Cvor<Tip> {pomocni->x, nullptr, tekuci};
                    tekuci=tekuci->sljedeci;
                    if(dlista.tekuci==pomocni) tmp=tekuci;
                    if(dlista.zadnji==pomocni) zadnji=tekuci;
                }
            }
            tekuci=tmp;
            velicina=dlista.velicina;
        } else {
            lista=nullptr;
            tekuci=nullptr;
            velicina=0;
        }
    }
    DvostrukaLista& operator=(const DvostrukaLista<Tip> &dlista)
    {
        if(&dlista==this) return *this;
        DvostrukaLista<Tip> l(dlista);
        int br_elem2=velicina;
        this->velicina=l.velicina;
        l.velicina=br_elem2;
        Cvor<Tip>* zamjenski=lista;
        lista=l.lista;
        l.lista=zamjenski;
        zamjenski=tekuci;
        tekuci=l.tekuci;
        l.tekuci=zamjenski;
        zamjenski=zadnji;
        zadnji=l.zadnji;
        l.zadnji=zamjenski;

        return *this;
    }
    int brojElemenata() const
    {
        return velicina;
    }
    Tip trenutni() const
    {
        if(velicina==0) throw std::domain_error("Lista je prazna");
        return tekuci->x;
    }
    Tip& trenutni()
    {
        if(velicina==0) throw std::domain_error("Lista je prazna");
        return tekuci->x;
    }
    bool sljedeci()
    {
        if(velicina==0) throw std::domain_error("Lista je prazna");
        if(tekuci->sljedeci==nullptr) return false;
        tekuci=tekuci->sljedeci;
        return true;
    }
    bool prethodni()
    {
        if(velicina==0) throw std::domain_error("Lista je prazna");
        if(tekuci==lista) return false;
        tekuci=tekuci->prethodni;
        return true;
    }
    void pocetak()
    {
        if(velicina==0) throw std::domain_error("Lista je prazna");
        tekuci=lista;
    }
    void kraj()
    {
        if(velicina==0) {
            throw std::domain_error("Lista je prazna");
        }
        tekuci=zadnji;
    }
    void obrisi()
    {
        if(velicina==0) throw std::domain_error("Lista je prazna");
        if(tekuci==lista) {
            if(velicina==1) {
                lista=nullptr;
                delete tekuci;
                tekuci=nullptr;
                zadnji=nullptr;
                velicina--;
            } else {
                lista=tekuci->sljedeci;
                delete tekuci;
                tekuci=lista;
                velicina--;
            }
            return;
        }
        Cvor<Tip>* pomocni1=tekuci->prethodni;
        pomocni1->sljedeci=tekuci->sljedeci;

        Cvor<Tip>* pomocni2=tekuci->sljedeci;
        if(pomocni2) {
            pomocni2->prethodni=pomocni1;
        }
        delete tekuci;
        tekuci=pomocni1->sljedeci;
        if(tekuci==nullptr) {
            tekuci=pomocni1;
            zadnji=pomocni1;
        }
        velicina--;
    }
    void dodajIspred(const Tip& el)
    {
        if(velicina==0) {
            lista=new Cvor<Tip> {el,nullptr,nullptr};
            tekuci=lista;
            zadnji=lista;
            velicina++;
        } else {
            if(tekuci==lista) {
                Cvor<Tip>* novi=new Cvor<Tip> {el,tekuci,nullptr};
                lista->prethodni=novi;
                lista=novi;
                velicina++;
            } else {
                Cvor<Tip>* novi=new Cvor<Tip> {el,tekuci,tekuci->prethodni};
                tekuci->prethodni->sljedeci=novi;
                tekuci->prethodni=novi;
                velicina++;
            }
        }
    }
    void dodajIza(const Tip& el)
    {
        Cvor<Tip>* novi=new Cvor<Tip> {el,nullptr,nullptr};
        if(velicina==0) {
            lista=novi;
            tekuci=novi;
            zadnji=novi;
        } else {
            novi->prethodni=tekuci;
            novi->sljedeci=tekuci->sljedeci;
            if(tekuci->sljedeci) {
                tekuci->sljedeci->prethodni=novi;
            }
            if(novi->sljedeci==nullptr) {
                zadnji=novi;
            }
            tekuci->sljedeci=novi;
        }
        velicina++;
    }
    Tip& operator[](int n)
    {
        if(n<0 || n>=velicina) throw std::range_error("Neispravan indeks");
        Cvor<Tip>* pomocni=lista;
        for(int i=0; i<n; i++) {
            pomocni=pomocni->sljedeci;
        }
        return pomocni->x;
    }
    Tip& operator[](int n)const
    {
        if(n>0 || n>=velicina) throw std::range_error("Neispravan indeks");
        Cvor<Tip>* pomocni=lista;
        for(int i=0; i<n; i++) {
            pomocni=pomocni->sljedeci;
        }
        return pomocni->x;
    }
};

template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n)
{
    DvostrukaLista<Tip>pom(*((DvostrukaLista<Tip>*)(&n)));

    if(pom.brojElemenata()==0) {
        throw std::domain_error("Lista nema nijedan element!");
    }

    pom.pocetak();
    Tip maksimum=pom.trenutni();

    for(int i=0; i<pom.brojElemenata(); i++) {
        if(pom.trenutni()>maksimum) {
            maksimum=pom.trenutni();
        }
        pom.sljedeci();
    }
    /*pom.kraj();
    if(pom.trenutni()>maksimum){
        maksimum=pom.trenutni();
    }*/

    return maksimum;
}

bool Test1(){           //DvostrukaLista: testiranje nekih jednostavnih metoda tipa brojElemenata, dodajIspred, trenutni, te redoslijed clanova u listi
    std::cout << "DvostrukaLista TEST 1" << std::endl;
    DvostrukaLista<int> dlista1;
    std::cout << "Trenutni broj elemenata, treba ispisati: 0, a nas ispis je: " << dlista1.brojElemenata() << std::endl;
    if(dlista1.brojElemenata()!=0) return false;
    
    dlista1.dodajIspred(1);
    dlista1.dodajIspred(2);
    dlista1.dodajIspred(3);
    std::cout << "Trenutni element, treba ispisati: 1, a nas ispis je: " << dlista1.trenutni() << std::endl;
    if(dlista1.trenutni()!=1) return false;
    std::cout << "Broj elemenata nakon dodavanja: 3, a nas ispis je: " << dlista1.brojElemenata() << std::endl;
    if(dlista1.brojElemenata()!=3) return false;
    
    return true;
}

bool TestDajMaksimum(){
    std::cout << "DvostrukaLista TEST dajMaksimum" << std::endl;
    DvostrukaLista<int> dlista;
    for(int i=5; i<100; i+=5){
        dlista.dodajIza(i);
    }
    
    std::cout << "Maksimalni element u listi je 95, a nas ispis je: " << dajMaksimum(dlista) << std::endl;
    if(dajMaksimum(dlista)!=95) return false;
    
    return true;
}

bool TestObrisi(){
    std::cout << "Testiranje metode obrisi" << std::endl;
    DvostrukaLista<int> dlista;
    for(int i=1; i<=100; i++){
        dlista.dodajIza(i);
    }
    std::cout << "Broj elemenata liste je 100, nas ispis je: " << dlista.brojElemenata() << std::endl;
    if(dlista.brojElemenata()!=100) return false;
    
    dlista.obrisi();
    std::cout << "Nakon brisanja elementa liste, broj elemenata je 99, a nas ispis je: " << dlista.brojElemenata() << std::endl;
    if(dlista.brojElemenata()!=99) return false;
    
    return true;
}

bool TestOperatorZagrade(){
    DvostrukaLista<int> dlista;
    for(int i=1; i<=100; i++){
        dlista.dodajIza(i);
        dlista.sljedeci();
    }
    
    std::cout << "Element na indeksu 0 je 1, a nas ispis je: " << dlista[0] << std::endl;
    if(dlista[0]!=1) return false;
    
    std::cout << "Element na indeksu 99 je 100, a nas ispis je: " << dlista[99] << std::endl;
    if(dlista[99]!=100) return false;
    
    return true;
}

bool TestPocetak(){
    DvostrukaLista<int> dlista;
    for(int i=1; i<=100; i++){
        dlista.dodajIza(i);
        dlista.sljedeci();
    }
    
    dlista.pocetak();
    std::cout << "Nakon primjene metode pocetak, trenutni element je 1, a nas ispis je " << dlista.trenutni() << std::endl;
    if(dlista.trenutni()!=1) return false;
    
    return true;
}

bool TestKraj(){
    DvostrukaLista<int> dlista;
    for(int i=1; i<=50; i++){
        dlista.dodajIza(i);
        dlista.sljedeci();
    }
    
    dlista.kraj();
    std::cout << "Nakon primjene metode pocetak, trenutni element je 50, a nas ispis je " << dlista.trenutni() << std::endl;
    if(dlista.trenutni()!=50) return false;
    
    return true;
}

bool TestTrenutni(){
    DvostrukaLista<int> dlista;
    for(int i=1; i<=20; i++){
        dlista.dodajIza(i);
        dlista.sljedeci();
    }
    
    dlista.kraj();
    std::cout << "Nakon primjene metode trenutni, trenutni element je 20, a nas ispis je " << dlista.trenutni() << std::endl;
    if(dlista.trenutni()!=20) return false;
    
    return true;
}

bool TestOperatorDodjele(){
    DvostrukaLista<int> dlista;
    for(int i=1; i<=60; i++){
        dlista.dodajIza(i);
        dlista.sljedeci();
    }
    
    DvostrukaLista<int> dlista2=dlista;
    
    std::cout << "Nakon primjene metode trenutni, trenutni element je 60, a nas ispis je " << dlista2.trenutni() << std::endl;
    if(dlista2.trenutni()!=60) return false;
    
    return true;
}

bool TestKonstruktor(){
    DvostrukaLista<int> dlista;
    for(int i=1; i<=10; i++){
        dlista.dodajIza(i);
        dlista.sljedeci();
    }
    
    DvostrukaLista<int> dlista2(dlista);
    
    std::cout << "Nakon primjene metode trenutni, trenutni element je 10, a nas ispis je " << dlista2.trenutni() << std::endl;
    if(dlista2.trenutni()!=10) return false;
    
    return true;
}

int main()
{
    if(Test1() && TestDajMaksimum() && TestObrisi() && TestOperatorZagrade() && TestPocetak()
        && TestKraj() && TestTrenutni() && TestOperatorDodjele() && TestKonstruktor()){
        std::cout << "Prosli su svi testovi!" << std::endl;
    }
    else{
        std::cout << "Neki testovi nisu prosli!" << std::endl;
    }
    return 0;
}