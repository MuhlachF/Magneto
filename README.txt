---------------------------------
1. Principe du sondage éléctrique
---------------------------------
La prospection électrique consiste à injecter des courants sur une paire 
d’électrodes plantées dans le sol et à mesurer des potentiels en d’autres 
points de la surface. 

Livre Applied Geophysics 1: Electrical Methods
Auteur principal : Nicolas Florsch / Co-auteur : Frédéric MUHLACH
Relié: 220 pages
Editeur : ISTE Press - Elsevier (20 novembre 2017)
Langue : Anglais
ISBN-10: 1785481991
ISBN-13: 978-1785481994

-----------------------------------
2. Présentation du projet : Magneto
-----------------------------------

Le projet de réalisation qui vous est ici présenté est destiné à la prospection 
électrique du sous-sol par mesure de résistivité. Cet appareil est «opensource» 
et vous pourrez donc modifier à votre guise le programme qu’il contient pour 
l’adapter à vos besoins ou ajouter de nouvelles fonctionnalités. 

Cet appareil est uniquement dédié à l’acquisition de valeurs analogiques, 
telles qu’intensités et tensions. Les signaux sont ensuite convertis au format 
numérique et sauvegardés sur un support de stockage pour y être analysés, à
l’aide d’un logiciel spécialisé de géophysique. Il s’agit de déterminer la 
variation spatiale des résistivités du sous-sol.

-------------------------
3. Architecture du projet
-------------------------
                          |---------------|             |-------------|
                          | Affichage des |             |  Horodatage | 
                          | informations  |             |    + GPS    |
                          |---------------|             |-------------|
                                   ^                            |
                                   |                            V                
    |------------|          |------------|         |------------------------|
    | Conversion |--------->| Traitement |-------->| Sauvegarde des données |
    | Analog/Num |          |    (µc)    |         |       collectées       |
    |------------|          |------------|         |------------------------|
                                   |
                                   v
                            |------------|
                            | Saisie des |
                            | paramètres |
                            |------------|
	
----------------------------------------
4. Présentation des composants matériels
----------------------------------------
4.1 Le microcontrôleur
----------------------------------------
Le microcontrôleur est un circuit intégré (ou IC Integrated Circuit) dont les 
tâches principales sont :
	• Le traitement des informations,
	• Le pilotage des périphériques connectés sur ses broches.

Notre choix s’est porté sur la version Arduino Mega2560, qui dispose de la 
quantité de mémoire suffisante pour faire tourner le programme. 

Remarque : le modèle UNO, reste utilisable, à condition de renoncer à 
l’acquisition des données GPS.

https://www.arduino.cc/en/Main/ArduinoBoardMega2560

-----------------------------------------
4.2 Le convertisseur analogique-numérique
-----------------------------------------
Le convertisseur analogique numérique CAN (ou ADC Analog to Digital Converter) 
est un dispositif électronique qui traduit des signaux analogiques en données 
numériques.

Les entrées des CAN acceptent généralement deux modes de fonctionnement :
	• Single Ended : tension mesurée sur un seul fil, avec pour référence la 
	masse du montage. 
	• Différentiel : tension mesurée entre entre deux fils ; les interférences, 
	présentes sur les deux voies, ne sont donc pas prises en compte.
	
Le convertisseur ADS1115 (16 bits de résolution) est ici utilisé dans sa version
«breakout Board » par la société Adafruit :

https://www.adafruit.com/product/1085

-------------------------
4.3 Le Clavier analogique
-------------------------
périphérique proposé par Sparkfun (VKey PRT12080 )

https://www.sparkfun.com/products/12080

Mapping du clavier :

#############################
#        #         #        #
#   1    # 2 ou ^  #   3    #                 
#        #         #        #
#############################
#        #         #        #
#   4    # 5 ou OK #   6    #
#        #         #        #
#############################
#        #         #        #
#   7    # 8 ou v  #   9    #
#        #         #        #
#############################
# cls .  #         #        #
# ou ret #    0    #  Enter #
#        #         #        #
#############################

---------------------
4.4 Ecran LCD RVB I²C
---------------------
Ecran de 2 lignes de 16 caractères, qui affiche les informations de paramétrage 
et les menus. Un rétroéclairage peut être activé.

http://wiki.seeed.cc/Grove-LCD_RGB_Backlight/

--------------
4.5 Module RTC
--------------
L’horloge RTC (Real Time Clock) indique l'heure à la nanoseconde près. La carte 
proposée au format Grove par la société Seeed ,dont le composant central est un 
circuit DS1307.

http://wiki.seeed.cc/Grove-RTC/

--------------------------------------
4.6 Lecteur de carte µSD au format SPI
--------------------------------------
Support de stockage utilisé pour sauvegarder toutes les données au fil des 
acquisitions.

https://www.adafruit.com/product/254


-------------
5. Connexions
-------------

|------------------------------------------------------------------------------|
|       Périphérique      |   Connexion et alimentation    | Protocole utilisé |
|------------------------------------------------------------------------------|
| Clavier                 | + -> 5 Volts et GND (masse)    | Analogique        |
|                         | Out -> Entrée Arduino A0       |                   |
|------------------------------------------------------------------------------|
| Option (GPS Adafruit)   | Vin (5Volts) et GND (masse)    | Série             |
|                         | TX -> RX1 Arduino              |                   |
|                         | RX -> TX1 Arduino              |                   |
|------------------------------------------------------------------------------|
| Lecteur de carte microSD| + -> 5 Volts et GND (masse)    | SPI               |
| (Adafruit)              | CLK -> port 52 (SCK)           |                   |
|                         | DO -> port 50 (MISO)           |                   |
|                         | DI -> port 51 (MOSI)           |                   |
|                         | CS -> port 53 (SS)             |                   |
|------------------------------------------------------------------------------|
| ADS1115 (Adafruit)      | VDD -> 5 Volts et GND (masse)  |                   |
|                         | SCL -> port 21 (SCL)           | I²C               |
|                         | SDA -> port 20 (SDA)           | (Adr : 0x48)      |
|                         | ADR -> GND                     |                   |
|------------------------------------------------------------------------------|
| Module RTC (Grove) 	  | VDD -> 5 Volts et GND (masse)  | I²C               |
|                         | SCL -> port 21 (SCL)           |                   |
|                         | SDA -> port 20 (SDA)           |                   |
|------------------------------------------------------------------------------|
| Ecran LCD (Grove) 	  | VDD -> 5 Volts et GND (masse)  | I²C               |
|                         | SCL -> port 21 (SCL)           |                   |
|                         | SDA -> port 20 (SDA)           |                   |
|------------------------------------------------------------------------------|







