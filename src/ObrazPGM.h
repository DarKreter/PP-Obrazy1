#ifndef OBRAZPGM_H
#define OBRAZPGM_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define DL_LINII 1024      // Dlugosc buforow pomocniczych

//Struktura sluzaca przechowywaniu pliku PGM
struct ObrazPGM_t
{
    enum Status_e { good, bad }status;  //Przechowuje informacje czy na pliku mozna wykonywac jakies operacje
    int** data;                         //Tablica dwuwymiarowa alokowana dynamicznie i przechowuje wszystkie piksele
    int x, y, maxSzarosc;               //Rozmiar obrazka i skala szarosci
    char nazwa[100];                    //Nazwa pliku
    char comments[DL_LINII];            //komentarze z pliku
    FILE* fileHandle;                   //Uchwyt do pliku
    char statusINFO[100];               //Informacje o statusie pliku, jesli status == bad nalezy wypisac by sprawdzic jaki jest blad

};

struct Macierz3x3_t
{
    int tab[3][3];
};

/*
    Funckja ktora inicjalizuje wszystkie zmienne struktury ObrazPGM_t
    Ustawia jej wartosci ktore syngalizuja ze struktura jest gotowa do odczytu z pliku oraz rezerwacji pamieci,
    ale nie do uruchamiania na niej filtrow
*/
void Constructor(struct ObrazPGM_t*);

/*
    Funkcja zwalnia pamiec zarezerwowana przez tablice struktury
    Na koniec wywoluje funkcje Contructor by ustawic domyslne dane
*/
void Wyczysc(struct ObrazPGM_t*);

/*
    Odczytuje dane z pliku
    Wymaga abysmy do zmiennej skladowej "nazwa" wpisali wczesniej nazwe pliku ktory chcemy wczytac
*/
void Odczyt(struct ObrazPGM_t*);

/*
    Zapisuje dane z pamieci komputera do pliku
    Nazwe pliku nalezy wczesniej wpisac do odpowiedniej zmiennej skaldowej struktury ObrazPGM_t
*/
void Zapis(struct ObrazPGM_t*);

/*
    Funkcja ktora nalezy wywolac jesli wystapil blad
    Jako drugi argument podajemy komunikat bledu ktory zostanie skopiowany w odpowiednie miejsce w strukturze
*/
void ErrorOccurred(struct ObrazPGM_t*, char*);

/*
    Wyswietla na ekranie obrazek
*/
void Wyswietl(char*);

/*
    Dodaje komentarza w pliku PGM tekst ktory podamy jako drugi argument
    Przed naszym komentarzem jeszcze doda dzisiejsza date i godzine wykonania tego komentarza
*/
void DodajKomentarz(struct ObrazPGM_t*, char*);
#endif