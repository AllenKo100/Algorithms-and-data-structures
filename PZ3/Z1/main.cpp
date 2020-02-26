#include <iostream>
#include <vector>

//STEK PREKO VECTORA
/*template <typename Tip>
class Stek
{
    std::vector<Tip> stek;
public:
    Stek() {}
    void brisi()
    {
        //pozivamo funkciju clear da obrise sve elemente steka
        stek.clear();
    }
    void stavi(const Tip& el)
    {
        //ubacujemo element na vrh steka
        stek.push_back(el);
    }
    Tip skini()
    {
        if(stek.size()==0) throw std::range_error("Izuzetak");
        Tip el = stek[stek.size()-1];
        stek.pop_back();
        return el;
    }
    Tip& vrh()
    {
        //vracamo element koji smo zadnji stavili u stek
        return stek[stek.size()-1];
    }
    int brojElemenata()
    {
        return stek.size();
    }
};*/

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
            niz=new Tip*[kapacitet+int(kapacitet/2)] {};

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

//STEK PREKO IMPLEMENTIRANE KLASE NIZLISTA
/*template <typename Tip>
class Stek
{
    NizLista<Tip> stek;
public:
    Stek() {}
    void brisi()
    {
        int br_elem=stek.brojElemenata();
        for(int i=0; i<br_elem; i++) {
            this->stek.obrisi();
        }
    }
    void stavi(const Tip& el)
    {
        this->stek.kraj();
        this->stek.dodajIza(el);
    }
    Tip skini()
    {
        this->stek.kraj();
        Tip el=stek.trenutni();
        this->stek.obrisi();
        return el;
    }
    Tip& vrh()
    {
        this->stek.kraj();
        return this->stek.trenutni();
    }
    int brojElemenata()
    {
        return this->stek.brojElemenata();
    }
};*/

template <typename Tip>
class Stek{
    JednostrukaLista<Tip> stek;
public:
    Stek() {}
    void brisi(){
        int broj_elemenata=stek.brojElemenata();
        for(int i=0; i<broj_elemenata; i++){
            stek.obrisi();
        }
    }
    void stavi(const Tip& el){
        if(stek.brojElemenata()>0){
            stek.pocetak();
        }
        stek.dodajIspred(el);
    }
    Tip skini(){
        if(stek.brojElemenata()==0){
            throw std::logic_error("U steku nema elemenata!");
        }
        stek.pocetak();
        Tip element=stek.trenutni();
        stek.obrisi();
        return element;
    }
    Tip& vrh(){
        if(stek.brojElemenata()==0){
            throw std::logic_error("U steku nema elemenata!");
        }
        stek.pocetak();
        return stek.trenutni();
    }
    int brojElemenata(){
        return stek.brojElemenata();
    }
};

bool Test1(){                   //testiranje metode stavi, zajedno sa brojElemenata
    std::cout << "TEST1" << std::endl;
    Stek<int> stek;
    for(int i=1; i<5; i++){
        stek.stavi(i);
    }
    std::cout << "Ovaj stek ima 4 elementa, nas ispis je: " << stek.brojElemenata() << std::endl;
    if(stek.brojElemenata()!=4) return false;
    
    return true;
}

bool Test2(){                   //testiranje metoda stavi, skini i obrisi
    std::cout << "TEST2" << std::endl;
    Stek<int> stek;
    for(int i=1; i<=10; i++){
        stek.stavi(i);
    }
    std::cout << "Ovaj stek ima 10 elementa, nas ispis je: " << stek.brojElemenata() << std::endl;
    if(stek.brojElemenata()!=10) return false;
    stek.skini();
    stek.skini();
    stek.skini();
    std::cout << "Ovaj stek sada ima 7 elemenata, nas ispis je: " << stek.brojElemenata() << std::endl;
    if(stek.brojElemenata()!=7) return false;
    stek.brisi();
    std::cout << "Stek nakon brisanja treba imati 0 elemenata, nas ispis je: " << stek.brojElemenata() << std::endl;
    if(stek.brojElemenata()!=0) return false;
    
    return true;
}

bool Test3(){                   //testiranje metode vrh
    std::cout << "TEST3" << std::endl;
    Stek<int> stek;
    for(int i=0; i<20; i++){
        stek.stavi(i+1);
    }
    std::cout << "Vrh ovog steka je 20, nas ispis je: " << stek.vrh() << std::endl;
    if(stek.vrh()!=20) return false;
    stek.skini();
    std::cout << "Sada je vrh naseg steka 19, nas ispis je: " << stek.vrh() << std::endl;
    if(stek.vrh()!=19) return false;
    
    return true;
}
bool Test4(){
    std::cout << "TEST4" << std::endl;
    Stek<int> stek;
    stek.stavi(22);
    std::cout << "Broj elemenata je 1, nas ispis je: " << stek.brojElemenata() << std::endl;
    if(stek.brojElemenata()!=1) return false;
    stek.skini();
    std::cout << "Broj elemenata je 0, nas ispis: " << stek.brojElemenata() << std::endl;
    if(stek.brojElemenata()!=0) return false;
    
    return true;
}

bool Test5(){
    std::cout << "TEST5" << std::endl;
    Stek<int> stek;
    for(int i=0; i<=50; i+=10){
        stek.stavi(i);
    }
    std::cout << "Broj elemenata je 6, nas ispis je: " << stek.brojElemenata() << std::endl;
    if(stek.brojElemenata()!=6) return false;
    std::cout << "Element na vrhu je 50, nas ispis je: " << stek.vrh() << std::endl;
    if(stek.vrh()!=50) return false;
    stek.skini();
    std::cout << "Element na vrhu je 40, nas ispis je: " << stek.vrh() << std::endl;
    if(stek.vrh()!=40) return false;
    stek.skini();
    std::cout << "Element na vrhu je 30, nas ispis je: " << stek.vrh() << std::endl;
    if(stek.vrh()!=30) return false;
    stek.skini();
    std::cout << "Element na vrhu je 20, nas ispis je: " << stek.vrh() << std::endl;
    if(stek.vrh()!=20) return false;
    stek.skini();
    std::cout << "Element na vrhu je 10, nas ispis je: " << stek.vrh() << std::endl;
    if(stek.vrh()!=10) return false;
    stek.skini();
    std::cout << "Element na vrhu je 0, nas ispis je: " << stek.vrh() << std::endl;
    if(stek.vrh()!=0) return false;
    stek.skini();
    
    return true;
}


int main()
{
    if(Test1() && Test2() && Test3() && Test4() && Test5()){
        std::cout << "Prosli su svi testovi !" << std::endl;
    }
    else{
        std::cout << "Neki testovi nisu prosli !" << std::endl;
    }
    return 0;
}
