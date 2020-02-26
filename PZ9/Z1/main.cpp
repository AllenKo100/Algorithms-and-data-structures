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

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
    int kapacitet;
    int broj_elemenata;
    std::pair<TipKljuca, TipVrijednosti> *hniz;
    unsigned int (*hfunkcija)(TipKljuca kljuc, unsigned int opseg) = 0;
public:
    HashMapa(){
        kapacitet=1000;
        broj_elemenata=0;
        hniz=nullptr;
    }
    HashMapa(HashMapa<TipKljuca, TipVrijednosti> &hmapa){
        kapacitet=hmapa.kapacitet;
        broj_elemenata=hmapa.broj_elemenata;
        hfunkcija=hmapa.hfunkcija;
        hniz=new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
        for(int i=0; i<hmapa.kapacitet; i++){
            hniz[i]=hmapa.hniz[i];
        }
    }
    HashMapa &operator= (HashMapa<TipKljuca, TipVrijednosti> &hmapa){
        if(this==&hmapa) return *this;
        delete[] hniz;
        kapacitet=hmapa.kapacitet;
        broj_elemenata=hmapa.broj_elemenata;
        hfunkcija=hmapa.hfunkcija;
        hniz=new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
        for(int i=0; i<hmapa.kapacitet; i++){
            hniz[i]=hmapa.hniz[i];
        }
        return *this;
    }
    ~HashMapa(){
        this->obrisi();
    }
    void obrisi(){
        delete[] hniz;
        kapacitet=1000;
        broj_elemenata=0;
        hniz=nullptr;
    }
    void obrisi(const TipKljuca &k){
        if(broj_elemenata==0){
            throw std::domain_error("Trenutno nema elemenata u mapi!");
        }
        unsigned int indeks=hfunkcija(k, kapacitet);
        int tmp=-1;
        if(hniz[indeks].first==k){
            tmp=indeks;
        }
        if(tmp==-1){
            for(int i=indeks+1; i<kapacitet; i++){
                if(hniz[i].first==k){
                    tmp=i;
                    break;
                }
            }
        }
        if(tmp==-1){
            for(int i=0; i<indeks; i++){
                if(hniz[i].first==k){
                    tmp=i;
                    break;
                }
            }
        }
        if(tmp==-1){
            throw std::domain_error("Taj element nije u mapi!");
        }
        hniz[tmp].first=TipKljuca();
        hniz[tmp].second=TipVrijednosti();
        
        broj_elemenata--;
    }
    int brojElemenata() const{
        return broj_elemenata;
    }
    void definisiHashFunkciju(unsigned int (*funkcija)(TipKljuca, unsigned int)){
        if(funkcija==nullptr){
            throw std::domain_error("Nije definisana funkcija!");
        }
        hfunkcija=funkcija;
    }
    TipVrijednosti operator[] (TipKljuca k) const{
        unsigned int indeks=hfunkcija(k, kapacitet);
        if(hniz[indeks].first==k){
            return hniz[indeks].second;
        }
        for(int i=indeks+1; i<kapacitet; i++){
            if(hniz[i].first==k) return hniz[i].second;
        }
        for(int i=0; i<indeks; i++){
            if(hniz[i].first==k) return hniz[i].second;
        }
        
        return TipVrijednosti();
    }
    TipVrijednosti &operator[] (TipKljuca k){
        if(hfunkcija==nullptr){
            throw std::domain_error("Nije definisana hash funkcija!");
        }
        unsigned int indeks=hfunkcija(k, kapacitet);
        if(broj_elemenata==0){
            //Ako nema elemenata u mapi, moramo je kreirat
            hniz=new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
            hniz[indeks].first=k;
            hniz[indeks].second=TipVrijednosti();
            broj_elemenata++;
            for(int i=0; i<kapacitet; i++){
                if(i!=indeks) hniz[i].first=TipKljuca();
            }
            return hniz[indeks].second;
        }
        else{
            if(hniz[indeks].first==k){
                return hniz[indeks].second;
            }
            for(int i=indeks+1; i<kapacitet; i++){
                if(hniz[i].first==k){
                    return hniz[i].second;
                }
            }
            for(int i=0; i<indeks; i++){
                if(hniz[i].first==k){
                    return hniz[i].second;
                }
            }
            //Sada pravimo za slucaj ako nema elementa
            //sa poslanim kljucem u mapi
            for(int i=indeks+1; i<kapacitet; i++){
                if(hniz[i].first==TipKljuca()){
                    hniz[i].first=k;
                    hniz[i].second=TipVrijednosti();
                    broj_elemenata++;
                    return hniz[i].second;
                    //fakticki vracamo podrazumijevanu vrijednost
                    //za TipVrijednosti ako nije prije bilo tog elementa
                }
            }
            for(int i=0; i<indeks; i++){
                if(hniz[i].first==TipKljuca()){
                    hniz[i].first=k;
                    hniz[i].second=TipVrijednosti();
                    broj_elemenata++;
                    return hniz[i].second;
                }
            }
            //Sada moramo obraditi jos slucaj ukoliko je popunjen kapacitet
            //To podrazumijeva da uradimo realokaciju te mape, sa povecanim kapacitetom
            if(broj_elemenata==kapacitet){
                kapacitet=kapacitet*2;
                std::pair<TipKljuca, TipVrijednosti> *pomocni=new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
                for(int i=0; i<broj_elemenata; i++){
                    pomocni[i].first=hniz[i].first;
                    pomocni[i].second=hniz[i].second;
                }
                delete[] hniz;
                hniz=pomocni;
                pomocni=nullptr;
            }
            hniz[broj_elemenata].first=k;
            hniz[broj_elemenata].second=TipVrijednosti();
            broj_elemenata++;
            return hniz[broj_elemenata-1].second;
        }
    }
};

