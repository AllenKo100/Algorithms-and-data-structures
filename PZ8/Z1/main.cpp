#include <iostream>
#include <utility>
#include <string>
#include <ctime>
#include <vector>

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
class NizMapa : public Mapa<TipKljuca, TipVrijednosti>
{
    std::pair<TipKljuca, TipVrijednosti> *niz;
    int kapacitet;
    int broj_elemenata;
public:
    NizMapa()
    {
        kapacitet=100;
        broj_elemenata=0;
        niz=nullptr;
    }
    NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &mapa)
    {
        if(this == &mapa) return;
        else {
            int velicina=mapa.brojElemenata();
            niz=new std::pair<TipKljuca, TipVrijednosti>[velicina];
            kapacitet=mapa.kapacitet;
            broj_elemenata=mapa.broj_elemenata;
            for(int i=0; i<velicina; i++) {
                niz[i].first=mapa.niz[i].first;
                niz[i].second=mapa.niz[i].second;
            }
        }
    }
    NizMapa &operator =(const NizMapa<TipKljuca, TipVrijednosti> &mapa)
    {
        if(this == &mapa) return *this;
        else {
            delete[] niz;
            int velicina=mapa.brojElemenata();
            niz=new std::pair<TipKljuca, TipVrijednosti>[velicina];
            kapacitet=mapa.kapacitet;
            broj_elemenata=mapa.broj_elemenata;
            for(int i=0; i<velicina; i++) {
                niz[i].first=mapa.niz[i].first;
                niz[i].second=mapa.niz[i].second;
            }
            return *this;
        }
    }
    ~NizMapa()
    {
        this->obrisi();
    }
    int brojElemenata() const
    {
        return broj_elemenata;
    }
    void obrisi()
    {
        delete[] niz;
        niz=nullptr;
        kapacitet=100;
        broj_elemenata=0;
    }
    void obrisi(const TipKljuca &k)
    {
        if(broj_elemenata==0) {
            throw std::domain_error("Trenutno nema elemenata u mapi!");
        }

        int indexElementa=-1;
        for(int i=0; i<broj_elemenata; i++) {
            if(niz[i].first==k) {
                indexElementa=i;
                break;
            }
        }
        if(indexElementa==-1) {
            throw std::domain_error("Taj element nije prisutan u mapi!");
        }

        for(int i=indexElementa; i<broj_elemenata-1; i++) {
            niz[i].first=niz[i+1].first;
            niz[i].second=niz[i+1].second;
        }
        broj_elemenata--;
    }
    TipVrijednosti operator[] (TipKljuca k) const
    {
        for(int i=0; i<broj_elemenata; i++) {
            if(niz[i].first==k) {
                return niz[i].second;
            }
        }

        return TipVrijednosti();
    }
    TipVrijednosti &operator[] (TipKljuca k)
    {
        if(broj_elemenata!=0) {
            for(int i=0; i<broj_elemenata; i++) {
                if(niz[i].first==k) {
                    return niz[i].second;
                }
            }
            //Sada vršimo dealokaciju ukoliko smo dostigli makismalni kapacitet
            if(broj_elemenata==kapacitet) {
                kapacitet=kapacitet*2;
                std::pair<TipKljuca, TipVrijednosti> *pomocni=new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
                for(int i=0; i<broj_elemenata; i++) {
                    pomocni[i].first=niz[i].first;
                    pomocni[i].second=niz[i].second;
                }
                delete[] niz;
                niz=pomocni;
                pomocni=nullptr;
            }
            //ukoliko nismo pronasli vrijednost, onda postavljamo novi kljuc
            niz[broj_elemenata].first=k;
            //Vrijednost ce biti default u zavisnosti kojeg je tipa
            niz[broj_elemenata].second=TipVrijednosti();
            broj_elemenata++;

            return niz[broj_elemenata-1].second;
        } else {
            niz=new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
            niz[0].first=k;
            niz[0].second=TipVrijednosti();
            broj_elemenata++;
            return niz[0].second;
        }
    }
};

