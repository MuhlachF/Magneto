/*
  Centrale d'acquisition des mesures / magnétomètre
  

  Author : Frédéric MUHLACH
  2017-05-10

  Magneto-mesure

  VERSIONS DU PROGRAMME

  Version : 0.1  (2017/05/10) -> Prise en compte du clavier analogique
  Version : 0.2  (2017/05/11) -> Intégration et navigation dans les menus
  Version : 0.3  (2017/05/12) -> Ajout du module de paramétrage des mesures
  Version : 0.4  (2017/05/17) -> Configuration des modes de mesures
  Version : 0.5  (2017/05/30) -> Intégration du CAN
                DEBUG : NOM DE L'IDENTIFIANT LIMITE A 6 CARACTERES
  Version : 0.6  (2017/05/31) -> Ajout menu configuration spécifique
                Schlumberger ou Wenner
  Version : 0.7  (2017/05/31) -> Debug Timer clavier
  Version : 0.8  (2017/05/31) -> Debug BLOCAGE MENU PRINCIPAL
  Version : 0.9  (2017/06/01) -> Mesures spécifiques suivant Schlumberger
  Version : 0.10 (2017/06/01) -> Warning si nbMesures non défini
  Version : 0.11 (2017/06/01) -> Initialisation du support de stockage / carte SD
                              -> Augmentation de la luminosité / affichage extérieur
  Version : 0.12 (2017/06/02) -> Ajout fonction lectureValeurFloat() permettant la saisie de nombres décimaux
  Version : 0.13 (2017/06/03) -> Optimisation du code + Ajout fonction SuperFloatString
                              -> passage de précision 32 à 64 bits
  Version : 0.14 (2017/06/03) -> Optimisation fonction et ajout constante de précision LCD et FICHIER
                              -> Version buggée sur SuperString et coeff
  Version : 0.15 (2017/06/06) -> La precision des floats est données par String(CHAINE,PRECISION)
                              -> ATTENTION PRECISION DES CALCULS EN MODE FLOAT (ARDUINO UNO ET MEGA FLOAT CODE SUR 4 OCTETS !!!)
                              -> Intégration des mesures spécifiques suivant Wenner
  Version : 0.16 (2017/06/08) -> Optimisation du calcul des constantes / balayage des tableaux
                              -> simplificiation des menus 
  Version : 0.17 (2017/06/08) -> Ajout du sous-menu / paramétrage dipole-dipole
  Version : 0.18 (2017/06/08) -> Configuration des parametres dipole-dipole

  Version : 0.17 (2017/06/09) -> ajout du Menu I
                              -> réorganisation des fonctions / modification de la portée de certaines variables
                              -> Suppression d'un maximum de delay() pouvant être la source de nombreux problèmes
                              -> ajout du paramétrage des capteurs magnétiques
                              -> suppression du type float au profit du double (précision de 64 bits sur Arduino Due)

  Version : 0.18 (2017/06/09) -> Acquisition des données suivant profil pour les sondages electriques Dipôle-Dipôle et magnétiques
                              -> Ajout des commentaires
                              -> Debug confspecifique
                              -> Ajout de données / flux de sortie 

  Version : 0.19 (2017/06/13) -> Correction d'un Bug -> initialisation carte SD (Alimentation USB)
                              -> Correction du formatage pour le flux de sortie Schlumberger
                              -> Passage en gain de 2 pour des tensions d'entrée de -2.048 à 2.048V
                              -> Vérification des mesures
                              -> Attention en cas de recharge de carte SD faire un reset sur la carte pour prise en compte
  Version : 0.20 (2017/06/14) -> Fonction acquisition Dipole-Dipole    
  Version : 0.21 (2017/06/14) -> Fonction acquisition Dipole-Dipole  
  Version : 0.22 (2017/06/14) -> Ajout du sous-menu positionnement
  Version : 0.23 (2017/06/15) -> Modification menu Freq-Walking(Hz)
                              -> Correction d'un bug / débordement menu mesureI
                              -> Valeurs décimales sur 4 digits 
                              -> Remplacement des valeurs MN et AB (schlumberger) par OM et OA
                              -> Remplacement de Resistance du Shunt par Facteur de Conversion
                              -> Suppression du menu nombre de mesures -> possibilité de quitter le programme en cours
                              -> modification du menu
                              
                              1) Configuration
                                  0-Identifiant      // identifiant de fichier valeur numérique de 0 à 999999
                                  1-Mode             // selection d'un mode pour l'acquisition des mesures (mesures specifiques, selon profil ou carte)
                                  2-Mesures de I     // I est mesurée ou un coefficient est renseigné
                                  3-Frequence(Bst)   // Fréquence utilisée pour les sondages suivants profils ou cartes
                                  4-COnf Dip-Dip     // Configuration des electrodes pour les mesures dipole-dipole
                                  5-Dist capteurs    // Distance en cm entre les capteurs
                                  6-Conf profil      // configuration du profil : distance/X et pas/X
                                  7-Conf carte       // configuration de la carte : distance/X et /Y et pas/X et/Y
                                  8-Positionnement   // Choix pour l'utilisateur -> Aucun, position manuelle, GPS
                                  9-Synchro GPS      // Renseigne sur l'état de synchronisation avec les GPS
                                  10-Freq Keyboard   // Vitesse du clavier
                                  11-LCD-retro       // Configuration des valeurs du retro-éclairage de l'écran LCD
                                  12-Reglage Heure   // Réglage de l'heure
                                  13-Save config     // Permet de sauvegardes la configuration courante
                                  14-Load config     // Lecture du fichier de configuration sur la carte
                                  15-Reset config    // Réinitialise les données et le fichier de configuration
                                  
                              2) Start               // Lancement du processus d'acquisition en fonction de la configuration sélectionnée
                              
Version : 0.24 (2017/06/15) -> Modification de la valeur du retro eclairage                           
Version : 0.25 (2017/07/18) -> Refactoring de codes / acquisition des mesures 
Version : 0.26 (2017/07/18) -> Affichage des conversions en temps réel
Version : 0.27 (2017/07/19) -> Corrections bugs affichage mineurs
Version : 0.28 (2017/07/19) -> Nouvelle unité de mesure adpotée : cm au lieu du m
Version : 0.29 (2017/07/19) -> refactoring de codes et optmisation mémoire / flux de sortie
Version : 0.30 (2017/07/20) -> Ajout de la fonction inversion / Profil et Carte
Version : 0.31 (2017/07/20) -> Réinitiatilisation des valeurs par défaut dans le fichier de configuration
Version : 0.31 (2017/07/21) -> Ajout de la fonction de sauvegarde en EEPROM
Version : 0.32 (2017/07/21) -> Intégration du module RTC
Version : 0.33 (2017/07/26) -> Ajout de la fonctionnalité mesure suivant profil
Version : 0.34 (2017/07/26) -> Lisibilité du programme -> Ajout des énumérations
Version : 0.35 (2017/07/26) -> Simplificiation du progamme / gestion des mesures
Version : 0.36 (2017/07/26) -> Simplificiation du progamme 2 : fonction mesure()
Version : 1.0  (2017/07/27) -> Version de production
                                  
  A faire : Modifier le comportement en cas de non saisie -> retourner la valeur par défaut ajouter argument par défaut dans les fonctions valeurNum et valeurFloat
            //Ajout d'un bip sonore métronome + Bip de fin de des mesures
            //Ajouter un menu pour régler la vitesse du métronome
            // Intégration des coordonnées GPS
            Intégration de l'ADS1220
            //Intégration d'une librairie FLoat64 pour codage des Floats sur 64 bits
            //Le top : intégration d'une IRQ permettant de lancer la calibration du clavier analogique
            //Intégration des mesures magnétiques (distance des sondes)
            
*/

/*
 * STRUCTURE DE l'EEPROM
 * 
 * 
 * 
 */
#define VERSION_MAJEURE 1
#define VERSION_MINEURE 0
/*
  DESCRIPTION DES CONNEXIONS
  
  SD CARD / BUS SPI
  Connect the 5V pin to the 5V pin on the Arduino
  Connect the GND pin to the GND pin on the Arduino
  Connect CLK to pin 13 or 52 (MEGA)
  Connect DO to pin 12 or 50 (MEGA)
  Connect DI to pin 11 or 51 (MEGA)
  Connect CS to pin 10 or 53 (MEGA)

  WIRING CAN ADS115 / BUS I2C 
  Adresse : 
  
  MAPPING DES TOUCHES DU CLAVIER
  ######################################
  #    1     #   2(HAUT)  #     3      #
  ######################################
  # 4(GAUCHE)#  5(SELECT) #  6(DROITE) #
  ######################################
  #    7     #   8(BAS)   #     9      #
  ######################################
  #CLS / ESC et , #   0   #    OK      #
  ######################################
*/

/*
 * DECLARATION DES LIBRAIRES
 */
#include <Wire.h>               // Librairie dédiée à la gestion des communications I2C
#include "rgb_lcd.h"            // Librairie dédiée à l'affichage LCD RGB
#include <SPI.h>                // Librairie dédiée à aux communications SPI
#include <SD.h>                 // Librairie de gestion de la carte SD
#include <Adafruit_ADS1015.h>   // Gestion du CAN / ici le module ADS1115 est utilisé pour les conversions
#include <EEPROM.h>             // Librairie relative à l'EEPROM de l'Arduino
#include "DS1307.h"             // Gestion du module RTC
//#include <Adafruit_GPS.h>
//#include <SoftwareSerial.h>

/*
 * DEFINITION DES CONSTANTES
 */