unsigned int pomocnaHFunkcija(int ulaz, unsigned int max){
    /*unsigned int suma=5381;
    for(int i=0; i<ulaz; i++){
        suma=suma*33+ulaz;
    }
    return suma%max;*/
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
    //Testiranje performansi NizMapa, BinStabloMapa, te HashMapa
    NizMapa<int, int> nm;
    BinStabloMapa<int, int> bsm;
    HashMapa<int, int> hm;
    hm.definisiHashFunkciju(pomocnaHFunkcija);
    std::vector<int> vek;
    
    //Petlja za stavljanje 10000 random brojeva u jedan vektor 
    for(int i=0; i<10000; i++){
        vek.push_back(rand()&10000);
    }
    
    std::cout << "Slijede rezultati testiranja performansi NizMapa, BinStabloMapa, te HashMapa:" << std::endl;
    
    clock_t vrijeme1;
    clock_t vrijeme2;
    int ukvrijeme;
    
    //Testiranje stavljanje elemenata u mape
    
    //stavljanje elemenata u NizMapa
    vrijeme1=clock();
    for(int i=0; i<10000; i++){
        nm[vek[i]]=i;
    }
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za dodavanje 10000 elemenata u NizMapa je " << ukvrijeme << " milisekundi." << std::endl;
    
    //stavljanje elemenata u BinStabloMapa
    vrijeme1=clock();
    for(int i=0; i<10000; i++){
        bsm[vek[i]]=i;
    }
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za dodavanje 10000 elemenata u BinStabloMapa je " << ukvrijeme << " milisekundi." << std::endl;
    
    //stavljanje elemenata u HashMapa
    vrijeme1=clock();
    for(int i=0; i<10000; i++){
        hm[vek[i]]=i;
    }
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za dodavanje 10000 elemenata u HashMapa je " << ukvrijeme << " milisekundi." << std::endl;
    
    //Testiranje brzine pristupa svim clanovima mapa
    
    //pristup clanovima u NizMapa
    vrijeme1=clock();
    for(int i=0; i<10000; i++){
        nm[vek[i]];
    }
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za pristup svim clanovima u NizMapa je " << ukvrijeme << " milisekundi." << std::endl;
    
    //pristup clanovima u BinStabloMapa
    vrijeme1=clock();
    for(int i=0; i<10000; i++){
        bsm[vek[i]];
    }
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za pristup svim clanovima u BinStabloMapa je " << ukvrijeme << " milisekundi." << std::endl;
    
    //pristup clanovima u HashMapa
    vrijeme1=clock();
    for(int i=0; i<10000; i++){
        hm[vek[i]];
    }
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za pristup svim clanovima u HashMapa je " << ukvrijeme << " milisekundi." << std::endl;
    
    //Testiranje brzine brisanja clanova
    
    //brisanje kod NizMapa
    
    //prvo dodajmo neke clanove, radit cemo za 5000 elemenata
    for(int i=0; i<5000; i++){
        nm[i]=i;
    }
    //sada brisemo
    vrijeme1=clock();
    for(int i=0; i<5000; i++){
        nm.obrisi(i);
    }
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za brisanje 5000 clanova u NizMapa je " << ukvrijeme << " milisekundi." << std::endl;
    
    //brisanje kod BinStabloMapa
    
    //prvo dodajmo neke clanove, radit cemo takodjer sa 10000 elemenata
    for(int i=0; i<5000; i++){
        bsm[i]=i;
    }
    //sada brisemo
    vrijeme1=clock();
    for(int i=0; i<5000; i++){
        bsm.obrisi(i);
    }
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za brisanje 5000 clanova u BinStabloMapa je " << ukvrijeme << " milisekundi." << std::endl;
    
    //brisanje kod HashMapa
    
    //prvo dodajmo kao maloprije
    for(int i=0; i<5000; i++){
        hm[i]=i;
    }
    //sada brisemo
    vrijeme1=clock();
    for(int i=0; i<5000; i++){
        try{
            hm.obrisi(i);
        } catch(...){
            
        }
    }
    vrijeme2=clock();
    
    ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme potrebno za brisanje 5000 clanova u HashMapa je " << ukvrijeme << " milisekundi." << std::endl;
    
    std::cout << "To bi bili rezultati za sve mape urađene do sad, te sada možemo uporediti performanse, a komentar se nalazi na kraju koda" << std::endl;
    
    //Kada pogledamo rezultate testiranja koje smo dobili za dodavanje elemenata, ta razlika za NizMapu i BinStabloMapu nije velika, međutim s povećanjem n ona će rasti, 
    //jer je kompleksnost BinStabloMape O(logn), dok je kod NizMape O(n). Kod HashMape je situacija slabija, što je vjerovatno posljedica kolizija i heš funkcije koju smo izabrali, što dosta uspori dodavanje,
    //ali naravno to sve ovisi od heš funkcije koju koristimo
    
    //Pristup elementima je opet najbrza BinStabloMapa, međutim to se sve može poboljšati kod HashMapa u zavisnosti od HashFunkcije
    //funkcija koju smo sad odabrali nije bas efikasna, te zbog toga vjerovatno imamo i kolizija više
    //dok kod NizMape je situacija malo slabija nego kod BinStabloMape, a malo bolja od HashMape
    
    //Brisanje je također najbrže kod BinStabloMape, dok je HashMapa tu na drugom mjestu, čime je brža kod brisanja od NizMape, ali sve zavisi od brzine nalaska hash elementa
    
    return 0;
}
