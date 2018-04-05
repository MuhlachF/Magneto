/*************************************************************
Protocentral_ADS1220.ino
Exemple d'utilisation de la nouvelle librairie
Author : Frédéric MUHLACH
|ads1220 pin label| Pin Function         |Arduino Connection UNO| MEGA |
|-----------------|:--------------------:|---------------------:|:-----
| DRDY            | Data ready Output pin|  D6                  | D6           
| MISO            | Slave Out            |  D12                 | D50
| MOSI            | Slave In             |  D11                 | D51
| SCLK            | Serial Clock         |  D13                 | D52 
| CS              | Chip Select          |  D7                  | D7
| DVDD            | Digital VDD          |  +5V                 |
| DGND            | Digital Gnd          |  Gnd                 |
| AN0-AN3         | Analog Input         |  Analog Input        |
| AVDD            | Analog VDD           |  -                   |
| AGND            | Analog Gnd           |  -                   |
****************************************************************************/
/* Configuration du CAN 
 * Pour les entrées en mode différentiel
 * Commande :  * ADS1220.set_mode(INPUT_SINGLE_ENDED_AIN0);
 * Paramètres : INPUT_DIFF_AIN0_AIN1 
                INPUT_DIFF_AIN0_AIN2
                INPUT_DIFF_AIN0_AIN3
                INPUT_DIFF_AIN1_AIN2
                INPUT_DIFF_AIN1_AIN3
                INPUT_DIFF_AIN2_AIN3
                INPUT_DIFF_AIN1_AIN0
                INPUT_DIFF_AIN3_AIN2

 * Pour les entrées en mode Single Ended
                INPUT_SINGLE_ENDED_AIN0
                INPUT_SINGLE_ENDED_AIN1
                INPUT_SINGLE_ENDED_AIN2
                INPUT_SINGLE_ENDED_AIN3

 * Vitesse de sampling pour le mode NORMAL  : 20, 45, 90, 175, 330, 600 et 1000 SPS
 * Vitesse de sampling pour le mode TURBO   : 40, 90, 180, 350, 660, 1200 et 2000 SPS
 * 
 * Pour la mise en place d'un filtrage numérique (uniquement pour 20SPS)
 * Commande :   ADS1220.set_filter(REJECTION_50_60HZ)
 * Paramètres : NOFILTER
 *              REJECTION_50_60HZ
 *              REJECTION_50HZ
 *              REJECTION_60HZ
 
 * Pour le mode de fonctionnement du convertisseur
 * Commande :   ADS1220.set_operating_mode(NORMAL)
 * Paramètres : NORMAL
 *              DUTY_CYCLE
 *              TURBO

*/

/* Versionning :
 *  0.1 : Intégration de la librairie ADS1220 et tests de fonctionnement
 *  0.2 : Passage sur Arduino MEGA 2560
 *  0.3 : Intégration d'un menu simplifié
 *    1.  Start scan
      2.  Output data
      3.  Set parameters
          a.  Grid size (max 50*50m) ?
          b.  Interval (increment 0.2m avec max interval = 1m)
          c.  SPS (Samples Per Second)
          d.  Sensors (mono composante ou 3D)
          e.  Reject (activation du filtre rejecteur 50 ou 60Hz)
          f.  Space sensors (distance entre les deux capteurs mono composante)
          g.  Save config
          h.  System reset
      4.  Calibrate
 *  0.4 : Prise en compte du mode TURBO 
 *  0.5 : passage à 660SPS par convertisseur pour diminuer les temps de multiplexage entre CAN
 *  0.6 : Ajout du supersampling pour l'acquisition des signaux
 *  0.7 : Prise en compte des erreurs Offset et Gain sur les 3 entrées
 *  0.8 : Ajout du lecteur de carte SD / initialisation    
 *  0.9 : Prise en compte de l'interface de navigation
 *  0.10 : Conversion parallèle avec 2 CAN / synchronisation des CAN 
 *  0.11 : refactoring de code -> Optimisation de la fonction d'acquisition 
 *  0.12 : Vitesse de commutation fixée à 0
 *  0.13 : Ouverture de fichier
 *  0.14 : Mise en place d'un timer pour la synchronisation des conversions
 *  0.15 : intégration d'une reinit synchro SPI dans la librairie
 *  
 */