#define WIRE_CLAVIER A0                   // Le clavier est connecté sur le port A0 de l'Arduino
#define TOLERANCE_ANALOG_CLAVIER 15       // Tolérance appliquée au clavier analogique par seuil (boutons 1 à 12)
#define VITESSE_ECHANTILLONNAGE_MAX 200   // Vitesse d'échantillonage maximum pour les mesures profil et carte
#define VITESSE_ACQUISITION 200           // Vitesse d'acquisition des mesures 200ms
#define VALEUR_I_MAX 10000                // valeur de I max en mA
#define DISTANCE_MAX 1000000              // Distance MN Max en cm (mesures Schlumberger)
#define NB_MESURES_MAX 1000               // Nombre de mesures max sur X
#define KEYBOARD_FREQ_MIN 100             // Frequence min du clavier
#define KEYBOARD_FREQ_MAX 500             // Frequence max du clavier
#define LUMINOSITE_MAX 255                // Valeur maximale de la luminosité
#define NOM_FICHIER_MIN 1                 // Nom du fichier Min 
#define NOM_FICHIER_MAX 999999            // Nom du fichier Max
#define CHIP_SELECT 5                     // Selection de la carte SD / BUS SPI

/*
 * ATTENTION LES CALCULS EN VIRGULES FLOTTANTES SONT DONNEES A TITRE D'INDICATION
 * double ET double SUR UNO ET MEGA SUR 4 OCTETS
 */
#define PRECISION 4                       // précision de 4 décimales sur l'écran LCD

rgb_lcd lcd;                              // Objet lcd couleur déclaré
int valeurRetro = 0;                      // Valeur du rétroéclairage
String flux;                              // Tampon pour le flux de sauvegarde sur carte SD
File monFichier;                          // l'objet monFichier sera utilisé pour l'enregistrement sur carte SD
DS1307 clock;                             // Définition de l'objet clock pour le module RTC

/* 
 * TYPE DE SONDAGE 
 */
enum eTypeSondage {Schlumberger,Wenner,Dipole,Magnetique};
enum ePositionnement {Aucun,Manuel,GPS};
enum eMesureI {Mesuree,Determinee};
enum eMode {Specifique,Profil,Carte};

/*
 * DECLARATION DES FONCTIONS : HEADER
 */
String LectureValeurNum(String affichage);              // Fonction permettant la lecture d'un nombre entier saisi au clavier
String LectureValeurFloat(String affichage);            // Fonction permettant la lecture d'un nombre décimal saisi au clavier
String lectureClavierNum();
String printTime(bool affichage);                       // Affiche la date et l'heure sur l'écran LCD et retourne un Timestamp
void pause();                                           // Fonction permettant d'effectuer une pause dans le programme (attente appui sur ENTER)
void affichageMenuPrincipal();                          // Affichage du menu principal
void affichageMenuConfiguration();                      // Affichage du sous-menu permettant de configurer l'instrument de mesures
void affectRecord();
void affectVar();
void initVar();                                         // Reinit des variables
void chargementConfig();                                // Chargement de la configuration à partir de l'EEPROM
void sauvegardeConfig();                                // Sauvegarde de la configuration dans l'EEPROM
int affichageMenuMode();                                // Affichage des types de sondage (sondages spécifiques, selon profil ou carte)
int affichageMenuConfSpec(int navBasse,int navHaute);   // Affichage des modes de mesures (Schlumberger, Wenner, Dipôle-Dipôle, ou magnetique)
int affichageMenuMesureI();                             // Sous-menu permettant de fixer I ou de le mesurer via la resistance de Shunt
int affichageMenuPositionnement();                      // Sous-menu permettant de choisir le type de positionnement
int modifFrequence();                                   // Frequence du CAN pour les mesures profil et carte
int modifNombreMesures();                               // Nombre de meusres spécifiques
String navigationMenu();                                // Fonction permettant de naviguer dans les menus
void lancementMesures();                                // Lancement des mesures (START est sélectionné)
String mesures(int numeroMesure);                       // Fonction dédiée aux mesures (Schlumberger, Wenner, Dipole...)
void mesuresCartes(int CoordX,int CoordY,int numMesure);// Alimentation des données suivant une carte
void alimentationFlux(enum eTypeSondage sondage);        // Enregistrement de l'entête sur sur carte µSD
void lancementAcquisition();                            // Acquisition et conversion 16 bits

String calculDelta (long &ValeurXMin, long &ValeurXmax, long &DistanceProfil, int &nbValeur, double &delta, String axe); // Fonction qui permet de calculer les valeurs Delta
bool verifFile (String identFichier);                   // Vérification de l'existance d'un fichier sur la carte SD
bool saveData (String identFichier,String Data);        // Fonction de sauvegarde de données

/*
 * DECLARATION DES MENUS
 */
const String menuGeneral[]={"1-CONFIGURATION","2-START"};
const String menuConfiguration[]=                         {"0-Identifiant","1-Mode","2-Mesure de I",
                                                          "3-Freq-Walking","4-Conf Dip-Dip","5-Dist capteurs",
                                                          "6-Conf profil","7-Conf carte","8-Positionnement",
                                                          "9-Synchro GPS","10-Freq Keyboard","11-LCD retro",
                                                          "12-Reglage Heure,","13-Save config","14-Load config","15-Reset config"};
                                                          
/*
 * DECLARATION DES VARIABLES
 */
int choixMenuPrincipal = 0;       // variable qui permet de naviguer dans le menu principal


/*
 * TABLEAUX DE MENU
 */
const String menuMode[]={"a->Mesures spec","b->Dyn/profil","c->Dyn/carte"};                                   // Sondage, Profil ou Carte
const String menuConfSpecifique[] = {"a->Schlumberger","b->Wenner","c->Dip-Dipole","d->Magnetique"};          // Dispositif Schlumberger, Wenner, Dipôle-Dipôle, Magnétique
const String menuMesureI[] = {"a->I mesuree","b->I fixee"};                                                   // I est soit mesurée à partir de U2 soit renseignee
const String menuPositionnement[] = {"a->Aucun","b->Manuel","c->GPS"};                                        // Positionnement 

/*
 * CONSTANTES / NOMBRE D'ELEMENTS DANS LES TABLEAUX
 */
const int NB_ELEMENTS_MENU_PRINCIPAL = sizeof(menuGeneral)/sizeof(*menuGeneral);                   // Calcul du nombre d'éléments du menu général
const int NB_ELEMENTS_MENU_CONFIGURATION = sizeof(menuConfiguration)/sizeof(*menuConfiguration);   // Calcul du nombre d'éléments du menu de configuration
const int NB_ELEMENTS_MENU_MODE = sizeof(menuMode)/sizeof(*menuMode);                              // Calcul du nombre d'éléments du menu mode
const int NB_ELEMENTS_MENU_CONF_SPEC = sizeof(menuConfSpecifique)/sizeof(*menuConfSpecifique);     // Calcul du nombre d'éléments du menu configuration spécifique
const int NB_ELEMENTS_MENU_MESURE_I = sizeof(menuMesureI)/sizeof(*menuMesureI);                    // Calcul du nombre d'éléments du menu configuration mesure I
const int NB_ELEMENTS_MENU_POSITIONNEMENT = sizeof(menuPositionnement)/sizeof(*menuPositionnement);// Calcul du nombre d'éléments du menu positionnement

/*
 * VARIABLES / CAN
 */
Adafruit_ADS1115 ads;         // Déclaration du convertisseur
double multiplier = 0.125F; /* ADS1115  @ // 1x gain   +/- 4.096V (16-bit results) */
//double multiplier = 0.0625F;  /* ADS1115  @ // 2x gain   +/- 2.048V (16-bit results) */

/*
 * VARIABLES DE CONFIGURATION GLOBALES
 */
long timerBack = 0;                                 // Variable utilisé pour le Timer du clavier
long timerNow = 0;                                  // Variable utilisé pour le Timer du clavier
String identifiant = "000000";                      // Nom du fichier par défaut
enum eMode mode = Specifique;                       // 0->Specifique, 1->Profil, 2->Carte
enum eTypeSondage confSpecifique = Schlumberger;    // 0->Schlumberger, 1-> Wenner, 2-> Dipole, 3->Magnetique
bool checkCard;

/* 
 *  VARIABLES MESURES SONDAGES ELECTRIQUES
 */
double tensionCanBus1;                 // Conversion de la valeur numérique (16 bits) en valeur décimale pour U1
double tensionCanBus2;                 // Conversion de la valeur numérique (16 bits) en valeur décimale pour U2
double intensiteDeduiteCanBus2;        // Calcul de l'intensité I2 déduite de U2 
String tensionCanBus1Str;              // Conversion de la tension U1 en chaîne de caractères
String tensionCanBus2Str;              // Conversion de la tension U2 en chaîne de caractères
String intensiteDeduiteCanBus2Str;     // Conversion de l'intensité I2 en chaîne de caractères

  
/*
 * VARIABLES SPECIFIQUES SCHLUMBERGER ET WENNER
 */
long distanceOA = 0;
long distanceOM = 0;
long distanceA = 0;

/*
 * CONFIGURATION GLOBALES
 */
enum eMesureI mesureI = Mesuree;                     // 0->I mesuree, 1-> I determinee
enum ePositionnement positionnement = Aucun;         // 0->Aucun, 1->Manuel, 2->GPS;
long keyboardTimer = 200;                            // frequence du clavier par défaut;

/* 
 *  VARIABLES DE GESTION DES SONDAGES ELECTRIQUES
 */
bool utilisationCoeffI = false;       // Si utilisationCoeff == false alors I est mesuré ;
double intensiteFixee = 1.0;          // sinon intensiteFixee est utilisée dans les calculs;
double facteurConversion = 1.0;       // Permet de calculer I2 à partir de U2 du Shunt;

/*
 * VARIABLES DE CONFIGURATION DIPOLE-DIPOLE
 */
long distanceElectrodesA = 10;        // Distance entre electrodes en cm;
int valeurN = 1;                      // Valeur de N

 /*
 * VARIBLES DE CONFIGURATION SONDAGE MAGNETIQUE
 */
int hauteurCapteur1 = 0;              // Hauteur du premier capteur magnetique
int hauteurCapteur2 = 0;              // Hauteur du second capteur magnetique
int vitesseEchantillonnage = 10;      // Utilisé en prise de mesures / profil et Carte 10Hz par défaut;

