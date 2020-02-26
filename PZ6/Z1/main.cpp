#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>

//Selection Sort implementiran
template <typename Tip>
void selection_sort(Tip* niz, int vel)
{
    int min_index;

    for(int i=0; i<vel-1; i++) {
        min_index=i;
        for(int j=i+1; j<vel; j++) {
            if(niz[j]<niz[min_index]) {
                min_index=j;
            }
        }

        Tip temp=niz[min_index];
        niz[min_index]=niz[i];
        niz[i]=temp;
    }
}

//Bubble Sort implementiran
template <typename Tip>
void bubble_sort(Tip* niz, int vel)
{
    for(int i=0; i<vel-1; i++) {
        for(int j=0; j<vel-i-1; j++) {
            if(niz[j]>niz[j+1]) {
                Tip temp=niz[j];
                niz[j]=niz[j+1];
                niz[j+1]=temp;
            }
        }
    }
}

//U nastavku implementiran Quick Sort, te neke pomocne funkcije potrebne za njega
template <typename Tip>
int Particija(Tip* niz, int najmanji, int najveci)
{
    Tip pivot=niz[najmanji];
    int p=(najmanji+1);

    while(p<=najveci) {
        if(niz[p]<=pivot) {
            p++;
        } else {
            break;
        }
    }

    for(int i=p+1; i<=najveci; i++) {
        if(niz[i]<pivot) {
            Tip temp=niz[p];
            niz[p]=niz[i];
            niz[i]=temp;
            p++;
        }
    }
    Tip temp=niz[najmanji];
    niz[najmanji]=niz[p-1];
    niz[p-1]=temp;

    return p-1;
}

template <typename Tip>
void quickSort(Tip* niz, const int najmanji, const int najveci)
{
    if(najmanji<najveci) {
        int p=Particija(niz, najmanji, najveci);

        quickSort(niz, najmanji, p-1);
        quickSort(niz, p+1, najveci);
    }

}

template <typename Tip>
void quick_sort(Tip* niz, int vel)
{
    quickSort(niz, 0, vel-1);
}
//Kraj Quick Sorta

//U nastavku implementiran Merge Sort, te neke pomocne funkcije potrebne za njega
template <typename Tip>
void Merge(Tip* niz, int l, int p, int q, int u)
{
    int i=0;
    int j=q-l;
    int k=l;
    Tip *pomocni=new Tip[u+1-l];
    for(int m=0; m<=u-l; m++) {
        pomocni[m]=niz[l+m];
    }
    while(i<=p-l && j<=u-l) {
        if(pomocni[i]<pomocni[j]) {
            niz[k]=pomocni[i++];
        } else {
            niz[k]=pomocni[j++];
        }
        k++;
    }
    while(i<=p-l) {
        niz[k++]=pomocni[i++];
    }
    while(j<=u-l) {
        niz[k++]=pomocni[j++];
    }
    delete[] pomocni;
}

template <typename Tip>
void MergeSort(Tip* niz, int pocetak, int kraj)
{
    if(pocetak<kraj) {
        int p=(pocetak+kraj-1)/2;
        int q=p+1;

        //radimo Merge Sort rekurzivno, tako sto dijelimo niz
        MergeSort(niz, pocetak, p);
        MergeSort(niz, q, kraj);
        //spajamo sortirane nizove u jedan
        Merge(niz, pocetak, p, q, kraj);
    }
}

template <typename Tip>
void merge_sort(Tip* niz, int vel)
{
    MergeSort(niz, 0, vel-1);
}
//Kraj Merge Sorta

void ucitaj(std::string filename, int*& niz, int &vel)
{
    //definisimo ulazni tok za ovu funkciju
    std::ifstream tok(filename);
    int trenutniBroj;
    //napravit cemo vektor u koji cemo smjestat brojeve iz datoteke
    std::vector<int> v;
    //kupimo sve brojeve iz datoteke
    while(tok >> trenutniBroj) {
        v.push_back(trenutniBroj);
    }
    int velicina=v.size();
    int *pomocni=new int[velicina];
    for(int i=0; i<velicina; i++) {
        pomocni[i]=v.at(i);
    }
    niz=pomocni;
    vel=velicina;
}