#include "Protocentral_ADS1220.h"
#include "rgb_lcd.h"
#include <SD.h>

// DESCRIPTEUR DE FICHIER
File myFile;

// IDENTIFIANTS DES CONVERTISSEURS
#define CAN1                  1
#define CAN2                  2
#define CAN3                  3

// CHIP SELECT BUS SPI
#define SDCARD_CS_PIN         8
#define ADS1220_1_CS_PIN      7
#define ADS1220_1_DRDY_PIN    6
#define ADS1220_2_CS_PIN      5
#define ADS1220_2_DRDY_PIN    4
#define ADS1220_3_CS_PIN      3
#define ADS1220_3_DRDY_PIN    2

// VITESSE DE COMMUTATION Bonne valeur -> TIME_BURST 4000, SS=8 -> 22Hz 1200SPS bruit de 3nT
//                                      -> TIME_BURST 6000, SS=1 -> 70Hz 350SPS bruit de 3nT
//                                     -> TIME_BURST 12000, SS=1 -> 55Hz 180SPS bruit de 2nT
//                                     -> TIME BURST 24000, SS=1 -> 40Hz 90SPS bruit de 1nT
//                                     -> TIME BURST 25000, SS=1 ->      
// MARCHE OK TIME_BURST 15000 et 180SPS 4SS environ 25Hz
#define TIME_BURST           50000  //en µs Baisser cette valeur augmente les erreurs de conversion ! 
                                   // La valeur est à adapter avec le nombre de SPS
#define TIME_SYNCHRO         0
#define TIME_SPI             200     // 

// SUPERSAMPLING 
#define SAMPLES               1 // 20 -> 12Hz, 15 -> 16Hz

// TAILLE TABLEAU
#define LENGTH_ACQUISITION    500  // Avec TIME_BURST=2000, SAMPLES=20, LENGTH_ACQUISITION=400 => MAX 10s de SAMPLES sans interruption

// Interface de navigation
#define KEYBOARD_F            150
#define BTN_DOWN              10
#define BTN_UP                11
#define BTN_ENTER             12
#define BTN_ESCAPE            13
#define ITEMS_MENU_PRINCIPAL  4
#define ITEMS_MENU_SECONDAIRE 8

// Capteur Magnetique 3D
#define X_SENSOR 1
#define Y_SENSOR 2
#define Z_SENSOR 3
#define SENSOR_3D_SENS_X 35 
#define SENSOR_3D_OFFSET_X 0      // sensibilité du capteur magnétique 1V = 70µT
//#define SENSOR_3D_OFFSET_X -69.923  // Offset du capteur magnétique (tension positive sur -70 à 70µT) -> 0 à 2V 

#define SENSOR_3D_SENS_Y 35     // sensibilité du capteur magnétique 1V = 70µT
#define SENSOR_3D_OFFSET_Y 0
//#define SENSOR_3D_OFFSET_Y -69.75  // Offset du capteur magnétique (tension positive sur -70 à 70µT) -> 0 à 2V 

#define SENSOR_3D_SENS_Z 35     // sensibilité du capteur magnétique 1V = 70µT
#define SENSOR_3D_OFFSET_Z 0
//#define SENSOR_3D_OFFSET_Z -69.66  // Offset du capteur magnétique (tension positive sur -70 à 70µT) -> 0 à 2V 

#define DIGIT_PRECISION 3    // Précision de l'affichage au nT    


int codeTouche;
long int timer = millis();
long int timerBack = timer;
int navigationMenuPrincipal = 0;


// Déclaration ECRAN LCD
rgb_lcd lcd;
#define colorR 0
#define colorG 0
#define colorB 0

