---------------------------------
1. Principe du sondage �l�ctrique
---------------------------------
La prospection �lectrique consiste � injecter des courants sur une paire 
d��lectrodes plant�es dans le sol et � mesurer des potentiels en d�autres 
points de la surface. 

Livre Applied Geophysics 1: Electrical Methods
Auteur principal : Nicolas Florsch / Co-auteur : Fr�d�ric MUHLACH
Reli�: 220 pages
Editeur : ISTE Press - Elsevier (20 novembre 2017)
Langue : Anglais
ISBN-10: 1785481991
ISBN-13: 978-1785481994

-----------------------------------
2. Pr�sentation du projet : Magneto
-----------------------------------

Le projet de r�alisation qui vous est ici pr�sent� est destin� � la prospection 
�lectrique du sous-sol par mesure de r�sistivit�. Cet appareil est �opensource� 
et vous pourrez donc modifier � votre guise le programme qu�il contient pour 
l�adapter � vos besoins ou ajouter de nouvelles fonctionnalit�s. 

Cet appareil est uniquement d�di� � l�acquisition de valeurs analogiques, 
telles qu�intensit�s et tensions. Les signaux sont ensuite convertis au format 
num�rique et sauvegard�s sur un support de stockage pour y �tre analys�s, �
l�aide d�un logiciel sp�cialis� de g�ophysique. Il s�agit de d�terminer la 
variation spatiale des r�sistivit�s du sous-sol.

-------------------------
3. Architecture du projet
-------------------------
						  |---------------|             |-------------|
						  | Affichage des |             |  Horodatage | 
						  | informations  |				|    + GPS    | 
   						  |---------------|             |-------------|
                                   ^                            |
                                   |                            V                
    |------------|          |------------|         |------------------------|
    | Conversion |--------->| Traitement |-------->| Sauvegarde des donn�es |
    | Analog/Num |          |    (�c)    |         |       collect�es       |
    |------------|          |------------|         |------------------------|
	                               |
								   v
						    |------------|
							| Saisie des |
							| param�tres |
							|------------|
	
	
	
----------------------------------------
4. Pr�sentation des composants mat�riels
----------------------------------------
4.1 Le microcontr�leur
----------------------------------------
Le microcontr�leur est un circuit int�gr� (ou IC Integrated Circuit) dont les 
t�ches principales sont :
	� Le traitement des informations,
	� Le pilotage des p�riph�riques connect�s sur ses broches.

Notre choix s�est port� sur la version Arduino Mega2560, qui dispose de la 
quantit� de m�moire suffisante pour faire tourner le programme. 

Remarque : le mod�le UNO, reste utilisable, � condition de renoncer � 
l�acquisition des donn�es GPS.

https://www.arduino.cc/en/Main/ArduinoBoardMega2560

-----------------------------------------
4.2 Le convertisseur analogique-num�rique
-----------------------------------------
Le convertisseur analogique num�rique CAN (ou ADC Analog to Digital Converter) 
est un dispositif �lectronique qui traduit des signaux analogiques en donn�es 
num�riques.

Les entr�es des CAN acceptent g�n�ralement deux modes de fonctionnement :
	� Single Ended : tension mesur�e sur un seul fil, avec pour r�f�rence la 
	masse du montage. 
	� Diff�rentiel : tension mesur�e entre entre deux fils ; les interf�rences, 
	pr�sentes sur les deux voies, ne sont donc pas prises en compte.
	
Le convertisseur ADS1115 (16 bits de r�solution) est ici utilis� dans sa version
�breakout Board � par la soci�t� Adafruit :

https://www.adafruit.com/product/1085

-------------------------
4.3 Le Clavier analogique
-------------------------
p�riph�rique propos� par Sparkfun (VKey PRT12080 )

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
4.4 Ecran LCD RVB I�C
---------------------
Ecran de 2 lignes de 16 caract�res, qui affiche les informations de param�trage 
et les menus. Un r�tro�clairage peut �tre activ�.

http://wiki.seeed.cc/Grove-LCD_RGB_Backlight/

--------------
4.5 Module RTC
--------------
L�horloge RTC (Real Time Clock) indique l'heure � la nanoseconde pr�s. La carte 
propos�e au format Grove par la soci�t� Seeed ,dont le composant central est un 
circuit DS1307.

http://wiki.seeed.cc/Grove-RTC/

--------------------------------------
4.6 Lecteur de carte �SD au format SPI
--------------------------------------
Support de stockage utilis� pour sauvegarder toutes les donn�es au fil des 
acquisitions.

https://www.adafruit.com/product/254


-------------
5. Connexions
-------------

|------------------------------------------------------------------------------|
|       P�riph�rique      |   Connexion et alimentation    | Protocole utilis� |
|------------------------------------------------------------------------------|
| Clavier                 | + -> 5 Volts et GND (masse)    | Analogique        |
|                         | Out -> Entr�e Arduino A0       |                   |
|------------------------------------------------------------------------------|
| Option (GPS Adafruit)   | Vin (5Volts) et GND (masse)    | S�rie             |
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
|                         | SCL -> port 21 (SCL)           | I�C               |
|                         | SDA -> port 20 (SDA)           | (Adr : 0x48)      |
|                         | ADR -> GND                     |                   |
|------------------------------------------------------------------------------|
| Module RTC (Grove) 	  | VDD -> 5 Volts et GND (masse)  | I�C               |
|                         | SCL -> port 21 (SCL)           |                   |
|                         | SDA -> port 20 (SDA)           |                   |
|------------------------------------------------------------------------------|
| Ecran LCD (Grove) 	  | VDD -> 5 Volts et GND (masse)  | I�C               |
|                         | SCL -> port 21 (SCL)           |                   |
|                         | SDA -> port 20 (SDA)           |                   |
|------------------------------------------------------------------------------|







