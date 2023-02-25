#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define Debut 510 // 8 h30 en "temps machine"
#define Fin 1050 // 17h30
#define FinAccueil 1020 // 17h

typedef struct client{ //Structure contenant les données du client
  int TpsArrive;
  int DureeAttente;
  int TpsService;
  int DebutService;
  int FinService;
}Client;

typedef struct queueclient{ //Element composant la liste chaînée
  Client donneeclient;
  struct queueclient *suivant;
}QueueClient;

typedef struct{ // Tête de la liste chaînée
  QueueClient *tete;
}ListeClient;

typedef struct horloge{ // Sert à convertir le temps (entier) en heure / minutes par la suite
  int heures;
  int minutes;
}Horloge;


float aleatoire();
Horloge TempsEnHeure(int temps);
void ModifierParametres(float *para);
void Simu1Jour(float *para);
void AjouterClientEnFin(ListeClient *listeclient, Client client);
void SupprimerPremierClient(ListeClient *listeclient);
void ProchainEvenement(ListeClient *listeclient, Client client, int *queue, float *queueMoy,int *temps);
void SimuMultiJour(float *para);
void Menu(float *parametres);


// Fonctions utilisées pour les 2 questions
float aleatoire() // Créer un nombre pseudo-aléatoire (explication du choix dans les livrables)
{
    int nombre = 0;
    for(int i =0; i<6;i ++){
        nombre += rand()%10 * pow(10,i);
    }
    return nombre/pow(10,6);   
}

Horloge TempsEnHeure(int temps){ // Transforme le temps (int) en heures / minutes (utile pour les printf)
  Horloge TpsEnMinutes;
  TpsEnMinutes.heures = temps/60;
  TpsEnMinutes.minutes = temps - TpsEnMinutes.heures*60;
  return TpsEnMinutes;
}

void ModifierParametres(float *para)
{
  bool quitter = false;
  int choix;
  float temposrv;
  while (!quitter)
  {
    printf("\n------------------- Redirection -------------------\n");
    printf("\n1  Modifier Lambda\n");
    printf("2  Modifier minsrv et maxsrv\n");
    printf("3  Modifier le nombre de jours de la simulation avec indicateur de performance\n");
    printf("4  quitter\n");
    printf("choix : ");
    scanf("%d", &choix);
    printf("\n------------------- Redirection -------------------\n");

    switch (choix)
    {
      case 1 :
        printf("Lambda vaut : %f \nQuelle valeur voulez vous lui donner ? \n",para[0]);
        scanf("%f",&para[0]);
        printf("Lambda vaut maintenant : %f",para[0]);
        break;
      case 2 :
        printf("minsrv vaut : %f et maxsrv vaut : %f \nQuelle valeur voulez vous leur donner ?\n",para[1],para[2]);
        scanf("%f,%f",&para[1],&para[2]);
        if(para[1] > para[2]) // Condition si on se trompe entre minsrv et maxsrv
        {
          para[1] = para[1] + para[2]; // Alors on échange juste les valeurs des deux variables
          para[2] = para[1] - para[2];
          para[1] = para[1] - para[2];
        }
        printf("[minsrv,maxsrv] valent maintenant : [%f,%f]",para[1],para[2]);
        break;
      case 3 :
        printf("Le nombre de jours vaut : %d \nQuelle valeur voulez vous lui donner ? \n",(int)(para[3]));// Entier "nécessaire" içi
        scanf("%f",&para[3]); //On laisse quand même le choix d'écrire un float même si je prendrai la partie entière à la fin
        printf("Le nombre de jours vaut maintenant : %d",(int)(para[3]));
        break;
      case 4 :
        quitter = true;
        break;
      default :
        printf("Choisissez un nombre inférieur à 4\n");
        break;
    }
  }
}