#include <SPI.h>
#define PGA 1                 // Programmable Gain = 1
#define VREF 2.048            // Internal reference of 2.048V
#define VFSR VREF/PGA             
#define FSR (((long int)1<<23)-1)  

// Variables dédiées au convertisseur
volatile byte MSB;
volatile byte data;
volatile byte LSB;
volatile byte *SPI_RX_Buff_Ptr;

String outPutData="";

float magneticField; 
float magneticFieldX; 
float magneticFieldY; 
float magneticFieldZ; 

float magneticFieldBackX; 
float magneticFieldBackY; 
float magneticFieldBackZ; 

float MagneticFieldX = 0.0;
float MagneticFieldY = 0.0;
float MagneticFieldZ = 0.0;
float moyenne;

//long timerTab[LENGTH_ACQUISITION]={NULL};
float magneticFieldXTab[LENGTH_ACQUISITION]={NULL};
float magneticFieldYTab[LENGTH_ACQUISITION]={NULL};
float magneticFieldZTab[LENGTH_ACQUISITION]={NULL};

float sensorOffset = 0.0;
float sensorGain = 0.0;
 
Protocentral_ADS1220 ADS1220;
Protocentral_ADS1220 ADS1220_2;
Protocentral_ADS1220 ADS1220_3;

void setup()
{
  Serial.begin(250000);

  // Paramtrage de l'écran LCD 2 lignes de 16 caractères
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  
  // Initialisation du lecteur de carte SD
  pinMode(SDCARD_CS_PIN, OUTPUT);
  pinMode(SS, OUTPUT);
  activation_SD_CARD();
 
  if (!SD.begin(SDCARD_CS_PIN)) 
  {
    lcd.print("SD Problem"); 
    delay (500);
     
  }
  else
  {
    lcd.print("SD initialized.");
    delay (100);    
  }

  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile)
  {
    myFile.print("LANCEMENT DES ACQUISITIONS");
    myFile.println("");
    myFile.print("-----------------------------");
    myFile.println("");
    myFile.println("");
    
    myFile.close();
  }

  // Configuration interface
  pinMode (BTN_DOWN,INPUT_PULLUP);
  pinMode (BTN_UP,INPUT_PULLUP);
  pinMode (BTN_ENTER,INPUT_PULLUP);
  pinMode (BTN_ESCAPE,INPUT_PULLUP);
  
  // Initialisation des CAN ADS1220
  pinMode(ADS1220_1_CS_PIN, OUTPUT);  
  pinMode(ADS1220_1_DRDY_PIN, INPUT);  
  pinMode(ADS1220_2_CS_PIN, OUTPUT);  
  pinMode(ADS1220_2_DRDY_PIN, INPUT);
  pinMode(ADS1220_3_CS_PIN, OUTPUT);  
  pinMode(ADS1220_3_DRDY_PIN, INPUT);

  

  
  activation_ADS1220_1();
  ADS1220.set_CS_Pin(ADS1220_1_CS_PIN);
  ADS1220.set_DRDY_Pin(ADS1220_1_DRDY_PIN);
  ADS1220.begin();
  delayMicroseconds(TIME_SPI);
  ADS1220.set_mode(INPUT_DIFF_AIN1_AIN2);
  ADS1220.set_operating_mode(TURBO);
  ADS1220.set_data_rate(DR_180SPS);
  ADS1220.Continuous_conversion_mode_ON();
  ADS1220.set_filter(NOFILTER);
  ADS1220.PGA_ON(); 
  ADS1220.set_pga_gain(PGA_GAIN_4);
  delayMicroseconds(TIME_SPI);
  

  activation_ADS1220_2();
  ADS1220_2.set_CS_Pin(ADS1220_2_CS_PIN);
  ADS1220_2.set_DRDY_Pin(ADS1220_2_DRDY_PIN);
  delayMicroseconds(TIME_SPI);
  ADS1220_2.begin();
  ADS1220_2.set_mode(INPUT_DIFF_AIN0_AIN1);
  ADS1220_2.set_operating_mode(TURBO);
  ADS1220_2.set_data_rate(DR_180SPS);
  ADS1220_2.Continuous_conversion_mode_ON();
  ADS1220_2.set_filter(NOFILTER);
  ADS1220_2.PGA_ON(); 
  ADS1220_2.set_pga_gain(PGA_GAIN_4);
  delayMicroseconds(TIME_SPI);

  activation_ADS1220_3();
  ADS1220_3.set_CS_Pin(ADS1220_3_CS_PIN);
  ADS1220_3.set_DRDY_Pin(ADS1220_3_DRDY_PIN);
  delayMicroseconds(TIME_SPI);
  ADS1220_3.begin();
  ADS1220_3.set_mode(INPUT_DIFF_AIN0_AIN2);
  ADS1220_3.set_operating_mode(TURBO);
  ADS1220_3.set_data_rate(DR_180SPS);
  ADS1220_3.Continuous_conversion_mode_ON();
  ADS1220_3.set_filter(NOFILTER);
  ADS1220_3.PGA_ON(); 
  ADS1220_3.set_pga_gain(PGA_GAIN_4);
  delayMicroseconds(TIME_SPI);

  desactivation_SPI();
  affichageMenuPrincipal();     
}
 
