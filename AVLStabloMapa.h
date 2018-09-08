#ifndef AVLSTABLOMAPA_H_INCLUDED
#define AVLSTABLOMAPA_H_INCLUDED

#include "Mapa.h"
#include <iostream>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti>
{
private:
    struct Cvor{
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor* lijevo;
        Cvor* desno;
        Cvor* roditelj;
        int balans;

        Cvor(const TipKljuca &k, const TipVrijednosti &v, Cvor *l, Cvor *d, Cvor *r):
            kljuc(k), vrijednost(v), lijevo(l), desno(d), roditelj(r), balans(0) {}
        Cvor() : balans(0) {}
    };

    Cvor* korijen;
    int brojEl;
    TipVrijednosti podrazumijevana;


public:
    AVLStabloMapa(): Mapa<TipKljuca,TipVrijednosti> (), korijen(0), brojEl(0), podrazumijevana(TipVrijednosti()){}
    ~AVLStabloMapa(){
        obrisiSve(korijen);
    }

    AVLStabloMapa(const AVLStabloMapa<TipKljuca,TipVrijednosti> &m): korijen(0), brojEl(0), podrazumijevana(TipVrijednosti()){
        obilazakStabla(m.korijen, *this);
    }

    AVLStabloMapa<TipKljuca,TipVrijednosti> &operator=(const AVLStabloMapa <TipKljuca,TipVrijednosti> &m){
        if(this == &m)
            return *this;
        obrisiSve(korijen);
        korijen = 0;
        brojEl = 0;
        podrazumijevana = TipVrijednosti();
        obilazakStabla(m.korijen, *this);

        return *this;
    }

    void obilazakStabla(Cvor *pom, AVLStabloMapa<TipKljuca,TipVrijednosti> &avlsm){
        if(pom != nullptr){
            avlsm[pom->kljuc] = pom->vrijednost;
            obilazakStabla(pom->lijevo, avlsm);
            obilazakStabla(pom->desno, avlsm);
        }
    }

    int brojElemenata() const{
        return brojEl;
    }

    TipVrijednosti& operator[](const TipKljuca& kljuc){
        Cvor *pomCvor(dodaj(kljuc, TipVrijednosti(), korijen, 0));
        //azurirajBalans(pomCvor);          ne rade rotacije fino
        return pomCvor->vrijednost;
    }

    TipVrijednosti operator[](const TipKljuca& kljuc) const{
        Cvor *pomCvor = trazi(kljuc, korijen);

        if(pomCvor == 0)
            return podrazumijevana;
        else
            return pomCvor->vrijednost;
    }
    void obrisi(){
        obrisiSve(korijen);
        korijen = 0;
    }

     Cvor* trazi(const TipKljuca& kljuc, Cvor* pomCvor) const{
        if(pomCvor == 0)
            return nullptr;

        else if(kljuc == pomCvor->kljuc)
            return pomCvor;

        else if(kljuc < pomCvor->kljuc)
            return trazi(kljuc, pomCvor->lijevo);

        else
            return trazi(kljuc, pomCvor->desno);
    }

    Cvor *dodaj(const TipKljuca& kljuc, const TipVrijednosti& vrijednost, Cvor*& pomocni, Cvor * roditelj){
        if(!pomocni){
            pomocni = new Cvor(kljuc, vrijednost, 0, 0, roditelj);
            brojEl++;
            return pomocni;
        }

        else if(kljuc == pomocni->kljuc)
            return pomocni;

        else if(kljuc < pomocni->kljuc){
            return dodaj(kljuc ,vrijednost, pomocni->lijevo, pomocni);
        }
        else{
            return dodaj(kljuc ,vrijednost, pomocni->desno , pomocni);
        }
    }

    void obrisiSve(Cvor *pom){
        if(pom == 0)
            return;

        obrisiSve(pom->lijevo);
        obrisiSve(pom->desno);

        delete pom;
        brojEl--;
    }

     //funkcija obrisi iz knjige

    void obrisi(const TipKljuca &kljuc) {
        Cvor *pomocni(korijen), *pomocniRod(0);
        Cvor *t(0), *pp(0), *rp(0);

        //tražimo kljuè

        while(pomocni && pomocni->kljuc != kljuc){
            pomocniRod = pomocni;
            if(pomocni->kljuc > kljuc)
                pomocni = pomocni->lijevo;
            else
                pomocni = pomocni->desno;
        }

        if(!pomocni)
            throw "Nema trazenog kljuca\n";     //ako se dodje do kraja

        if(pomocni->lijevo == nullptr)
            rp = pomocni->desno;

        else if(pomocni->desno == nullptr)
            rp = pomocni->lijevo;

        else{
            pp = pomocni;
            rp = pomocni->lijevo;
            t = rp->desno;

            while(t){
                pp = rp;
                rp = t;
                t = rp->desno;
            }

            if (pp != pomocni){
                pp->desno = rp->lijevo;
                rp->lijevo = pomocni->lijevo;
            }
            rp->desno = pomocni->desno;
        }

        if (!pomocniRod)
            korijen = rp;

        else if (pomocni == pomocniRod->lijevo)
            pomocniRod->lijevo = rp;

        else
            pomocniRod->desno = rp;

        delete pomocni;
        brojEl--;
    }

/*
    funkcija azurirajBalans -- nešto ne radi kako treba uvijek, zakomentirala zbog ostalih testova

    void azurirajBalans(Cvor* dodani){
        if(dodani->roditelj == nullptr)
            return;

        if(dodani->roditelj->lijevo == dodani)
            (dodani->roditelj->balans)++;

        else
            (dodani->roditelj->balans)--;

        if(dodani->roditelj->balans < -1){
            if(dodani->balans > 0)
                desnaRotacija(dodani);
            lijevaRotacija(dodani->roditelj);
        }

        else if(dodani->roditelj->balans > 1){
            if(dodani->balans < 0) lijevaRotacija(dodani);
            desnaRotacija(dodani->roditelj);
        }

        if(dodani->roditelj != nullptr)
            azurirajBalans(dodani->roditelj);
    }

    void desnaRotacija(Cvor* pomocni){
        Cvor *parent = pomocni->roditelj;
        bool left;

        if(parent){
            if(parent->lijevo == pomocni)
                left = true;
            else
                left = false;
        }

        Cvor* pom = pomocni->lijevo;
        pomocni->lijevo = 0;

        if(!pom->desno)
            pom->desno = pomocni;

        else{
            pomocni->lijevo = pom->desno;
            pom->desno->roditelj = pomocni;
            pom->desno = pomocni;
        }

        pomocni->roditelj = pom;
        pomocni->balans = 0;

        if(parent){
            if(left){
                parent->lijevo = pom;
                pom->roditelj = parent;
                azurirajBalans(parent);
            }

            else{
            parent->desno = pom;
            pom->roditelj = parent;
            }
        }

        else{
            korijen = pom;
            korijen->roditelj = 0;
        }
    }

    void lijevaRotacija(Cvor* pomocni){
        Cvor *parent = pomocni->roditelj;
        bool left;

        if(parent){
            if(parent->lijevo == pomocni)
                left = true;
            else
                left = false;
        }

        Cvor* pom = pomocni->desno;
        pomocni->desno = 0;

        if(!pom->lijevo)
            pom->lijevo = pomocni;

        else{
            pomocni->desno = pom->lijevo;
            pom->lijevo->roditelj = pomocni;
            pom->lijevo = pomocni;
        }

        pomocni->roditelj = pom;
        pomocni->balans = 0;

        if(parent){
            if(left){
                parent->lijevo = pom;
                pom->roditelj = parent;
            }
            else{
                parent->desno = pom;
                pom->roditelj = parent;
            }
        }

        else{
            korijen = pom;
            korijen->roditelj = 0;
        }
    }

    */
};



#endif // AVLSTABLOMAPA_H_INCLUDED