// Question 1
void Simu1Jour(float *para)
{
  // Pour la simulation en une journée, pas besoin de système de mémoire donc on écrase les informations du dernier client à chaque fois
  // Cela permet de gagner en mémoire en évitant d'utiliser une liste chaînée
  FILE* fichier = NULL;
  fichier = fopen("InfoClient.txt", "w+");
  fprintf(fichier,"Temps D'arrivée | Durée d'attente | Debut Service | Fin Service | N° Client \n"); // Préparation mise en forme
  int nclient = 1,heures,minutes;
  Client client; // Création d'un element client et initialisation de ses données
  client.TpsArrive = Debut; // On suppose qu'il a attendu devant l'entrée avant l'ouverture
  client.DureeAttente = 0;
  client.TpsService = (int)(para[1] + (rand()%(int)(para[2]-para[1]+1)));//Formule pour obtenir un nombre entier entre les bornes
  client.DebutService = Debut;
  client.FinService = client.DebutService + client.TpsService;
  while (client.TpsArrive <= FinAccueil)
  {
    if(client.FinService < Fin){
      fprintf(fichier,"    %02dh %02dm          %02dh %02dm           %02dh %02dm        %02dh %02dm        %02d\n",
      TempsEnHeure(client.TpsArrive).heures,TempsEnHeure(client.TpsArrive).minutes,TempsEnHeure(client.DureeAttente).heures,TempsEnHeure(client.DureeAttente).minutes,
      TempsEnHeure(client.DebutService).heures,TempsEnHeure(client.DebutService).minutes,TempsEnHeure(client.FinService).heures,TempsEnHeure(client.FinService).minutes,nclient);
      //Indigeste à lire mais cela sert juste à écrire les données sur une ligne en tranformant le temps avec la fonction TempsEnHeure
      //Ecriture des données du futur client sur les données de l'ancien :
      float U = aleatoire();
      client.TpsArrive += ceilf(-log(1-U)/para[0]);//On prend l'arrondi au dessus / Impossible d'avoir moins d'une minute de délai entre les clients
      client.DureeAttente = client.FinService-client.TpsArrive; //client(N).FinService - client(N+1).TempsArrivée
      if(client.DureeAttente <0){ // Durée ne peut être négative
        client.DureeAttente = 0;
      }
      client.TpsService = (int)(para[1] + (rand()%(int)(para[2]-para[1]+1)));
      client.DebutService = client.TpsArrive + client.DureeAttente;
      client.FinService = client.DebutService + client.TpsService;
      
    }
    else{ // Si le client arrive avant 17h30 mais est servi après
      fprintf(fichier,"    %02dh %02dm           Fermé             Fermé          Fermé         %02d\n",
      TempsEnHeure(client.TpsArrive).heures,TempsEnHeure(client.TpsArrive).minutes,nclient);
      // On suppose que les clients alors ne rentre pas dans l'enseigne car le personnel ne peut les servir à temps
      // On calcule donc seulement leur temps d'arrivé devant l'enseigne
      float U = aleatoire();
      client.TpsArrive += ceilf(-log(1-U)/para[0]);
    }
    nclient ++;
    
  }
  fclose(fichier);
  printf("\n------------------- Redirection -------------------\n");
}

// Fonctions utile pour la question 2
void AjouterClientEnFin(ListeClient *listeclient, Client client)
{
  QueueClient* nouveauclient = malloc(sizeof(*nouveauclient));
  nouveauclient->donneeclient = client;
  nouveauclient->suivant = NULL;
  if(listeclient->tete == NULL)
  {
    listeclient->tete = nouveauclient;
  }
  else
  {
    QueueClient* elt = listeclient->tete;
    while(elt->suivant != NULL) // On cherche le dernier élement de la liste chaînée
        {
            elt = elt->suivant;
        }
        elt->suivant = nouveauclient;
  }
}

void SupprimerPremierClient(ListeClient *listeclient)
{
  if(listeclient->tete != NULL)
  {
    QueueClient* premierElement = listeclient->tete;
    listeclient->tete = premierElement->suivant;
  }
}

// Cette fonction permet d'ordonner les événements dans le temps afin de calculer la taille de la queue ou sa moyenne
void ProchainEvenement(ListeClient *listeclient, Client client, int *queue, float *queueMoy,int *temps)
{
  *queue = *queue + 1;
  Client clientEnService;
  clientEnService = listeclient->tete->donneeclient;
  while(clientEnService.FinService <= client.TpsArrive) // 1er événement : Un client a finit d'être servi
  {
    if(*queue >0)
    {
      *queue = *queue -1;
    }
    *queueMoy = *queueMoy + *queue*(clientEnService.FinService-*temps); // = durée pendant laquelle la queue a eu cette valeur
    *temps = clientEnService.FinService; // On avance le temps après l'événement
    SupprimerPremierClient(listeclient); // On supprime le client qui a finit d'être servi de la liste chaînée
    if(listeclient->tete == NULL)
    {
      break;
    }
    clientEnService = listeclient->tete->donneeclient; //Le suivant prend sa place
  }
  *queueMoy = *queueMoy + *queue*(client.TpsArrive - *temps); // Aucune événmenet jusqu'à l'arrivé du client
  *temps = client.TpsArrive; // On avance le temps après l'événement
  AjouterClientEnFin(listeclient,client); // On ajoute le nouveau client
}

