#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct bajnoksag
{
    char name[32];
    struct bajnoksag *next;
} bajnoksag;

typedef struct csapat
{
    char name[60];
    int force;
    int goals;
    struct csapat *next;
} csapat;

csapat *pop_csapat(csapat *head) //foglalt területek felszbadítása
{
    if (head != NULL)
    {
        csapat *p = head;
        head = head->next;
        free(p);
    }
    return head;
}
bajnoksag *pop_bajnoksag(bajnoksag *head)
{
    if (head != NULL)
    {
        bajnoksag *p = head;
        head = head->next;
        free(p);
    }
    return head;
}

void dispose_csapatlist(csapat *head)
{
    while (head != NULL)
        head = pop_csapat(head);
}
void dispose_bajnoksaglist(bajnoksag *head) //foglalt területek felszabadítása
{
    while (head != NULL)
        head = pop_bajnoksag(head);
}

bajnoksag *scan_bajnoksag() // bajnoksagok beolvasasa fajlbol megnyitja a bajnokság.txt nevű szöveges fájlt és soronként beolvassa, az egyes elemeket strázsa nélküli láncolt listába fűzi
{
    FILE *league = fopen("bajnoksag.txt", "r");
    bajnoksag *head = NULL;
    bajnoksag *new;
    bajnoksag *curr;
    char name[32];
    while (fgets(name, 32, league) != NULL)
    {
        new = (bajnoksag *)malloc(sizeof(bajnoksag));
        new->next = NULL;
        strcpy(new->name, name);
        if (head == NULL)
        {
            head = new;
            curr = head;
        }
        else
        {
            curr->next = new;
            curr = new;
        }
    }
    fclose(league);
    return head;
}

void print_bajnoksag(bajnoksag *head) // valaszthato bajnoksagok kiirasa stdoutra
{
    printf("Valassz bajnoksagot:\n");
    while (head != NULL)
    {
        printf("%s", head->name);
        head = head->next;
    }
}
char *bajnoksag_valaszt(char *s) // bajnoksag kivalasztasa
{
    scanf("%s", s);
    char *c = ".txt\0";
    strcat(s, c);
    return s;
}
csapat *bajnoksag_megnyit(char *bajnoksag) // megkapja a választott bajnokság nevét pl.Magyar.txt, megnyitja a szöveges fájlt, ha nem létezik újat kér, beolvassa a csapatokat és láncolt listába fűzi, a lista kezdőcímét adja vissza
{
    FILE *csapatok = fopen(bajnoksag, "r");
    while (csapatok == NULL)
    {
        printf("Nincs ilyen bajnoksag, valassz ujat\n");
        csapatok = fopen(bajnoksag_valaszt(bajnoksag), "r");
    }
    csapat *head = NULL;
    csapat *new;
    csapat *curr;
    int force = 0;
    char name[60];
    while (fscanf(csapatok, "%s", name) > 0) 
    {
        csapat *new = (csapat *)malloc(sizeof(csapat));
        new->next = NULL;
        strcpy(new->name, name);
        fscanf(csapatok, "%d", &new->force);
        new->goals = 0;
        if (head == NULL)
        {
            head = new;
            curr = head;
        }
        else
        {
            curr->next = new;
            curr = new;
        }
    }
    fclose(csapatok);
    return head;
}
void print_csapatok(csapat *teams, int sajat) // valaszthato csapatok kiirasa
{
    if (sajat == 1)
    {
        printf("\nValassz csapatot: \n");
    }
    else
    {
        printf("\nValassz ellenfel csapatot: \n");
    }
    while (teams != NULL)
    {
        for (int i = 0; i < teams->force; i++)
        {
            printf("*");
        }
        printf(" %s\n", teams->name);
        teams = teams->next;
    }
}

csapat *csapat_scan(csapat *teams, csapat **valasztottcsapat) //  beolvassa a csapat nevét, ha létezik ilyen nevű csapat törli ezt az elemet a láncolt listából, és ez lesz a választott csapat, ha nincs nem csinál semmit, újra kell választani, visszaadja a megváltozott láncolt lista címét és a választott csapatot a paraméterlistán (**)