/* 
 * VARIBALES DEDIEES AUX MESURES SUIVANT PROFIL 
 */
long XMinProfil = 0;                  // Valeur Xmin pour l'acquisition suivant un profil
long XMaxProfil = 0;                  // Valeur Xmax pour l'acquisition suivant un profil
long DistanceXProfil = 0;             // Longueur du profil sur X
int nbMesuresXProfil = 0;             // Nombre de mesures effectuées le long du profil
double DeltaXProfil = 0;              // Delta entre chaque point sur X

 /* 
 *  VARIABLES POUR MESURES SUIVANT CARTE
 */
long XMinCarte = 0;                   // Valeurs Xmin et XMax pour l'acquisition suivant une carte
long XMaxCarte = 0;
long YMinCarte = 0;                   // Valeurs Ymin et YMax pour l'acquisition suivant une carte
long YMaxCarte = 0;
long DistanceXCarte = 0;              // Longueur de la carte sur X et Y
long DistanceYCarte = 0;              
double DeltaXCarte = 0;               // Delta entre chaque point sur X et Y
double DeltaYCarte = 0;
int nbMesuresXCarte = 0;              // Nombre de mesures effectuées sur X et Y
int nbMesuresYCarte = 0;

double CurseurX = 0;                  // Coordonnées de déplacement sur X et Y
double CurseurY = 0;


/* 
 *  TABLEAU DE MAPPING DU CLAVIER
 */
int seuilsAnalogClavier[12] = {35,75,115,155,195,235,275,315,355,395,435,475};

/*
 * DECLARATION D'UNE STRUCTURE SAUVEGARDEE EN EEPROM
 */
struct record
{
  int mesureI;
  int positionnement;
  long keyboardTimer;
  
  bool utilisationCoeffI;
  double intensiteFixee;
  double facteurConversion;
  
  long distanceElectrodesA;
  int valeurN;
  
  int hauteurCapteur1;
  int hauteurCapteur2;
  int vitesseEchantillonnage;
  
  long XMinProfil;
  long XMaxProfil;
  long DistanceXProfil;
  int nbMesuresXProfil;
  double DeltaXProfil;
  
  long XMinCarte;
  long XMaxCarte;
  long YMinCarte;
  long YMaxCarte;
  long DistanceXCarte;
  long DistanceYCarte;
  double DeltaXCarte;
  double DeltaYCarte;
  int nbMesuresXCarte;
  int nbMesuresYCarte;
};

record aRec;

/* ----------------------------------------------------------------------------------------------- */

/************************/
/* DEBUT FONCTION SETUP */
/************************/
void setup() 
{
  
  clock.begin();      // Initiatialisation de l'horloge
   
  pinMode(SS, OUTPUT);

  lcd.begin(16, 2);   // Configuration de la zone d'affichage : 2 lignes de 16 caractères
  lcd.setRGB(valeurRetro,valeurRetro,valeurRetro); 
  delay (500);
  
  checkCard = SD.begin(CHIP_SELECT); 
  delay (500);

  lcd.print("SOND ELEC V"+String(VERSION_MAJEURE)+"."+String(VERSION_MINEURE));
  delay(1000);
  
  lcd.clear();
  
  if (!checkCard)     // Vérification de létat de la carte SD
  {
    lcd.print("PROBLEME CARTE SD");
    pause();
  }
  else
  {
    lcd.print ("CARTE SD OK");
    delay (1000);
  }
  
  affectRecord();     // Initialisation de la structure de sauvegarde
  chargementConfig(); // Chargement de la configuration à partir de l'EEPROM
  affectVar();        // Affectation dans les variables globales

  
  /*
   * CONFIGURATION CAN ADS1115 
   */
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads.setGain(GAIN_ONE);           // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  //ads.setGain(GAIN_TWO);         // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads.begin();

  /*
   * Mode debug
   */
  Serial.begin(9600); 
}
/**********************/
/* FIN FONCTION SETUP */
/**********************/

/* -----------------------------------------------------------------------------------------------------------*/

/*********************/
/* DEBUT BOUCLE LOOP */
/*********************/
void loop()
{
  // Demande du choix de l'utilisateur
  // LectureValeurNum("Parameter : ").
  //calibrationClavier(); // a lancer pour récupérer les seuils analogiques du clavier / Affichage LCD
  lcd.setRGB(valeurRetro,valeurRetro,valeurRetro);
  affichageMenuPrincipal();
}

/*******************/
/* FIN BOUCLE LOOP */
/*******************/

/* -----------------------------------------------------------------------------------------------------------*/

/*****************************/
/* DECLARATION DES FONCTIONS */
/*****************************/

/*********************************************/
/* BLOC : FONCTION DE CALIBRATION DU CLAVIER */
/*********************************************/
/*
 * Fonction de calibrage du clavier
 * Cette fonction est forcée manuellement dans la boucle Loop() du programme
 * Elle permet de visualiser les seuils pour chaque touche du clavier analogique
 * Les données sont affichées après conversion sur l'écran LCD (de 0 à 1023)
 * ces seuils sont renseignés dans le tableau seuilsAnalogClavier[12] 
 */
void calibrationClavier(){
   lcd.clear();
   lcd.print ("CALIBRATION CLAVIER");
   pause();
   int AnalogClavier = 0;   
   
    while (true)  // BOUCLE TOUJOURS !
    {    
      lcd.print("Appuyez sur : ");
      AnalogClavier = 0;
      while (AnalogClavier < TOLERANCE_ANALOG_CLAVIER) // Boucle exécutée tant qu'aucun bouton n'est pressé
      {
        AnalogClavier = analogRead(WIRE_CLAVIER);
      }  
      lcd.println(AnalogClavier);
    } // FIN BOUCLE TOUJOURS
}

/*************************************************/
/* FIN BLOC : FONCTION DE CALIBRATION DU CLAVIER */
/*************************************************/

/* -----------------------------------------------------------------------------------------------------------*/

/*************************/
/* BLOC : MENU PRINCIPAL */
/*************************/
/*
 * Affichage des éléments de navigation du menu principal
 */
void affichageMenuPrincipal(){
  // Variables locales
  String choix;
  
  lcd.clear();
  lcd.print(menuGeneral[choixMenuPrincipal]);
  
  choix = navigationMenu();
  
  if (choix.equals("2")) choixMenuPrincipal--;
  if (choix.equals("8")) choixMenuPrincipal++;
  choixMenuPrincipal=constrain(choixMenuPrincipal,0,NB_ELEMENTS_MENU_PRINCIPAL-1);

  if ((choix.equals("5"))&&(choixMenuPrincipal == 0)) affichageMenuConfiguration();
  if ((choix.equals("5"))&&(choixMenuPrincipal == 1)) lancementMesures();
}
/*****************************/
/* FIN BLOC : MENU PRINCIPAL */
/*****************************/

/* -----------------------------------------------------------------------------------------------------------*/

