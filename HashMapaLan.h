#ifndef HASHMAPALAN_H_INCLUDED
#define HASHMAPALAN_H_INCLUDED

#include <iostream>
#include "HashMapa.h"
#include "HashMapaLan.h"
#include <string>
#include <vector>

template<typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti>
{
    struct Cvor{
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *sljedeci;

        Cvor(TipKljuca klj, TipVrijednosti vr,  Cvor* slj){
            kljuc = klj;
            vrijednost = vr;
            sljedeci = slj;
        }
    };

    int kapacitet;
    int vel;
    TipVrijednosti podrazumijevanaVrijednost;
    Cvor** kljucevi;

    unsigned int (*hashFunkcija)(TipKljuca ulaz, unsigned int duz);

public:
    HashMapaLan(){
        kapacitet = 2000;
        vel = 0;
        hashFunkcija = nullptr;
        kljucevi = new Cvor*[kapacitet];
        podrazumijevanaVrijednost = TipVrijednosti();

        for(int i(0); i<kapacitet; i++)
            kljucevi[i] = nullptr;
    }

    HashMapaLan(const HashMapaLan &hm){

        vel = hm.vel;
        hashFunkcija = hm.hashFunkcija;
        podrazumijevanaVrijednost = TipVrijednosti();
        kapacitet = hm.kapacitet;
        kljucevi = new Cvor*[kapacitet];

        for(int i(0); i < kapacitet; i++)
            kljucevi[i] = 0;

        for(int i(0); i < kapacitet; i++){
            if(hm.kljucevi[i]){

                Cvor *pom1(hm.kljucevi[i]);
                kljucevi[i] = new Cvor(pom1->kljuc, pom1->vrijednost, 0);

                Cvor* pom2(kljucevi[i]);

                while(pom1->sljedeci){
                    pom1 = pom1->sljedeci;
                    pom2->sljedeci = new Cvor(pom1->kljuc, pom1->vrijednost, 0);
                    pom2 = pom2->sljedeci;
                }
            }
        }
    }

    HashMapaLan &operator =(const HashMapaLan &hm){
        if(&hm == this)
            return *this;

        for(int i(0); i < kapacitet; i++)
            if(kljucevi[i] != nullptr)
                delete kljucevi[i];

        delete[] kljucevi;

        hashFunkcija = hm.hashFunkcija;
        podrazumijevanaVrijednost = TipVrijednosti();
        vel = hm.vel;
        kapacitet = hm.kapacitet;
        kljucevi = new Cvor*[kapacitet];

        for(int i(0); i < kapacitet; i++)
            kljucevi[i] = nullptr;

        for(int i(0); i < kapacitet; i++){
                if(hm.kljucevi[i]){
                    Cvor* pom1(hm.kljucevi[i]);
                    kljucevi[i] = new Cvor(pom1->kljuc, pom1->vrijednost, 0);

                    Cvor* pom2(kljucevi[i]);

                    while(pom1->sljedeci){
                        pom1 = pom1->sljedeci;
                        pom2->sljedeci = new Cvor(pom1->kljuc, pom1->vrijednost, 0);
                        pom2 = pom2->sljedeci;
                    }
                }
            }

        return *this;
    }

    ~HashMapaLan(){

        for(int i(0); i < kapacitet; i++)
            if(kljucevi[i])
                delete kljucevi[i];

        delete[] kljucevi;
    }

    TipVrijednosti operator[](const TipKljuca& kljuc) const{
        if(!hashFunkcija)
            throw "Hash funkcija nije definirana";

        unsigned int indeks(hashFunkcija(kljuc, kapacitet));
        Cvor* pom(kljucevi[indeks]);

        while(pom){
            if(pom->kljuc == kljuc)
                return pom->vrijednost;
            pom = pom->sljedeci;
        }

        return podrazumijevanaVrijednost;
    }

    TipVrijednosti &operator[](const TipKljuca& kljuc){
        if(!hashFunkcija)
            throw "Hash funkcija nije definirana!";

        unsigned int indeks(hashFunkcija(kljuc, kapacitet));

        Cvor* pom(kljucevi[indeks]);

        while(pom){
            if(pom->kljuc == kljuc)
                return pom->vrijednost;
            pom = pom->sljedeci;
        }

        pom = kljucevi[indeks];

        if(!kljucevi[indeks]){
            kljucevi[indeks] = new Cvor(kljuc, TipVrijednosti(), nullptr);
            vel++;
            return kljucevi[indeks]->vrijednost;
        }

        else{
            while(pom->sljedeci && pom->sljedeci->kljuc > kljuc)
                pom = pom->sljedeci;

            Cvor *pom1(pom->sljedeci);

            pom->sljedeci = new Cvor(kljuc, TipVrijednosti(), pom1);
            vel++;

            return pom->sljedeci->vrijednost;
        }
        return podrazumijevanaVrijednost;
    }

    int brojElemenata() const{
        return vel;
    }

    void obrisi(){

        for(int i(0); i<kapacitet; i++){
            if(kljucevi[i] != nullptr)
                delete kljucevi[i];

            kljucevi[i] = nullptr;
        }

        vel = 0;
    }

    void obrisi(const TipKljuca &kljuc){
        if (!hashFunkcija)
            throw "Hash funkcija nije definirana!";

        unsigned int indeks(hashFunkcija(kljuc, kapacitet));

        Cvor* pom(kljucevi[indeks]);

        int br(0);

        while(pom){
            br++;
            pom = pom->sljedeci;
        }

        if(br == 1){
            delete kljucevi[indeks];
            kljucevi[indeks] = nullptr;
            vel--;
            return;
        }

        pom = kljucevi[indeks];

        while(pom->sljedeci && pom->sljedeci->kljuc != kljuc)
            pom = pom->sljedeci;

        Cvor* pom1(pom->sljedeci);
        pom1 = pom1->sljedeci;

        delete pom->sljedeci;
        pom->sljedeci = pom1;

        vel--;
    }

    void definisiHashFunkciju(unsigned int (*f)(TipKljuca ulaz, unsigned int velicina)){
        hashFunkcija = f;
    }

};

#endif // HASHMAPALAN_H_INCLUDED
