/**
 * Kostra programu pro 3. projekt IZP 2018/19
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 *
 * Vojtech Jurka (xjurka08)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO
    if(cap > 0)
    {
        c->obj = malloc(sizeof(struct obj_t)*cap); //alokuje pamet pro urcity pocet objektu
        assert(c->obj); //kontrola alokace
        c->capacity = cap;  //nastavi kapacitu shluku
        if(c->obj == NULL)  //zrusi inicializaci shluku pokud se nepodarilo pamet alokovat
        {
            c->capacity = 0;
            return;
        }
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    free(c->obj); //uvolni alokovanou pamet pro objekty
    c->obj = NULL; //zrusi inicializaci shluku
    c->capacity = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
    if(c->size >= c->capacity)  //pokud nedostacuje kapacita shluku, rozsiri ho
    {
        resize_cluster(c, c->capacity + CLUSTER_CHUNK);
    }
    c->obj[c->size] = obj;  //priradi na konec pole objektu objekt
    c->size = c->size + 1;  //zvysi pocet objektu v shluku
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    for(int i = 0; i < c2->size; i++) //vsechny objekty z c2 postupne priradi k c1 pomoci append_cluster
    {
        append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);  //seradi objekty pomoci ID
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO

    narr = narr -1;  //snizi pocet shluku v poli o jeden
    clear_cluster(&carr[idx]); //odstrani dany shluk
    for(int i = idx; i < narr; i++)  //vsechny shluky v poli od daneho shluku posune o jeden clanek smerem doleva
    {
        carr[i] = carr[i+1];
    }
    return narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO

    float distance = sqrtf( ((o1->x - o2->x)*(o1->x - o2->x)) +  ((o1->y - o2->y)*(o1->y - o2->y)) );  //vypocet vzdalenosti pomoci souradnic
    return distance;
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO

    float min_distance = 1500; //na zacatek se nastavi hodnota vyssi nez uhlopricka pole souradnic
    float distance;

    for(int i=0; i < c1->size; i++)  //kazdy vsechny objekty prvniho shluku se porovnaji se vsemi druheho
    {
        for(int j=0; j < c2->size; j++)
        {
            distance = obj_distance(&c1->obj[i], &c2->obj[j]); //vypocet vzdalenosti
            if(distance < min_distance) min_distance = distance; //pokud je vzdalenost mensi, ulozi se jako minimalni
        }
    }
    return min_distance;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    if(narr == 1) //pokud je v poli jen jeden shluk
    {
        *c1 = 0;
        *c2 = 0;
        return;
    }

    float min_distance = -1;
    float distance;

    for(int i = 0; i < narr; i++) //pocitani vzdalenosti mezi shluky v poli
    {
        for(int j = i + 1; j < narr; j++)
        {
            distance = cluster_distance(&carr[i], &carr[j]); //vypocet vzdalenosti
            if(distance < min_distance || min_distance == -1) //hledani minimalni vzdalenosti, v pripade -1 se poprve nastavuje min_distance
            {
                *c1 = i;
                *c2 = j;
                min_distance = distance;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO
    FILE *soubor = fopen(filename, "r"); //otevreni souboru
    if(soubor == NULL) //kontrola otevreni
    {
        printf("Neporadarilo se otevrit zdrojovy soubor.\n");
        return -1;
    }

    char radek[50];
    fgets(radek,50,soubor);  //nacteni prvniho radku s informacemi o poctu objektu do stringu
    char string_cisla_poctu_objektu[50];

    for(int i = 0; radek[i+6] != '\n'; i++)  //prevedeni cisla v textu na int
    {
        string_cisla_poctu_objektu[i] = radek[i+6];
    }

    int pocet_objektu = atoi(string_cisla_poctu_objektu);

    *arr = malloc(sizeof(struct cluster_t) * pocet_objektu); //alokace pameti pro pole shluku
    assert(*arr); //kontrola alokace

    int ID;
    float x, y;
    struct obj_t obj;
    for(int i = 0; i < pocet_objektu; i++)  //po radcich postupne nacita informace o objektu a objekt uklada do shluku v poli
    {
        fscanf(soubor, "%d %f %f", &ID, &x, &y);
        obj.id = ID;
        obj.x = x;
        obj.y = y;

        init_cluster(&(*arr)[i], 1);
        append_cluster(&(*arr)[i], obj);
    }
    fclose(soubor);
    return pocet_objektu;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;

    // TODO
    if (argc != 3) //kontrola spravne zadanych argumentu
    {
        printf("Spatne zadane argumenty.");
        return 0;
    }

    int final_pocet_shluku = atoi(argv[2]); //prevzeti hodnoty pozadovaneho poctu zhluku z argumentu funkce

    if(final_pocet_shluku < 1) //kontrola spravne zadaneho argumentu
    {
        printf("Spatne zadane argumenty.\n");
        return 0;
    }

    int narr = load_clusters(argv[1], &clusters); //nacteni vsech objektu do pole shluku
    if(narr == -1) return 0; //kontrola uspesnosti nacteni

    if(narr < final_pocet_shluku) //kontrola spravne zadaneho pozadovaneho poctu shluku
    {
        printf("Pozadovany pocet shluku je vetsi nez pocet nactenych objektu.\n");
        return 0;
    }

    int c1, c2; //promenne adres nejblizsich shluku v poli
    int pocet_iteraci = narr - final_pocet_shluku; //vypocet poctu iteraci
    for(int i = 0; i < pocet_iteraci; i++)
    {
        find_neighbours(clusters, narr, &c1, &c2);    //nalezeni dvou nejblizsich shluku v poli
        merge_clusters(&clusters[c1], &clusters[c2]); //zkopirovani objektu druheho shluku do shluku prvniho
        narr = remove_cluster(clusters, narr, c2);    //odstraneni druheho shluku
    }

    print_clusters(clusters, final_pocet_shluku); //vypis vsech shluku
    for(int i = 0; i < final_pocet_shluku; i++)   //uvolnovani dynamicky alokovane pameti vsech shluku
    {
        clear_cluster(&clusters[i]);
    }
    clear_cluster(clusters); //uvolneni dynamicky alokovane pameti pole shluku

    return 0;
}
