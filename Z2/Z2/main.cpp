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

//implementirana funkcija za binarnu pretragu
int binarnaPretraga(std::vector<int> vektor, int pocetak, int kraj, int trazeni){
    if(kraj<pocetak){
        return -1;
    }
    
    int sredina=(pocetak+kraj)/2;
    
    if(vektor.at(sredina)==trazeni) {
        return sredina;
    }
    else if(vektor.at(sredina)<trazeni){
        return binarnaPretraga(vektor, sredina+1, kraj, trazeni);
    }
    else{
        return binarnaPretraga(vektor, pocetak, sredina-1, trazeni);
    }
}

void pretraga(Stek<std::vector<int>> &s, int trazeni){
    if(s.brojElemenata()==0){
        std::cout << "Nema elementa" << std::endl;
        return;
    }
    
    std::vector<int> pomocni=s.skini();
    
    int pozicijaTrazenog=binarnaPretraga(pomocni, 0, pomocni.size()-1, trazeni);
    
    if(pozicijaTrazenog==-1){
        pretraga(s, trazeni);
    }
    else{
        std::cout << pozicijaTrazenog << " " << s.brojElemenata();
    }
    
    s.stavi(pomocni);
}

//Test kada je nadjen trazen element
void TestPretraga1(){
    std::vector<int> vek1{1,2,3,4,5};
    std::vector<int> vek2{10,15,20,25};
    std::vector<int> vek3{50,100,150,200};
    
    Stek<std::vector<int>> stek;
    
    stek.stavi(vek1);
    stek.stavi(vek2);
    stek.stavi(vek3);
    
    pretraga(stek, 15);
}

//Jos jedan test kada je nadjen trazeni element
void TestPretraga2(){
    std::vector<int> vek1{2,4,6,8,10};
    std::vector<int> vek2{100,200,300,400,500};
    std::vector<int> vek3{501,502,503,504,505};
    std::vector<int> vek4{600,610,620,630};
    std::vector<int> vek5{10000,100000,10000000};
    
    Stek<std::vector<int>> stek;
    
    stek.stavi(vek1);
    stek.stavi(vek2);
    stek.stavi(vek3);
    stek.stavi(vek4);
    stek.stavi(vek5);
    
    pretraga(stek, 620);
    
}

//Test kada nema trazenog elementa
void TestPretraga3(){
    std::vector<int> vek1{3,6,8,10};
    std::vector<int> vek2{11,12,13};
    std::vector<int> vek3{20,22,24,26,28,30};
    std::vector<int> vek4{100,200,300,400};
    std::vector<int> vek5{1000,10000,100000,1000000,111111111};
    
    Stek<std::vector<int>> stek;
    
    stek.stavi(vek1);
    stek.stavi(vek2);
    stek.stavi(vek3);
    stek.stavi(vek4);
    stek.stavi(vek5);
    
    pretraga(stek, 4);
}

int main() {
    std::cout << "Neki od rezultata funkcija za testiranje pretrage: " << std::endl;
    std::cout << "Pretraga 1: " << std::endl;
    TestPretraga1();
    std::cout << std::endl;
    std::cout << "Pretraga 2: " << std::endl;
    TestPretraga2();
    std::cout << std::endl;
    std::cout << "Pretraga 3: " << std::endl;
    TestPretraga3();
    std::cout << std::endl;
    
    return 0;
}
