#include <iostream>

#include <iostream>

//Koristit cemo DvostrukuListu koju smo napravili na prosloj zadaci da bi napravili DvostraniRed

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
    const Tip& operator[](int n)const
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

template <typename Tip>
class DvostraniRed {
    DvostrukaLista<Tip> dred;
public:
    DvostraniRed() {}
    ~DvostraniRed() {}
    int brojElemenata(){
        return dred.brojElemenata();
    }
    void brisi(){
        int broj_elemenata=dred.brojElemenata();
        for(int i=0; i<broj_elemenata; i++){
            dred.obrisi();
        }
    }
    Tip& vrh(){
        if(dred.brojElemenata()==0) throw std::domain_error("DvostraniRed nema elemenata!");
        dred.kraj();
        return dred.trenutni();
    }
    Tip& celo(){
        if(dred.brojElemenata()==0) throw std::domain_error("DvostraniRed nema elemenata!");
        dred.pocetak();
        return dred.trenutni();
    }
    void staviNaVrh(const Tip &el){
        if(dred.brojElemenata()>0) dred.kraj();
        dred.dodajIza(el);
    }
    Tip skiniSaVrha(){
        dred.kraj();
        Tip element=dred.trenutni();
        dred.obrisi();
        return element;
    }
    void staviNaCelo(const Tip &el){
        if(dred.brojElemenata()>0) dred.pocetak();
        dred.dodajIspred(el);
    }
    Tip skiniSaCela(){
        dred.pocetak();
        Tip element=dred.trenutni();
        dred.obrisi();
        return element;
    }
    Tip operator[] (int i) const{
        return dred[i];
    }
    Tip& operator[] (int i){
        return dred[i];
    }
};

bool TestOperatorUglaste(){
    DvostraniRed<int> dred;
    dred.staviNaVrh(1);
    dred.staviNaVrh(4);
    dred.staviNaVrh(5);
    
    std::cout << "Element s indeksom 1 je 4, a nas ispis je: " << dred[1] << std::endl;
    if(dred[1]!=4) return false;
    
    return true;
}

bool TestBrojElemenata(){
    DvostraniRed<int> dred;
    for(int i=1; i<=10; i++){
        dred.staviNaVrh(i);
    }
    
    std::cout << "Broj elemenata je 10, a nas ispis je: " << dred.brojElemenata() << std::endl;
    if(dred.brojElemenata()!=10) return false;
    
    return true;
}

bool TestBrisi(){
    DvostraniRed<double> dred;
    for(int i=10; i<=200; i+=10){
        dred.staviNaCelo(i);
    }
    
    std::cout << "Broj elemenata je 20, a nas ispis je: " << dred.brojElemenata() << std::endl;
    if(dred.brojElemenata()!=20) return false;
    
    dred.brisi();
    std::cout << "Nakon brisanja broj elemenata je 0, a nas ispis je: " << dred.brojElemenata() << std::endl;
    if(dred.brojElemenata()!=0) return false;
    
    return true;
}

bool TestVrh(){
    DvostraniRed<int> dred;
    for(int i=2; i<=20; i+=2){
        dred.staviNaVrh(i);
    }
    
    std::cout << "Element na vrhu je 20, a nas ispis je: " << dred.vrh() << std::endl;
    if(dred.vrh()!=20) return false;
    
    return true;
}

bool TestCelo(){
    DvostraniRed<int> dred;
    for(int i=50; i<=2500; i++){
        dred.staviNaCelo(i);
    }
    
    std::cout << "Element na celu je 2500, a nas ispis je: " << dred.celo() << std::endl;
    if(dred.celo()!=2500) return false;
    
    return true;
}

bool TestStaviNaVrh(){
    DvostraniRed<int> dred;
    dred.staviNaVrh(11);
    dred.staviNaVrh(22);
    dred.staviNaVrh(33);
    dred.staviNaVrh(44);
    dred.staviNaVrh(55);
    
    if(dred.brojElemenata()!=5 && dred.vrh()!=55) return false;
    
    return true;
}

bool TestSkiniSaVrha(){
    DvostraniRed<int> dred;
    dred.staviNaVrh(99);
    dred.staviNaVrh(999);
    dred.staviNaVrh(9999);
    dred.staviNaVrh(99999);
    dred.staviNaVrh(999999);
    
    if(dred.skiniSaVrha()!=999999) return false;
    
    return true;
}

bool TestStaviNaCelo(){
    DvostraniRed<int> dred;
    dred.staviNaCelo(10);
    dred.staviNaCelo(100);
    dred.staviNaCelo(1000);
    dred.staviNaCelo(10000);
    dred.staviNaCelo(100000);
    dred.staviNaCelo(1000000);
    
    if(dred.brojElemenata()!=6 && dred.celo()!=10) return false;
    
    return true;
}

bool TestSkiniSaCela(){
    DvostraniRed<int> dred;
    dred.staviNaCelo(7);
    dred.staviNaCelo(77);
    dred.staviNaCelo(777);
    dred.staviNaCelo(7777);
    
    if(dred.skiniSaCela()!=7777) return false;
    
    return true;
}

int main() {
    if(TestOperatorUglaste() && TestBrojElemenata() && TestBrisi() && TestVrh() && TestCelo()
        && TestStaviNaVrh() && TestSkiniSaVrha() && TestStaviNaCelo() && TestSkiniSaCela()){
        std::cout << "Prosli svi testovi!" << std::endl;
    }
    else{
        std::cout << "Neki testovi nisu prosli!" << std::endl;
    }
    return 0;
}