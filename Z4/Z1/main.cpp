#include <iostream>
#include <utility>
#include <string>
#include <ctime>
#include <vector>

using namespace std;

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

template <typename TipKljuca, typename TipVrijednosti>
class AVLCvor{
public:
    AVLCvor(){
        lijevo=desno=roditelj=nullptr;
    }
    AVLCvor(const TipKljuca& k, const TipVrijednosti& v){
        kljuc=k;
        vrijednost=v;
    }
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    int balans=0;
    AVLCvor<TipKljuca, TipVrijednosti> *lijevo=0, *desno=0, *roditelj=0;
};

template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    AVLCvor<TipKljuca, TipVrijednosti> *korijen;
    int broj_elemenata;
    
    //Osim sto smo atribute definisali u privatnom dijelu
    //definisat cemo i neke pomocne metode, kojima pristupamo samo iz ove klase
    //te zbog toga se one nalaze u privatnom dijelu ove klase
    //to su metode Umetni, dodavanje, brisi, Preorder
    //te azurirajBalans zajedno sa pratecim rotirajUdesno i rotirajUlijevo
    void Umetni(AVLCvor<TipKljuca, TipVrijednosti>* &z){
        AVLCvor<TipKljuca, TipVrijednosti> *y=0, *x=korijen;
        while(x!=0){
            y=x;
            if(z->kljuc < x->kljuc) x=x->lijevo;
            else x=x->desno;
        }
        bool imaLiBurazera=false;
        if(y==0){
            korijen=z;
            broj_elemenata++;
            imaLiBurazera=true;
        }
        else{
            if(z->kljuc < y->kljuc){
                y->lijevo=z;
                z->roditelj=y;
                broj_elemenata++;
                if(y->desno) {
                    imaLiBurazera=true;
                    y->balans=0;
                }
            }
            else{
                y->desno=z;
                z->roditelj=y;
                broj_elemenata++;
                if(y->lijevo) {
                    imaLiBurazera=true;
                    y->balans=0;
                }
            }
        }
        if(!imaLiBurazera) azurirajBalans(z);
    }
    void azurirajBalans(AVLCvor<TipKljuca, TipVrijednosti>* &cvor) {
        if(cvor->roditelj==nullptr) return;

        if(cvor->roditelj->lijevo==cvor){
            cvor->roditelj->balans++;
        }
        else {
            cvor->roditelj->balans--;
        }
        
        if(cvor->roditelj->balans==0)
            return;
        
        AVLCvor<TipKljuca, TipVrijednosti>* pom1 = cvor->roditelj;
        AVLCvor<TipKljuca, TipVrijednosti>* pom2 = cvor;
        if(cvor->balans>0 && cvor->roditelj->balans<-1) {
            rotirajUdesno(pom2);
            rotirajUlijevo(pom1);
        }
        else if(cvor->balans<0 && cvor->roditelj->balans>1) {
            rotirajUlijevo(pom2);
            rotirajUdesno(pom1);
        }
        else if(cvor->roditelj->balans<-1) {
            rotirajUlijevo(pom1);
        }
        else if(cvor->roditelj->balans>1) {
            rotirajUdesno(pom1);
        }
        else azurirajBalans(cvor->roditelj);
    }
    void rotirajUdesno(AVLCvor<TipKljuca, TipVrijednosti>* &cvor) {
        if(cvor->roditelj && cvor->roditelj->lijevo==cvor){
            cvor->roditelj->lijevo=cvor->lijevo;
        }
        else if(cvor->roditelj){
            cvor->roditelj->desno=cvor->lijevo;
        }
        AVLCvor<TipKljuca, TipVrijednosti>* pomocni=cvor->lijevo;
        bool jelImaLijevo=false;
        if(cvor->lijevo->desno) {
            cvor->lijevo=pomocni->desno;
            pomocni->desno->roditelj=cvor;
            pomocni->desno=nullptr;
            jelImaLijevo=true;
        }
        pomocni->roditelj=cvor->roditelj;
        if(cvor->roditelj){
            cvor->roditelj=pomocni;
        }
        else {
            cvor->roditelj=pomocni;
            korijen=pomocni;
        }
        pomocni->desno=cvor;
        pomocni->balans=0;
        cvor->balans=0;
        if(!jelImaLijevo){
            cvor->lijevo=nullptr;
        }
    }
    void rotirajUlijevo(AVLCvor<TipKljuca, TipVrijednosti>* &cvor) {
        if(cvor->roditelj && cvor->roditelj->lijevo==cvor) {
            cvor->roditelj->lijevo=cvor->desno;
        }
        else if(cvor->roditelj) {
            cvor->roditelj->desno=cvor->desno;
        }
        AVLCvor<TipKljuca, TipVrijednosti>* pomocni=cvor->desno;
        bool jelImaDesno=false;
        if(cvor->desno->lijevo) {
            cvor->desno=pomocni->lijevo;
            pomocni->lijevo->roditelj=cvor;
            pomocni->lijevo=nullptr;
            jelImaDesno=true;
        }
        pomocni->roditelj=cvor->roditelj;
        if(cvor->roditelj){
            cvor->roditelj=pomocni;
        }
        else {
            cvor->roditelj=pomocni;
            korijen=pomocni;
        }
        pomocni->lijevo=cvor;
        cvor->balans=0;
        pomocni->balans=0;
        if(!jelImaDesno){
            cvor->desno=nullptr;
        }
    }
    TipVrijednosti& dodavanje(TipKljuca k){
        AVLCvor<TipKljuca, TipVrijednosti> *tmp=new AVLCvor<TipKljuca, TipVrijednosti>(k, TipVrijednosti{});
        this->Umetni(tmp);
        return tmp->vrijednost;
    }
    void brisi(AVLCvor<TipKljuca, TipVrijednosti>* tmp){
        if(tmp){
            brisi(tmp->lijevo);
            brisi(tmp->desno);
            delete tmp;
        }
        korijen=nullptr;
    }
    void Preorder(AVLCvor<TipKljuca, TipVrijednosti>* tmp){
        if(tmp!=0){
            this->dodavanje(tmp->kljuc)=tmp->vrijednost;
            Preorder(tmp->lijevo);
            Preorder(tmp->desno);
        }
    }