void loop()
{
 codeTouche = testTouche();

 switch (codeTouche)
 {
   case BTN_DOWN: 
   navigationMenuPrincipal++;
   navigationMenuPrincipal=constrain(navigationMenuPrincipal,0,ITEMS_MENU_PRINCIPAL-1);
   affichageMenuPrincipal();
   break;

   case BTN_UP:
   navigationMenuPrincipal--;
   navigationMenuPrincipal=constrain(navigationMenuPrincipal,0,ITEMS_MENU_PRINCIPAL-1);
   affichageMenuPrincipal();
   break;

   case BTN_ENTER:

   switch(navigationMenuPrincipal)
   {
      case 0: 
      do
      {
        acquisition();         
      }while (digitalRead(BTN_ESCAPE));
      break;
   }
   
   break;

   case BTN_ESCAPE:
   affichageMenuPrincipal();
   break;

   default:
   break;
 } 

}

void acquisition()
{ 
  

  for (int index=0;index<LENGTH_ACQUISITION;index++)
  {
    MagneticFieldX = 0.0;
    MagneticFieldY = 0.0;
    MagneticFieldZ = 0.0;
    timer = micros();
    timerBack = timer;
    /*
    for (int i=0 ; i<SAMPLES ; i++)
    {
      */
      while (timer < (timerBack + TIME_BURST))
      {
        timer = micros();
      }
      timerBack = timer;
      //delayMicroseconds(TIME_SPI);
      magneticFieldX = Conversion_Analogique_Numerique_ADS1220(X_SENSOR,CAN1);    
      MagneticFieldX = MagneticFieldX + magneticFieldX;
      delayMicroseconds(TIME_SPI);
      
      magneticFieldY = Conversion_Analogique_Numerique_ADS1220(Y_SENSOR,CAN2);
      MagneticFieldY = MagneticFieldY + magneticFieldY;      
      delayMicroseconds(TIME_SPI);
  
      magneticFieldZ = Conversion_Analogique_Numerique_ADS1220(Z_SENSOR,CAN3);
      MagneticFieldZ = MagneticFieldZ + magneticFieldZ;
      delayMicroseconds(TIME_SPI);
/*
    }
    /*
    MagneticFieldX = MagneticFieldX / SAMPLES;
    MagneticFieldY = MagneticFieldY / SAMPLES;
    MagneticFieldZ = MagneticFieldZ / SAMPLES;
    */
    
    
    
    
   magneticField = sqrt(MagneticFieldX*MagneticFieldX+MagneticFieldY*MagneticFieldY+MagneticFieldZ*MagneticFieldZ);
   //timerTab[index] = millis(); 
   Serial.print (millis());
    Serial.print("\t");
    Serial.print(MagneticFieldX,5);
    Serial.print("\t");
    
    Serial.print(MagneticFieldY,5);
    Serial.print("\t");

    Serial.print(MagneticFieldZ,5);
    Serial.print("\t");
    Serial.print(magneticField,5);
    Serial.print("\n");
   magneticFieldXTab[index]=magneticFieldX;
   magneticFieldYTab[index]=magneticFieldY;
   magneticFieldZTab[index]=magneticFieldZ;
     
  AffichageLCD(index);    
  }
  // Activation du lecteur de carte pour enregistrement des mesures
  
  activation_SD_CARD();
  
   myFile = SD.open("DATA.txt", FILE_WRITE);
   if (myFile)
   {
 
      for (int index=0 ; index<LENGTH_ACQUISITION ; index++)
    {
      //myFile.print(timerTab[index]);
      //myFile.print(";");
      myFile.print(magneticFieldXTab[index],6);
      myFile.print(";");
      myFile.print(magneticFieldYTab[index],6);
      myFile.print(";");
      myFile.print(magneticFieldZTab[index],6);
      myFile.println("");
    }
    myFile.close();
    /*
    Serial.print("sauvegarde effectuee");
    Serial.print("\n");  
    */
  }
  
  activation_ADS1220_1();
  ADS1220.SynchroRestart();
  Conversion_Analogique_Numerique_ADS1220(X_SENSOR,CAN1);
  Conversion_Analogique_Numerique_ADS1220(X_SENSOR,CAN2);
  Conversion_Analogique_Numerique_ADS1220(X_SENSOR,CAN3);
  delayMicroseconds(TIME_SYNCHRO);

}
float Conversion_Analogique_Numerique_ADS1220(int Sensor_Canal, int Convertisseur)
{
  // Rappel des configurations / Canal
  switch (Sensor_Canal)
  {
   case X_SENSOR : 
     sensorOffset = SENSOR_3D_OFFSET_X;
     sensorGain = SENSOR_3D_SENS_X;
     break;

   case Y_SENSOR : 
     sensorOffset = SENSOR_3D_OFFSET_Y;
     sensorGain = SENSOR_3D_SENS_Y;
     break;

   case Z_SENSOR : 
     sensorOffset = SENSOR_3D_OFFSET_Z;
     sensorGain = SENSOR_3D_SENS_Z;
     break;
  }
  long int bit32;
  long int bit24;
  byte *config_reg;
  bool lock = false;

  switch (Convertisseur)
  {
    case CAN1 : 
      activation_ADS1220_1();
      SPI_RX_Buff_Ptr = ADS1220.Read_Data();
      SPI_RX_Buff_Ptr = ADS1220.Read_Data();
      if(ADS1220.NewDataAvailable = true)
      {
        ADS1220.NewDataAvailable = false;
        lock = true;
      }
      desactivation_SPI();
      break;

    case CAN2 : 
      activation_ADS1220_2();
      SPI_RX_Buff_Ptr = ADS1220_2.Read_Data();
      SPI_RX_Buff_Ptr = ADS1220_2.Read_Data();
      if(ADS1220_2.NewDataAvailable = true)
      {
        ADS1220_2.NewDataAvailable = false;
        lock = true;
      }
      desactivation_SPI();
      break;

    case CAN3 : 
      activation_ADS1220_3();
      SPI_RX_Buff_Ptr = ADS1220_3.Read_Data();
      SPI_RX_Buff_Ptr = ADS1220_3.Read_Data();
      if(ADS1220_3.NewDataAvailable = true)
      {
        ADS1220_3.NewDataAvailable = false;
        lock = true;
      }
     desactivation_SPI();
      break;
  }
  
  
  if(lock)
  {
  lock = false;
  MSB = SPI_RX_Buff_Ptr[0];    
  data = SPI_RX_Buff_Ptr[1];
  LSB = SPI_RX_Buff_Ptr[2];

  bit24 = MSB;
  bit24 = (bit24 << 8) | data;
  bit24 = (bit24 << 8) | LSB;                   // Converting 3 bytes to a 24 bit int
    
  bit24= ( bit24 << 8 );
  bit32 = ( bit24 >> 8 );                      // Converting 24 bit two's complement to 32 bit two's complement
  
  return (float)(((((bit32*VFSR)/FSR)*sensorGain))+sensorOffset);     //In  µT
  //return (float)((bit32*VFSR*1000)/FSR);     //In  mV
  } 
}


