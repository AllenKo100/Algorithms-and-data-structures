#include <iostream>
#include <vector>
#include <utility>
#include <list>

using namespace std;

//Zadaća 5, Zadatak 1
//Početak
template <typename TipKljuca, typename TipVrijednosti>
struct CvorZaHash {
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    CvorZaHash *next=nullptr;
};

template <typename TipKljuca, typename TipVrijednosti>
class Mapa
{
public:
    Mapa() {}
    virtual ~Mapa() {}
    virtual TipVrijednosti &operator[] (TipKljuca k)=0;
    virtual TipVrijednosti operator[] (TipKljuca k) const=0;
    virtual int brojElemenata() const=0;
    virtual void obrisi()=0;
    virtual void obrisi(const TipKljuca &k)=0;
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {
    unsigned int kapacitet;
    unsigned int broj_elemenata;
    CvorZaHash<TipKljuca, TipVrijednosti>** niz;
    unsigned int (*hfunkcija)(TipKljuca kljuc, unsigned int opseg)=0;
public:
    HashMapaLan() {
        kapacitet= 1000;
        broj_elemenata=0;
        niz=nullptr;
    }
    HashMapaLan(HashMapaLan<TipKljuca, TipVrijednosti> &mapa) {
        kapacitet=mapa.kapacitet;
        broj_elemenata=mapa.broj_elemenata;
        hfunkcija=mapa.hfunkcija;
        
        niz=new CvorZaHash<TipKljuca, TipVrijednosti>*[kapacitet];
        for(int i=0; i<kapacitet; i++) niz[i]=nullptr;
        
        for(int i=0; i<mapa.kapacitet; i++) {
            if(mapa.niz[i] != nullptr) {
                niz[i] = new CvorZaHash<TipKljuca, TipVrijednosti>();
                niz[i]->kljuc = mapa.niz[i]->kljuc;
                niz[i]->vrijednost = mapa.niz[i]->vrijednost;
                auto a = niz[i];
                auto b = mapa.niz[i];
                while(b->next != nullptr) {
                    a->next = new CvorZaHash<TipKljuca, TipVrijednosti>();
                    b=b->next;
                    a->next->kljuc = b->kljuc;
                    a->next->vrijednost = b->vrijednost;
                }
            }
        }
    }
    HashMapaLan &operator=(const HashMapaLan<TipKljuca, TipVrijednosti> &mapa) {
        if(this==&mapa) return *this;
        
        for(int i=0; i<kapacitet; i++) {
            if(niz[i] != nullptr) {
                while(niz[i] != nullptr) {
                    auto tmp = niz[i];
                    niz[i] = niz[i]->next;
                    delete tmp;
                }
                niz[i] = nullptr;
            }
        }
        delete[] niz;
        
        kapacitet=mapa.kapacitet;
        broj_elemenata=mapa.broj_elemenata;
        hfunkcija=mapa.hfunkcija;
        
        niz = new CvorZaHash<TipKljuca, TipVrijednosti>*[kapacitet];
        for(int i=0; i<kapacitet; i++) niz[i] = nullptr;
        
        for(int i=0; i<mapa.kapacitet; i++) {
            if(mapa.niz[i]!=nullptr) {
                niz[i]=new CvorZaHash<TipKljuca, TipVrijednosti>();
                niz[i]->kljuc=mapa.niz[i]->kljuc;
                niz[i]->vrijednost=mapa.niz[i]->vrijednost;
                auto a = niz[i];
                auto b = mapa.niz[i];
                while(b->next != nullptr) {
                    a->next = new CvorZaHash<TipKljuca, TipVrijednosti>();
                    b=b->next;
                    a->next->kljuc = b->kljuc;
                    a->next->vrijednost = b->vrijednost;
                }
            }
        }
        return *this;
    }
    int brojElemenata() const {
        return broj_elemenata;
    }
    void obrisi() {
        if(broj_elemenata==0) {
            return;
        }
        for(int i=0; i<kapacitet; i++) {
            if(niz[i]!=nullptr) {
                while(niz[i]!=nullptr) {
                    auto tmp=niz[i];
                    niz[i]=niz[i]->next;
                    delete tmp;
                }
                niz[i]=nullptr;
            }
        }
        delete[] niz;
        broj_elemenata=0;
        kapacitet=1000;
        niz=nullptr;
    }
    void obrisi(const TipKljuca& k) {
        if(broj_elemenata==0) {
            throw std::domain_error("Taj element nije u mapi!");
        }
        unsigned int indeks=hfunkcija(k, kapacitet);
        bool imaLiGa=false;
        if(niz[indeks]!=nullptr) {
            if(niz[indeks]->kljuc==k) {
                imaLiGa=true;
                niz[indeks]->vrijednost=TipVrijednosti();
                broj_elemenata--;
                return;
            }
            auto tmp=niz[indeks];
            while(tmp!=nullptr) {
                if(tmp->next!=nullptr) {
                    auto tmp2=tmp->next;
                    if(tmp2->kljuc==k) {
                        imaLiGa=true;
                        tmp2->vrijednost=TipVrijednosti();
                        break;
                    }
                }
                tmp=tmp->next;
            }
            broj_elemenata--;
        }
        if(imaLiGa==false) {
            throw std::domain_error("Taj element nije u mapi!");
        }
    }
    ~HashMapaLan<TipKljuca, TipVrijednosti>() {
        //Pozvat ćemo samo metodu obrisi bez parametara
        //jer ona radi isto što i destruktor
        this->obrisi();
    }
    void definisiHashFunkciju(unsigned int (*funkcija)(TipKljuca, unsigned int)) {
        if(funkcija==nullptr) {
            throw std::domain_error("Nije definisana funkcija!");
        }
        hfunkcija=funkcija;
    }
    TipVrijednosti operator[] (TipKljuca k) const {
        if(hfunkcija==0) {
            throw std::domain_error("Nije definisana Hash funkcija!");
        }
        unsigned int indeks=hfunkcija(k,kapacitet);
        if(niz[indeks]!=nullptr) {
            auto tmp = niz[indeks];
            while(tmp != nullptr) {
                if(tmp->kljuc==k) {
                    return tmp->vrijednost;
                }
                tmp=tmp->next;
            }
        }
        return TipVrijednosti();
    }
    TipVrijednosti &operator[] (TipKljuca k) {
        if(hfunkcija==0) {
            throw std::domain_error("Nije definisana Hash funkcija!");
        }
        unsigned int indeks=hfunkcija(k,kapacitet);
        //radimo prvo slucaj ako nema elemenata u mapi
        if(broj_elemenata==0) {
            niz=new CvorZaHash<TipKljuca, TipVrijednosti>*[kapacitet];
            niz[indeks]=new CvorZaHash<TipKljuca, TipVrijednosti>();
            niz[indeks]->kljuc=k;
            niz[indeks]->vrijednost=TipVrijednosti();
            broj_elemenata++;
            
            for(int i=0; i<kapacitet; i++) {
                if(i!=indeks) {
                    niz[i]=nullptr;
                }
            }
            return niz[indeks]->vrijednost;
        }
        else {
            if(niz[indeks]!=nullptr) {
                auto tmp = niz[indeks];
                while(tmp!=nullptr) {
                    if(tmp->kljuc==k) return tmp->vrijednost;
                    tmp=tmp->next;
                }
            }
            if(niz[indeks]==nullptr) {
                niz[indeks]=new CvorZaHash<TipKljuca,TipVrijednosti>();
                niz[indeks]->kljuc=k;
                niz[indeks]->vrijednost=TipVrijednosti();
                broj_elemenata++;
                return niz[indeks]->vrijednost;
            }
            else {
                if(k < niz[indeks]->kljuc) {
                    auto tmp = niz[indeks];
                    niz[indeks]=new CvorZaHash<TipKljuca, TipVrijednosti>();
                    niz[indeks]->kljuc=k;
                    niz[indeks]->vrijednost=TipVrijednosti();
                    niz[indeks]->next=tmp;
                    broj_elemenata++;
                    return niz[indeks]->vrijednost;
                }
                
                auto temp=niz[indeks];
                auto prethodni=niz[indeks];
                
                while(temp->next != nullptr && temp->kljuc < k) {
                    prethodni=temp;
                    temp=temp->next;
                }
                if(temp->next==nullptr) {
                    temp->next=new CvorZaHash<TipKljuca, TipVrijednosti>();
                    temp->next->kljuc=k;
                    temp->next->vrijednost=TipVrijednosti();
                    broj_elemenata++;
                    return temp->next->vrijednost;
                }
                else {
                    auto tmp2=new CvorZaHash<TipKljuca, TipVrijednosti>();
                    tmp2->kljuc=k;
                    tmp2->vrijednost=TipVrijednosti();
                    prethodni->next=tmp2;
                    tmp2->next=niz[indeks];
                    broj_elemenata++;
                    return tmp2->vrijednost;
                }
            }
        }
    }
};

//KRAJ Zadataka1

//Zadaća 5, Zadatak 1B
//Početak

template <typename TipOznake>
class Grana;
template <typename TipOznake>
class Cvor;
template <typename TipOznake>
class GranaIterator;

template <typename TipOznake>
class UsmjereniGraf {
protected:
    UsmjereniGraf() {}
public:
    UsmjereniGraf(int brojCvorova) {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const=0;
    virtual void postaviBrojCvorova(int brojCvorova)=0;
    virtual void dodajGranu(int polazni, int dolazni, float tezina)=0;
    virtual void obrisiGranu(int polazni, int dolazni)=0;
    virtual void postaviTezinuGrane(int polazni, int dolazni, float tezina)=0;
    virtual float dajTezinuGrane(int polazni, int dolazni) const=0;
    virtual bool postojiGrana(int polazni, int dolazni) const=0;
    virtual void postaviOznakuCvora(int cvor, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuCvora(int cvor) const=0;
    virtual void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuGrane(int polazni, int dolazni) const=0;
    virtual Grana<TipOznake> dajGranu(int polazni, int dolazni) =0;
    virtual Cvor<TipOznake> dajCvor(int cvor) =0;
    GranaIterator<TipOznake> dajGranePocetak() {
        return ++GranaIterator<TipOznake>(this,0,-1);
    }
    GranaIterator<TipOznake> dajGraneKraj() {
        return GranaIterator<TipOznake>(this,dajBrojCvorova(),0);
    }
};

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake> {
    struct PodaciGrane {
        TipOznake oznaka;
        float tezina;
        bool postoji;
    };
    std::vector<std::vector<PodaciGrane>> matrica;
    std::vector<TipOznake> oznakeCvorova;
public:
    MatricaGraf(int brojCvorova) : UsmjereniGraf<TipOznake>(brojCvorova){
        postaviBrojCvorova(brojCvorova);
    }
    int dajBrojCvorova() const {
        return matrica.size();
    }
    void postaviBrojCvorova(int brojCvorova) {
        if(brojCvorova < matrica.size()) {
            throw "ne moze se smanjiti broj cvorova";
        }
        
        PodaciGrane nepostojeca;
        nepostojeca.postoji=false;
        
        for(int i=0; i<matrica.size(); i++){
            matrica[i].resize(brojCvorova, nepostojeca);
        }
        
        std::vector<PodaciGrane> prazanRed(brojCvorova, nepostojeca);
        matrica.resize(brojCvorova, prazanRed);
        oznakeCvorova.resize(brojCvorova);
    }
    void dodajGranu(int polazni, int dolazni, float tezina) {
        PodaciGrane g;
        g.tezina=tezina;
        g.postoji=true;
        matrica[polazni][dolazni]=g;
    }
    void obrisiGranu(int polazni, int dolazni) {
        matrica[polazni][dolazni].postoji=false;
    }
    void postaviTezinuGrane(int polazni, int dolazni, float tezina) {
        matrica[polazni][dolazni].tezina=tezina;
    }
    float dajTezinuGrane(int polazni, int dolazni) {
        return matrica[polazni][dolazni].tezina;
    }
    bool postojiGrana(int polazni, int dolazni) {
        return matrica[polazni][dolazni].postoji;
    }
    void postaviOznakuCvora(int cvor, TipOznake oznaka) {
        oznakeCvorova[cvor]=oznaka;
    }
    TipOznake dajOznakuCvora(int cvor) const {
        return oznakeCvorova[cvor];
    }
    void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka) {
        matrica[polazni][dolazni].oznaka=oznaka;
    }
    TipOznake dajOznakuGrane(int polazni, int dolazni) const {
        return matrica[polazni][dolazni].oznaka;
    }
    GranaIterator<TipOznake> dajGranePocetak() {
        GranaIterator<TipOznake> it(this, 0, -1);
        return ++it;
    }
    GranaIterator<TipOznake> dajGraneKraj() {
        GranaIterator<TipOznake> it(this, -1, -1);
        return it;
    }
    GranaIterator<TipOznake> dajSljedecuGranu(int polazni, int dolazni) {
        for(int i=polazni; i<matrica.size(); i++){
            for(int j=0; j<matrica.size(); j++) {
                if(i==polazni && j<=dolazni) continue;
                if(matrica[i][j].postoji) {
                    return GranaIterator<TipOznake>(this, i, j);
                }
            }
        }
        return GranaIterator<TipOznake>(this, -1, -1);
    }
};

template <typename TipOznake>
class Grana {
    UsmjereniGraf<TipOznake>* graf;
    int polazni, dolazni;
public:
    Grana(UsmjereniGraf<TipOznake>* graf, int polazni, int dolazni) : graf(graf), polazni(polazni), dolazni(dolazni) {}
    float dajTezinu() const {
        return graf -> dajTezinuGrane(polazni, dolazni);
    }
    void postaviTezinu(float tezina) {
        graf -> postaviTezinuGrane(polazni, dolazni, tezina);
    }
    TipOznake dajOznaku() const {
        return graf -> dajOznakuGrane(polazni, dolazni);
    }
    void postaviOznaku(TipOznake oznaka) {
        graf -> postaviOznakuGrane(polazni, dolazni, oznaka);
    }
    Cvor<TipOznake> dajPolazniCvor() const {
        return graf -> dajCvor(polazni);
    }
    Cvor<TipOznake> dajDolazniCvor() const {
        return graf -> dajCvor(dolazni);
    }
};

template <typename TipOznake>
class Cvor {
    UsmjereniGraf<TipOznake>* graf;
    int redniBroj;
public:
    Cvor(UsmjereniGraf<TipOznake>* graf, int redniBroj) : graf(graf), redniBroj(redniBroj) {}
    TipOznake dajOznaku() const {
        return graf -> dajOznakuCvora(redniBroj);
    }
    void postaviOznaku(TipOznake oznaka) {
        graf -> postaviOznakuCvora(redniBroj, oznaka);
    }
    int dajRedniBroj() const {
        return redniBroj;
    }
};

template <typename TipOznake>
class GranaIterator {
    UsmjereniGraf<TipOznake>* graf;
    int polazni, dolazni;
public:
    GranaIterator(UsmjereniGraf<TipOznake>* graf, int polazni, int dolazni) :
        graf(graf), polazni(polazni), dolazni(dolazni) {}
    Grana<TipOznake> operator*() {
        return Grana<TipOznake>(graf, polazni, dolazni);
    }
    bool operator==(const GranaIterator& iter) const {
        return (graf == iter.graf && polazni == iter.polazni && dolazni == iter.dolazni);
    }
    bool operator!=(const GranaIterator& iter) const {
        return !(*this == iter);
    }
    GranaIterator& operator++() {
        do {
            if(dolazni+1 >= graf->dajBrojCvorova()) {
                dolazni=0;
                polazni++;
            } else {
                dolazni++;
            }
        } while(polazni < graf->dajBrojCvorova() && graf->postojiGrana(polazni, dolazni) == false);
        
        return *this;
    }
    GranaIterator operator++(int) {
        GranaIterator tmp(*this);
        ++(*this);
        return tmp;
    }
};

//Clan za ListaGraf
template <typename TipOznake>
struct Clan {
    int cvor;
    float tezina;
    TipOznake oznaka;
};

template <typename TipOznake>
class ListaGraf : public UsmjereniGraf<TipOznake> {
    std::vector<TipOznake> oznakeCvorova;
    std::vector<std::list<Clan<TipOznake>>> lista;
    //Napravimo i iteratore kako bi se lakše kretali kroz elemente
    typename std::list<Clan<TipOznake>>::iterator trazenje(int i, int j) {
        auto it=lista[i].begin();
        while(it!=lista[i].end()) {
            if(j==it->cvor) {
                return it;
            }
            else if(j<it->cvor) {
                return lista[i].end();
            }
            it++;
        }
        return it;
    }
    typename std::list<Clan<TipOznake>>::const_iterator trazenje(int i, int j) const {
        auto it=lista[i].begin();
        while(it!=lista[i].end()) {
            if(j==it->cvor) {
                return it;
            }
            else if(j<it->cvor) {
                return lista[i].end();
            }
            it++;
        }
        return it;
    }
    //Pomoćne funkcije za rijesavanje problema
    //Nije potrebno da su public
    void jelIspravanCvor(int broj) const {
        if(broj<0 || broj>=dajBrojCvorova()) throw std::domain_error("Neispravan!");
    }
    void jelIspravnaGrana(int polazni, int dolazni) const {
        jelIspravanCvor(polazni);
        jelIspravanCvor(dolazni);
    }
    void jelImaGrana(int polazni, int dolazni) const {
        jelIspravnaGrana(polazni, dolazni);
        if(postojiGrana(polazni, dolazni)==false) {
            throw std::domain_error("Ne postoji grana!");
        }
    }
public:
    ListaGraf(int brojCvorova) {
        if(brojCvorova<0) {
            throw std::domain_error("Ne može biti manje od 0!");
        }
        oznakeCvorova=std::vector<TipOznake>(brojCvorova);
        lista=std::vector<std::list<Clan<TipOznake>>>(brojCvorova, std::list<Clan<TipOznake>>());
    }
    int dajBrojCvorova() const {
        return lista.size();
    }
    void postaviBrojCvorova(int brojCvorova) {
        if(brojCvorova<dajBrojCvorova()) {
            throw std::domain_error("Ne može biti manje od trenutnog broja!");
        }
        oznakeCvorova.resize(brojCvorova);
        lista.resize(brojCvorova);
    }
    void dodajGranu(int polazni, int dolazni, float tezina=0) {
        //Testirajmo na ispravnost granu
        jelIspravnaGrana(polazni, dolazni);
        //Baca izuzetak ako nije ispravna
        
        auto it=lista[polazni].begin();
        while(it!=lista[polazni].end() && dolazni>it->cvor) it++;
        Clan<TipOznake> pomocni;
        pomocni.oznaka=TipOznake();
        pomocni.tezina=tezina;
        pomocni.cvor=dolazni;
        //Ubacimo to u listu
        lista[polazni].insert(it, pomocni);
    }
    void obrisiGranu(int polazni, int dolazni) {
        //Baci izuzetak ako nema
        jelImaGrana(polazni, dolazni);
        lista[polazni].erase(trazenje(polazni, dolazni));
    }
    void postaviTezinuGrane(int polazni, int dolazni, float tezina=0) {
        jelImaGrana(polazni, dolazni);
        trazenje(polazni, dolazni)->tezina=tezina;
    }
    float dajTezinuGrane(int polazni, int dolazni) const {
        jelImaGrana(polazni, dolazni);
        return trazenje(polazni, dolazni)->tezina;
    }
    bool postojiGrana(int polazni, int dolazni) const {
        jelIspravnaGrana(polazni, dolazni);
        return trazenje(polazni, dolazni)!=lista[polazni].end();
    }
    void postaviOznakuCvora(int cvor, TipOznake oznaka) {
        jelIspravanCvor(cvor);
        oznakeCvorova[cvor]=oznaka;
    }
    TipOznake dajOznakuCvora(int cvor) const {
        jelIspravanCvor(cvor);
        return oznakeCvorova[cvor];
    }
    void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka) {
        jelImaGrana(polazni, dolazni);
        trazenje(polazni, dolazni)->oznaka=oznaka;
    }
    TipOznake dajOznakuGrane(int polazni, int dolazni) const {
        jelImaGrana(polazni, dolazni);
        return trazenje(polazni, dolazni)->oznaka;
    }
    Grana<TipOznake> dajGranu(int polazni, int dolazni) {
        jelImaGrana(polazni, dolazni);
        return Grana<TipOznake>(this, polazni, dolazni);
    }
    Cvor<TipOznake> dajCvor(int cvor) {
        jelIspravanCvor(cvor);
        return Cvor<TipOznake>(this, cvor);
    }
};

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake>* graf, std::vector<Cvor<TipOznake>>& bfsObilazak, Cvor<TipOznake> cvor){
    bool jelPosjecen=false;
    for(int i=0; i<bfsObilazak.size(); i++){
        if(cvor.dajRedniBroj()==bfsObilazak[i].dajRedniBroj()){
            jelPosjecen=true;
            break;
        }
    }
    if(!jelPosjecen){
        bfsObilazak.push_back(cvor);
    }
    int brojCvorova=graf->dajBrojCvorova();
    for(int i=0; i<brojCvorova; i++){
        if(!jelPosjecen){
            bfs(graf, bfsObilazak, graf->dajCvor(i));
        }
    }
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake>* graf, std::vector<Cvor<TipOznake>>& dfsObilazk, Cvor<TipOznake> cvor){
    bool jelPosjecen=false;
    for(int i=0; i<dfsObilazk.size(); i++){
        if(cvor.dajRedniBroj()==dfsObilazk[i].dajRedniBroj()){
            jelPosjecen=true;
            break;
        }
    }
    if(!jelPosjecen){
        dfsObilazk.push_back(cvor);
    }
    int brojCvorova=graf->dajBrojCvorova();
    for(int i=0; i<brojCvorova; i++){
        if(graf->postojiGrana(cvor.dajRedniBroj(), i) && jelPosjecen==false){
            dfs(graf, dfsObilazk, graf->dajCvor(i));
        }
    }
}

