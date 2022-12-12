#include "FiltryPGM.h"

void MenuFiltrow(struct ObrazPGM_t*);

/*
    DOKUMENTACJA:
    Autor:								Daniel Sadowski
    Data utworzenia programu:			08.12.2020
    Wersja:								1.5(Aktualna)
    Modyfikacje:
        Program nie posiada stalych modyfikowalnych
    
    Kompilacja:
        Aby skompilowaæ kod nale¿y mieæ w jednym folderze nastêpuj¹ce pliki:
        main.c, FiltryPGM.h, FiltryPGM.c, ObrazPGM.h, ObrazPGM.c
        Kod nale¿y kompilowac linijk¹:
        gcc -Wall -pedantic -std=c99 main.c FiltryPGM.c ObrazPGM.c -lm

    Opis korzystania z programu:
        Program nalezy uruchomic i postepowac zgodnie z komunikatami na ekranie
        Program nie przyjmuje argumentow wywolania
*/

int main() 
{
    struct ObrazPGM_t obraz;
    Constructor(&obraz);

    int wybor = 0, state = 1;
    do
    {
        system("clear");
        printf("Witaj w programie sluzacym przetwarzaniu obrazow PGM!\n");
        printf("1 - WCZYTAJ PLIK\n");
        printf("2 - ZAKONCZ DZIALANIE\n");
        if (state)
            printf("Prosze podac opcje: ");
        else
            printf("Nie poprawna opcja! Prosze podac jeszce raz: ");

        //Wczytujemy wybor uzytkownika
        scanf("%d", &wybor);
        switch(wybor)
        {
        case 1: //wczytanie pliku
            printf("Prosze podac nazwe pliku: ");
            scanf("%s", obraz.nazwa);   //wczytujemy nazwe
            Odczyt(&obraz);             
            if (obraz.status == bad)    //Jesli wystapil blad
            {
                fprintf(stderr, "Wystapil blad podczas otwierania pliku!\nOpis bledu:\t%s\n\n", obraz.statusINFO);
                
                printf("Press enter to continue");
                getchar();  //Dwukrotnie poniewa¿ znak nowej linii zostaje w buforze
                getchar();

                Wyczysc(&obraz);    //Nie wiadomo w ktorym momencie wystapil wiec dla bezpieczenstwa lepiej wyzerowac wszystko
            }
            else
                MenuFiltrow(&obraz);    //Jesli sie udalo przchodzimy do menu z filtrami
            state = 1;                  
            break;
        case 2:
            printf("Praca z toba to byl zaszczyt!\n");
            exit(0);
            break;
        default:
            state = 0;
        }
    }
    while (wybor != 2);

}

void MenuFiltrow(struct ObrazPGM_t* obraz)
{
    //zmienne na przyszly uzytek
    int wybor = 0, state = 1, temp = 0;
    float temp2 = 0;
    struct Macierz3x3_t maska;

    do
    {
        system("clear");
        printf("Oto lista dostepnych filtrow plikow PGM:\n");
        printf("1  - NEGATYW\n");
        printf("2  - PROGOWANIE\n");
        printf("3  - POLPROGOWANIE CZERNI\n");
        printf("4  - POLPROGOWANIE BIELI\n");
        printf("5  - ZMIANA POZIOMOW\n");
        printf("6  - KOREKCJA GAMMA\n");
        printf("7  - KONTUROWANIE\n");
        printf("8  - ROZMYCIE POZIOME\n");
        printf("9  - ROZMYCIE PIONOWE\n");
        printf("10 - ROZCIAGANIE HISTOGRAMU\n");
        printf("11 - LINIOWA FILTRACJA KONTEKSTOWA\n");
        printf("12 - WYSWIETL PLIK\n");
        printf("13 - ZAPISZ PLIK\n");
        printf("14 - POWROT\n");

        if (state)
            printf("Prosze podac opcje: ");
        else
            printf("Nie poprawna opcja! Prosze podac jeszce raz: ");
        //Wczytanie opcji od uzytkownika
        scanf("%d", &wybor);
        switch (wybor)
        {
        case 1:
            Negatyw(obraz);
            break;
        case 2:
            printf("Prosze podac wartosc progu(w procentach): ");
            scanf("%d", &temp);
            Progowanie(obraz, temp);
            break;
        case 3:
            printf("Prosze podac wartosc progu(w procentach): ");
            scanf("%d", &temp);
            PolprogowanieCzerni(obraz, temp);
            break;
        case 4:
            printf("Prosze podac wartosc progu(w procentach): ");
            scanf("%d", &temp);
            PolprogowanieBieli(obraz, temp);
            break;
        case 5:
            printf("Prosze podac wartosc progu dla czerni(w procentach): ");
            scanf("%d", &temp);
            printf("Prosze podac wartosc progu dla bieli(w procentach): ");
            scanf("%d", &wybor);
            ZmianaPoziomow(obraz, temp, wybor);
            break;
        case 6:
            printf("Prosze podac wartosc wspolczynnika gamma: ");
            scanf("%f", &temp2);
            KorekcjaGamma(obraz, temp2);
            break;
        case 7:
            Konturowanie(obraz);
            break;
        case 8:
            printf("Prosze podac wartosc promien rozmycia(dodatni): ");
            scanf("%d", &temp);
            RozmywaniePoziome(obraz, temp);
            break;
        case 9:
            printf("Prosze podac wartosc promien rozmycia(dodatni): ");
            scanf("%d", &temp);
            RozmywaniePionowe(obraz, temp);
            break;
        case 10:
            RozciaganieHistogramu(obraz);
            break;
        case 11:
            printf("Prosze podac maske filtracji: \n");

            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                    scanf("%d", &maska.tab[i][j]);

            LiniowaFiltracjaKontekstowa(obraz, &maska);
            break;
        case 12:
            strcpy(obraz->nazwa, "temp.pgm");
            Zapis(obraz);
            Wyswietl("temp.pgm");
            break;
        case 13:
            printf("Prosze podac nazwe do jakiego mam zapisac plik: ");
            scanf("%s", obraz->nazwa);
            Zapis(obraz);
            break;
        case 14: //powrot
            return;
        default:
            state = 0;
        }
        //Sprawdzenie czy operacja sie powiodla
        //Jesli tak wyswietlenie komunikatu o sukcesie
        //Jesli nie wyswietlenie komuniaktu o bledzie
        if (state != 0)
        {
            if (obraz->status == good)
                printf("Operacja zakonczona sukcesem!\n");
            else
                fprintf(stderr, "Wystapil blad podczas przetwarzania pliku!\nOpis bledu:\t%s\n\n", obraz->statusINFO);

            printf("Press enter to continue");
            getchar();  //Dwukrotnie poniewa¿ znak nowej linii zostaje w buforze
            getchar();

        }


    } while (1);

}