/*************************************************/
/* BLOC : DEBUT FONCTION SOUS-MENU CONFIGURATION */
/*************************************************/
void affichageMenuConfiguration()
{
  // Déclaration des variables locales
  String choix;
  int choixMenuConfiguration = 0; // Variable permettant de naviguer dans le menu de configuration
  
  lcd.clear();
  lcd.print(menuConfiguration[choixMenuConfiguration]);

  /************************************/
  /* BLOC : NAVIGATION DANS SOUS-MENU */
  /************************************/
  while (!choix.equals("clear"))
  {
    choix = navigationMenu();

    if (choix.equals("2")) choixMenuConfiguration--;
    if (choix.equals("8")) choixMenuConfiguration++;
    choixMenuConfiguration=constrain(choixMenuConfiguration,0,NB_ELEMENTS_MENU_CONFIGURATION-1);

    /********************************/
    /* BLOC : SELECTION IDENTIFIANT */
    /********************************/
    if ((choix.equals("5"))&&(choixMenuConfiguration == 0)) // ITEM IDENTIFIANT
    {
      String buffer; 
      do
      {
        buffer = LectureValeurNum("Id actuel:"+identifiant);
        if (buffer.length() > 6) 
        {
           lcd.clear();
           lcd.print ("MAX 6 CHIFFRES");
           delay (1000);
        }
        else
        {
          identifiant = buffer;
        }
        
      } while (buffer.length() > 6);          // Boucle exécutée tant que longueur max de l'ID > 6
      
      if (!verifFile(identifiant))            // Vérification de l'existence du fichier
      {
        lcd.clear();
        lcd.print(identifiant+" SERA CREE");
      }
      pause();                                
    }
    /************************************/
    /* FIN BLOC : SELECTION IDENTIFIANT */
    /************************************/
    /* ----------------------------------------------------------- */
    /***************************************/
    /* BLOC : SELECTION DU TYPE DE SONDAGE */
    /***************************************/
    /*
     * CHOIX DU TYPE DE SONDAGE
     * L'utilisateur effectue un choix suivant les mesures à effectuer : spécifiques, suivant profil ou carte
     * Ensuite il sélectionne le mode de sondage proposée (Schlumberger, Wenner...) 
     * Le choix des modes de sondage proposés et fonction des mesures à effectuer
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 1))
    {
      mode = affichageMenuMode();

      // Mesures spécifiques
      if (mode == Specifique) 
          confSpecifique=affichageMenuConfSpec(0,2);  // Affichage des modes / Schlumberger, Wenner et Dipole-Dipole
         
      // Mesures selon profil ou carte
      if ((mode == Profil)||(mode == Carte)) 
          confSpecifique=affichageMenuConfSpec(2,3);  // Affichage des modes Dipole-Dipole et  magnétique
    }
    /*******************************************/
    /* FIN BLOC : SELECTION DU TYPE DE SONDAGE */
    /*******************************************/
    /* ----------------------------------------------------------- */
    /***************************/
    /* BLOC : MESURE INTENSITE */
    /***************************/
    /*
     * Mesure de l'intensité
     * 2 possibilités : 
     *      - soit la tension est mesurée sur un Shunt pour en déduire l'intensité
     *      - soit il n'existe pas d'appareillage pour mesurer cette tension et un coefficient est alors renseigné
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 2)) 
    {
      mesureI=affichageMenuMesureI(); 

      if (mesureI == Mesuree)                             // I sera mesurée
      {
        utilisationCoeffI = false;
        String Tampon;
        Tampon=LectureValeurFloat("Fact conv :"+String(facteurConversion));

        char carray[Tampon.length() + 1];           // Ici, on cherche à convertir un objet String en double
        Tampon.toCharArray(carray, sizeof(carray)); 
        facteurConversion = atof(carray);             
      }
      else                                           // Sinon I est déterminée par une constante
      {
        utilisationCoeffI = true;
        intensiteFixee = (LectureValeurNum("I(mA):"+String(intensiteFixee))).toInt();
        intensiteFixee = constrain (intensiteFixee,0,VALEUR_I_MAX);
      }
    }
    /*******************************/
    /* FIN BLOC : MESURE INTENSITE */
    /*******************************/
    /* ----------------------------------------------------------- */  
    /****************************/
    /* BLOC : FREQUENCE SONDAGE */
    /****************************/
    /*
     * Frequence du sondage pour les profils et les cartes
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 3)) 
    {
      vitesseEchantillonnage = (LectureValeurNum("Frequence(Hz) :"+String(vitesseEchantillonnage))).toInt(); 
      vitesseEchantillonnage = constrain (vitesseEchantillonnage,1,VITESSE_ECHANTILLONNAGE_MAX);     
    }
    /********************************/
    /* FIN BLOC : FREQUENCE SONDAGE */
    /********************************/
    /* ----------------------------------------------------------- */
    /*************************************************/
    /* BLOC : CONFIGURATION ELECTRODES DIPOLE-DIPOLE */
    /*************************************************/
    /*
     * Configuration des distances pour le sondage de type dipole-dipole 
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 4)) 
    {
      String Tampon;
      distanceElectrodesA=(LectureValeurNum("Dist a(cm):"+String(distanceElectrodesA))).toInt();      
      valeurN = (LectureValeurNum("Valeur N :"+String(valeurN))).toInt();
    }
    /*****************************************************/
    /* FIN BLOC : CONFIGURATION ELECTRODES DIPOLE-DIPOLE */
    /*****************************************************/
    /* ----------------------------------------------------------- */
    /******************************************/
    /* BLOC : CONFIGURATION DISTANCE CAPTEURS */
    /******************************************/
    /* 
     * Configuration de la distance des capteurs magnétiques 
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 5))
    {
      hauteurCapteur1 = (LectureValeurNum("H(cm) Cpt1 :"+String(hauteurCapteur1))).toInt();
      hauteurCapteur2 = (LectureValeurNum("H(cm) Cpt2 :"+String(hauteurCapteur2))).toInt();
    }
    /**********************************************/
    /* FIN BLOC : CONFIGURATION DISTANCE CAPTEURS */
    /**********************************************/
    /* ----------------------------------------------------------- */
    /*******************************/
    /* BLOC : CONFIGURATION PROFIL */
    /*******************************/  
    /* 
     * Configuration des données pour les mesures suivants profils
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 6))
    {
      String valeurDeltaX = calculDelta (XMinProfil,XMaxProfil,DistanceXProfil,nbMesuresXProfil,DeltaXProfil,"X");
      lcd.setCursor(0,1);
      LectureValeurNum("DELTA X="+valeurDeltaX);
    }
    /***********************************/
    /* FIN BLOC : CONFIGURATION PROFIL */
    /***********************************/  
    /* ----------------------------------------------------------- */
    /******************************/
    /* BLOC : CONFIGURATION CARTE */
    /******************************/  
    /*
     * Configuration des données pour les mesures suivants carte 
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 7)) 
    {
      String valeurDeltaX=calculDelta (XMinCarte,XMaxCarte,DistanceXCarte,nbMesuresXCarte,DeltaXCarte,"X");
      String valeurDeltaY=calculDelta (YMinCarte,YMaxCarte,DistanceYCarte,nbMesuresYCarte,DeltaYCarte,"Y");
      lcd.clear();
      lcd.print("DELTA X="+valeurDeltaX);
      lcd.setCursor(0,1);
      lcd.print("DELTA Y="+valeurDeltaY);
      pause();
    }
    /**********************************/
    /* FIN BLOC : CONFIGURATION CARTE */
    /**********************************/  
    /* ----------------------------------------------------------- */ 
    /***************************************/
    /* BLOC : CONFIGURATION POSITIONNEMENT */
    /***************************************/ 
    /* 
     * Configuration du mode de positionnement
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 8)) 
    {
       positionnement=affichageMenuPositionnement(); 
    }
    /*******************************************/
    /* FIN BLOC : CONFIGURATION POSITIONNEMENT */
    /*******************************************/ 
    /* ----------------------------------------------------------- */
    /***********************************/
    /* BLOC : VERIFICATION SYNCHRO GPS */
    /***********************************/ 
    /*
     * Vérification de la synchronisation GPS
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 9)) 
    {
 
    }
    /***************************************/
    /* FIN BLOC : VERIFICATION SYNCHRO GPS */
    /***************************************/ 
    /* ----------------------------------------------------------- */
    /******************************************/
    /* BLOC : CONFIGURATION FREQUENCE CLAVIER */
    /******************************************/ 
    /*
     * Paramétrage de la fréquence du clavier
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 10)) 
    {
      keyboardTimer = (LectureValeurNum("Kbrd Timer :"+String(keyboardTimer))).toInt();
      keyboardTimer = constrain (keyboardTimer,KEYBOARD_FREQ_MIN,KEYBOARD_FREQ_MAX);      
    }
    /**********************************************/
    /* FIN BLOC : CONFIGURATION FREQUENCE CLAVIER */
    /**********************************************/ 
    /* ----------------------------------------------------------- */
    /**********************************/
    /* BLOC : CONFIGURATION ECRAN LCD */
    /**********************************/ 
    /* 
     *  Activation du rétro-éclairage de l'écran LCD
     *  Modification de la couleur RGB
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 11))
    {
      valeurRetro = (LectureValeurNum("Lumin (0-255) :"+String(valeurRetro))).toInt();
      valeurRetro = constrain (valeurRetro,0,LUMINOSITE_MAX); 
    }
    /**************************************/
    /* FIN BLOC : CONFIGURATION ECRAN LCD */
    /**************************************/
    /* ----------------------------------------------------------- */
    /************************/
    /* BLOC : REGLAGE HEURE */
    /************************/
    /* 
     * Les données de configuration sont sauvegardées sur la carte SD 
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 12))
    {
        
      int annee = (LectureValeurNum("ANNEE :")).toInt();
      annee = constrain (annee,2017,9000);
      int mois = (LectureValeurNum("MOIS (1-12) :")).toInt();
      mois = constrain (mois,1,12);
      int jour = (LectureValeurNum("JOUR (1-31) :")).toInt();
      jour = constrain (jour,1,31);
      int heure = (LectureValeurNum("HEURE (0-23) :")).toInt();
      heure = constrain (heure,0,23);
      int  minute = (LectureValeurNum("MIN (0-59) :")).toInt();
      minute = constrain (minute,0,59);

      clock.fillByYMD(annee,mois,jour);   
      clock.fillByHMS(heure,minute,0);

      clock.setTime();                    // Ecrit les données sur la puce RTC

      printTime(1);                       // Affichage de l'heure et de la date
      
    }
    /****************************/
    /* FIN BLOC : REGLAGE HEURE */
    /****************************/
    /* ----------------------------------------------------------- */
    /*************************************************/
    /* BLOC : SAUVERGARDE CONFIGURATION SUR CARTE SD */
    /*************************************************/
    /* 
     * Les données de configuration sont sauvegardées sur la carte SD 
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 13))
    {
      sauvegardeConfig();
    }
    /*****************************************************/
    /* FIN BLOC : SAUVERGARDE CONFIGURATION SUR CARTE SD */
    /*****************************************************/
    /* ----------------------------------------------------------- */
    /********************************************************/
    /* BLOC : CHARGEMENT CONFIGURATION A PARTIR DE CARTE SD */
    /********************************************************/
    /*
     * Les données de configuration sont chargées à partir de la carte SD 
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 14))
    {
      chargementConfig();
      affectVar();
      
    }
    /************************************************************/
    /* FIN BLOC : CHARGEMENT CONFIGURATION A PARTIR DE CARTE SD */
    /************************************************************/
    /* ----------------------------------------------------------- */
    /**********************************/
    /* BLOC : REINITIALISATION CONFIG */
    /**********************************/
    /*
     * Chargement des paramètres par défaut
     */
    if ((choix.equals("5"))&&(choixMenuConfiguration == 15))
    {
      initVar();
      affectRecord();
      sauvegardeConfig();
    }

    /**************************************/
    /* FIN BLOC : REINITIALISATION CONFIG */
    /**************************************/
    
    lcd.clear();
    lcd.print(menuConfiguration[choixMenuConfiguration]);
  }
  /****************************************/
  /* FIN BLOC : NAVIGATION DANS SOUS-MENU */
  /****************************************/
} 
/*****************************************************/
/* FIN BLOC / DEBUT FONCTION SOUS-MENU CONFIGURATION */
/*****************************************************/

/* --------------------------------------------------------------------------------- */

/***********************************/
/* BLOC : FONCTION CALCUL DE DELTA */
/***********************************/
/*
 * calculDelta (Distance_Maximale_renseignée, Nombre_de_mesures_renseignée, retour_calcul_Dela, tolerance_max,tolerance_nombre_mesures_max,nom de l'axe)
 */
