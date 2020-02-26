#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//Nije naglaseno da ne mozemo koristiti swap iz algorithm, ali napravit cemo mi nasu swapFunkciju
template <typename Tip>
void swapFunkcija(Tip& x, Tip& y){
    Tip temp=x;
    x=y;
    y=temp;
}

//Prvi zadatak sa zadaće koji traži da se implementira Radix Sort
int dajMaksimalni(vector<int> &a){
    int maks=a[0];
    for(int i=1; i<a.size(); i++){
        if(a[i]>maks){
            maks=a[i];
        }
    }
    return maks;
}

void countSort(vector<int> &a, int exp){
    int n=a.size();
    vector<int> pomocni(a.size());
    int i, count[10]={0};
    
    for(i=0; i<n; i++){
        count[(a[i]/exp)%10]++;
    }
    
    for(i=1; i<10; i++){
        count[i]+=count[i-1];
    }
    
    for(i=n-1; i>=0; i--){
        pomocni[count[(a[i]/exp)%10]-1]=a[i];
        count[(a[i]/exp)%10]--;
    }
    
    for(i=0; i<n; i++){
        a[i]=pomocni[i];
    }
}

void radixSort(vector<int> &a){
    int m=dajMaksimalni(a);
    
    for(int exp=1; m/exp>0; exp*=10){
        countSort(a, exp);
    }
}
//Kraj prvog zadatka

//Drugi zadatak sa zadaće u kojem ćemo implementirati neke funkcije sa Gomilom
//Unutar ovog bloka su također pomoćne funkcije tipa popraviDolje i popraviGore
//koje ćemo korisiti za implementiranje ostalih kao što su stvoriGomilu
void popraviDolje(vector<int> &a, int i){
    while(i<a.size()/2){//JeLiList
        int veci=2*i+1;
        int dd=2*i+2;
        if(dd<a.size() && a[dd]>a[veci]){
            veci=dd;
        }
        if(a[i]>a[veci]) return;
        //std::swap(a[i],a[veci]);//koristimo swap jer je dozvoljeno koristenje algorithm biblioteke
        swapFunkcija(a[i],a[veci]);
        i=veci;
    }
}

void popraviGore(vector<int> &a, int i){
    while(i!=0 && a[i]>a[(i-1)/2]){     //(i-1)/2 nam ustvari predstavlja Roditelj
        //std::swap(a[i], a[(i-1)/2]);
        swapFunkcija(a[i], a[(i-1)/2]);
        i=(i-1)/2;
    }
}

void stvoriGomilu(vector<int> &a){
    int velicina=a.size();
    for(int i=velicina/2; i>=0; i--){
        popraviDolje(a,i);
    }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina){
    a.push_back(umetnuti);
    velicina++;
    popraviGore(a, a.size()-1);
}

int izbaciPrvi(vector<int> &a, int &velicina){
    if(a.size()==0) throw "Gomila je prazna!";
    int pomocni=a[0];
    velicina--;
    //std::swap(a[0], a[velicina]);
    swapFunkcija(a[0], a[velicina]);
    vector<int> vek(velicina);
    for(int i=0; i<vek.size(); i++){
        vek[i]=a[i];
    }
    stvoriGomilu(vek);
    for(int i=0; i<vek.size(); i++){
        a[i]=vek[i];
    }
    return pomocni;
}

void gomilaSort(vector<int> &a){
    stvoriGomilu(a);
    vector<int> pomocni(a.size());
    int velicina=a.size();
    for(int i=0; i<velicina; i++){
        pomocni[i]=a[0];
        a.erase(a.begin());
        stvoriGomilu(a);
    }
    for(int i=0; i<velicina; i++){
        a.push_back(pomocni[velicina-1-i]);      //idemo od kraja prema prvom da obrnemo poredak
    }
}
//Kraj drugog zadatka

void Test1() {              //test radixSorta
    vector<int> a;
    a.push_back(5);
    a.push_back(4);
    a.push_back(3);
    a.push_back(2);
    a.push_back(1);
    cout << "Niz prije sortiranja: " << endl;
    for(int i=0; i<a.size(); i++) cout << a[i] << " ";
    cout << endl;
    cout << "Niz poslije sortiranja: " << endl;
    radixSort(a);
    for(int i=0; i<a.size(); i++) cout << a[i] << " ";
    cout << endl;
}

void Test2() {              //test stvoriGomilu
    vector<int> a;
    a.push_back(111);
    a.push_back(23);
    a.push_back(153);
    a.push_back(942);
    a.push_back(333);
    a.push_back(904);
    stvoriGomilu(a);
    cout << "Gomila vectora a izgleda ovako: " << endl;
    for(int i=0; i<a.size(); i++) cout << a[i] << " ";
    cout << endl;
}

void Test3() {              //test gomilaSort
    vector<int> a;
    a.push_back(425);
    a.push_back(132);
    a.push_back(536);
    a.push_back(9999);
    a.push_back(12);
    a.push_back(154);
    gomilaSort(a);
    cout << "Gomila nakon gomilaSort: " << endl;
    for(int i=0; i<a.size(); i++) cout << a[i] << " ";
    cout << endl;
}

void Test4() {              //test umetniUGomilu
    vector <int> a;
    a.push_back(674);
    a.push_back(32);
    a.push_back(4367);
    a.push_back(1532);
    a.push_back(24);
    a.push_back(1);
    a.push_back(876);
    a.push_back(980);
    int velicina=a.size();
    stvoriGomilu(a);
    umetniUGomilu(a, 101010, velicina);
    umetniUGomilu(a, 232323, velicina);
    umetniUGomilu(a, 4367, velicina);
    umetniUGomilu(a, 54312, velicina);
    cout << "Gomila prije sortiranja: " << endl;
    for(int i=0; i<velicina; i++) cout << a[i] << " ";
    cout << endl;
    cout << "Gomila poslije sortiranja: " << endl;
    gomilaSort(a);
    for(int i=0; i<velicina; i++) cout << a[i] << " ";
    cout << endl;
}

void Test5() {              //test izbaciPrvi
    vector<int> a;
    a.push_back(468);
    a.push_back(31);
    a.push_back(980);
    a.push_back(16354);
    a.push_back(3324);
    a.push_back(0);
    a.push_back(167);
    a.push_back(7098);
    int velicina=a.size();
    stvoriGomilu(a);
    cout << "Gomila prije izbacivanja prvog: " << endl;
    for(int i=0; i<velicina; i++) cout << a[i] << " ";
    cout << endl;
    izbaciPrvi(a, velicina);
    cout << "Gomila nakon izbacivanja prvog: " << endl;
    for(int i=0; i<velicina; i++) cout << a[i] << " ";
    cout << endl;
}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}