void TestListaGraf() {
    //napravimo graf prvo
    UsmjereniGraf<bool> *g1 = new ListaGraf<bool> (8);
    g1->dodajGranu(1, 2, 3);
    g1->dodajGranu(1, 5, 2);
    g1->dodajGranu(1, 7, 1);
    g1->dodajGranu(1, 8, 9);
    g1->dodajGranu(2, 3, 4);
    g1->dodajGranu(2, 4, 7);
    g1->dodajGranu(2, 8, 3);
    g1->dodajGranu(3, 4, 2);
    g1->dodajGranu(3, 6, 2);
    g1->dodajGranu(3, 8, 3);
    g1->dodajGranu(4, 6, 5);
    g1->dodajGranu(4, 7, 7);
    g1->dodajGranu(5, 6, 8);
    g1->dodajGranu(5, 8, 10);
    //testirajmo sada iterator
    std::cout << "Polazni/Dolazni/Težina" << std::endl;
    for(GranaIterator<bool> iter = g1->dajGranePocetak(); iter != g1->dajGraneKraj(); ++iter){
            std::cout << (*iter).dajPolazniCvor().dajRedniBroj() << " "
                      << (*iter).dajDolazniCvor().dajRedniBroj() << " "
                      << (*iter).dajTezinu() << std::endl;
    }
    std::cout << std::endl;
    //moramo obrisat da ne bi bilo curenja memorije
    delete g1;
}