{
    csapat *ptr = teams;
    csapat *q;
    csapat *sajatcsapat = (csapat *)malloc(sizeof(csapat));
    sajatcsapat->next = NULL;
    int talalt = 1;
    while (talalt)
    {
        ptr = teams;
        scanf("%s", sajatcsapat->name);
        if (strcmp(teams->name, sajatcsapat->name) == 0)
        {
            sajatcsapat->goals = 0;
            sajatcsapat->force = teams->force;
            ptr = teams;
            teams = teams->next;
            talalt = 0;
            free(ptr);
        }
        else
        {
            q = ptr;
            ptr = ptr->next;

            while (ptr != NULL)
            {
                if (strcmp(ptr->name, sajatcsapat->name) == 0)
                {
                    sajatcsapat->force = ptr->force;
                    sajatcsapat->goals = 0;
                    q->next = ptr->next;
                    free(ptr);
                    talalt = 0;
                    break;
                }
                ptr = ptr->next;
                q = q->next;
            }
        }
        if (talalt)
        {
            printf("Nincs ilyen csapat:( valassz ujra\n");
        }
    }
    *valasztottcsapat = sajatcsapat;
    return teams;
}
csapat *merkozes(csapat *sajat_csapat, csapat *ellenfel_csapat, csapat *valasztott_bajnoksag) // randomszám generátorral kiszámítja a gólokat, majd kiírja a standard outputra, haszeretnénk még játszani meghívja saját magát
{
    srand(time(NULL));
    double ero = (sajat_csapat->force) / 10.0;
    int lottgol = (0.8 + ero) * (rand() % 5);
    ero = (ellenfel_csapat->force) / 10.0;
    int ellenlottgol = (0.8 + ero) * (rand() % 5);

    printf("%s [%d]-[%d] %s", sajat_csapat->name, lottgol, ellenlottgol, ellenfel_csapat);
    sajat_csapat->goals += lottgol;
    if (lottgol == ellenlottgol)
    {
        printf("\nDONTETLEN\n");
    }
    else if (lottgol > ellenlottgol)
    {
        printf("\nNYERTEL\n");
    }
    else
    {
        printf("\nVESZiTETTEL\n");
    }
    int jatek;

    while (1)
    {
        printf("\nSzeretnel meg jatszani? igen:nyomd meg az 1-est  nem:nyomd meg a 0-t\n");
        scanf("%d", &jatek);

        if (jatek == 1 || jatek == 0)
        {
            break;
        }
    }
    if (jatek)
    {
        print_csapatok(valasztott_bajnoksag, 0);
        valasztott_bajnoksag = csapat_scan(valasztott_bajnoksag, &ellenfel_csapat);
        merkozes(sajat_csapat, ellenfel_csapat, valasztott_bajnoksag); // rekurzivan hivja magat amedddig jatszani szeretnenk
    }
    return valasztott_bajnoksag;
}
void ranglista_kiir(csapat *sajatcsapat) // ranglistaba iras
{
    printf("a lott goljaid szama:%d\n", sajatcsapat->goals);
    printf("Add meg a neved(max 20 karakter, ekezetek nelkul):");
    char nev[21];
    scanf("%s", nev);
    FILE *ranglista = fopen("ranglista.txt", "a");
    fprintf(ranglista, "\n%s, golok:%d", nev, sajatcsapat->goals);

    fclose(ranglista);
}
void ranglista_megtek() // kiírja a standard outputra a korábbi felhasználók eredményeit
{
    int megtek;
    while (1)
    {
        printf("Meg akarod tekinteni a ranglistat? igen:nyomd meg az 1-est nem:nyomd meg a 0-t");
        scanf("%d", &megtek);
        if (megtek == 1 || megtek == 0)
        {
            break;
        }
    }
    if (megtek)
    {
        char sor[30];
        FILE *ranglista = fopen("ranglista.txt", "r");
        while (fgets(sor, 30, ranglista) != NULL)
        {
            printf("%s\n", sor);
        }
        fclose(ranglista);
    }
    printf("Vege 0");
    int vege; scanf("%d",&vege);
}
int main() // main csak a függvények hívása
{
    bajnoksag *bajnoksagok = scan_bajnoksag();

    print_bajnoksag(bajnoksagok);

    char *s;
    csapat *valasztott_bajnoksag = bajnoksag_megnyit(bajnoksag_valaszt(s));

    print_csapatok(valasztott_bajnoksag, 1);

    csapat *sajatcsapat;

    valasztott_bajnoksag = csapat_scan(valasztott_bajnoksag, &sajatcsapat);

    csapat *ellenfel_csapat;
    print_csapatok(valasztott_bajnoksag, 0);

    valasztott_bajnoksag = csapat_scan(valasztott_bajnoksag, &ellenfel_csapat);

    valasztott_bajnoksag = merkozes(sajatcsapat, ellenfel_csapat, valasztott_bajnoksag);

    ranglista_kiir(sajatcsapat);

    ranglista_megtek();
    dispose_csapatlist(ellenfel_csapat);
    dispose_csapatlist(sajatcsapat);
    dispose_bajnoksaglist(bajnoksagok);
    dispose_csapatlist(valasztott_bajnoksag);
    return 0;
}