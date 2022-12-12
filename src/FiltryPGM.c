#include "ObrazPGM.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Negatyw(struct ObrazPGM_t* obrazPGM)
{
    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            obrazPGM->data[i][j] = obrazPGM->maxSzarosc - obrazPGM->data[i][j];

    DodajKomentarz(obrazPGM, "Wykonano operacje negatywu!");
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Progowanie(struct ObrazPGM_t* obrazPGM, short procent)
{
    //Sprawdzenie blednych danych wejsciowych
    if (procent > 100 || procent < 0)
    {
        ErrorOccurred(obrazPGM, "Prosze podac progowanie w procentach(od 0 do 100)!");
        return;
    }
    //Przeliczenie procentow na prog
    int prog = procent * obrazPGM->maxSzarosc * 0.01;
    //printf("%d\n", prog);

    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            if (obrazPGM->data[i][j] <= prog)
                obrazPGM->data[i][j] = 0;
            else
                obrazPGM->data[i][j] = obrazPGM->maxSzarosc;

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje progowania(%d%%)!", procent);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void PolprogowanieCzerni(struct ObrazPGM_t* obrazPGM, short procent)
{
    //Sprawdzenie blednych danych wejsciowych
    if (procent > 100 || procent < 0)
    {
        ErrorOccurred(obrazPGM, "Prosze podac progowanie w procentach(od 0 do 100)!");
        return;
    }
    //Wyliczenie progu
    int prog = procent * obrazPGM->maxSzarosc * 0.01;
    //printf("%d\n", prog);

    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            if (obrazPGM->data[i][j] <= prog)
                obrazPGM->data[i][j] = 0;

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje progowania czerni(%d%%)!", procent);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void PolprogowanieBieli(struct ObrazPGM_t* obrazPGM, short procent)
{
    //Sprawdzenie blednych danych wejsciowych
    if (procent > 100 || procent < 0)
    {
        ErrorOccurred(obrazPGM, "Prosze podac progowanie w procentach(od 0 do 100)!");
        return;
    }
    //Wyliczenie progu
    int prog = procent * obrazPGM->maxSzarosc * 0.01;
    //printf("%d\n", prog);

    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            if (obrazPGM->data[i][j] > prog)
                obrazPGM->data[i][j] = obrazPGM->maxSzarosc;

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje progowania bieli(%d%%)!", procent);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ZmianaPoziomow(struct ObrazPGM_t* obrazPGM, short czern, short biel)
{
    //Sprawdzenie blednych danych wejsciowych
    if (czern > 100 || czern < 0 || biel < 0 || biel > 100)
    {
        ErrorOccurred(obrazPGM, "Prosze podac progowanie w procentach(od 0 do 100)!");
        return;
    }
    if (czern >= biel)
    {
        ErrorOccurred(obrazPGM, "Progowanie bieli musi byc wieksze niz progowanie czerni dla zmiany poziomow!");
        return;
    }

    //Wyliczenie progow
    int progBiel = biel * obrazPGM->maxSzarosc * 0.01;
    int progCzern = czern * obrazPGM->maxSzarosc * 0.01;
    //printf("%d\n", prog);

    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            if (obrazPGM->data[i][j] <= progCzern)
                obrazPGM->data[i][j] = 0;
            else if (obrazPGM->data[i][j] > progBiel)
                obrazPGM->data[i][j] = obrazPGM->maxSzarosc;
            else
                obrazPGM->data[i][j] = (obrazPGM->data[i][j] - progCzern) * (obrazPGM->maxSzarosc / (progBiel - progCzern));

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje zmiany poziomow(czern - %d%%, biel - %d%%)!", czern, biel);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void KorekcjaGamma(struct ObrazPGM_t* obrazPGM, float gamma)
{
    //Modyfikacja pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            obrazPGM->data[i][j] = pow(((float)obrazPGM->data[i][j] / (float)obrazPGM->maxSzarosc), 1.0 / gamma) * (float)obrazPGM->maxSzarosc;

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje progowania gamma(wspolczynnik - %f)!", gamma);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void Konturowanie(struct ObrazPGM_t* obrazPGM)
{
    //Stworzenie tablicy ktora bedzie kopia naszych pikseli
    int** copy;
    if ( (copy = malloc(sizeof(int*) * obrazPGM->y)) == NULL)
    {
        ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji Konturowanie!");
        return;
    }
    for (int i = 0; i < obrazPGM->y; i++)
        if( (copy[i] = malloc(sizeof(int) * obrazPGM->x)) == NULL)
        {
            ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji Konturowanie!");
            return;
        }

    //Skopiowanie do niej danych
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            copy[i][j] = obrazPGM->data[i][j];

    //Odpowiednie modyfikacje pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            if (j + 1 < obrazPGM->x && i + 1 < obrazPGM->y) //Sprawdzamy czy nie wychodzimy poza zakres tablicy
            {
                short nowaWartosc = abs(copy[i][j + 1] - copy[i][j]) + abs(copy[i + 1][j] - copy[i][j]);
                if (nowaWartosc > obrazPGM->maxSzarosc) //Niektore wartosci pikseli wychodza poza zakres przez co trzeba im wpisac wartosc max
                    nowaWartosc = obrazPGM->maxSzarosc;

                obrazPGM->data[i][j] = nowaWartosc;
            }
            else
                obrazPGM->data[i][j] = 0;

    //Zwolnienie pamieci dla kopii
    for (int i = 0; i < obrazPGM->y; i++)
        free(copy[i]);
    free(copy);

    DodajKomentarz(obrazPGM, "Wykonano operacje konturowania!");
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void RozmywaniePoziome(struct ObrazPGM_t* obrazPGM, short promien)
{
    //Sprawdzenie poprawnosci przysylanych argumentow
    if (promien < 0)
    {
        ErrorOccurred(obrazPGM, "Promien powinien byc wiekszy niz 0!");
        return;
    }

    //Sprawdzenie poprawnosci przysylanych argumentow
    int** copy;
    if ((copy = malloc(sizeof(int*) * obrazPGM->y)) == NULL)
    {
        ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
        return;
    }
    for (int i = 0; i < obrazPGM->y; i++)
        if ((copy[i] = malloc(sizeof(int) * obrazPGM->x)) == NULL)
        {
            ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
            return;
        }

    //Skopiowanie do niej danych
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            copy[i][j] = obrazPGM->data[i][j];

    //Odpowiednie modyfikacje pikseli
    int licznik, k, max;
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
        {
            //Zerujemy aktualna wartosc piksela
            obrazPGM->data[i][j] = 0;

            //Na koniec biezemy srednia wiec musimy wiedziec z ilu elementow
            licznik = 0; 
            //w zaleznosci od promienia powinnismy zaczac iles pikseli w lewo
            //Jednak jesli wychodzimy poza zakres tablicy zaczynamy od poczatku wiersza
            k = (j - promien >= 0 ? j - promien : 0);
            //Jak powyzej w zaleznosci czy wychodzi poza tablice ustalamy max piksel ktory bedziemy brac pod uwage
            max = (promien + j >= obrazPGM->x ? obrazPGM->x - 1 : promien + j);

            //Sumujemy wszystkie piksele z poprzednio wyliczonego przedzialu
            for (; k <= max; k++, licznik++)
                obrazPGM->data[i][j] += copy[i][k];

            //Bierzemy srednia
            obrazPGM->data[i][j] /= licznik;
        }

    //Zwalniamy pamiec kopii
    for (int i = 0; i < obrazPGM->y; i++)
        free(copy[i]);
    free(copy);

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje rozmycia poziomego(promien - %d)!", promien);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void RozmywaniePionowe(struct ObrazPGM_t* obrazPGM, short promien)
{
    //Sprawdzenie poprawnosci przysylanych argumentow
    if (promien < 0)
    {
        ErrorOccurred(obrazPGM, "Promien powinien byc wiekszy niz 0!");
        return;
    }

    //Sprawdzenie poprawnosci przysylanych argumentow
    int** copy;
    if ((copy = malloc(sizeof(int*) * obrazPGM->y)) == NULL)
    {
        ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
        return;
    }
    for (int i = 0; i < obrazPGM->y; i++)
        if ((copy[i] = malloc(sizeof(int) * obrazPGM->x)) == NULL)
        {
            ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
            return;
        }

    //Skopiowanie danych do kopii
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            copy[i][j] = obrazPGM->data[i][j];


    //Odpowiednie modyfikacje pikseli
    int k, licznik, max;
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
        {
            //Zerujemy aktualna wartosc piksela
            obrazPGM->data[i][j] = 0;

            //Na koniec biezemy srednia wiec musimy wiedziec z ilu elementow
            licznik = 0;
            //w zaleznosci od promienia powinnismy zaczac iles pikseli w gore
            //Jednak jesli wychodzimy poza zakres tablicy zaczynamy od poczatku kolumny
            k = (i - promien >= 0 ? i - promien : 0);
            //Jak powyzej w zaleznosci czy wychodzi poza tablice ustalamy max piksel ktory bedziemy brac pod uwage
            max = (promien + i >= obrazPGM->y ? obrazPGM->y - 1 : promien + i);

            //Sumujemy wszystkie piksele z poprzednio wyliczonego przedzialu
            for (; k <= max; k++, licznik++)
                obrazPGM->data[i][j] += copy[k][j];

            //Bierzemy srednia
            obrazPGM->data[i][j] /= licznik;
        }

    //Zwalniamy pamiec kopii
    for (int i = 0; i < obrazPGM->y; i++)
        free(copy[i]);
    free(copy);

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje rozmycia pionowego(promien - %d)!", promien);
    DodajKomentarz(obrazPGM, temp);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void RozciaganieHistogramu(struct ObrazPGM_t* obrazPGM)
{
    int min = obrazPGM->maxSzarosc, max = 0;

    //Ustalenie max i min
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
        {
            if (obrazPGM->data[i][j] > max)
                max = obrazPGM->data[i][j];
            if (obrazPGM->data[i][j] < min)
                min = obrazPGM->data[i][j];
        }

    //modyfikacje pikseli
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            obrazPGM->data[i][j] = (obrazPGM->data[i][j] - min) * ((float)obrazPGM->maxSzarosc / (float)(max - min));

    DodajKomentarz(obrazPGM, "Wykonane operacje rozciagania histogramu!");
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void LiniowaFiltracjaKontekstowa(struct ObrazPGM_t* obrazPGM, struct Macierz3x3_t* maska)
{
    int max, min;
    //Stworzenie kopii
    int** copy;
    if ((copy = malloc(sizeof(int*) * obrazPGM->y)) == NULL)
    {
        ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
        return;
    }
    for (int i = 0; i < obrazPGM->y; i++)
        if ((copy[i] = malloc(sizeof(int) * obrazPGM->x)) == NULL)
        {
            ErrorOccurred(obrazPGM, "Blad podczas alokacji pamieci w funkcji RozmywaniePoziome!");
            return;
        }

    //Skopiowanie danych do kopii
    for (int i = 0; i < obrazPGM->y; i++)
        for (int j = 0; j < obrazPGM->x; j++)
            copy[i][j] = obrazPGM->data[i][j];

    //Odpowiednie modyfikacje pikseli
    for (int i = 1; i < obrazPGM->y - 1; i++)
        for (int j = 1; j < obrazPGM->x - 1; j++)
        {
            obrazPGM->data[i][j] =  maska->tab[0][0] * copy[i - 1][j - 1]  +  maska->tab[0][1] * copy[i - 1][j]   + maska->tab[0][2] * copy[i - 1][j + 1]
                                     +  maska->tab[1][0] * copy[i][j - 1]      +  maska->tab[1][1] * copy[i][j]       + maska->tab[1][2] * copy[i][j + 1]
                                     +  maska->tab[2][0] * copy[i + 1][j - 1]  +  maska->tab[2][1] * copy[i + 1][j]   + maska->tab[2][2] * copy[i + 1][j + 1];
            if (i ==  1 && j == 1)
                max = min = obrazPGM->data[i][j];
            else if (obrazPGM->data[i][j] > max)
                max = obrazPGM->data[i][j];
            else if (obrazPGM->data[i][j] < min)
                obrazPGM->data[i][j] = min;

        }

    //Spradzenie czy w masce mam wartosci ujemne
    short ujemnaMASKA = 0, sumaMASKI = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            if (maska->tab[i][j] < 0)
                ujemnaMASKA = 1;
            sumaMASKI += maska->tab[i][j];
        }

    if (ujemnaMASKA == 1)   //Jesli ujemne
    {
        for (int i = 0; i < obrazPGM->y; i++)
            for (int j = 0; j < obrazPGM->x; j++)
                obrazPGM->data[i][j] = (float)(obrazPGM->data[i][j] - min) / (float)(max - min) * (float)obrazPGM->maxSzarosc;
    }
    else                    //Jesli dodatnie
    {
        for (int i = 0; i < obrazPGM->y; i++)
            for (int j = 0; j < obrazPGM->x; j++)
                obrazPGM->data[i][j] = (float)obrazPGM->data[i][j] / (float)sumaMASKI;
    }

    //Zwalniamy pamiec kopii
    for (int i = 0; i < obrazPGM->y; i++)
        free(copy[i]);
    free(copy);

    char temp[DL_LINII];
    sprintf(temp, "Wykonano operacje liniowej filtracji kontekstowej [%d, %d, %d; %d, %d, %d; %d, %d, %d]!", 
        maska->tab[0][0], maska->tab[0][1], maska->tab[0][2], maska->tab[1][0], maska->tab[1][1], maska->tab[1][2],
        maska->tab[2][0], maska->tab[2][1], maska->tab[2][2]);
    DodajKomentarz(obrazPGM, temp);
}
