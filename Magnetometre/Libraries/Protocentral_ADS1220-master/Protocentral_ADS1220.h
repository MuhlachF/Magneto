//#ifndef ads1292r_h
//#define ads1292r_h

#include "Arduino.h"

#include "SPI.h" // necessary library

#define SPI_MASTER_DUMMY   0xFF
#define RESET 0x06                                                    //Send the RESET command (06h) to make sure the ADS1220 is properly reset after power-up
#define START 0x08                                         //Send the START/SYNC command (08h) to start converting in continuous conversion mode
//#define WREG  0x43
#define WREG  0x40
//#define RREG  0x23
#define RREG  0x20
#define CONFIG_REG0_ADDRESS 0x00
#define CONFIG_REG1_ADDRESS 0x01
#define CONFIG_REG2_ADDRESS 0x02
#define CONFIG_REG3_ADDRESS 0x03


#define REG_CONFIG_DR_20SPS 0x00
#define REG_CONFIG_DR_45SPS 0x20
#define REG_CONFIG_DR_90SPS 0x40
#define REG_CONFIG_DR_175SPS 0x60
#define REG_CONFIG_DR_330SPS 0x80
#define REG_CONFIG_DR_600SPS 0xA0
#define REG_CONFIG_DR_1000SPS 0xC0
#define REG_CONFIG_DR_MASK 0xE0

#define DR_20SPS 20
#define DR_45SPS 45
#define DR_90SPS 90
#define DR_175SPS 175
#define DR_330SPS 330
#define DR_600SPS 600
#define DR_1000SPS 1000

// Ajout Frédéric MUHLACH
// Ajout des nouvelles valeurs d'échantillonnage pour le mode TURBO
#define DR_40SPS 20
#define DR_90SPS2 45
#define DR_180SPS 90
#define DR_350SPS 175
#define DR_660SPS 330
#define DR_1200SPS 600
#define DR_2000SPS 1000

// Ajout Frédéric MUHLACH
// Configuration du mode de fonctionnement du CAN
#define REG_CONFIG_MODE_NORMAL 0x00
#define REG_CONFIG_MODE_DUTY_CYCLE 0x08
#define REG_CONFIG_MODE_TURBO 0x10
#define REG_CONFIG_DR_MASK 0x18

#define NORMAL 0
#define DUTY_CYCLE 1
#define TURBO 2


#define REG_CONFIG_PGA_GAIN_1 0x00
#define REG_CONFIG_PGA_GAIN_2 0x02
#define REG_CONFIG_PGA_GAIN_4 0x04
#define REG_CONFIG_PGA_GAIN_8 0x06
#define REG_CONFIG_PGA_GAIN_16 0x08
#define REG_CONFIG_PGA_GAIN_32 0x0A
#define REG_CONFIG_PGA_GAIN_64 0x0C
#define REG_CONFIG_PGA_GAIN_128 0x0E
#define REG_CONFIG_PGA_GAIN_MASK 0x0E

#define PGA_GAIN_1 1
#define PGA_GAIN_2 2
#define PGA_GAIN_4 4
#define PGA_GAIN_8 8
#define PGA_GAIN_16 16
#define PGA_GAIN_32 32
#define PGA_GAIN_64 64
#define PGA_GAIN_128 128

// Ajout Frédéric MUHLACH
// Définition de nouvelles constantes pour l'activation du filtrage en entrées
// filtre de rejection 50-60Hz uniquement pour 20SPS
#define REG_CONFIG_NOFILTER 0x00
#define REG_CONFIG_50_AND_60_REJECTION 0x10
#define REG_CONFIG_50_REJECTION 0x20
#define REG_CONFIG_60_REJECTION 0x20
#define REG_CONFIG_FILTER_MASK 0x30




#define NOFILTER 0
#define REJECTION_50_60HZ 1
#define REJECTION_50HZ 2
#define REJECTION_60HZ 3