void SimuMultiJour(float *para){
  // Initialisation des variables pour l'indicateur de performance
  float tempsReponseMoy = 0, queueMoyTotal = 0, nbrNonServisTot = 0; 
  int nbrclientTot = 1, queueMax = 0; 
  printf("\nTaux journalier de clients non servis :\n"); // Préparation mise en forme
  for(int i=0; i < (int)(para[3]);i++){ 
    // Initialisation des variables qui se réintialisent chaque jour
    bool dernierclient = true; //Utile par la suite pour connaître le dernier client avant fermeture
    int nclient = 1, temps = Debut, queue = 0, tpsArrDernierClient = 0;
    float TauxNonServis = 0, queueMoyJournalier = 0;
    //Initialisation des données du premier client
    Client client;
    client.TpsArrive = Debut;
    client.DureeAttente = 0;
    client.TpsService = (int)(para[1] + (rand()%(int)(para[2]-para[1]+1)));
    client.DebutService = Debut;
    client.FinService = client.DebutService + client.TpsService;
    //On crée une liste chaînée et place les données du premier client à l'intérieur
    ListeClient *listeclient = malloc(sizeof(*listeclient));
    QueueClient *premierclient = malloc(sizeof(*premierclient));
    premierclient->donneeclient = client;
    premierclient->suivant = NULL;
    listeclient->tete = premierclient;
    //Même fonctionnement des boucles que pour la question 1
    while (client.TpsArrive <= FinAccueil)
    {
      if(client.FinService < Fin){
        //Calcule des données du prochain client
        float U = aleatoire();
        client.TpsArrive += ceilf(-log(1-U)/para[0]);
        client.DureeAttente = client.FinService-client.TpsArrive;
        if(client.DureeAttente <0)
        {
          client.DureeAttente = 0;
        }
        client.TpsService = (int)(para[1] + (rand()%(int)(para[2]-para[1]+1)));
        client.DebutService = client.TpsArrive + client.DureeAttente;
        client.FinService = client.DebutService + client.TpsService;
        ProchainEvenement(listeclient,client,&queue,&queueMoyJournalier,&temps);
        // Calcule de certaines performances
        if(queue > queueMax){
          queueMax = queue;
        }
        tempsReponseMoy += client.FinService - client.TpsArrive;
      }
      else{ // Si le client arrive avant 17h30 mais est servi après
        if(dernierclient) // Si c'est le dernier client servi
        {
          dernierclient = false;
          tpsArrDernierClient = client.TpsArrive;
          // Je calcule le temps moyen de queue journalier avec comme date l'ouverture et comme fin le temps d'arrivé du dernier servi
          queueMoyJournalier = queueMoyJournalier / (tpsArrDernierClient - Debut);
        }
        float U = aleatoire();
        client.TpsArrive += ceilf(-log(1-U)/para[0]);
        TauxNonServis=TauxNonServis + 1; // Chaque client dans cette boucle n'est pas servi
      }
      nclient ++;
    }
    if(tpsArrDernierClient == 0) // Si tout le monde a été servi
      {
        queueMoyJournalier= queueMoyJournalier /(Fin - Debut);
      }
    printf("Jour : %02d / Taux : %f\n", i+1,TauxNonServis/nclient);
    queueMoyTotal += queueMoyJournalier;
    nbrclientTot += nclient;
    nbrNonServisTot += TauxNonServis;
  }
  float debitjournalier = nbrclientTot/para[3]; // Débit journalier moyen
  tempsReponseMoy = tempsReponseMoy / (nbrclientTot-nbrNonServisTot);
  printf("\nTaille moyenne de la fille d'attente : %f\nTaille maximum de la fille d'attente : %d\nDebit journalier moyen : %f\nTemps de reponse moyen : %fm\n",queueMoyTotal/para[3],queueMax,debitjournalier,tempsReponseMoy);
  printf("\n------------------- Redirection -------------------\n");
}

void Menu(float *parametres)
{
  bool quitter = false;
  int choix;
  
  while (!quitter)
  {
    printf("\n1  Modifier les parametres de la modelisation.\n");
    printf("2  Realiser une simulation d'une journee et obtenir le fichier resume.\n");
    printf("3  Simuler sur plusieurs jours et obtenir les indicateurs de performance.\n");
    printf("4  quitter\n");
    printf("choix : ");
    scanf("%d", &choix);
    
    switch (choix)
    {
      case 1 :
        ModifierParametres(parametres);
        break;
      case 2 :
      
        Simu1Jour(parametres);
        break;
      case 3 :
        SimuMultiJour(parametres);
        break;
      case 4 :
        quitter = true;
        break;
      default :
        printf("choix incorrect\n");
        break;
    }
  }
}

void main()
{
    srand(time(NULL));
    float parametres[4] = {0.1,2,6,3}; // Lambda, Minserv, Maxserv, Jours
    // Ordre de grandeur Lambda : Petite enseigne -> Lambda = 0.1, Grande enseigne -> Lambda = 1
    int heures,minutes;
    Menu(parametres);
}