void AffichageLCD(int index)
{
  lcd.clear();
  lcd.print(index);
  lcd.setCursor(2,0);
  lcd.print("MAG1="+String(magneticField,DIGIT_PRECISION)+" "); 
}

void affichageMenuPrincipal()
{
  desactivation_SPI();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print ("---MAIN MENU----");
  lcd.setCursor(0,1);
   switch (navigationMenuPrincipal)
 {
    case 0:
    lcd.print("1-> START");
    break;

    case 1:
    lcd.print("2-> OUTPUT DATA");
    break;

    case 2:
    lcd.print("3-> SET PARAM"); 
    break;

    case 3:
    lcd.print("4-> CALIBRATE"); 
    break;
 }
}
/* 
 *  FONCTION TEST TOUCHE
 *  RENVOI LA VALEUR DE LA TOUCHE APPUYEE
 *  AUCUN PARAMETRE
 */
int testTouche()
{  
  timer = millis();
  if ((timer - timerBack) > KEYBOARD_F)
  {   
      if (!digitalRead(BTN_DOWN))
      { 
        timerBack = timer;
        return BTN_DOWN;
      }

      if (!digitalRead(BTN_UP))
      { 
        timerBack = timer;
        return BTN_UP;
      }
      
      if (!digitalRead(BTN_ENTER))
      { 
        timerBack = timer;
        return BTN_ENTER;
      }

      if (!digitalRead(BTN_ESCAPE))
      { 
        timerBack = timer;
        return BTN_ESCAPE;
      }    
      timerBack=timer;
      
  }
    return 0;  
}