void TestBFS() {
    std::cout << "BFS testiranje" << std::endl;
    UsmjereniGraf<bool> *g2 = new ListaGraf<bool> (4);
    g2->dodajGranu(0, 1, 2.3);
    g2->dodajGranu(0, 2, 4.2);
    g2->dodajGranu(1, 2, 1.5);
    g2->dodajGranu(2, 0, 1.2);
    g2->dodajGranu(2, 3, 1.7);
    g2->dodajGranu(3, 3, 2.6);
    for (GranaIterator<bool> iter = g2->dajGranePocetak();
         iter != g2->dajGraneKraj(); ++iter)
      cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
           << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
           << (*iter).dajTezinu() << "; ";
    std::cout << "BFS" << std::endl;
    //testirajmo BFS
    std::vector<Cvor<bool> > bfs_vek;
    bfs(g2, bfs_vek, g2->dajCvor(0));
    for (int i=0; i<bfs_vek.size(); i++) {
        std::cout << bfs_vek[i].dajRedniBroj() << " ";
    }
    std::cout << std::endl;
    delete g2;
}

void TestDFS() {
    std::cout << "DFS testiranje" << std::endl;
    UsmjereniGraf<bool> *g3 = new ListaGraf<bool> (4);
    g3->dodajGranu(0, 1, 2.3);
    g3->dodajGranu(0, 2, 4.2);
    g3->dodajGranu(1, 2, 1.5);
    g3->dodajGranu(2, 0, 1.2);
    g3->dodajGranu(2, 3, 1.7);
    g3->dodajGranu(3, 3, 2.6);
    for (GranaIterator<bool> iter = g3->dajGranePocetak();
         iter != g3->dajGraneKraj(); ++iter)
      cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
           << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
           << (*iter).dajTezinu() << "; ";
    std::cout << "DFS" << std::endl;
    //testirajmo DFS
    std::vector<Cvor<bool> > dfs_vek;
    dfs(g3, dfs_vek, g3->dajCvor(0));
    for (int i=0; i<dfs_vek.size(); i++) {
        std::cout << dfs_vek[i].dajRedniBroj() << " ";
    }
    std::cout << std::endl;
    delete g3;
}

