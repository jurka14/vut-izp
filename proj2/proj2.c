/* Implementace funkce umocneni a logaritmu
                    by
               Vojtech Jurka
               ( xjurka08 )
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void incorrectArguments() //funkce, ktera vypisuje napovedu v pripade spatne zadanych argumentu
{
    printf(
           "\nSpatne zadane argumenty.\nPro vypocet prirozeneho logaritmu z"
           " cisla X n N iteracich zadejte:\n\nproj2 --log X N\n\nPro vypocet"
           " exponencialni funkce z cisla Y s obecnym zakladem X\nv N iteracich"
           " zadejte:\n\nproj2 --pow X Y N\n\n"
          );

}
void incorrectVariables() //funkce, ktera vypisuje napovedu v pripade spatne zadanych promennych v argumentech
{
    printf(
           "\nSpatne zadane promenne.\nPro vypocet prirozeneho logaritmu z"
           "cisla X n N iteracich zadejte cislo X\n v oboru R+ a cislo N v "
           "oboru N.\n\nPro vypocet exponencialni funkce z cisla Y s"
           "obecnym zakladem X\nv N iteracich zadejte cislo X v oboru R+,"
           " cislo Y v oboru R a cislo N v oboru N\n\n"
          );
}

double taylor_log(double x, unsigned int n) //funkce pro vypocet logaritmu pomoci taylorova polynomu (T.P.)
{
    if(x <= 1) //vypocet v pripade, ze je argument logaritmu vetsi nebo roven nule
    {
        x = 1 - x; // uprava argumentu kvuli jeho zasazeni ve vzorci T.P.
        double vysledek = 0; //prvni clanek polynomu se odecita od nuly
        double newx = x; // promenna pro vypocet umocneneho argumentu

        for(unsigned int i = 1; i <= n; i++) // kazda iterace cyklu znamena jeden clanek polynomu (podle zadaneho poctu iteraci)
        {
            vysledek = vysledek - (newx / i);
            newx = newx * x;
        }
        return vysledek;
    }

    else //vypocet v pripade, ze je argument logaritmu vetsi nez 1
    {
        x = (x - 1) / x; // princip cyklu je stejny, jen se lisi v postupu vypoctu vzorce T.P.
        double vysledek = 0;
        double newx = x;

        for(unsigned int i = 1; i <= n; i++)
        {
            vysledek = vysledek + (newx / i);
            newx = newx * x;
        }
        return vysledek;
    }

}

double cfrac_log(double x, unsigned int n) //funkce pro vypocet logaritmu pomoci zretezenych zlomku
{
    x = (x - 1) / (1 + x); //argument se musi opet upravit kvuli jeho zasazeni
    double vysledek = 1; /*Ve zretezenem zlomku se pri postupnem vypoctu zacina u nejspodnejsiho jmenovatele,
                           jsem zvolil jako pocatecni hodnotu 1, jelikoz je to neutralni prvek pri deleni.*/

    if (n != 1) //podmínka pro zachovani presnosti vysledku pri jedne iteraci
    {
        for(unsigned int i = n; i >= 1; i--) //cislo n oznacuje pocet iteraci, coz znamena pocet deleni ve zretezenem zlomku.
        {
            vysledek = ((2 * i) - 1) - ((i * i * x * x) / vysledek);
        }
    }

    vysledek = (2 * x) / vysledek; //nakonec se provede finalni operace deleni, ktera se trochu lisi od tech ostatnich
    return vysledek;
}

double taylor_pow(double x, double y, unsigned int n) /*funkce pro vypocet exp. funkce pomoci logaritmu pocitaneho metodou T.P.
                                                        funguje stejne jako funkce popsana nize, jen pouziva k vypoctu jinou funkci
                                                        pro vypocet logaritmu */
{
    double vysledek = 1;
    double newy = y;
    double delitel = 1;
    double logaritmus = taylor_log(x, n);
    double newlogaritmus = logaritmus;

    for(unsigned int i = 1; i < n; i++)
    {
        delitel = delitel * i;
        vysledek = vysledek + ((newy * newlogaritmus) / delitel);
        newy = newy * y;
        newlogaritmus = newlogaritmus * logaritmus;
    }

    return vysledek;
}