/*template <typename TipKljuca, typename TipVrijednosti>
struct Cvor {
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    Cvor *lijevi=nullptr;
    Cvor *desni=nullptr;
    Cvor *roditelj=nullptr;
};*/

template <typename TipKljuca, typename TipVrijednosti>
class BSCvor{
public:
    BSCvor(){
        lijevo=desno=roditelj=nullptr;
    }
    BSCvor(const TipKljuca& k, const TipVrijednosti& v, BSCvor *l=nullptr, BSCvor *d=nullptr, BSCvor *rod=nullptr){
        kljuc=k;
        vrijednost=v;
        lijevo=l;
        desno=d;
        roditelj=rod;
    }
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    BSCvor<TipKljuca, TipVrijednosti> *lijevo, *desno, *roditelj;
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    BSCvor<TipKljuca, TipVrijednosti> *korijen;
    int broj_elemenata;
    
    //Osim sto smo atribute definisali u privatnom dijelu
    //definisat cemo i neke pomocne metode, kojima pristupamo samo iz ove klase
    //te zbog toga se one nalaze u privatnom dijelu ove klase
    //to su metode Umetni, dodavanje, brisi, Preorder
    void Umetni(BSCvor<TipKljuca, TipVrijednosti>* &z){
        BSCvor<TipKljuca, TipVrijednosti> *y=0, *x=korijen;
        while(x!=0){
            y=x;
            if(z->kljuc < x->kljuc) x=x->lijevo;
            else x=x->desno;
        }
        if(y==0){
            korijen=z;
            broj_elemenata++;
        }
        else{
            if(z->kljuc < y->kljuc){
                y->lijevo=z;
                z->roditelj=y;
                broj_elemenata++;
            }
            else{
                y->desno=z;
                z->roditelj=y;
                broj_elemenata++;
            }
        }
    }
    TipVrijednosti& dodavanje(TipKljuca k){
        BSCvor<TipKljuca, TipVrijednosti> *tmp=new BSCvor<TipKljuca, TipVrijednosti>(k, TipVrijednosti{});
        this->Umetni(tmp);
        return tmp->vrijednost;
    }
    void brisi(BSCvor<TipKljuca, TipVrijednosti>* tmp){
        if(tmp){
            brisi(tmp->lijevo);
            brisi(tmp->desno);
            delete tmp;
        }
        korijen=nullptr;
    }
    void Preorder(BSCvor<TipKljuca, TipVrijednosti>* tmp){
        if(tmp!=0){
            this->dodavanje(tmp->kljuc)=tmp->vrijednost;
            Preorder(tmp->lijevo);
            Preorder(tmp->desno);
        }
    }
public:
    BinStabloMapa(){
        korijen=nullptr;
        broj_elemenata=0;
    }
    BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo){
        korijen=nullptr;
        broj_elemenata=0;
        BSCvor<TipKljuca, TipVrijednosti> *pomocni = stablo.korijen;
        Preorder(pomocni);
        
    }
    BinStabloMapa& operator= (const BinStabloMapa<TipKljuca, TipVrijednosti> &stablo){
        if(this==&stablo) return *this;
        this->obrisi();
        korijen=nullptr;
        BSCvor<TipKljuca, TipVrijednosti>* pomocni=stablo.korijen;
        Preorder(pomocni);
        return *this;
    }
    ~BinStabloMapa<TipKljuca, TipVrijednosti>(){
        brisi(korijen);
    }
    TipVrijednosti operator[] (TipKljuca k) const{
        auto tmp=korijen;
        while(tmp!=0 && k!=tmp->kljuc){
            if(k<tmp->kljuc) tmp=tmp->lijevo;
            else tmp=tmp->desno;
        }
        if(tmp!=nullptr) return tmp->vrijednost;
        TipVrijednosti pomocni{};
        return pomocni;
    }
    TipVrijednosti &operator[] (TipKljuca k){
        auto tmp=korijen;
        while(tmp!=0 && k!=tmp->kljuc){
            if(k<tmp->kljuc) tmp=tmp->lijevo;
            else tmp=tmp->desno;
        }
        if(tmp!=nullptr) return tmp->vrijednost;
        
        BSCvor<TipKljuca, TipVrijednosti> *pomocni=new BSCvor<TipKljuca, TipVrijednosti>(k, TipVrijednosti{});
        this->Umetni(pomocni);
        return pomocni->vrijednost;
    }
    void obrisi(){
        brisi(korijen);
        broj_elemenata=0;
    }
    void obrisi(const TipKljuca &k){
        BSCvor<TipKljuca, TipVrijednosti> *p=korijen, *q=0;
        BSCvor<TipKljuca, TipVrijednosti> *tmp=0, *pp=0, *rp=0;
        while(p!=0 && k!=p->kljuc){
            q=p;
            if(k<p->kljuc) p=p->lijevo;
            else p=p->desno;
        }
        if(p==0) throw std::logic_error("Nije pronadjen element");
        if(p->lijevo==0) rp=p->desno;
        else if(p->desno==0) rp=p->lijevo;
        else{
            pp=p;
            rp=p->lijevo;
            tmp=rp->desno;
            while(tmp!=0){
                pp=rp;
                rp=tmp;
                tmp=rp->desno;
            }
            if(pp!=p){
                pp->desno=rp->lijevo;
                rp->lijevo=p->lijevo;
                pp->roditelj=rp;
                rp->roditelj=q;
                if(pp->desno) pp->desno->roditelj=pp;
            }
            rp->desno=p->desno;
        }
        if(q==0) korijen=rp;
        else if(p==q->lijevo) q->lijevo=rp;
        else q->desno=rp;
        delete p;
        broj_elemenata--;
    }
    int brojElemenata() const{
        return broj_elemenata;
    }
};