void generisi(std::string filename, int vel)
{
    //definisimo izlazni tok za ovu funkciju
    std::ofstream tok(filename);
    for(int i=0; i<vel; i++) {
        tok << rand() << " ";
    }
}

template <typename Tip>
bool TestSortiranja(Tip *niz, int vel){
    for(int i=0; i<vel-1; i++){
        if(niz[i+1]<niz[i]) return false;
    }
    return true;
}

bool TestBubble(){
    int niz[5]={1,5,2,3,6};
    bubble_sort(niz, 5);
    for(int i=0; i<4; i++){
        if(niz[i]>niz[i+1]) return false;
    }
    return true;
}

bool TestSelection(){
    int niz[5]={7,4,3,11,6};
    selection_sort(niz, 5);
    for(int i=0; i<4; i++){
        if(niz[i]>niz[i+1]) return false;
    }
    return true;
}

bool TestQuick(){
    int niz[5]={10,12,100,11,6};
    quick_sort(niz, 5);
    for(int i=0; i<4; i++){
        if(niz[i]>niz[i+1]) return false;
    }
    return true;
}

bool TestMerge(){
    int niz[5]={1,11,10,500,2};
    merge_sort(niz, 5);
    for(int i=0; i<4; i++){
        if(niz[i]>niz[i+1]) return false;
    }
    return true;
}

void Program()
{
    int *niz;
    int vel;
    
    ucitaj("datoteka", niz, vel);
    
    int opcija;
    std::cout << "Dobrodošli u program \"Sortiranje niza\", odaberite koji sort želite koristiti (ukucajte odgovarajući broj), na raspolaganju su sljedeći:" << std::endl;
    std::cout << "1.Bubble sort" << std::endl;
    std::cout << "2.Selection sort" << std::endl;
    std::cout << "3.Quick sort" << std::endl;
    std::cout << "4.Merge sort" << std::endl;
    std::cout << "Unesite svoju opciju: ";
    std::cin >> opcija;
    std::cout << std::endl;
    
    clock_t vrijeme1;
    clock_t vrijeme2;
    
    switch(opcija){
        case 1:
            vrijeme1=clock();
            bubble_sort(niz, vel);
            vrijeme2=clock();
            break;
        case 2:
            vrijeme1=clock();
            selection_sort(niz, vel);
            vrijeme2=clock();
            break;
        case 3:
            vrijeme1=clock();
            quick_sort(niz, vel);
            vrijeme2=clock();
            break;
        case 4:
            vrijeme1=clock();
            merge_sort(niz, vel);
            vrijeme2=clock();
            break;
        default:
            std::cout << "Neispravan unos!" << std::endl;
    }
    int ukvrijeme=(vrijeme2-vrijeme1)/(CLOCKS_PER_SEC/1000);
    std::cout << "Ukupno vrijeme izvrsavanja sortiranja je: " << ukvrijeme << " ms" << std::endl;
    
    std::cout << "Provjerimo sada da li je niz zaista sortiran: " << std::endl;
    if(TestSortiranja(niz, vel)){
        std::cout << "Sortiranje uspjesno!" << std::endl;
    }
    else{
        std::cout << "Sortiranje neuspjesno!" << std::endl;
    }
    
    std::ofstream tok("novi");
    for(int i=0; i<vel; i++){
        tok << niz[i] << " ";
    }
    
    delete[] niz;
}

int main(){
    Program();
    
    std::cout << std::endl;
    std::cout << "Evo jos nekoliko testova" << std::endl;
    if(TestBubble() && TestSelection() && TestQuick()){
        std::cout << "Svi testovi prosli!" << std::endl;
    }
    else{
        std::cout << "Neki testovi nisu prosli!" << std::endl;
    }
    return 0;
}