String calculDelta (long &ValeurXMin, long &ValeurXMax, long &DistanceProfil, int &nbValeur, double &delta, String axe)
{
  
  ValeurXMin = (LectureValeurNum(axe+"Min(cm):"+String(ValeurXMin))).toInt();
  ValeurXMin = constrain (ValeurXMin,0,DISTANCE_MAX);
  ValeurXMin = inverse (ValeurXMin);
  
  ValeurXMax = (LectureValeurNum(axe+"Max(cm):"+String(ValeurXMax))).toInt();
  ValeurXMax = constrain (ValeurXMax,0,DISTANCE_MAX);
  ValeurXMax = inverse (ValeurXMax);
  
  DistanceProfil = ValeurXMax - ValeurXMin;
    
  nbValeur = (LectureValeurNum("NB MESURES/"+axe+" :"+String(nbValeur))).toInt();
  nbValeur = constrain (nbValeur,2,NB_MESURES_MAX);
  delta = double(DistanceProfil) / double(nbValeur-1);

  String stringVal = String(delta,PRECISION);
    
  return stringVal;
}
/***************************************/
/* FIN BLOC : FONCTION CALCUL DE DELTA */
/***************************************/

/* --------------------------------------------------------------------------------- */

/***********************************/
/* BLOC : FONCTION INVERSE A = -A  */
/***********************************/
/*
 * Inverse la valeur donnée en paramètre
 */
long inverse(long valeur)
{
 String touche;
 bool validation = false;
 timerBack = millis();
 lcd.clear();
 lcd.print("1 -> inverser");
 lcd.setCursor (0,1);
 lcd.print(valeur); 
 
  // On demande la saisie tant que le bouton Ok n'a pas été appuyé
  while (validation != true) 
    {
       
      // Mise en place d'un timer pour éviter les répétitions des touches
      timerNow = millis();
      //Serial.println(timerNow);
      
      if ((timerNow - timerBack) >= keyboardTimer)
        {
          touche = lectureClavierNum();
          timerBack = millis();
                  
          if (touche.equals("ok")) // La saisie est validée
          {
            validation = true;
                      
            return valeur;
          }
  
          else if (touche.equals("1")) // On souhaite inverser le signe
          {
            valeur = -valeur;
          }
          lcd.clear();
          lcd.print("1 -> inverser");
          lcd.setCursor (0,1);
          lcd.print(valeur); 
       } 
   }
}

/***********************************/
/* BLOC : FONCTION INVERSE A = -A  */
/***********************************/

/* --------------------------------------------------------------------------------- */

/*****************************/
/* BLOC : FONCTION MENU MODE */
/*****************************/
/*
 * menuMode={"Sondage","Suivant profil","Suivant carte"};
*/
int affichageMenuMode(){
  
  // Variables locales
  String choix;
  int choixMenuMode;
  
  lcd.clear();
  if ( mode == choixMenuMode ) lcd.print("*");
  lcd.print(menuMode[choixMenuMode]);

  while (!choix.equals("clear"))
  {
    choix = navigationMenu();
  
    if (choix.equals("2")) choixMenuMode--;
    if (choix.equals("8")) choixMenuMode++;
    choixMenuMode=constrain(choixMenuMode,0,NB_ELEMENTS_MENU_MODE-1);

    if (choix.equals("5")) return choixMenuMode;  
    if (choix.equals("clear")) return mode;  
       
    lcd.clear();
    if ( mode == choixMenuMode ) lcd.print("*");
    lcd.print(menuMode[choixMenuMode]);
  }
}
/*********************************/
/* FIN BLOC : FONCTION MENU MODE */
/*********************************/

/* --------------------------------------------------------------------------------- */

/*************************************************/
/* BLOC : FONCTION MENU CONFIGURATION SPECIFIQUE */
/*************************************************/
int affichageMenuConfSpec(int navBasse, int navHaute){    // Tranche d'options affichées
  // Variables locales
  String choix;
  int choixMenuConfSpec = navBasse;
  lcd.clear();
  lcd.print("*");
  lcd.print(menuConfSpecifique[navBasse]);

  while (!choix.equals("clear"))
  {
    choix = navigationMenu();
  
    if (choix.equals("2")) choixMenuConfSpec--;
    if (choix.equals("8")) choixMenuConfSpec++;
    choixMenuConfSpec=constrain(choixMenuConfSpec,navBasse,navHaute);

    if (choix.equals("5")) return choixMenuConfSpec;  
          
    lcd.clear();
    if ( confSpecifique == choixMenuConfSpec ) lcd.print("*");
    lcd.print(menuConfSpecifique[choixMenuConfSpec]);
  }
}

/*****************************************************/
/* FIN BLOC : FONCTION MENU CONFIGURATION SPECIFIQUE */
/*****************************************************/

/* --------------------------------------------------------------------------------- */

/************************************/
/* BLOC : FONCTION MENU MESURE DE I */
/************************************/
/*
 * menuMesureI={"I calculee","I determinee"};
*/
int affichageMenuMesureI(){
  
  // Variables locales
  String choix;
  int choixMenuMesureI;
  
  lcd.clear();
  if ( mesureI == choixMenuMesureI ) lcd.print("*");
  lcd.print(menuMesureI[choixMenuMesureI]);

  while (!choix.equals("clear"))
  {
    choix = navigationMenu();
  
    if (choix.equals("2")) choixMenuMesureI--;
    if (choix.equals("8")) choixMenuMesureI++;
    choixMenuMesureI=constrain(choixMenuMesureI,0,NB_ELEMENTS_MENU_MESURE_I-1);

    if (choix.equals("5")) return choixMenuMesureI;  
    if (choix.equals("clear")) return mesureI;  
       
    lcd.clear();
    if ( mesureI == choixMenuMesureI ) lcd.print("*");
    lcd.print(menuMesureI[choixMenuMesureI]);
  }
}
/****************************************/
/* FIN BLOC : FONCTION MENU MESURE DE I */
/****************************************/

/* --------------------------------------------------------------------------------- */

/***************************************/
/* BLOC : FONCTION MENU POSITIONNEMENT */
/***************************************/
int affichageMenuPositionnement(){
  
  // Variables locales
  String choix;
  int choixMenuPositionnement;
  
  lcd.clear();
  if ( positionnement == choixMenuPositionnement ) lcd.print("*");
  lcd.print(menuPositionnement[choixMenuPositionnement]);

  while (!choix.equals("clear"))
  {
    choix = navigationMenu();
  
    if (choix.equals("2")) choixMenuPositionnement--;
    if (choix.equals("8")) choixMenuPositionnement++;
    choixMenuPositionnement=constrain(choixMenuPositionnement,0,NB_ELEMENTS_MENU_POSITIONNEMENT-1);

    if (choix.equals("5")) return choixMenuPositionnement;  
    if (choix.equals("clear")) return positionnement;  
       
    lcd.clear();
    if ( positionnement == choixMenuPositionnement ) lcd.print("*");
    lcd.print(menuPositionnement[choixMenuPositionnement]);
  }
}
/*******************************************/
/* FIN BLOC : FONCTION MENU POSITIONNEMENT */
/*******************************************/

/* --------------------------------------------------------------------------------- */

/**************************************************/
/* BLOC : GESTION DE LA NAVIGATION DANS LES MENUS */
/**************************************************/
 String navigationMenu()
{
 String touche;
 String valeurNum;
 
 timerBack = millis();

while (touche.equals(""))
  {
   timerNow = millis();
   //Serial.println(timerNow);
   if ((timerNow - timerBack) > keyboardTimer)
        {
          touche = lectureClavierNum();
          timerBack = timerNow;
          return touche;
        } // FIN DU TIMER / VERROU / TOUCHES
  } // APPUI SUR UNE TOUCHE 
} // FIN FONXTION NAVIGATION MENU
/******************************************************/
/* FIN BLOC : GESTION DE LA NAVIGATION DANS LES MENUS */
/******************************************************/

/* --------------------------------------------------------------------------------- */

/*******************************************************************/
/* BLOC : LECTURE D'UNE VALEUR ENTIERE NUMERIQUE SAISIE AU CLAVIER */
/*******************************************************************/
String LectureValeurNum(String affichage)
{
 String touche;
 String valeurNum;
 bool validation = false;
 timerBack = millis();
 lcd.clear();
 lcd.print(affichage); 
 
  // On demande la saisie tant que le bouton Ok n'a pas été appuyé
  while (validation != true) 
    {
    // Mise en place d'un timer pour éviter les répétitions des touches
    timerNow = millis();
    //Serial.println(timerNow);
    
    if ((timerNow - timerBack) >= keyboardTimer)
      {
        touche = lectureClavierNum();
        timerBack = millis();
                
        if (touche.equals("ok")) // La saisie est validée
        {
          validation = true;               
          return valeurNum;
        }

        else if (touche.equals("clear")) // On efface la saisie
        {
          if (valeurNum.length() > 0)
          //{
          valeurNum.remove(valeurNum.length()-1,1);
          //}
        }

        else  // On mémorise le chiffre
        {
           valeurNum = valeurNum + touche;
        }

        // Si touche

        lcd.clear();
        lcd.print(affichage);
        lcd.setCursor (2,1);
        lcd.print(valeurNum);      
     }
   }
}
/***********************************************************************/
/* FIN BLOC : LECTURE D'UNE VALEUR ENTIERE NUMERIQUE SAISIE AU CLAVIER */
/***********************************************************************/

/* --------------------------------------------------------------------------------- */

/*********************************************/
/* BLOC : PAUSE PROGRAMME / APPUI SUR TOUCHE */
/*********************************************/
void pause()
{
 String touche;
 bool validation = false;

 timerBack = millis();
  
  // On demande la saisie tant que le bouton Ok n'a pas été appuyé
  while (validation != true) 
    {
    // Mise en place d'un timer pour éviter les répétitions des touches
    timerNow = millis();
    //Serial.println(timerNow);
    
    if ((timerNow - timerBack) >= keyboardTimer)
      {
        touche = lectureClavierNum();
        timerBack = millis();
                
        if (touche.equals("ok")) // La saisie est validée
        {
          validation = true;             
        }
     }
   }
}
/*************************************************/
/* FIN BLOC : PAUSE PROGRAMME / APPUI SUR TOUCHE */
/*************************************************/

