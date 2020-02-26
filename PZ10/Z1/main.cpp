#include <iostream>
#include <vector>

using namespace std;

template <typename TipOznake>
class Grana;
template <typename TipOznake>
class Cvor;
template <typename TipOznake>
class GranaIterator;

template <typename TipOznake>
class UsmjereniGraf {
public:
    UsmjereniGraf(int brojCvorova) {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const=0;
    virtual void postaviBrojCvorova(int brojCvorova)=0;
    virtual void dodajGranu(int polazni, int dolazni, float tezina)=0;
    virtual void obrisiGranu(int polazni, int dolazni)=0;
    virtual void postaviTezinuGrane(int polazni, int dolazni, float tezina)=0;
    virtual float dajTezinuGrane(int polazni, int dolazni)=0;
    virtual bool postojiGrana(int polazni, int dolazni)=0;
    virtual void postaviOznakuCvora(int cvor, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuCvora(int cvor) const=0;
    virtual void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuGrane(int polazni, int dolazni) const=0;
    Grana<TipOznake> dajGranu(int polazni, int dolazni) {
        return Grana<TipOznake>(this, polazni, dolazni);
    }
    Cvor<TipOznake> dajCvor(int cvor) {
        return Cvor<TipOznake>(this, cvor);
    }
    virtual GranaIterator<TipOznake> dajGranePocetak() =0;
    virtual GranaIterator<TipOznake> dajGraneKraj() =0;
    virtual GranaIterator<TipOznake> dajSljedecuGranu(int polazni, int dolazni) =0;
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
        /*Podaci Grane nepostojeca;
        nepostojeca.postoji=false;
        
        matrica.resize(brojCvorova);
        for(int i=0; i<brojCvorova; i++){
            matrica[i].resize(brojCvorova, nepostojeca);
        }*/
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
        
        //matrica.resize(brojCvorova);
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
        if(polazni == -1 && dolazni == -1) {
            throw "iterator pokazuje iza kraja";
        }
        GranaIterator g = graf -> dajSljedecuGranu(polazni, dolazni);
        polazni = g.polazni;
        dolazni = g.dolazni;
        return *this;
    }
    GranaIterator operator++(int) {
        GranaIterator tmp(*this);
        ++(*this);
        return tmp;
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

void TestMatricaGraf() {
    //napravimo graf prvo
    UsmjereniGraf<bool> *g1 = new MatricaGraf<bool> (8);
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
    std::cout << "BFS" << std::endl;
    UsmjereniGraf<bool> *g2 = new MatricaGraf<bool> (4);
    g2->dodajGranu(0, 1, 2.3);
    g2->dodajGranu(0, 2, 4.2);
    g2->dodajGranu(1, 2, 1.5);
    g2->dodajGranu(2, 0, 1.2);
    g2->dodajGranu(2, 3, 1.7);
    g2->dodajGranu(3, 3, 2.6);
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
    std::cout << "DFS" << std::endl;
    UsmjereniGraf<bool> *g3 = new MatricaGraf<bool> (4);
    g3->dodajGranu(0, 1, 2.3);
    g3->dodajGranu(0, 2, 4.2);
    g3->dodajGranu(1, 2, 1.5);
    g3->dodajGranu(2, 0, 1.2);
    g3->dodajGranu(2, 3, 1.7);
    g3->dodajGranu(3, 3, 2.6);
    //testirajmo DFS
    std::vector<Cvor<bool> > dfs_vek;
    dfs(g3, dfs_vek, g3->dajCvor(0));
    for (int i=0; i<dfs_vek.size(); i++) {
        std::cout << dfs_vek[i].dajRedniBroj() << " ";
    }
    std::cout << std::endl;
    delete g3;
}

void TestBFS2() {
    std::cout << "BFS2" << std::endl;
    UsmjereniGraf<bool> *g4 = new MatricaGraf<bool> (8);
    g4->dodajGranu(0, 1, 1.2);
    g4->dodajGranu(0, 2, 3.2);
    g4->dodajGranu(0, 3, 1.7);
    g4->dodajGranu(1, 4, 4.2);
    g4->dodajGranu(2, 5, 3.3);
    g4->dodajGranu(3, 6, 5.1);
    g4->dodajGranu(4, 7, 3.1);
    //testirajmo BFS
    std::vector<Cvor<bool> > bfs_vek;
    bfs(g4, bfs_vek, g4->dajCvor(0));
    for (int i=0; i<bfs_vek.size(); i++) {
        std::cout << bfs_vek[i].dajRedniBroj() << " ";
    }
    std::cout << std::endl;
    delete g4;
}

void TestDFS2() {
    std::cout << "DFS2" << std::endl;
    UsmjereniGraf<bool> *g5 = new MatricaGraf<bool> (8);
    g5->dodajGranu(0, 1, 1.2);
    g5->dodajGranu(0, 2, 3.2);
    g5->dodajGranu(0, 3, 1.7);
    g5->dodajGranu(1, 4, 4.2);
    g5->dodajGranu(2, 5, 3.3);
    g5->dodajGranu(3, 6, 5.1);
    g5->dodajGranu(4, 7, 3.1);
    //testirajmo DFS
    std::vector<Cvor<bool> > dfs_vek;
    bfs(g5, dfs_vek, g5->dajCvor(0));
    for (int i=0; i<dfs_vek.size(); i++) {
        std::cout << dfs_vek[i].dajRedniBroj() << " ";
    }
    std::cout << std::endl;
    delete g5;
}

int main() {
    
    std::cout << "Uradimo nekoliko testova:" << std::endl;
    
    TestMatricaGraf();
    TestBFS();
    TestDFS();
    TestBFS2();
    TestDFS2();
    
    std::cout << std::endl;
    
    //Ispod su autotestovi, samo ubačeni u main za isprobavanje
    
    //Testiranje iteratora
    try{
        UsmjereniGraf<bool> *g = new MatricaGraf<bool> (3);
        g->dodajGranu(0, 1, 2.5);
        g->dodajGranu(0, 0, 1.2);
        g->dodajGranu(1, 2, 0.1);
        g->dodajGranu(0, 0, 3.14);
        for(GranaIterator<bool> iter = g->dajGranePocetak(); iter != g->dajGraneKraj(); ++iter){
            std::cout << (*iter).dajPolazniCvor().dajRedniBroj() << " "
                      << (*iter).dajDolazniCvor().dajRedniBroj() << " "
                      << (*iter).dajTezinu() << std::endl;
        }
        delete g;
    } catch(...) {
        std::cout << "Izuzetak!" << std::endl;
    }
    std::cout << std::endl;
    //Testiranje BFS pretrage
    UsmjereniGraf<bool> *g1 = new MatricaGraf<bool>(6);
    g1->dodajGranu(0, 1, 2.5);
    g1->dodajGranu(1, 2, 1.2);
    g1->dodajGranu(1, 3, 0.1);
    g1->dodajGranu(2, 4, 3.14);
    g1->dodajGranu(2, 3, 2.73);
    g1->dodajGranu(3, 5, 1);
    g1->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > bfs_obilazak;
    bfs(g1, bfs_obilazak, g1->dajCvor(0));
    for (int i = 0; i < bfs_obilazak.size(); i++)
      cout << bfs_obilazak[i].dajRedniBroj() << ",";
    delete g1;
    std::cout << std::endl;
    //Testiranje DFS pretrage
    UsmjereniGraf<bool> *g2 = new MatricaGraf<bool>(6);
    g2->dodajGranu(0, 1, 2.5);
    g2->dodajGranu(1, 2, 1.2);
    g2->dodajGranu(1, 3, 0.1);
    g2->dodajGranu(2, 4, 3.14);
    g2->dodajGranu(2, 3, 2.73);
    g2->dodajGranu(3, 5, 1);
    g2->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > dfs_obilazak;
    dfs(g2, dfs_obilazak, g2->dajCvor(0));
    for (int i = 0; i < dfs_obilazak.size(); i++)
      cout << dfs_obilazak[i].dajRedniBroj() << ",";
    delete g2;
    std::cout << std::endl;
    return 0;
}