double taylorcf_pow(double x, double y, unsigned int n) /*funkce pro vypocet exp. funkce pomoci logaritmu pocitaneho metodou
                                                          zretezenych zlomku*/
{
    double vysledek = 1; //kazdy clanek polynomu se pricita k jednicce
    double newy = y; //promenna pro vypocet mocniny expomentu
    double delitel = 1; //delitel prvniho zlomku je faktorial z 1, coz je 1
    double logaritmus = cfrac_log(x, n); //volani funkce pro vypocet logaritmu ze zakladu
    double newlogaritmus = logaritmus; //promenna pro vypocet mocniny vysledku logaritmu

    for(unsigned int i = 1; i < n; i++) // cyklus se opakuje podle zadaneho poctu iteraci
    {
        delitel = delitel * i;  // vypocet faktorialu z delitele
        vysledek = vysledek + ((newy * newlogaritmus) / delitel); //pricitani jednoho clanku polynomu k celku
        newy = newy * y; //vypocet nove hodnoty exponentu
        newlogaritmus = newlogaritmus * logaritmus; //vypocet nove hodnoty logaritmu
    }

    return vysledek;
}

int main(int argc, char *argv[])
{
    if((argc != 5) && (argc != 4)) //vypis napovedy v pripade spatne zadanych argumentu
    {
        incorrectArguments();
        return 0;
    }

    if(strcmp(argv[1],"--log") == 0) //pripad argumentu volajiciho vypocet logaritmu
    {
        if(atoi(argv[3]) > 0 && atof(argv[2]) > 0) //pocet iteraci i argument musi byt vetsi nez 0
        {
            double reflog = log(atof(argv[2])); //vypocet logaritmu pomoci standardni funkce
            printf("\n       log(%g) = %.12g\n", atof(argv[2]), reflog); //vystup na obrazovku

            if(atof(argv[2]) == 1)
            {
                printf("Pro toto cislo nelze pouzit techniku retezenych zlomku.\n"); //vystup na obrazovku (nelze delit nulou)
            }
            else
            {
                double cfraclog = cfrac_log(atof(argv[2]),(unsigned int) atoi(argv[3])); //vypocet logaritmu pomoci zretezenych zlomku
                printf(" cfrac_log(%g) = %.12g\n", atof(argv[2]), cfraclog); //vystup na obrazovku
            }

            double taylorlog = taylor_log(atof(argv[2]),(unsigned int) atoi(argv[3])); //vypocet logaritmu pomoci T.P.
            printf("taylor_log(%g) = %.12g\n\n", atof(argv[2]), taylorlog); //vystup na obrazovku
        }

        else
        {
            incorrectVariables(); //vypis napovedy v pripade spatne zadanych promennych v argumentech
        }
    }

    else if(strcmp(argv[1],"--pow") == 0) //pripad argumentu volajiciho vypocet exp. fce
    {
        if(atof(argv[2]) > 0 && atoi(argv[4]) > 0) //pocet iteraci i zaklad musi byt vetsi nez 0
        {
            double refpow = pow(atof(argv[2]), atof(argv[3])); //vypocet exp. fce pomoci standardni funkce
            printf("\n         pow(%g,%g) = %.12g\n", atof(argv[2]), atof(argv[3]), refpow); //vystup na obrazovku

            //vypocet exp. fce taylorovym polynomem s vypoctem logaritmu pomoci T.P.
            double taylorpow = taylor_pow(atof(argv[2]), atof(argv[3]), (unsigned int) atoi(argv[4]));
            printf("  taylor_pow(%g,%g) = %.12g\n", atof(argv[2]), atof(argv[3]), taylorpow); //vystup na obrazovku

            if(atof(argv[2]) == 1)
            {
                printf("Pro toto cislo nelze pouzit techniku retezenych zlomku"); //vystup na obrazovku (nelze delit nulou)
            }

            else
            {   //vypocet exp. fce taylorovym polynomem s vypoctem logaritmu pomoci zretezenych zlomku
                double taylorcfpow = taylorcf_pow(atof(argv[2]), atof(argv[3]), (unsigned int) atoi(argv[4]));
                printf("taylorcf_pow(%g,%g) = %.12g\n\n", atof(argv[2]), atof(argv[3]), taylorcfpow); //vystup na obrazovku
            }
        }

        else
        {
            incorrectVariables(); //vypis napovedy v pripade spatne zadanych promennych v argumentech
        }

    }
    else
    {
        incorrectArguments(); //vypis napovedy v pripade spatne zadanych argumentu
    }

    return 0;
}