int main() {
    //Testiramo performanse BinStabloMapa i NizMapa
    std::cout << "Rezultati mjerenja vremena izvrsenja opercija dodavanja kod NizMapa i BSM: " << std::endl;
    NizMapa<int, int> nm;
    BinStabloMapa<int, int> bsm;
    std::vector<int> vek;
    
    for(int i=0; i<10000; i++){
        vek.push_back(rand()%10000);
    }
    
    clock_t vrijeme1;
    clock_t vrijeme2;
    
    //Uradimo prvo dodavanje elemenata za NizMapa
    vrijeme1=clock();
    for(int i=0; i<vek.size(); i++){
        nm[vek[i]]=i;
    }
    vrijeme2=clock();
    int ukupnonm1=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za dodavanje 10000 elemenata u NizMapu je " << ukupnonm1 << " milisekundi." << std::endl;
    
    //Dodavanje elemenata za BSM
    vrijeme1=clock();
    for(int i=0; i<vek.size(); i++){
        nm[vek[i]]=i;
    }
    vrijeme2=clock();
    int ukupnobsm1=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za dodavanje 10000 elemenata u BSM je " << ukupnobsm1 << " milisekundi." << std::endl;
    
    //Ispitajmo sada pristup elementima
    std::cout << "Rezultati mjerenja vremena izvrsenja operacija pristupa elementima kod NizMapa i BSM: " << std::endl;
    //Idemo prvo za NizMapa
    vrijeme1=clock();
    for(int i=0; i<vek.size(); i++){
        nm[vek[i]];
    }
    vrijeme2=clock();
    int ukupnonm2=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za pristup 10000 elemenata u NizMapi je " << ukupnonm2 << " milisekundi." << std::endl;
    
    //Idemo sada za BSM
    vrijeme1=clock();
    for(int i=0; i<vek.size(); i++){
        bsm[vek[i]];
    }
    vrijeme2=clock();
    int ukupnobsm2=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za pristup 10000 elemenata u BSM je " << ukupnobsm2 << " milisekundi." << std::endl;
    
    return 0;
}