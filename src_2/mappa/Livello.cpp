#include "../generale/libs.hpp"
//Generazione 

/* 

Crea dei file .lvl con dei bei layout.
0, ovvero lo spawn non deve essere al centro.
  Il posto dove si spawna viene deciso *in base* allo 0, non viceversa

tutte le stanze devono essere contigue e non diagonali, ricordati come sono fatte le porte






IGNORA TUTTO QUESTO
VVVVVVVVVVVVVVVVVVV

Livello -> 5x5
Generazione di un livello standard:
  Quindi generi un livello con 4 stanze tutte collegate
   
Poi per la generazione:
  -  new Livello();
  Loop con:
    - trovaprimolibero() : trova la prima stanza a partire dal centro che ha almeno uno dei quattro lati liberi
                           la prima stanza che trova questo metodo è la stanza centrale ([2][2]), poi va a quelle affinco, poi a quelle affianco di quelle affianco, etc.
    - generaStanza() : sceglie, a partire dalla stanza ritornata da "trovaprimolibero", una stanza casuale tra le quattro affianco, la genera se può (se stanza * è nullo), se non può ne sceglie un'altra
                       se non può sceglierne nessuna, allora esci da questo metodo e non aumenti il contatore del "numero di stanze generate"

      C'è un contatore quando generi le stanze, in questo loop, per certi valori il "new Stanza()" avrà parametri particolari (ex: numero 8 -> miniboss, numero 16 -> boss);

  Puoi decidere se far si che, dopo ogni generaStanza, si salvi la posizione di questa nuova stanza per creare la prossima
                    oppure che, dopo ogni generaStanza, richiama il metodo "trovaprimolibero" e usi queste coordinate per la generazione delle stanze 

  NON interpretare le stanze in questa funzione, ma dopo, quando hai finito
  La funzione "interpretaStanza()" di Stanza, inizializza la matrice cchar_t e la matrice logica in base a 4 bool, che indicano la presenza di altre stanze affianco o meno
                       
Devi far si che sia possibile fare
  void livello.stampa();
    // stampa la stanza attuale, tenuta come parametro
  void livello.cambiaStanza(int direzione)
    // che chiede di poter cambiare la stanza in base alla direzione (0 -> nord, 1->est, etc.)
  bool livello.camminabile(int x, int y)
    // Ritorna vero se si può camminare in queste coordinate, falso se non
  

  Dopo che vengono implementate altre entità, devi poter andare anche a fare tipo "livello.controllaNemici(player, entita)" -> dentro, per ogni entita, si controlla se player.contatto(entita[i])
```

Livello::Livello(){
    int current_x = 2;
    int current_y = 2;
    int stanza_counter = 0;
    while(stanza_counter < 16) {
        if(matrice_livello [current_y] [current_x] != NULL){
            scegli_lato(matrice_livello , &current_y, &current_x);
        //} else if (stanza_counter == 8 || stanza_counter == 16){
        //  matrice_livello [current_y] [current_y] = new Stanza(1);
        //    stanza_counter++;            
        } else{
            matrice_livello [current_y] [current_y] = new Stanza(0);
            stanza_counter++;
            matrice_livello [current_y] [current_y]->scegli_stanza();
        }
    }
}
*/

Livello::Livello(){
  int current_x = 0;
  int current_y = 0;
  int stanza_counter = 0;

  //alloco lo spazio per la matrice del livello
  this -> matrice_livello = new Stanza ** [DIM_MATRICE_LIVELLO_Y];
  for(int i=0; i < DIM_MATRICE_LIVELLO_Y; i++){
    this -> matrice_livello [i] = new Stanza * [DIM_MATRICE_LIVELLO_X];
    for(int j=0; j < DIM_MATRICE_LIVELLO_X; j++){
      this -> matrice_livello [i][j] = NULL; //da vedere
    }
  }

  FILE * fin;
  char livello_da_scegliere [100];  // Stringa contenente il nome del file della mappa
  //int idMappa = (rand() % 1 )+ 1; // Impostazione dell'id casuale della mappa

  // Creazione della stringa che contiene il percorso al file del livello scelto
  sprintf(livello_da_scegliere, "./mappa/matrici_livello/livello%s%d.lvl");
  fin = fopen( livello_da_scegliere , "r");
  
  //Leggo il file e mi trascrivo i numeri in una matrice di interi temporanea
  int matrice_numerica [DIM_MATRICE_LIVELLO_Y][DIM_MATRICE_LIVELLO_X];
  for(int i= 0; i < 5; i++){
      for(int j = 0; j < 5; j++){
          matrice_numerica [i][j] = fgetc(fin) - (int)'0'; // Traduco i numeri ascii in interi
       }
      fgetc(fin);
  }
  fclose(fin);
  

  //Imposto l'id della stanza da creare
  for(int i = 0; i < DIM_MATRICE_LIVELLO_Y; i++){
    for(int j = 0; j < DIM_MATRICE_LIVELLO_X; j++){
      if(matrice_numerica[i] [j] != 3){
        if(matrice_numerica[i] [j] == 0){       //mi serve per capire da dove iniziare
          current_x = j;
          current_y = i;
        }
        matrice_livello [i] [j] = new Stanza(matrice_numerica[i] [j]);
      } else {
        matrice_livello [i] [j] = NULL;
      }
    }
  }

  //alloco memoria per la matrice di livello

}