void activation_SD_CARD()
{
  digitalWrite(ADS1220_1_CS_PIN,HIGH);
  digitalWrite(ADS1220_2_CS_PIN,HIGH);
  digitalWrite(ADS1220_3_CS_PIN,HIGH);
  delayMicroseconds(TIME_SPI);
  digitalWrite(SDCARD_CS_PIN,LOW); 
  delayMicroseconds(TIME_SPI);
}

void activation_ADS1220_1()
{
  digitalWrite(SDCARD_CS_PIN,HIGH);
  digitalWrite(ADS1220_2_CS_PIN,HIGH); 
  digitalWrite(ADS1220_3_CS_PIN,HIGH); 
  delayMicroseconds(TIME_SPI);
  digitalWrite(ADS1220_1_CS_PIN,LOW);
  delayMicroseconds(TIME_SPI);
}

void activation_ADS1220_2()
{
  digitalWrite(SDCARD_CS_PIN,HIGH);
  digitalWrite(ADS1220_1_CS_PIN,HIGH); 
  digitalWrite(ADS1220_3_CS_PIN,HIGH); 
  delayMicroseconds(TIME_SPI);
  digitalWrite(ADS1220_2_CS_PIN,LOW);
  delayMicroseconds(TIME_SPI);
}

void activation_ADS1220_3()
{
  digitalWrite(SDCARD_CS_PIN,HIGH);
  digitalWrite(ADS1220_1_CS_PIN,HIGH); 
  digitalWrite(ADS1220_2_CS_PIN,HIGH); 
  delayMicroseconds(TIME_SPI);
  digitalWrite(ADS1220_3_CS_PIN,LOW);
  delayMicroseconds(TIME_SPI);
}

void desactivation_SPI()
{
  digitalWrite(SDCARD_CS_PIN,HIGH);
  digitalWrite(ADS1220_1_CS_PIN,HIGH); 
  digitalWrite(ADS1220_2_CS_PIN,HIGH); 
  digitalWrite(ADS1220_3_CS_PIN,HIGH); 
  delayMicroseconds(TIME_SPI);
}
/* 
 *  FIN FONCTION TEST TOUCHE
 */


 
 
