#include "mazzo.h"
#include <time.h>

/*IDEA: anzichè salvare lo stato della partita alla chiusura potremmo diversificare il menù
in partita rapida e nuova partita: la prima opzione genera una partita senza salvataggi, la seconda
salva in automatico ad ogni turno lo stato della partita così da non aver problemi nel definire
un'opzione per il salvataggio della partita*/


int next = 0;

int createDeck(card_t ptom[MAX_C]) {
    int i=0; //miscellanee
    int freq[TYP_C] = {5,3,3,4,2,1,9,8,4,1,4,3,2,5,4,3,3,3,5};
    int cas;

    FILE *fp;
    fp=fopen ("deckTyp.txt","r"); //popolamento array carte con lettura da file
    if (fp==NULL) { return 1; }
    printf("prova");

    srand(time(NULL));
    while (i<MAX_C) { //randomizza ordine carte e genera array
        do {
            cas=rand()%TYP_C; // [0,19]
        } while (freq[cas]==0); // Repeat if there is no more card of that type

        // 11*sizeof(char)*cas = Dimension in byte of one row
        fseek(fp, 11*sizeof(char)*cas, SEEK_SET);
        fscanf(fp, "%c %c %c %c %d \n", &ptom[i].edges[0], &ptom[i].edges[1], &ptom[i].edges[2], &ptom[i].edges[3], &ptom[i].campus);
        ptom[i].wifi=rand()%2; // [0,1]

        freq[cas]--;
        i++;
    }
    fclose(fp);
    return 0;
}

int saveCard(card_t ptoc[MAX_C], int x, int y) {
    FILE *fp;
    fp=fopen ("save/mazzo.txt","w");
    if (fp) { return 1; } //Return Error Code

    fprintf(fp, "%c %c %c %c %d %d %d %d \n",
        ptoc->edges[0],
        ptoc->edges[1],
        ptoc->edges[2],
        ptoc->edges[3],
        ptoc->campus,
        ptoc->wifi,
        x,
        y
    );

    fclose(fp);
    return 0;
}

int saveDeck(card_t ptom[MAX_C], int index) {
	int i;
    FILE *fp;
    fp=fopen ("save/mazzo.txt","a");
    if (fp) { return 1; } //Return Error Code

    for (i = index; i < MAX_C; i++) {
        fprintf(fp, "%c %c %c %c %d %d -1 -1 \n",
            ptom[i].edges[0],
            ptom[i].edges[1],
            ptom[i].edges[2],
            ptom[i].edges[3],
            ptom[i].campus,
            ptom[i].wifi);
    }
    fprintf(fp,"%d", index);

    fclose(fp);
    return 0;
}

int loadCards(card_t *matrix[MAX_C][MAX_C], card_t ptom[MAX_C]) {
    int index,x,y,i;
    FILE *fp;
    fp=fopen ("save/mazzo.txt","r");
    if (fp) { return 1; } //Return Error Code

    //Extract the index number.
    fseek(fp, -1 * (long)sizeof(int), SEEK_END);
    fscanf(fp, "%d", index);
    rewind(fp);

    //Prime (72-index) carte hanno anche le coordinate perciò creo due cicli differenziati
    for (i = 0; i < index; i++) {
        fscanf(fp, "%c %c %c %c %d %d %d %d \n",
            ptom[i].edges[0],
            ptom[i].edges[1],
            ptom[i].edges[2],
            ptom[i].edges[3],
            ptom[i].campus,
            ptom[i].wifi,
            x,
            y
        );
        matrix[x][y] = &ptom[i];
    }
    for (i = index; i < MAX_C; i++)
        fscanf(fp, "%c %c %c %c %d %d -1 -1 \n",
            ptom[i].edges[0],
            ptom[i].edges[1],
            ptom[i].edges[2],
            ptom[i].edges[3],
            ptom[i].campus,
            ptom[i].wifi
        );
    fclose(fp);
    return 0;
}