unsigned int hashf1(int ulaz, unsigned int max){
    int suma;
    suma -= (suma);
    suma ^= (suma>>17);
    suma -= (suma<<9);
    suma ^= (suma<<4);
    suma -= (suma<<3);
    suma ^= (suma<<10);
    suma ^= (suma>>15);
    return suma%max;
}

int main() {
    
    //Testirajmo još ListuGraf
    UsmjereniGraf<bool> *g = new ListaGraf<bool>(6);
    g->dodajGranu(0, 1, 4.2);
    g->dodajGranu(1, 0, 3.1);
    g->dodajGranu(1, 2, 2.1);
    g->dodajGranu(0, 0, 5.6);
    g->dodajGranu(2, 0, 2.2);
    g->dodajGranu(2, 1, 3);
    for (GranaIterator<bool> iter = g->dajGranePocetak();
         iter != g->dajGraneKraj(); ++iter)
      cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
           << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
           << (*iter).dajTezinu() << "; ";
    delete g;
    
    std::cout << std::endl;
    //Testirajmo BFS i DFS
    std::cout << "Testirajmo BFS i DFS na nekim random grafovima: " << std::endl;
    TestBFS();
    TestDFS();
    std::cout << std::endl;
    
    
    std::cout << "Testirajmo HashMapaLan: " << std::endl;
    //Testiranje HashMapLan
    HashMapaLan<int, int> hml;
    hml.definisiHashFunkciju(hashf1);
    std::cout << "Ubacimo 4 elementa: " << std::endl;
    hml[1]=10;
    std::cout << hml[1] << " ";
    hml[10]=1;
    std::cout << hml[10] << " ";
    hml[11]=234;
    std::cout << hml[11] << " ";
    hml[32]=0;
    std::cout << hml[32] << " ";
    std::cout << std::endl;
    std::cout << "Trenutni broj elemenata " << hml.brojElemenata() << std::endl; //treba bit 4
    //testirajmo brisanje
    hml.obrisi(1);
    hml.obrisi(32);
    std::cout << "Nakon brisanja 2, " << hml.brojElemenata() << std::endl; //treba bit 2
    hml.obrisi();
    std::cout << "Nakon brisanja svih, " << hml.brojElemenata() << std::endl; //treba bit 0
    return 0;
}
