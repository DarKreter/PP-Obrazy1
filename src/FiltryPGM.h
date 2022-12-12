#ifndef FILTRYPGM_H
#define FILTRYPGM_H

#include "ObrazPGM.h"

/*
	Funkcja ktora naklada negatyw na obraz wczytany do obiektu
*/
void Negatyw(struct ObrazPGM_t*);

/*
	Funkcja progujaca nasz obraz
	Jako drugi argument przyjmuje wartosc w procentach progowania
	Procenty musza byc z przedzialu [0;100] i byc liczba calkowita
*/
void Progowanie(struct ObrazPGM_t*, short);

/*
	Filtr ktory zajmuje sie polprogowaniem czerni
	Jako drugi argument przyjmuje wartosc w procentach progowania
	Procenty musza byc z przedzialu [0;100] i byc liczba calkowita
*/
void PolprogowanieCzerni(struct ObrazPGM_t*, short);

/*
	Filtr ktory zajmuje sie polprogowaniem bieli
	Jako drugi argument przyjmuje wartosc w procentach progowania
	Procenty musza byc z przedzialu [0;100] i byc liczba calkowita
*/
void PolprogowanieBieli(struct ObrazPGM_t*, short);

/*
	Filtr ktory zajmuje sie zmiana wartosci czerni i bieli
	Jako drugi  argument przyjmuje nowa wartosc dla CZERNI w procentach
	Jako trzcie argument przyjmuje nowa wartosc dla BIELI  w procentach
	Procenty musza byc z przedzialu [0;100] i byc liczba calkowita
*/
void ZmianaPoziomow(struct ObrazPGM_t*, short, short);

/*
	Wprowadza korekcje gamma dla podanego wspolczynnika gamma jako drugi argument
*/
void KorekcjaGamma(struct ObrazPGM_t*, float);

/*
	Filtr konturujacy obraz
	Funkcja moze zwrocic blad jesli nie uda jej sie zaalokowac pamieci dla kopii naszego obrazu
*/
void Konturowanie(struct ObrazPGM_t*);

/*
	Rozciaga obraz poziomo wedlug podanego jako drugi argument promienia
	Drugi argument musi byc dodatni oraz calkowity
*/
void RozmywaniePoziome(struct ObrazPGM_t*, short);

/*
	Rozciaga obraz pionowo wedlug podanego jako drugi argument promienia
	Drugi argument musi byc dodatni oraz calkowity
*/
void RozmywaniePionowe(struct ObrazPGM_t*, short);

/*
	Rozciaga histogram do wartosci maksymalnych(o ile nie sa wykorzystane)
*/
void RozciaganieHistogramu(struct ObrazPGM_t*);

/*
	Funkcja ktora wymaga przyslania maski i wykona filtr splotu
*/
void LiniowaFiltracjaKontekstowa(struct ObrazPGM_t*, struct Macierz3x3_t*);

#endif