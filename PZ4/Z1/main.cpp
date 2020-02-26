#include <iostream>

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

template <typename Tip>
class Red
{
    JednostrukaLista<Tip> red;
public:
    Red() {}
    void brisi()
    {
        int broj_elemenata=red.brojElemenata();
        for(int i=0; i<broj_elemenata; i++) {
            red.obrisi();
        }
    }
    void stavi(const Tip& el)
    {
        if(red.brojElemenata()>0) {
            red.kraj();
        }
        red.dodajIza(el);
    }
    Tip skini()
    {
        red.pocetak();
        Tip element=red.trenutni();
        red.obrisi();
        return element;
    }
    Tip& celo()
    {
        if(red.brojElemenata()==0) throw "Red prazan";
        red.pocetak();
        return red.trenutni();
    }
    int brojElemenata()
    {
        return red.brojElemenata();
    }
};

bool TestBrisi(){
    Red<int> red;
    for(int i=0; i<10; i++){
        red.stavi(i);
    }
    std::cout << "Broj elemenata reda je 10, a nas ispis je: " << red.brojElemenata() << std::endl;
    if(red.brojElemenata()!=10) return false;
    red.brisi();
    std::cout << "Broj elemenata reda nakon brisanje je 0, a nas ispis je: " << red.brojElemenata() << std::endl;
    if(red.brojElemenata()!=0) return false;
    
    return true;
}

bool TestBrojElemenata(){
    Red<int> red;
    for(int i=1; i<=50; i++){
        red.stavi(i);
    }
    std::cout << "Broj elemenata reda je 50, a nas ispis je: " << red.brojElemenata() << std::endl;
    if(red.brojElemenata()!=50) return false;
    
    Red<int> red2(red);
    red2.skini();
    std::cout << "Broj elemenata reda je 49, a nas ispis je: " << red2.brojElemenata() << std::endl;
    if(red2.brojElemenata()!=49) return false;
    
    return true;
}

bool TestStavi(){
    Red<int> red;
    red.stavi(100);
    red.stavi(200);
    red.stavi(300);
    red.stavi(400);
    red.stavi(500);
    
    std::cout << "Broj elemenata reda je 5, a nas ispis je: " << red.brojElemenata() << std::endl;
    if(red.brojElemenata()!=5) return false;
    
    return true;
}

bool TestCelo(){
    Red<int> red;
    for(int i=5; i<11; i++){
        red.stavi(i);
    }
    std::cout << "Element na celu reda je 5, a nas ispis je: "<< red.celo() << std::endl;
    if(red.celo()!=5) return false;
    
    red.skini();
    
    std::cout << "Sljedeci element na celu reda je 6, a nas ispis je: " << red.celo() << std::endl;
    if(red.celo()!=6) return false;
    
    return true;
}

bool TestSkini(){
    Red<int> red1;
    Red<int> red2;
    
    for(int i=0; i<10; i++){
        red1.stavi(i+10);
        red2.stavi(i+1);
    }
    
    red1.skini();
    red2.skini();
    red2.skini();
    
    std::cout << "Nakon skidanja elementa iz reda1, broj elemenata je 9, a nas ispis je: " << red1.brojElemenata() << std::endl;
    if(red1.brojElemenata()!=9) return false;
    std::cout << "Nakon skidanja elementa iz reda2, broj elemenata je 8, a nas ispis je: " << red2.brojElemenata() << std::endl;
    if(red2.brojElemenata()!=8) return false;
    
    return true;
}


int main()
{
    if(TestBrisi() && TestBrojElemenata() && TestStavi() && TestCelo() && TestSkini()){
        std::cout << "Prosli su svi testovi!" << std::endl;
    }
    else{
        std::cout << "Neki testovi nisu prosli" << std::endl;
    }
    return 0;
}
