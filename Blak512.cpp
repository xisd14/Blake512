
/************BLAKE512************/
/******Wrzos Bartosz I4Y4S1******/
#include<stdio.h>
#include<iostream>
#include<string>
#include<stdint.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include<sstream>
#include<iomanip>
#define ROT(x,n) (((x)<<(64-n))|((x)>>(n)))
 
using namespace std;
 
 /*Deklaracja funckji*/
 
void G(int a, int b, int c, int d, int i, int r, uint64_t* h, uint64_t* m);
void padding(uint64_t* wiadomosc, unsigned int dlugosc, unsigned int& ileblokow);
 
 /*Stałe*/
 
const uint8_t per[10][16] =
{
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
        { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
        { 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
        { 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
        { 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
        { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
        { 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
        { 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
        { 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
        { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 },
 
};
 
const uint64_t cst[16] =
{
        0x243F6A8885A308D3ULL, 0x13198A2E03707344ULL, 0xA4093822299F31D0ULL, 0x082EFA98EC4E6C89ULL,
        0x452821E638D01377ULL, 0xBE5466CF34E90C6CULL, 0xC0AC29B7C97C50DDULL, 0x3F84D5B5B5470917ULL,
        0x9216D5D98979FB1BULL, 0xD1310BA698DFB5ACULL, 0x2FFD72DBD01ADFB7ULL, 0xB8E1AFED6A267E96ULL,
        0xBA7C9045F12C7F99ULL, 0x24A19947B3916CF7ULL, 0x0801F2E2858EFC16ULL, 0x636920D871574E69ULL
};
 
uint64_t h[8] =
{
        0x6A09E667F3BCC908ULL, 0xBB67AE8584CAA73BULL, 0x3C6EF372FE94F82BULL, 0xA54FF53A5F1D36F1ULL,
        0x510E527FADE682D1ULL, 0x9B05688C2B3E6C1FULL, 0x1F83D9ABFB41BD6BULL, 0x5BE0CD19137E2179ULL
};
//zmienne globalne
uint64_t* mPtr = NULL;
int xp=0;
 
 
 
int main(int argn, char* args[])
{
		//deklarowanie tablicy dla przechwywania wiadomosci
        uint64_t* wiadomosc;
 		//deklarowanie licznika dladlugosc wiadomosci
        unsigned int dlugosc_wiadomosci;
        //deklarowanie licznika ilosci blokow
        unsigned int ileblokow;
 
 		//jezeli wprowadzimy wiadomosc != od zera 
        if (argn >=2) 
        {
        		//przypisanie wartosci oderanych z konsoli do stringa 
                string wiadomosc_pomocnicza = string(args[1]);   
         		//wyliczenie ilosci blokow no pdstawie ilsci // znaki maja po 8 bitow ale ze jest hexa to 4 jeden blok to 1024 
                ileblokow = (wiadomosc_pomocnicza.length() * 8/2 / 1024 + 1) * 16;
 				//lokowanie pamiec dla wiadomosci 
                wiadomosc = (uint64_t*)malloc(sizeof(uint64_t)*ileblokow);
 				//przepisanie wiadomosci ze stringa do long long inta
                for (unsigned int i = 0; i < ileblokow; i++)
                {
                		//gdy waidomosc jest mniejsza niz 1024 to krotsza wiadomosc przesuwamy do lewej i uzupelniamy zerami z prawej 
                        if ((i * 16) < wiadomosc_pomocnicza.length())
                        {
                        		//konwersja ze stringa na inta 
                                istringstream konw_pom(wiadomosc_pomocnicza.substr(i * 16, 16).c_str());
                                //przerzucenie z  do tablicy wiadomosc
                                konw_pom >> hex >> wiadomosc[i];
                                //gdy waidomsc jest krotsza niz caly blok to przesuwamy do lewej i uzpelniami zerami,substr zwraca podciag znakow 
                                if (wiadomosc_pomocnicza.substr(i * 16, 16).length() < 16) 
								{
                                        wiadomosc[i] = wiadomosc[i] << ((16 - wiadomosc_pomocnicza.substr(i * 16, 16).length()) * 4);
 
                                }
 
                        }
 						//pozostale bloczki uzupełniamy zerami
                        else
                        {
                                wiadomosc[i] = 0;
                        }
                }
 				//wyznaczanie dlugosci wiadomosci 8/2 bo hexa 
                dlugosc_wiadomosci = wiadomosc_pomocnicza.length() * 4;
        }
 		//jezeli nie podamy wiadomsoci 
        else
        {
 
                wiadomosc = (uint64_t*)malloc(sizeof(uint64_t) * 16);
                for (int i = 0; i < 16; i++)
                {
                        wiadomosc[i] = 0;
                }
 
                dlugosc_wiadomosci = 0;
                ileblokow = 16;
        }
 
        
        //wykonanie paddingu
        padding(wiadomosc, dlugosc_wiadomosci, ileblokow);
        
        wiadomosc = mPtr;
        //deklarowanie soli
        const uint64_t s[4] =
        {
                0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000
        };
 		//deklarowanie licznika
        uint64_t t[2] =
        {
                0x0000000000000000, 0x0000000000000000
        };
 
        /*Inicjalizacja*/
 		//funckja odpowiedzialna za uzupelnanie macerzy V
 		//deklaracja macierzy V oraz pomocniczej
        uint64_t v[4][4], pomocnicza[16];
        int z;
        //przekopiowanie dlugosc wadomosci do pomocniczej wiad
        int wiad = dlugosc_wiadomosci;
        //przetwarznie blokow wiadomosci
        
        int pom_dlugosc = dlugosc_wiadomosci;
        uint64_t tablicaLicznikow[64];
        uint64_t xaz = 0;
        for (int its = 0; its < ileblokow / 16; its++)
        {
                if (pom_dlugosc > 1024)
                {
                        xaz += 1024;
                        pom_dlugosc -= 1024;
                }
                else if (pom_dlugosc > 894)
                {
                        xaz += pom_dlugosc;
                        pom_dlugosc = 0;
                }
                else if (pom_dlugosc == 0)
                        xaz = 0;
                else xaz += pom_dlugosc;
                tablicaLicznikow[its] = xaz;
        }
        
        for (unsigned int k = 0; k < ileblokow / 16; k++)
        {
        	xp=k;
        	t[0]=tablicaLicznikow[k];
				 /*Uzupełnienie dwóch wierszy macierzy 4x4 (V0-V7)*/
                z = 0;
                for (int i = 0; i <= 1; i++)
                {
                        for (int j = 0; j <= 3; j++)
                        {
                                v[i][j] = h[z];
                                z++;
                        }
                }
       
                /*wypelnienie macierzy na polach od V8 dp V11 */
                for (int i = 0; i <= 3; i++)
                {
                        v[2][i] = s[i] ^ cst[i];
 
                }
 
                /*wypełnienie macierzy na polach od V12 do V15*/
                int tmp = 0;
                for (int i = 0; i < 4; i++)
                {
 
                        v[3][i] = t[tmp] ^ cst[i + 4];
                        if (i == 1)
                        {
                                tmp++;
                        }
 
                }
 
                /*Przepisanie wartosc z macierzy 4x4 do jednowymiarowej tablicy pomocznicza*/
 
                int tmp1 = 0, tmp2 = 0;
                for (int i = 0; i < 16; i++)
                {
                        pomocnicza[i] = v[tmp1][tmp2];
 
                        if (tmp2 == 3)
                        {
                                tmp1++;
                                tmp2 = 0;
                        }
                        else tmp2++;
 
                }
 				/*Wywolanie funckji rundy*/
 				//K oznacza który blok jest przetwarzany
 			
                for (int r = 0; r < 16; ++r)
                {
                        G(0, 4, 8, 12, 0, r, pomocnicza, mPtr);
                        G(1, 5, 9, 13, 1, r, pomocnicza, mPtr);
                        G(2, 6, 10, 14, 2, r, pomocnicza, mPtr);
                        G(3, 7, 11, 15, 3, r, pomocnicza, mPtr);
                        G(0, 5, 10, 15, 4, r, pomocnicza, mPtr);
                        G(1, 6, 11, 12, 5, r, pomocnicza, mPtr);
                        G(2, 7, 8, 13, 6, r, pomocnicza, mPtr);
                        G(3, 4, 9, 14, 7, r, pomocnicza, mPtr);
 
                }
 				//wyznaczenie h wg. spefyfikacji
                for (int i = 0; i < 8; i++)
                {
                        h[i] = h[i] ^ s[i % 4] ^ pomocnicza[i] ^ pomocnicza[i + 8];
 
                }
        }
 
 
        //wypisanie gotowego wyniku
        for (int i = 0; i < 8; i++)
        {
                cout <<setfill('0')<<setw(16)<< hex << h[i] ;
        }
 
        return 0;
}
 //funcka wyliczajaca padding 
void padding(uint64_t* wiadomosc, unsigned int dlugosc, unsigned int& ileblokow)
{
        if (1024 - (dlugosc % 1024) < 130)
        { // jesli padding nie zmiesci się w jednym bloku
                ileblokow += 16;
                uint64_t* nowawiadomosc = (uint64_t*)malloc(sizeof(uint64_t)*(ileblokow));
                for (unsigned int i = 0; i < ileblokow; i++)
				{
                        nowawiadomosc[i] = wiadomosc[i];
                }
                for (int i = 0; i < 16; i++) {
                        nowawiadomosc[i + ileblokow - 16] = 0;
                }
                int ktoryblok = dlugosc / 64;
                uint64_t poczatek_paddingu = 0x8000000000000000;
                nowawiadomosc[ktoryblok] += (poczatek_paddingu >> (dlugosc % 64));
                uint64_t koniec_paddingu[3] = { 1, 0, dlugosc };
                nowawiadomosc[ileblokow - 3] += koniec_paddingu[0];
                nowawiadomosc[ileblokow - 2] += koniec_paddingu[1];
                nowawiadomosc[ileblokow - 1] += koniec_paddingu[2];
                delete wiadomosc;
                wiadomosc = nowawiadomosc;
                mPtr = nowawiadomosc;
                
        }
 
        else
        {
                // jeśli padding zmieści się jeszcze w tym bloku
                uint64_t koniec_paddingu[3] = { 1, 0, dlugosc };
                int ktoryblok = dlugosc / 64;
                uint64_t poczatek_paddingu = 0x8000000000000000;
                wiadomosc[ktoryblok] += (poczatek_paddingu >> (dlugosc % 64));
                wiadomosc[ileblokow - 3] += koniec_paddingu[0];
                wiadomosc[ileblokow - 2] += koniec_paddingu[1];
                wiadomosc[ileblokow - 1] += koniec_paddingu[2];
                mPtr = wiadomosc;        
        }
        
}
 //funkcja rundy
void G(int a, int b, int c, int d, int i, int r, uint64_t* pomocnicza, uint64_t* m)
{
        pomocnicza[a] = pomocnicza[a] + pomocnicza[b] + (m[16*xp + per[r % 10][2 * i]] ^ cst[per[r % 10][2 * i + 1]]);
        pomocnicza[d] = ROT(pomocnicza[d] ^ pomocnicza[a], 32);
        pomocnicza[c] = pomocnicza[c] + pomocnicza[d];
        pomocnicza[b] = ROT(pomocnicza[b] ^ pomocnicza[c], 25);
        pomocnicza[a] = pomocnicza[a] + pomocnicza[b] + (m[16*xp + per[r % 10][2 * i + 1]] ^ cst[per[r % 10][2 * i]]);
        pomocnicza[d] = ROT(pomocnicza[d] ^ pomocnicza[a], 16);
        pomocnicza[c] = pomocnicza[c] + pomocnicza[d];
        pomocnicza[b] = ROT(pomocnicza[b] ^ pomocnicza[c], 11);
}
