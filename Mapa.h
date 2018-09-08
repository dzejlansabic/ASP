#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

template <typename TipKljuca, typename TipVrijednosti>
class Mapa
{

public:
    Mapa() {}
    virtual TipVrijednosti operator[](const TipKljuca& k) const = 0;
    virtual TipVrijednosti& operator[](const TipKljuca& k) = 0;
    virtual int brojElemenata() const=0;
    virtual void obrisi() =0;
    virtual void obrisi(const TipKljuca& kljuc)=0;
    Mapa(const Mapa& m) {}
    virtual ~Mapa() {}
};

#endif // MAPA_H_INCLUDED