/* --------------------------------------------------------------------------------- */

/**********************************************************/
/* BLOC : LECTURE D'UNE VALEUR DECIMALE SAISIE AU CLAVIER */
/**********************************************************/
String LectureValeurFloat(String affichage)
{
 String touche;
 String valeurNum;
 bool validation = false;
 timerBack = millis();
 lcd.clear();
 lcd.print(affichage); 
 
  // On demande la saisie tant que le bouton Ok n'a pas été appuyé
  while (validation != true) 
    {
    // Mise en place d'un timer pour éviter les répétitions des touches
    timerNow = millis();
    //Serial.println(timerNow);
    
    if ((timerNow - timerBack) >= keyboardTimer)
      {
        touche = lectureClavierNum();
        timerBack = millis();
                
        if (touche.equals("ok")) // La saisie est validée
        {
          validation = true;
          return valeurNum;
        }

        else if (touche.equals("clear")) // On ajoute la virgule
        {
          valeurNum = valeurNum + ".";
        }

        else  // On mémorise le chiffre
        {
           valeurNum = valeurNum + touche;
        }

        // Si touche

        lcd.clear();
        lcd.print(affichage);
        lcd.setCursor (2,1);
        lcd.print(valeurNum);      
     }
   }
}
/**************************************************************/
/* FIN BLOC : LECTURE D'UNE VALEUR DECIMALE SAISIE AU CLAVIER */
/**************************************************************/

/* --------------------------------------------------------------------------------- */

