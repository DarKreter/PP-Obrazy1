#include "ObrazPGM.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Constructor(struct ObrazPGM_t* obraz)
{
    obraz->status = bad;
    obraz->fileHandle = NULL;
    obraz->data = NULL;
    strcpy(obraz->nazwa, "");
    strcpy(obraz->comments, "");
    strcpy(obraz->statusINFO, "Plik nie zostal jeszcze wczytany!");
    obraz->x = obraz->y = obraz->maxSzarosc = 0;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Wyczysc(struct ObrazPGM_t* obrazPGM)
{
    if (obrazPGM->data != NULL)
    {
        for (int i = 0; i < obrazPGM->y; i++)
            free(obrazPGM->data[i]);
        free(obrazPGM->data);
    }
    if (obrazPGM->fileHandle != NULL)
        fclose(obrazPGM->fileHandle);

    //Na koniec wpisujemy domniemane wartosci
    Constructor(obrazPGM);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Zapis(struct ObrazPGM_t* obrazPGM)
{
    obrazPGM->fileHandle = fopen(obrazPGM->nazwa, "w");             //otwieramy plik do pisania
    if (obrazPGM->fileHandle == NULL)                       //Jesli pliku nie udalo sie otworzyc
    {
        ErrorOccurred(obrazPGM, "Nie udalo sie zapisac pliku!");
        return;
    }

    fprintf(obrazPGM->fileHandle, "P2\n");                                                          //Wpisujemy format pliku (zawsze ten sam)    
    fprintf(obrazPGM->fileHandle, "%s", obrazPGM->comments);                                        //Komentarze jakie byly i jakie dodalismy
    fprintf(obrazPGM->fileHandle, "%d %d\n%d\n", obrazPGM->x, obrazPGM->y, obrazPGM->maxSzarosc);   //Podajemy wymiary oraz skale szarosci

    //Petla ktora zapisuje wszystkie piksele
    for (int i = 0; i < obrazPGM->y; i++)       
    {
        for (int j = 0; j < obrazPGM->x; j++)
            fprintf(obrazPGM->fileHandle, "%4d", obrazPGM->data[i][j]);
        fprintf(obrazPGM->fileHandle, "\n");
    }

    //Zamykamy plik i jednoczesnie sprawdzamy czy poprawnie sie zamknal
    if (fclose(obrazPGM->fileHandle) == EOF)
    {
        ErrorOccurred(obrazPGM, "Nie udalo sie zapisac pliku!");
        return;
    }

    obrazPGM->status = good;
    return;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Odczyt(struct ObrazPGM_t* obrazPGM)
{
    char* buf = (char*)malloc(sizeof(char) * DL_LINII);         // bufor pomocniczy do czytania naglowka i komentarzy
    if (buf == NULL)
    {
        ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji Odczyt!");
        return;
    }
    int znak = 0;                                                // zmienna pomocnicza do czytania komentarzy

    obrazPGM->fileHandle = fopen(obrazPGM->nazwa, "r");     //Realnie otwieramy plik w trybie czytania    
    if (obrazPGM->fileHandle == NULL)                       //Sprawdzamy czy sie poprawnie otworzyl
    {
        ErrorOccurred(obrazPGM, "Bledna sciezka do pliku!");
        return;
    }

    //Wczytanie pierwszej linii i sprawdzenie przy okazji czy udalo sie to zrobic
    if (fgets(buf, DL_LINII, obrazPGM->fileHandle) == NULL)
    {
        ErrorOccurred(obrazPGM, "Plik jest pusty!");
        return;
    }

    //Sprawdzenie czy jest to plik PGM formatu P2
    if ((buf[0] != 'P') || (buf[1] != '2'))
    {
        ErrorOccurred(obrazPGM, "To nie jest plik formatu PGM!");
        return;
    }

    //Pominiecie komentarzy
    do {
        znak = fgetc(obrazPGM->fileHandle);
        if (znak == '#')     //Jesli linia rozpoczyna sie od znaku # to jest komentarzem 
        {
            if (fgets(buf, DL_LINII, obrazPGM->fileHandle) == NULL)
            {
                ErrorOccurred(obrazPGM, "Plik konczy sie na komentarzach!");
                return;
            }
            strcat(obrazPGM->comments, "#");
            strcat(obrazPGM->comments, buf);
        }
        else    //Jesli znak okazal sie nie # cofamy sie jeden do tylu bo to nie byl jednak komentarz
            ungetc(znak, obrazPGM->fileHandle);

    } while (znak == '#');   //Powtarzamy dopoki sa to komentarze

    // Pobranie wymiarow obrazu i liczby odcieni szarosci
    if (fscanf(obrazPGM->fileHandle, "%d %d %d", &obrazPGM->x, &obrazPGM->y, &obrazPGM->maxSzarosc) != 3)
    {
        ErrorOccurred(obrazPGM, "Brak wymiarow obrazu lub liczby stopni szarosci!");
        return;
    }

    //Stworzenie tablicy dynamicznej na podstawie wymiarow
    if ( (obrazPGM->data = malloc(sizeof(int*) * obrazPGM->y)) == NULL)
    {
        ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji Odczyt!");
        return;
    }
    for (int i = 0; i < obrazPGM->y; i++)
        if ( (obrazPGM->data[i] = malloc(sizeof(int) * obrazPGM->x)) == NULL)
        {
            ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji Odczyt!");
            return;
        }
        

    // Pobranie obrazu i zapisanie w tablicy
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            if (fscanf(obrazPGM->fileHandle, "%d", &(obrazPGM->data[i][j])) != 1)
            {
                ErrorOccurred(obrazPGM, "Niewlasciwe wymiary obrazu!");
                return;
            }

    //Zamykamy plik i jednoczesnie sprawdzamy czy poprawnie sie zamknal
    if (fclose(obrazPGM->fileHandle) == EOF)
    {
        ErrorOccurred(obrazPGM, "Nie udalo sie wczytac pliku!");
        return;
    }

    obrazPGM->status = good;
    return;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ErrorOccurred(struct ObrazPGM_t* obrazPGM, char* info)
{
    strcpy(obrazPGM->statusINFO, info);
    obrazPGM->status = bad;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Wyswietl(char* n_pliku)
{
    //Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"
    char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

    strcpy(polecenie, "display ");  /* konstrukcja polecenia postaci */
    strcat(polecenie, n_pliku);     /* display "nazwa_pliku" &       */
    strcat(polecenie, " &");
    system(polecenie);             /* wykonanie polecenia        */
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void DodajKomentarz(struct ObrazPGM_t* obrazPGM, char* what)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char temp[DL_LINII];
    sprintf(temp, "# %02d.%02d.%d %02d:%02d:%02d - ",
        tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    strcat(obrazPGM->comments, temp);
    strcat(obrazPGM->comments, what);
    strcat(obrazPGM->comments, "\n");
}