public:
    AVLStabloMapa(){
        korijen=nullptr;
        broj_elemenata=0;
    }
    AVLStabloMapa(const AVLStabloMapa<TipKljuca, TipVrijednosti> &stablo){
        korijen=nullptr;
        broj_elemenata=0;
        AVLCvor<TipKljuca, TipVrijednosti> *pomocni = stablo.korijen;
        Preorder(pomocni);
        
    }
    AVLStabloMapa& operator= (const AVLStabloMapa<TipKljuca, TipVrijednosti> &stablo){
        if(this==&stablo) return *this;
        this->obrisi();
        korijen=nullptr;
        AVLCvor<TipKljuca, TipVrijednosti>* pomocni=stablo.korijen;
        Preorder(pomocni);
        return *this;
    }
    ~AVLStabloMapa<TipKljuca, TipVrijednosti>(){
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
        
        AVLCvor<TipKljuca, TipVrijednosti> *pomocni=new AVLCvor<TipKljuca, TipVrijednosti>(k, TipVrijednosti{});
        this->Umetni(pomocni);
        return pomocni->vrijednost;
    }
    void obrisi(){
        brisi(korijen);
        broj_elemenata=0;
    }
    void obrisi(const TipKljuca &k){
        AVLCvor<TipKljuca, TipVrijednosti> *p=korijen, *q=0;
        AVLCvor<TipKljuca, TipVrijednosti> *tmp=0, *pp=0, *rp=0;
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
            p->desno->roditelj=rp;
        }
        if(q==0) {
            korijen=rp;
            if(korijen) korijen->roditelj=nullptr;
        }
        else if(p==q->lijevo) {
            q->lijevo=rp;
            if(rp) rp->roditelj=q;
        }
        else {
            q->desno=rp;
            if(rp) rp->roditelj=q;
        }
        if(p->roditelj && p->roditelj->lijevo==p) p->roditelj->balans--;
        else if(p->roditelj) p->roditelj->balans++;
        delete p;
        broj_elemenata--;
    }
    int brojElemenata() const{
        return broj_elemenata;
    }
};

int main() {
    
    //U nastavku slijedi testiranje efikanosti BSM i AVL
    BinStabloMapa<int, int> bsm;
    AVLStabloMapa<int, int> avl;
    
    //Testirajmo prvo dodavanje clanova u ove mape
    
    clock_t vrijeme1;
    clock_t vrijeme2;
    int ukvrijeme;
    int velicina=5000;
    
    //BSM
    vrijeme1=clock();
    
    for(int i=0; i<velicina; i++) {
        bsm[i]=i;
    }
    
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    
    std::cout << "Vrijeme potrebno za dodavanje 1000 elemenata u BSM je " << ukvrijeme << " ms." << std::endl;
    
    //AVL
    vrijeme1=clock();
    
    for(int i=0; i<velicina; i++) {
        avl[i]=i;
    }
    
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    
    std::cout << "Vrijeme potrebno za dodavanje 1000 elemenata u AVL je " << ukvrijeme << " ms." << std::endl;
    
    //Testirajmo sada brisanje elemenata u BSM i AVL
    
    //BSM
    vrijeme1=clock();
    
    for(int i=0; i<velicina; i++) {
        bsm.obrisi(i);
    }
    
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    
    std::cout << "Vrijeme potrebno za brisanje 1000 elemenata u BSM je " << ukvrijeme << " ms." << std::endl;
    
    //AVL
    vrijeme1=clock();
    
    for(int i=0; i<velicina; i++) {
        avl.obrisi(i);
    }
    
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    
    std::cout << "Vrijeme potrebno za brisanje 1000 elemenata u AVL je " << ukvrijeme << " ms." << std::endl;
    
    //Vidimo koliko je efikasnija AVL u odnosu na BSM, pogotovo kod dodavanja elemenata
    return 0;
}
