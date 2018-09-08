#include <iostream>
#include <string>
#include <vector>
#include "Mapa.h"
#include "HashMapa.h"
#include "HashMapaLan.h"
#include "AVLStabloMapa.h"
#include <ctime>


//funkcija s testova
unsigned int funk(int ulaz, unsigned int max){
	unsigned int suma=23;
	suma = suma*31 + ulaz;
	return suma % max;
}

using namespace std;

int main()
{
    HashMapa<int, int> hm;
    hm.definisiHashFunkciju(funk);

    HashMapaLan<int, int> hml;
    hml.definisiHashFunkciju(funk);

    hm[1] = 10;
    hm[2] = 20;
    hm[3] = 30;
    hm[4] = 40;
    hm[5] = 50;

    cout << hm[1] << " ";
    hm.obrisi(2);

    cout << hm[1] << " " << hm[3] << " " << hm[4] << " " << hm[5] << " "  << hm[2];

    cout << " " << ((hm.brojElemenata() == 5) ? "OK" : "NOT OK") << std::endl;

     //TEST AVLSTABLO

    AVLStabloMapa <int, int> m7;
    m7[10] = 1;
    m7[11] = 2;
    m7[12] = 5;
    m7[13] = 6;
    m7[14] = 4;
    m7[15] = 15;

    for(int i(10); i < 16; i++){
        std::cout << m7[i] << "  ";
    }

    std::cout << ((m7.brojElemenata() == 6) ? "OK" : "NOT OK") << std::endl;

     int vel = 123456;

    clock_t t1 = clock();

    for(int i(0); i < vel; i++){
        hm[i] = 1;
    }

    clock_t t2 = clock();

    clock_t t3 = clock();
    for(int i(0); i < vel; i++){
        hml[i] = 1;
    }
    clock_t t4 = clock();

    auto a = (t2 - t1)/ (CLOCKS_PER_SEC / 1000);
    auto b = (t4 - t3)/ (CLOCKS_PER_SEC / 1000);

    cout << "Dodavanje " << vel << " elemenata u HashMapa je trajalo: " << a << " ms" << endl;
    cout << "Dodavanje " << vel << " elemenata u HashMapaLan je trajalo: " << b << " ms" << endl;

    //TEST ZA IZUZETAK HASHMAPE

    try{
        HashMapa<std::string,std::string> m;
        std::string s1("Sarajevo"), s2("Zagreb");
        m[s1] = "BiH";
        m[s2] = "Hrvatska";
        std::cout << m.brojElemenata() << " " << m[s2];
    }

    catch(const char msg[]){
        cout << msg << endl;
    }

    //TEST ZA IZUZETAK HASHMAPE LAN

     try{
        HashMapaLan<std::string,std::string> hml;
        std::string s1("Sarajevo"), s2("Zagreb");
        hml[s1] = "BiH";
        hml[s2] = "Hrvatska";
        std::cout << hml.brojElemenata() << " " << hml[s2];
    }

    catch(const char msg[]){
        cout << msg << endl;
    }

    return 0;
}