// Ajout Frédéric MUHLACH
// Définition de nouvelles constantes pour l'acquisition en mode SINGLE ENDED
// Les configurations les plus courantes sont renseignées
// Pour les entrées en mode différentiel
#define INPUT_DIFF_AIN0_AIN1 0
#define INPUT_DIFF_AIN0_AIN2 1
#define INPUT_DIFF_AIN0_AIN3 2
#define INPUT_DIFF_AIN1_AIN2 3
#define INPUT_DIFF_AIN1_AIN3 4
#define INPUT_DIFF_AIN2_AIN3 5
#define INPUT_DIFF_AIN1_AIN0 6
#define INPUT_DIFF_AIN3_AIN2 7

// Pour les entrées en mode Single Ended
#define INPUT_SINGLE_ENDED_AIN0 8
#define INPUT_SINGLE_ENDED_AIN1 9
#define INPUT_SINGLE_ENDED_AIN2 10
#define INPUT_SINGLE_ENDED_AIN3 11

// Valeurs des configurations
#define REG_CONFIG_MODE_DIFF_AIN0_AIN1 0x00
#define REG_CONFIG_MODE_DIFF_AIN0_AIN2 0x10
#define REG_CONFIG_MODE_DIFF_AIN0_AIN3 0x20
#define REG_CONFIG_MODE_DIFF_AIN1_AIN2 0x30
#define REG_CONFIG_MODE_DIFF_AIN1_AIN3 0x40
#define REG_CONFIG_MODE_DIFF_AIN2_AIN3 0x50
#define REG_CONFIG_MODE_DIFF_AIN1_AIN0 0x60
#define REG_CONFIG_MODE_DIFF_AIN3_AIN2 0x70
#define REG_CONFIG_MODE_SINGLE_ENDED_AIN0 0x80
#define REG_CONFIG_MODE_SINGLE_ENDED_AIN1 0x90
#define REG_CONFIG_MODE_SINGLE_ENDED_AIN2 0xA0
#define REG_CONFIG_MODE_SINGLE_ENDED_AIN3 0xB0
#define REG_CONFIG_PGA_INPUT_MASK 0xF0
// Fin ajout

//#define PGA_ON (Config_Reg0 & ~_BV(0))
//#define PGA_OFF (Config_Reg0 | _BV(0))

// Suppression des define pour une gestion modulaire
//#define ADS1220_CS_PIN 7
//#define ADS1220_DRDY_PIN 6


#define _BV(bit) (1<<(bit))  

class Protocentral_ADS1220
{
  private:
    uint8_t Config_Reg0;
    uint8_t Config_Reg1;
    uint8_t Config_Reg2;
    uint8_t Config_Reg3;
    
    // Ajout Fred
    // Définition de nouvelles variables de configuration
    uint8_t ads1220_cs_pin;
    
    // Par défaut DRDY est positionné sur la broche 6 de l'arduino
    uint8_t ads1220_drdy_pin;
    
  
    
  public:
    uint8_t NewDataAvailable;
    


    Protocentral_ADS1220();
    void begin(void);
    
    // Récriture des fonctions statiques -> passage en paramètre de la variable ads1220_cs_pin 
    //static void SPI_Start();
    //static void SPI_Reset();    
    //static void SPI_Command(unsigned char data_in);
    static void SPI_Start(uint8_t cs_pin);
    static void SPI_Reset(uint8_t cs_pin);
    static void SPI_Command(unsigned char data_in, uint8_t cs_pin);
    
    
    
    void writeRegister(uint8_t address, uint8_t value);
    uint8_t readRegister(uint8_t address);
    uint8_t * Read_Data(void);
    uint8_t * get_config_reg(void);

    void PGA_OFF(void);
    void PGA_ON(void);
    // Ajout de la fonction de configuration du Chip Select
    void Continuous_conversion_mode_ON(void);
    void Single_shot_mode_ON(void);
    void set_data_rate(int datarate);
    void set_pga_gain(int pgagain);
	// Ajout de la fonction permettant d'appliquer un filtrage 50-60Hz uniquement pour 20SPS
	void set_filter(int filtertype);
	// Ajout de la fonction set_mode permettant de configurer le mode de mesure
    void set_mode(int mode);
    // Ajout de la fonction set_operating_mode permettant de configurer le convertisseur en mode Turbo
    void set_operating_mode(int operatingMode);
    void set_CS_Pin(int cs_pin);
    void set_DRDY_Pin (int drdy_pin);
    void displayConfig();
    void SynchroRestart();
};