/***************************/
/* BLOC MAPPING DU CLAVIER */
/***************************/
String lectureClavierNum()
{
  String Buffer;
  int AnalogClavier = 0;
  
    // La boucle se répète tant qu'aucun bouton n'est appuyé 
    while (AnalogClavier < 25) 
    {
      AnalogClavier = analogRead(WIRE_CLAVIER);
      //Serial.println(AnalogClavier);
    }
    if ((AnalogClavier >= seuilsAnalogClavier[0] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[0] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "ok";
    if ((AnalogClavier >= seuilsAnalogClavier[1] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[1] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "0";
    if ((AnalogClavier >= seuilsAnalogClavier[2] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[2] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "clear";
    if ((AnalogClavier >= seuilsAnalogClavier[3] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[3] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "9";
    if ((AnalogClavier >= seuilsAnalogClavier[4] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[4] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "8";
    if ((AnalogClavier >= seuilsAnalogClavier[5] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[5] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "7";
    if ((AnalogClavier >= seuilsAnalogClavier[6] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[6] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "6";
    if ((AnalogClavier >= seuilsAnalogClavier[7] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[7] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "5";
    if ((AnalogClavier >= seuilsAnalogClavier[8] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[8] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "4";
    if ((AnalogClavier >= seuilsAnalogClavier[9] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[9] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "3";
    if ((AnalogClavier >= seuilsAnalogClavier[10] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[10] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "2";
    if ((AnalogClavier >= seuilsAnalogClavier[11] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[11] + TOLERANCE_ANALOG_CLAVIER)) Buffer = "1";    
//    Serial.println(AnalogClavier);
    return Buffer;
}
/*******************************/
/* FIN BLOC MAPPING DU CLAVIER */
/*******************************/

/* --------------------------------------------------------------------------------- */

/*******************************************/
/* BLOC : FONCTION ACQUISITION DES MESURES */
/*******************************************/
String mesures(int numeroMesure)
{
  String tampon;                       // Collecte les informations à sauvegarder
  String tamponPosition;               // Collecte les informations pour les positions
  String tamponDistance;               // Informations complémentaires à sauvegarder
  double rhoa;                         // Variable dédié au calcul de Rhoa
  String rhoaStr;                      // Conversion de Rhoa en chaîne de caractères
   
  tamponPosition = fluxPositionnement(); // Récupération du flux de données pour le positionnement

  // Calcul de la résisitivité apparente 
  if (confSpecifique == Schlumberger)
  {
    distanceOA = (LectureValeurNum("Distance OA(cm):")).toInt();
    distanceOA = constrain (distanceOA,1,DISTANCE_MAX);
    
    lancementAcquisition();
    
    rhoa = PI * (double(distanceOA)/100*double(distanceOA)/100 - double(distanceOM)/100*double(distanceOM)/100) / (2 * double(distanceOM)/100) * (tensionCanBus1 / intensiteDeduiteCanBus2);
    tamponDistance = String (distanceOA)+","+String(distanceOM);
  }
  else if (confSpecifique == Wenner)
  {
    distanceA = (LectureValeurNum("Distance A(cm):")).toInt();
    distanceA = constrain (distanceA,1,DISTANCE_MAX);
    lancementAcquisition();  
    rhoa = 2 * PI * float(distanceA)/100 * (tensionCanBus1 / intensiteDeduiteCanBus2);
    tamponDistance = String(distanceA);
  }
  
  else if (confSpecifique == Dipole)
  {
    rhoa = PI * double(distanceElectrodesA)/100 * valeurN * (valeurN + 1)*(valeurN + 2) * (tensionCanBus1 / intensiteDeduiteCanBus2);  
    tamponDistance = "";
    lancementAcquisition();  
  }
     
  // Conversion des données en chaine de caractères
  rhoaStr = String(rhoa,PRECISION);

  // Affichage des calculs sur l'écran LCD
  affichageCalculs (tensionCanBus1Str,intensiteDeduiteCanBus2Str,rhoaStr);

  // Alimentation du tampon avec les données collectées
  tampon = String(numeroMesure)+","+tamponDistance+","+rhoaStr+","+tensionCanBus1Str+","+tensionCanBus2Str+","+intensiteDeduiteCanBus2Str+","+tamponPosition+"\n";
 
  return tampon;
}

void mesuresCartes(int CoordX,int CoordY,int numMesure)
{
   CurseurX = DeltaXCarte*(CoordX-1);
   CurseurY = DeltaYCarte*(CoordY-1);
            
   lcd.clear();
   lcd.print("Cd:"+String(CoordX)+"/"+String(nbMesuresXCarte)+"-"+String(CoordY)+"/"+String(nbMesuresYCarte));
   lcd.setCursor(0,1);
   lcd.print(String(CurseurX,0)+"-"+String(CurseurY,0)+"cm");
   pause();

   String fluxDeDonnees = mesures(numMesure);
   Serial.print(fluxDeDonnees);
        
   saveData(identifiant,fluxDeDonnees);//FONCTION DE SAUVEGARDE DES DONNEES
}

/***********************************************/
/* FIN BLOC : FONCTION ACQUISITION DES MESURES */
/***********************************************/

/* --------------------------------------------------------------------------------- */

/***************************************/
/* BLOC : FONCTION FLUX POSITIONNEMENT */
/***************************************/
String fluxPositionnement()
{  
   String tampon="";
   long positionnementX = 0;                // Variables utilisées pour les informations de positionnement
   long positionnementY = 0;                
   
   if ((positionnement == Aucun) && ((mode == Profil) || (mode == Carte)))
   {
    positionnementX = CurseurX;
    positionnementY = CurseurY;
   }
   
   else if (positionnement == Manuel)    // Positionnement manuel
   {
    positionnementX = (LectureValeurNum("Position X(cm) :")).toInt();
    positionnementY = (LectureValeurNum("Position Y(cm) :")).toInt();
   }

  tampon = String(positionnementX)+","+String(positionnementY);
  return tampon;
}



/*************************************************/
/* FIN BLOC : FONCTION ACQUISITION DIPOLE-DIPOLE */
/*************************************************/

/* --------------------------------------------------------------------------------- */

/*************************************/
/* BLOC : FONCTION AFFICHAGE DONNEES */
/*************************************/
/* 
 * Affichage des données mesurées et calculées pour les sondages electriques 
 */
void affichageCalculs(String tensionU1, String intensiteI2, String resistanceApp)
{
  lcd.clear();
  lcd.print("U1 : "+tensionU1+"mV");
  lcd.setCursor(0,1);
  lcd.print("I2 : "+intensiteI2+"mA");
  pause();

    // Affichage de la résistance apparente
  lcd.clear();
  lcd.print("Rhoa (approx) :");
  lcd.setCursor(0,1);
  lcd.print(resistanceApp+" Ohms/m");
  pause();
 }
/*****************************************/
/* FIN BLOC : FONCTION AFFICHAGE DONNEES */
/*****************************************/

/* --------------------------------------------------------------------------------- */

/********************************************************/
/* BLOC : FONCTION DE VERIFICATION FICHIER SUR CARTE SD */
/********************************************************/
/* 
 *  verifFile = 0 -> opération effectuée
 *  verifFile = 1 -> problème rencontré
 */
bool verifFile(String identFichier)
{
  lcd.clear();
  // Vérifier la présence d'un fichier portant le même nom
  if (SD.exists(identFichier))
  {
    lcd.print("FILE : "+identFichier);
    lcd.setCursor(0,1);
    lcd.print("TROUVE");
    pause();
    return 1;
  }
  else
  {
    lcd.print("FILE : "+identFichier);
    lcd.setCursor(0,1);
    lcd.print("NON TROUVE");
    pause();
    return 0;
  }
}
/************************************************************/
/* FIN BLOC : FONCTION DE VERIFICATION FICHIER SUR CARTE SD */
/************************************************************/

/* --------------------------------------------------------------------------------- */

/*******************************************/
/* BLOC : FONCTION ACQUISITION DES MESURES */
/*******************************************/

void lancementAcquisition()
{
  int AnalogClavier = 0;
    
  while (!((AnalogClavier >= seuilsAnalogClavier[0] - TOLERANCE_ANALOG_CLAVIER) && (AnalogClavier <= seuilsAnalogClavier[0] + TOLERANCE_ANALOG_CLAVIER)))
  {
    lcd.clear();
    tensionCanBus1 = double(ads.readADC_Differential_0_1()) * multiplier; // Mesure de la tension V1 et conversion à l'aide du coefficient
    tensionCanBus1Str = String(tensionCanBus1,PRECISION);
    
    lcd.print("U1:"+tensionCanBus1Str+"mV");
    lcd.setCursor(2,0);
    if (utilisationCoeffI)                                            // La valeur de I est fixée par l'utilisateur
    {
      intensiteDeduiteCanBus2 = intensiteFixee;
      tensionCanBus2Str = "I FIXEE";
    }
    else                                                              // Sinon la mesure de I est effectuée via le Shunt
    {
      tensionCanBus2 = double(ads.readADC_Differential_2_3()) * multiplier;             // Mesure de la tension V2 du Shunt pour en déduire I2
      tensionCanBus2Str = String(tensionCanBus2,PRECISION);
      intensiteDeduiteCanBus2 = tensionCanBus2 * facteurConversion; 
    }   
    intensiteDeduiteCanBus2Str = String(intensiteDeduiteCanBus2,PRECISION);              
    lcd.setCursor(0,1);
    lcd.print("I2:"+intensiteDeduiteCanBus2Str+"mA");

    
    delay (200);
    AnalogClavier=analogRead(WIRE_CLAVIER);
  }
}

/***********************************************/
/* FIN BLOC : FONCTION ACQUISITION DES MESURES */
/***********************************************/

/* --------------------------------------------------------------------------------- */

/***********************************************************/
/* BLOC : FONCTION DE SAUVERGARDE DES DONNEES SUR CARTE SD */
/***********************************************************/
/* 
 *  saveData = 0 -> enregistrement effectué
 *  saveData = 1 -> problème lors de l'enregistrement
 */
bool saveData (String identFichier,String Data)
/*
 * FONCTION GERANT LA SAUVEGARDE DES FLUX 
 * Renvoie 0 si la sauvegarde s'est bien passée
 * et 1 si un problème a été constaté
 */
{
  monFichier = SD.open(identFichier, FILE_WRITE);
  
  // 
  if (monFichier) // Si le fichier est Ok, on peut lancer les opérations d'écriture
  {  
    monFichier.print(Data);
    delay(200);
    monFichier.close(); // On ferme proprement le fichier
    return 0;
  } 
  else 
  {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("PROBLEME SAUVERGARDE");
    pause();
    return 1;   // Problème à l'ouverture du fichier
  }
} //FIN FONCTION SAUVEGARDE
/***************************************************************/
/* FIN BLOC : FONCTION DE SAUVERGARDE DES DONNEES SUR CARTE SD */
/***************************************************************/

/* --------------------------------------------------------------------------------- */

/*******************************/
/* BLOC / ALIMENTATION DU FLUX */
/*******************************/
/* 
         *  FORMAT : 
         *  -----------------------------------
         *  
         *  JJ/MM/YY HH-MM
         *  IDENTIFIANT DE FICHIER : ID
         *  
         *  SONDAGE ELECTRIQUE SPECIFIQUE : TYPE DE MESURE
         *  
         *  CONSTANTES DEFINIES :
         *  I calculée ou fixée
         *  Distance a
         *  Coefficient N
         *  
         *  ID de mesure,RHOA,U1,I2,DELTA,(GPS)
         */
void alimentationFlux (enum eTypeSondage sondage)
{
  String TypedeSondage = "";
  String constantesComplementaires="";
  String EtiquetteVariablesSondage="";
  
  if (sondage == Schlumberger)
  {
    TypedeSondage = "SCHLUMBERGER";
    EtiquetteVariablesSondage = "DISTANCE OA(cm), DISTANCE OM(cm)";
  }
  else if (sondage == Wenner)
  {
    TypedeSondage = "WENNER";
    EtiquetteVariablesSondage = "DISTANCE A(cm)";
  }
  else if (sondage == Dipole)
  {
    TypedeSondage = "DIPOLE-DIP";
    constantesComplementaires = "DISTANCE ENTRE ELECTRODES A(cm) :"+String(distanceElectrodesA)+"\nVALEUR DE N :"+String(valeurN)+"\n";
  }
  else if (sondage == Magnetique)
  {
    TypedeSondage = "MAGNETIQUE";
  }
  
  flux = "-----------------------------\n\n";
  flux = flux + printTime(0);
  flux =  flux + "\nIDENTIFIANT DES MESURES : "+identifiant+"\n"+"MESURE :"+TypedeSondage+"\n"+"\nCONSTANTES DEFINIEES :";  
                        
  if (utilisationCoeffI == true)
  {
     flux = flux+"\nINTENSITE I2 FIXEE :"+intensiteFixee;
  }
  else
  {
     flux = flux+"\nINTENSITE I2 CALCULEE, FACTEUR DE CONVERSION :"+facteurConversion;
  }
  flux = flux+"\n"+constantesComplementaires;
  
  flux = flux+"\n\n"+"ID MESURE, "+EtiquetteVariablesSondage+", RHOA(Ohms/m), TENSION U1(mV), TENSION U2 (mV), COUTANT I2(mA), POSITION (X,Y) OU GPS\n";
         
  Serial.print(flux);
  // AJOUTER L'OPTION GPS
  saveData(identifiant,flux);
  
   lcd.setCursor(0,1);
   lcd.print(TypedeSondage);
   
   pause();
}



/***********************************/
/* FIN BLOC / ALIMENTATION DU FLUX */
/***********************************/

/* --------------------------------------------------------------------------------- */

/*************************/
/* BLOC / INIT VARIABLES */
/*************************/
void initVar()
{
  mesureI = Mesuree;                        
  positionnement = Aucun ;               
  keyboardTimer = 200;             

  utilisationCoeffI = false;       
  intensiteFixee = 1.0;      
  facteurConversion = 1.0;
 
  distanceElectrodesA = 10;
  valeurN = 1;

  hauteurCapteur1 = 0;
  hauteurCapteur2 = 0;
  vitesseEchantillonnage = 10;

  XMinProfil = 0;
  XMaxProfil = 0;
  DistanceXProfil = 0;
  nbMesuresXProfil = 0;
  DeltaXProfil = 0.0;
 
  XMinCarte = 0;
  XMaxCarte = 0;
  YMinCarte = 0;
  YMaxCarte = 0;
  DistanceXCarte = 0;
  DistanceYCarte = 0;
  DeltaXCarte = 0;
  DeltaYCarte = 0;
  nbMesuresXCarte = 0;
  nbMesuresYCarte = 0;
}
/*****************************/
/* FIN BLOC / INIT VARIABLES */
/*****************************/

/* --------------------------------------------------------------------------------- */

/********************************/
/* BLOC / AFFECTATION VARIABLES */
/********************************/
void affectVar()
{
  mesureI = aRec.mesureI;                       
  positionnement = aRec.positionnement;
  keyboardTimer = aRec.keyboardTimer;
  utilisationCoeffI = aRec.utilisationCoeffI;
  intensiteFixee = aRec.intensiteFixee;
  facteurConversion = aRec.facteurConversion;
  distanceElectrodesA = aRec.distanceElectrodesA;
  valeurN = aRec.valeurN;
  hauteurCapteur1 = aRec.hauteurCapteur1;
  hauteurCapteur2 = aRec.hauteurCapteur2;
  vitesseEchantillonnage = aRec.vitesseEchantillonnage;
  XMinProfil = aRec.XMinProfil;
  XMaxProfil = aRec.XMaxProfil;
  DistanceXProfil = aRec.DistanceXProfil;
  nbMesuresXProfil = aRec.nbMesuresXProfil;
  DeltaXProfil = aRec.DeltaXProfil;
  XMinCarte = aRec.XMinCarte;
  XMaxCarte = aRec.XMaxCarte;
  YMinCarte = aRec.YMinCarte;
  YMaxCarte = aRec.YMaxCarte;
  DistanceXCarte = aRec.DistanceXCarte;
  DistanceYCarte = aRec.DistanceYCarte;
  DeltaXCarte = aRec.DeltaXCarte;
  DeltaYCarte = aRec.DeltaYCarte;
  nbMesuresXCarte = aRec.nbMesuresXCarte;
  nbMesuresYCarte = aRec.nbMesuresYCarte;
}
/*******************************/
/* FIN BLOC / AFFECT VARIABLES */
/*******************************/

/* --------------------------------------------------------------------------------- */

/*************************/
/* BLOC / INIT STRUCTURE */
/*************************/

void affectRecord()
{
  aRec.mesureI = mesureI;
  aRec.positionnement = positionnement;
  aRec.keyboardTimer = keyboardTimer;
  aRec.utilisationCoeffI = utilisationCoeffI;
  aRec.intensiteFixee = intensiteFixee;
  aRec.facteurConversion = facteurConversion;
  aRec.distanceElectrodesA = distanceElectrodesA;
  aRec.valeurN = valeurN;
  aRec.hauteurCapteur1 = hauteurCapteur1;
  aRec.hauteurCapteur2 = hauteurCapteur2;
  aRec.vitesseEchantillonnage = vitesseEchantillonnage;
  aRec.XMinProfil = XMinProfil;
  aRec.XMaxProfil = XMaxProfil;
  aRec.DistanceXProfil = DistanceXProfil;
  aRec.nbMesuresXProfil = nbMesuresXProfil;
  aRec.DeltaXProfil = DeltaXProfil;
  aRec.XMinCarte = XMinCarte;
  aRec.XMaxCarte = XMaxCarte;
  aRec.YMinCarte = YMinCarte;
  aRec.YMaxCarte = YMaxCarte;
  aRec.DistanceXCarte = DistanceXCarte;
  aRec.DistanceYCarte = DistanceYCarte;
  aRec.DeltaXCarte = DeltaXCarte;
  aRec.DeltaYCarte = DeltaYCarte;
  aRec.nbMesuresXCarte = nbMesuresXCarte;
  aRec.nbMesuresYCarte = nbMesuresYCarte; 
}

/* --------------------------------------------------------------------------------- */

/******************************************/
/* BLOC / CHARGEMENT CONFIGURATION EEPROM */
/******************************************/
void chargementConfig()
{
  int eeAddress = 0;

  lcd.clear();
  lcd.print("CHARGEMENT CFG..");

  EEPROM.get(eeAddress, aRec);

  lcd.setCursor(0,1);
  lcd.print("CHARGEMENT OK !");
  delay(1000);
}

/**********************************************/
/* FIN BLOC / CHARGEMENT CONFIGURATION EEPROM */
/**********************************************/

/* --------------------------------------------------------------------------------- */

/*******************************************/
/* BLOC / SAUVERGARDE CONFIGURATION EEPROM */
/*******************************************/
void sauvegardeConfig()
{
  int eeAddress = 0;
  
  lcd.clear();
  lcd.print("EFFACEMENT EEPROM..");
  for (int i = 0 ; i < EEPROM.length() ; i++) 
  {
    EEPROM.write(i, 0);
  }
  lcd.setCursor(0,1);
  lcd.print("EFFACEMENT OK !");
  delay(500);
  lcd.clear();
  lcd.print("SAUVEGARDE EN COURS");

  affectRecord();
  EEPROM.put(eeAddress, aRec);
  lcd.setCursor(0,1);
  lcd.print("SAUVEGARDE OK !");
  pause();
}

/***********************************************/
/* FIN BLOC / SAUVERGARDE CONFIGURATION EEPROM */
/***********************************************/

/* --------------------------------------------------------------------------------- */

/**********************************/
/* BLOC / AFFICHAGE DATE ET HEURE */
/**********************************/

String printTime(bool affichage)
{
  String buffer = "";
  clock.getTime();
  buffer = String(clock.dayOfMonth)+"/"+String(clock.month)+"/"+String(clock.year)+" - "+String(clock.hour)+"H"+String(clock.minute);  

  if (affichage)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Date - Heure");
    lcd.setCursor(0,1);
    lcd.print(buffer);
    pause();  
  }
  return buffer;
}

/**************************************/
/* FIN BLOC / AFFICHAGE DATE ET HEURE */
/**************************************/

/* --------------------------------------------------------------------------------- */

/*****************************************/
/* BLOC / FONCTION LANCEMENT DES MESURES */
/*****************************************/
void lancementMesures()
{
  if (verifFile(identifiant)) // Si le fichier n'existe pas, il est créé
  {
    String result = LectureValeurNum("CONTINUER OUI(1)");
    if (!result.equals("1")) // Un fichier existe déjà et l'utilisateur souhaite quitter
    {
      return;
    }   
  }
  else  
    {
      lcd.print("CREATION DE : ");
      lcd.setCursor(0,1);
      lcd.print(identifiant);
      monFichier = SD.open(identifiant, FILE_WRITE);
      delay (200);
      monFichier.close();
      delay (200);
    } // FIN CREATION FICHIER/ / FIN VERIFICATION FICHIER
  
  /******************************/
  /* BLOC / MESURES SPECIFIQUES */
  /******************************/
  if (mode == Specifique)  // Mesures spécifiques
  {
    lcd.clear();
    lcd.print("Mesures specifiques");

    /**********************************/
    /* BLOC / DISPOSITIF SCHLUMBERGER */
    /**********************************/
    if (confSpecifique == Schlumberger)  // Dispositif Schlumberger distance AB varie et MN fixe
      {
        int i=1;
            
        alimentationFlux(Schlumberger); // Création et enregistrement de l'entête sur support SD

        // ACQUISITION DES DONNEES
        distanceOM = (LectureValeurNum("Distance OM(cm) :"+String(distanceOM))).toInt(); 
        distanceOM = constrain (distanceOM,1,DISTANCE_MAX);

        while (true)
        {
          String fluxDeDonnees = mesures(i);
          Serial.print(fluxDeDonnees);
          saveData(identifiant,fluxDeDonnees);//FONCTION DE SAUVEGARDE DES DONNEES
          if ((LectureValeurNum("Quitter ->1(OUI)")).toInt() == 1) break; 
          i++;
        }      
      } 
    /**************************************/
    /* FIN BLOC / DISPOSITIF SCHLUMBERGER */
    /**************************************/
    
    /* -------------------------------------------------------------- */
    
    /****************************/
    /* BLOC / DISPOSITIF WENNER */
    /****************************/
    if (confSpecifique == Wenner)                     
      {
         int i=1;
         alimentationFlux(Wenner); // Création et enregistrement de l'entête sur support SD

         while (true)
         {
           String fluxDeDonnees = mesures(i);
           Serial.print(fluxDeDonnees);
           saveData(identifiant,fluxDeDonnees);//FONCTION DE SAUVEGARDE DES DONNEES
           if ((LectureValeurNum("Quitter ->1(OUI)")).toInt() == 1) break; 
           i++;
         }    
      } 
    /********************************/
    /* FIN BLOC / DISPOSITIF WENNER */
    /********************************/

    /***********************************/
    /* BLOC / DISPOSITIF DIPOLE-DIPOLE */
    /***********************************/
    if (confSpecifique == Dipole)                     
      {
         int i=1;
         alimentationFlux(Dipole); // Création et enregistrement de l'entête sur support SD

        while (true)
        {
          String fluxDeDonnees = mesures(i);
          Serial.print(fluxDeDonnees);
          saveData(identifiant,fluxDeDonnees);//FONCTION DE SAUVEGARDE DES DONNEES
          if ((LectureValeurNum("Quitter ->1(OUI)")).toInt() == 1) break; 
          i++;
        }     
      } 
    /***************************************/
    /* FIN BLOC / DISPOSITIF DIPOLE DIPOLE */
    /*******************************ù*******/        
  } 
  /**********************************/
  /* FIN BLOC / MESURES SPECIFIQUES */
  /**********************************/

  /* -------------------------------------------------------------- */

  /*********************************/
  /* BLOC / MESURES SUIVANT PROFIL */
  /*********************************/
  if (mode == Profil)  // Mesures Selon Profil
  {
    lcd.clear();
    lcd.print("Mesures profil");
    pause();

    if (DeltaXProfil == 0)  // Alerte l'utilisateur si le calcul de delta n'a pas été effetué
    {
      lcd.clear();
      lcd.print("ATTENTION");
      lcd.setCursor(0,1);
      lcd.print("DELTA NON DEF!");
      pause();
      
    } // FIN BLOC ALERTE / NbMesures = 0

    /****************************************************/
    /* BLOC : MESURES PROFIL / DISPOSITIF DIPOLE-DIPOLE */
    /****************************************************/
    if (confSpecifique == Dipole)  // Configuration du dispositif suivant dipôle-dipôle
    {
      
      alimentationFlux (Dipole);

      for (int i=1 ; i<=nbMesuresXProfil ; i++)
      {
        lcd.clear();
        lcd.print("Mesure:"+String(i)+"/"+String(nbMesuresXProfil));
        lcd.setCursor(0,1);
        
        CurseurX = (i-1)*DeltaXProfil;
        
        lcd.print("PosX(cm):"+String(CurseurX,0));
        pause();
        
        String fluxDeDonnees = mesures(i);
        Serial.print(fluxDeDonnees);
        
        saveData(identifiant,fluxDeDonnees);//FONCTION DE SAUVEGARDE DES DONNEES
        if ((LectureValeurNum("Quitter ->1(OUI)")).toInt() == 1) break;
      }
      
    }
     /*******************************************************/
    /* FIN BLOC : MESURES PROFIL / DISPOSITIF DIPOLE-DIPOLE */
    /********************************************************/
  }
  /*********ù***************************/
  /* FIN BLOC / MESURES SUIVANT PROFIL */
  /*************************************/ 

  /* -------------------------------------------------------------- */

  /********************************/
  /* BLOC / MESURES SUIVANT CARTE */
  /********************************/
  if (mode == Carte)  // Mesures Selon Carte
  {
    lcd.clear();
    lcd.print("Mesures Carte");
    pause();

    if ((DeltaXCarte == 0) || (DeltaYCarte == 0))  // Alerte l'utilisateur si les calculs de delta n'ont pas pas été effetués
    {
      lcd.clear();
      lcd.print("ATTENTION");
      lcd.setCursor(0,1);
      lcd.print("DELTAS NON DEF!");
      pause();
    } // FIN BLOC ALERTE / NbMesures = 0

    /****************************************************/
    /* BLOC : MESURES PROFIL / DISPOSITIF DIPOLE-DIPOLE */
    /****************************************************/
    if (confSpecifique == Dipole)  // Configuration du dispositif suivant dipôle-dipôle
    {
      
      alimentationFlux (Dipole);
      int i=0;

      for (int j=1 ; j<=nbMesuresYCarte ; j++) 
      {
        
        if ( (j%2) == 1) // Sens aller
        {
          for (int k=1 ; k<=nbMesuresXCarte ; k++)
          {
            mesuresCartes(k,j,i);
            if ((LectureValeurNum("Quitter ->1(OUI)")).toInt() == 1) break;
          } // Balayage sur X
        } // Fin sens aller
        else            // Sens retour
        {
          for (int k=nbMesuresXCarte; k>=1 ; k--)
          {
            mesuresCartes(k,j,i);
            if ((LectureValeurNum("Quitter ->1(OUI)")).toInt() == 1) break;
          } // Balayage sur X
        } // Fin sens retour
      } // Fin balayage sur Y  
    } // Fin dispositif Dipole-Dipole
     /*******************************************************/
    /* FIN BLOC : MESURES PROFIL / DISPOSITIF DIPOLE-DIPOLE */
    /********************************************************/
  }
  /*********ù***************************/
  /* FIN BLOC / MESURES SUIVANT PROFIL */
  /*************************************/   
} 
/************************************/
/* FIN BLOC / LANCEMENT DES MESURES */
/************************************/
 


