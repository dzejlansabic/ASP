#ifndef HASHMAPA_H_INCLUDED
#define HASHMAPA_H_INCLUDED
#include "Mapa.h"

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti>
{
    int vel;
    int kapacitet;
    TipKljuca ** kljucevi;
    TipVrijednosti ** vrijednosti;

    TipKljuca podrazumijevaniKljuc;
    TipVrijednosti podrazumijevanaVrijednost;

    unsigned int (*hashFunkcija)(TipKljuca, unsigned int);
    bool *indeksi;
    bool pom;

    void povecajKapacitet(){
        int noviKap(kapacitet * 2);
        int n;

        TipKljuca ** nizK(new TipKljuca*[noviKap]);
        TipVrijednosti **nizV(new TipVrijednosti*[noviKap]);

        bool* pomIndeksi(new bool[noviKap]);

        for(int i(0); i < noviKap; i++)
            pomIndeksi[i] = false;

        for(int i(0); i < kapacitet; i++){

            if(indeksi[i] && *kljucevi[i] != podrazumijevaniKljuc){
                n = hashFunkcija(*kljucevi[i], noviKap);

                if(!pomIndeksi[n]){
                    pomIndeksi[n] = true;
                    nizK[n] = kljucevi[i];
                    nizV[n] = vrijednosti[i];
                }

                else{
                    int broj(1);
                    int j;

                    while(broj != noviKap){
                        j = (n + broj) % noviKap;

                        if(!pomIndeksi[j]){
                            nizK[j] = kljucevi[i];
                            nizV[j] = vrijednosti[i];
                            pomIndeksi[j] = true;
                            broj = noviKap;
                        }

                        else broj++;
                    }
                }
            }

            else if(indeksi[i] && *kljucevi[i] == podrazumijevaniKljuc){
                delete kljucevi[i];
                delete vrijednosti[i];
            }
        }

        kapacitet = noviKap;
        delete[] kljucevi;
        delete[] vrijednosti;
        delete[] indeksi;
        indeksi = pomIndeksi;
        kljucevi = nizK;
        vrijednosti = nizV;
    }


public:
    HashMapa() : vel(0), kapacitet(2539){

        kljucevi = new TipKljuca*[kapacitet];
        hashFunkcija = nullptr;
        indeksi = new bool[kapacitet];
        pom = false;
        vrijednosti = new TipVrijednosti*[kapacitet];
        podrazumijevaniKljuc = TipKljuca();
        podrazumijevanaVrijednost = TipVrijednosti();

        for(int i(0); i < kapacitet; i++)
            indeksi[i] = false;

    }

    ~HashMapa(){
        obrisi();
        delete[] vrijednosti;
        delete[] indeksi;
        delete[] kljucevi;
    }

    HashMapa(const HashMapa& n){
        kapacitet = n.kapacitet;
        vel = n.vel;
        kljucevi = new TipKljuca*[kapacitet];
        vrijednosti = new TipVrijednosti*[kapacitet];
        podrazumijevaniKljuc = n.podrazumijevaniKljuc;
        podrazumijevanaVrijednost = n.podrazumijevanaVrijednost;
        hashFunkcija = n.hashFunkcija;
        indeksi = new bool[n.kapacitet];
        pom = n.pom;

        int i(0);

        while(i < n.kapacitet){
            if(n.indeksi[i]){
                kljucevi[i] = new TipKljuca(*(n.kljucevi[i]));
                vrijednosti[i] = new TipVrijednosti(*(n.vrijednosti[i]));
                indeksi[i] = true;
            }

            else
                indeksi[i] = false;
            i++;
        }
    }

    HashMapa<TipKljuca, TipVrijednosti>& operator=(const HashMapa<TipKljuca, TipVrijednosti>& n){
         if(&n == this)
            return *this;

        while(kapacitet < n.kapacitet)
            povecajKapacitet();

        for(int i(0); i < n.kapacitet; i++){
            if(!indeksi[i] && n.indeksi[i]){
                kljucevi[i] = new TipKljuca(*(n.kljucevi[i]));
                vrijednosti[i] = new TipVrijednosti(*(n.vrijednosti[i]));
                indeksi[i] = true;
            }

            else if(indeksi[i] && n.indeksi[i]){
                *kljucevi[i] = *(n.kljucevi[i]);
                *vrijednosti[i] = *(n.vrijednosti[i]);
            }

            else if(indeksi[i] && !n.indeksi[i]){
                delete kljucevi[i];
                delete vrijednosti[i];
                indeksi[i] = false;
            }
        }

        if(n.kapacitet < kapacitet){
            int i(n.kapacitet);

            while(i != kapacitet){
                delete kljucevi[i];
                delete vrijednosti[i];
            }
        }

        kapacitet = n.kapacitet;
        hashFunkcija = n.hashFunkcija;
        vel = n.vel;
        pom = n.pom;

        return *this;
    }

    TipVrijednosti operator[](const TipKljuca& kljuc) const{
        if(!hashFunkcija)
            throw "Nije definirana hash funkcija";

        int n(hashFunkcija(kljuc, kapacitet));

        if(indeksi[n] && *kljucevi[n] == kljuc)
            return *vrijednosti[n];

        else if(!indeksi[n])
            return podrazumijevanaVrijednost;

        else{
            int i(1), j;

            while(i != kapacitet){

                j = (n + i ) % kapacitet;
                if(indeksi[j] && *kljucevi[j] == kljuc)
                    return *vrijednosti[j];
                i++;
            }
        }
    }

    TipVrijednosti &operator[](const TipKljuca& kljuc){
        if(!hashFunkcija)
            throw "Nije definirana hash funkcija";

        if(vel == kapacitet)
            povecajKapacitet();

        int n(hashFunkcija(kljuc, kapacitet));

        if(indeksi[n]  && *kljucevi[n] == kljuc)
            return *vrijednosti[n];

        else if(!indeksi[n]){
            kljucevi[n] = new TipKljuca(kljuc);
            vrijednosti[n] = new TipVrijednosti(podrazumijevanaVrijednost);
            vel++;
            indeksi[n] = true;
            return *vrijednosti[n];
        }

        else if(indeksi[n] && *kljucevi[n] == podrazumijevaniKljuc){
            *kljucevi[n] = kljuc;
            vel++;
            return *vrijednosti[n];
        }

        else{
            int i(1), j;
            while(i != kapacitet){
                j = (n + i) % kapacitet;

                if(indeksi[j] && *kljucevi[j] == kljuc){
                    return *vrijednosti[j];
                }

                if(!indeksi[j]){
                    kljucevi[j] = new TipKljuca(kljuc);
                    vrijednosti[j] = new TipVrijednosti(podrazumijevanaVrijednost);
                    vel++;
                    indeksi[j] = true;
                    return *vrijednosti[j];
                }

                else i++;
            }
        }
    }

    int brojElemenata() const{
        return vel;
    }

    void obrisi(){

        if(vel != 0 || pom){
            for(int i(0); i < kapacitet; i++){
                if(indeksi[i]){
                    delete kljucevi[i];
                    delete vrijednosti[i];
                    indeksi[i] = false;
                }
            }
        }

        pom = false;
        vel = 0;
    }

    void obrisi(const TipKljuca& kljuc){
        if(!hashFunkcija)
            throw "Nije definirana hash funkcija";

        if(vel == 0)
            return;

        int n(hashFunkcija(kljuc, kapacitet));

        if(!indeksi[n])
            return;

        if(*kljucevi[n] == kljuc){

            *kljucevi[n] = podrazumijevaniKljuc;
            *vrijednosti[n] = podrazumijevanaVrijednost;
            vel--;

            if(vel == 0)
                pom = true;
        }

        else if(*kljucevi[n] != kljuc){
            int i(1), j;

            while(i != kapacitet){
                j = (n + i) % kapacitet;

                if(indeksi[j] && *kljucevi[j] == kljuc){

                    *kljucevi[j] = podrazumijevaniKljuc;
                    *vrijednosti[j] = podrazumijevanaVrijednost;
                    i = kapacitet;

                    vel--;
                    if(vel == 0)
                        pom = true;
                }

                else i++;
            }
        }
    }

    void definisiHashFunkciju(unsigned int (*hashFunkcija)(TipKljuca, unsigned int)){
         this->hashFunkcija = hashFunkcija;
    }

};


#endif // HASHMAPA_H_INCLUDED
