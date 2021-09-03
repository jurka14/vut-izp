
/* TEXTOVY EDITOR BY VOJTECH JURKA (xjurka08) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void manual()
{
  printf("\nTEXT EDITOR - MANUAL\nPRIKAZY:\niCONTENT vlozi pred aktualni radek radek s obsahem CONTENT.\nbCONTENT vlozi na zacatek aktualniho radku obsah CONTENT.\naCONTENT vlozi na konec aktualniho radku obsah CONTENT.\nd smaze aktualni radek.\ndN smaze N radku (N > 0).\nr (remove EOL) na aktualnim radku odstrani znak konce radku.\nn posun aktualniho radku o jeden niz, tj. tiskne aktualni radek.\nnN posun aktualniho radku o N niz. (N > 0).\nq (quit) ukonci editaci, dalsi radky nezpracovava a ani netiskne.\ngX (goto) preskoc s aktualnim prikazem pro editaci na X-ty prikaz.\nX vyjadruje cislo radku (prikazu) v prikazovem souboru\n(1 <= X <= poctu radku prikazoveho souboru). Pozor, tento prikaz muze zpusobit,\nze se editor zacykli.\n\nEditor zpracovava prikazy po radcich a text upravuje tez po radcich.\n\nProgram spoustejte s argumentem nazvu souboru s prikazy a jako stdin\nuvedte nazev souboru s textem, ktery chcete upravovat.\n\n");
}

int main(int argc, char *argv[])
{
  //zkontroluje, jestli zadany spravny pocet argumentu, pokud ne, vypise napovedu
  if(argc != 2)
    {
     manual();
     return 0;
    }
  //otevre potrebne soubory a textova pole, se kterymi bude program pracovat
  FILE *prikazy;
  prikazy = fopen(argv[1], "r");
  char buff_text[1010];
  char buff_edit[1010];
  //cyklus, ktery sbira data ze souboru s prikazy a postupne je vyhodnocuje
  do
  {
    //pokud nezbyvaji zadne prikazy, vypise zbytek textu
    if(fgets(buff_edit, 1010, prikazy) == NULL)
    {
      while(fgets(buff_text, 1010, stdin) != NULL)
      {
        printf("%s", buff_text);
      }
      break;
    }

    if(buff_edit[0] == 'n')
    {

      //zjistovani poctu radku k preskoceni, pokud je za n napsano cislo
      if(buff_edit[1] != '\n')
      {
        //zde by pomohlo volani funkce atoi. Ta je ale zakazana, proto jsem musel implementovat vlastni funkci, ktera zjisti pocet radku z textu.
        int pocet_radku = 0;
        int cislo;
        int pocet_cisel;
        int adresa = 1;
        int nasobek;
        pocet_cisel = strlen(buff_edit)-2;
        if(pocet_cisel == 4) nasobek = 1000;
        if(pocet_cisel == 3) nasobek = 100;
        if(pocet_cisel == 2) nasobek = 10;
        if(pocet_cisel == 1) nasobek = 1;
//vezme vsechny cisla za sebou v textu a priradi jim spravnou decimalni hodnotu (stovky, desitky, jednotky, atd.) a secte je k sobe.
        for (int i = pocet_cisel; i > 0; i--)
        {

          cislo = buff_edit[adresa] - '0';
          adresa++;
          pocet_radku = pocet_radku + cislo * nasobek;
          nasobek = nasobek/10;
        }
        //preskakovani a vypisovani radku
        for(int i = 0; i < pocet_radku; i++)
        {
          if (fgets(buff_text, 1010, stdin) != NULL)
          {
            printf("%s", buff_text);
          }
        }
        continue;
      }
      //vypsani jednoho radku
      if (fgets(buff_text, 1010, stdin) != NULL)
      {
        printf("%s", buff_text);
      }
    }

    if(buff_edit[0] == 'i')
    //prikaz i vypise vsechna pismena z prikazoveho souboru krom toho prvniho.
    {
      for(unsigned int i=1; i < strlen(buff_edit); i++)
      {
        putchar(buff_edit[i]);
      }
    }

    if(buff_edit[0] == 'b')
      //prikaz b, napise na radek vsechny pismena z prikazoveho radku, krome toho prvniho. Za to prida radek ze vstupniho souboru.
    {
      for(unsigned int i=1; i < strlen(buff_edit); i++)
      {
        if(buff_edit[i] != '\n') putchar(buff_edit[i]);
      }

      fgets(buff_text, 1010, stdin);
      printf("%s", buff_text);
    }

    if(buff_edit[0] == 'a')
      //prikaz a dela to same, ale v opacnem poradi nez prikaz b
    {
      fgets(buff_text, 1010, stdin);
      int i = strlen(buff_text);
      buff_text[i-1] = 0;
      printf("%s", buff_text);


      for(unsigned int i=1; i < strlen(buff_edit); i++)
      {
        putchar(buff_edit[i]);
      }
      if(buff_edit[i] != '\n') printf("\n");
    }

    if(buff_edit[0] == 'd')
      //program se zde chova temer stejne jako pri prikazu n s tim rozdilem, ze radky nevypisuje
    {
      if(buff_edit[1] != '\n' && strlen(buff_edit) > 1)
      {
				//chtel jsem pouzit funkci atoi, ale zadani to zakazuje. Implementoval jsem proto vlastni funkci pro vypocet cisla z prikazu.
        printf("%s", buff_edit);
        int pocet_radku = 0;
        int cislo;
        int pocet_cisel;
        int adresa = 1;
        int nasobek;
        pocet_cisel = strlen(buff_edit)-2;
        if(pocet_cisel == 4) nasobek = 1000;
        if(pocet_cisel == 3) nasobek = 100;
        if(pocet_cisel == 2) nasobek = 10;
        if(pocet_cisel == 1) nasobek = 1;

        for (int i = pocet_cisel; i > 0; i--)
        {
          cislo = buff_edit[adresa] - '0';
          adresa++;
          pocet_radku = pocet_radku + cislo * nasobek;
          nasobek = nasobek/10;
        }
        for(int i = 0; i < pocet_radku; i++)
        {
          fgets(buff_text, 1010, stdin);
        }
        continue;
      }

      fgets(buff_text, 1010, stdin);
    }

    if(buff_edit[0] == 'r')
      //prikaz r vypise vsechny pismena z aktualniho radku textu, krome posledniho, coz je znak '\n'.
    {
      fgets(buff_text, 1010, stdin);
      for(unsigned int i=0; i < strlen(buff_text)-1; i++)
      {
        putchar(buff_text[i]);
      }
    }


  if(buff_edit[0] == 'g')
    {
//program se zde chová tez jako u prikazu n, ale nevypisuje radky ale ani je nepreskakuje. Rovnez zacina od prvniho radku pomoci resetu funkce fgets.
      int pocet_radku = 0;
      int cislo;
      int pocet_cisel;
      int adresa = 1;
      int nasobek;
      pocet_cisel = strlen(buff_edit)-2;
      if(pocet_cisel == 4) nasobek = 1000;
      if(pocet_cisel == 3) nasobek = 100;
      if(pocet_cisel == 2) nasobek = 10;
      if(pocet_cisel == 1) nasobek = 1;

      for (int i = pocet_cisel; i > 0; i--)
      {

        cislo = buff_edit[adresa] - '0';
        adresa++;
        pocet_radku = pocet_radku + cislo * nasobek;
        nasobek = nasobek/10;
      }
      //reset funkce fgets u radky s prikazy
      fclose(prikazy);
      prikazy = fopen(argv[1], "r");
      //preskakovani radku s prikazy
      for(int i = 1; i < pocet_radku; i++)
      {
        fgets(buff_edit, 1010, prikazy);
      }
    }
  //pokud je prikaz q, skonci program
  }
  while(buff_edit[0] != 'q');
  putchar('\n');

  fclose(prikazy);
  return 0;
}
