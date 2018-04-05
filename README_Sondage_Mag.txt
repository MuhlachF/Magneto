-----------------------------------
1. Présentation du projet : Magneto
-----------------------------------
Ici, nous vous proposons de concevoir un appareil dédié à la prospection 
magnétique des sols fonctionnant avec un capteur de champ magnétique 3 axes. 
Tous les sources du projet, programmes, librairies ou schémas électriques 
sont « open-source » ! Il vous sera donc possible de les télécharger librement 
depuis les plateformes web, qui vous seront présentées au fil de cette 
conception. 

Un livre en cours de parution aux editions ISTE

-----------------------------------
2. Caractéristiques de l'appareil
-----------------------------------
Le magnétomètre est en mesure d’effectuer des acquisitions (composantes X, Y 
et Z) à une vitesse d’échantillonnage d’environ 25Hz. Il est entièrement 
paramétrable et la navigation dans les menus ainsi que la configuration du 
système s’effectue par l’intermédiaire de 4 boutons disposés sur le boitier. 
Un afficheur LCD assure à un retour d’informations à son utilisateur. 
Les données collectées sont stockées sur une carte SD et pourront être 
exploitées ultérieurement dans un logiciel de géophysique. 


-------------------------
3. Architecture du projet
-------------------------
                            |---------------|  |-------------|
                            | Affichage des |  |  Horodatage | 
                            | informations  |  |    + GPS    |
                            |---------------|  |-------------|
                                   ^                  |
                                   |                  V                
|----------|   |----------|   |------------|   |------------|   |-------------|
| Triaxial |-->|          |-->| Conversion |-->| Traitement |-->| Sauvegarde  |
| Fluxgate |   | Filtrage |   | Analog/Num |   |    (µc)    |   | des données |
| Sensor   |   |----------|   |------------|   |------------|   | ------------|
|----------|                       |
                                   v
                            |------------|
                            | Saisie des |
                            | paramètres |
                            |------------|
	
----------------------------------------
4. Présentation des composants matériels
----------------------------------------
4.1 Le capteur Flux gate
----------------------------------------
Le capteur FLC3-70 est un magnétomètre fluxgate à trois axes fabriqué par la 
société « Stefan Meyer Instruments ». Il a été choisi pour sa stabilité et 
la précision de ces mesures qui peuvent s’étendre de -200µT à 200µT. 

-------------------------------------------
4.2 La carte dédiée au filtrage des signaux
-------------------------------------------
Le module de conversion analogique numérique doit dont être précédé d’un filtre 
passe-bas « anti-repliement » éliminant les signaux dont la fréquence est 
supérieure à Frequence d'échantillonnage divisée par 2. Le filtre passe-bas est 
paramétré sur environ 12Hz et la fréquence d’échantillonnage à 25Hz.

Le schéma electrique est consultable sur la plateforme EasysEDA :

https://easyeda.com/frederic.muhlach/magnetometer

-----------------------------------------
4.3 Le convertisseur analogique-numérique
-----------------------------------------
Le convertisseur analogique numérique CAN (ou ADC Analog to Digital Converter) 
est un dispositif électronique qui traduit des signaux analogiques en données 
numériques.

Les entrées des CAN acceptent généralement deux modes de fonctionnement :
	• Single Ended : tension mesurée sur un seul fil, avec pour référence la 
	masse du montage. 
	• Différentiel : tension mesurée entre entre deux fils ; les interférences, 
	présentes sur les deux voies, ne sont donc pas prises en compte.
	
Ce projet utilise le modèle ADS1220 de la société Texas Instrument. Nous vous
proposons de réaliser votre propore Shield. Les schémas électriques sont 
consultables à l'adresse suivante :

https://easyeda.com/search?wd=shield%20ads1220

----------------------------------------
4.4 Le microcontrôleur
----------------------------------------
Le microcontrôleur est un circuit intégré (ou IC Integrated Circuit) dont les 
tâches principales sont :
	• Le traitement des informations,
	• Le pilotage des périphériques connectés sur ses broches.

Deux version de l'Arduino peuvent être utilisées : 
 - La version Arduino Mega2560, 
 - et la version DUE

La version DUE dispose de plus de mémoire pour le stockage des données
temporaires ! Il sera néanoins nécessaire d'ajouter une EPROM pour 
la sauvegarde des paramètres utilisateurs


https://www.arduino.cc/en/Main/ArduinoBoardMega2560
https://store.arduino.cc/arduino-due

---------------------
4.5 Ecran LCD RVB I²C
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

Voir photo "assemblage_modules"