void Livello::crea_porte(){
  bool nord = false;
  bool sud = false;
  bool est = false;
  bool ovest = false;
  for(int i = 0; i < DIM_MATRICE_LIVELLO_Y; i++){
    for(int j = 0; j < DIM_MATRICE_LIVELLO_X; j++){
      if(this -> matrice_livello [i] [j] != NULL){
        if(this -> matrice_livello [i + 1] [j] != NULL){
          nord = true;
        }
        if (this -> matrice_livello [i - 1] [j] != NULL){
          sud = true;
        }
        if(this -> matrice_livello [i] [j + 1] != NULL){
          est = true;
        }
        if (this -> matrice_livello [i] [j - 1] != NULL){
          ovest = true;
        }
        this -> matrice_livello [i] [j] -> imposta_porte(nord, sud, est, ovest);
      }
    }
  }
};

//funzione stampa della morte che prende tutte le stampe

void Livello::stampa(){
  this->matrice_livello[this->current_y] [this->current_x]->stampa_stanza();
  
};


void Livello::cambia_stanza(int direzione){
  switch(direzione){
    case DIRECTION_NN:
    if(current_y-1 != 0){ //ricontrolla lo zero
      if(matrice_livello [current_y-1] [current_x] != NULL){
        this->current_y-1;
      }
    }
    break;
    case DIRECTION_OO:
    if(current_x-1 != 0){ //ricontrolla lo zero
      if(matrice_livello [current_y] [current_x-1] != NULL){
        this->current_x-1;
      }
    }
    break;
    case DIRECTION_EE:
    if(current_x+1 != DIM_MATRICE_LIVELLO_X){ //ricontrolla lo zero
      if(matrice_livello [current_y] [current_x+1] != NULL){
        this->current_x+1;
      }
    }
    break;
    case DIRECTION_SS:
    if(current_y+1 != DIM_MATRICE_LIVELLO_Y){ //ricontrolla lo zero
      if(matrice_livello [current_y+1] [current_x] != NULL){
        this->current_y+1;
      }
    }
    break;
  }
}
// logica della morte (continee tutte le logiche , fare una funzione che vede se nella stanza corrente se si interagisce con la port andando ancor a destra per esempio cambia di stanza)

void Livello::logica_della_morte(Player * player){
  //parte stanze
  if(matrice_livello [current_y] [current_y]->direzione_porta(player->getY(), player->getX()) == DIRECTION_NN){
    cambia_stanza(DIRECTION_NN);
  }
  if(matrice_livello [current_y] [current_y]->direzione_porta(player->getY(), player->getX()) == DIRECTION_OO){
    cambia_stanza(DIRECTION_OO);
  }
  if(matrice_livello [current_y] [current_y]->direzione_porta(player->getY(), player->getX()) == DIRECTION_EE){
    cambia_stanza(DIRECTION_EE);
  }
  if(matrice_livello [current_y] [current_y]->direzione_porta(player->getY(), player->getX()) == DIRECTION_SS){
    cambia_stanza(DIRECTION_SS);
  }
}


void Livello::stanza_corrente(){
  
}

void Livello::scegli_lato(Stanza *** matrice_livello , int * x, int *y){ 
    switch(rand() % 4){
        case 0:
        y++;
        break;
        case 1:
        x++;
        break;
        case 2:
        y--;
        break;
        case 3:
        x--;
        break;
    }
}