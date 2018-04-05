#ifndef DEF_ARDUINO // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_ARDUINO // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus
#include <Arduino.h>
#endif

#ifndef DEF_SPI // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_SPI // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus
#include <SPI.h>
#endif

#ifndef DEF_PROTO_HEADER // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_PROTO_HEADER // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus
#include "Protocentral_ADS1220.h"
#endif

 
void Protocentral_ADS1220::writeRegister(uint8_t address, uint8_t value)
{
  digitalWrite(ads1220_cs_pin,LOW);
  delay(5);
  SPI.transfer(WREG|(address<<2));      	
  SPI.transfer(value); 
  delay(5);
  digitalWrite(ads1220_cs_pin,HIGH);
}  

uint8_t Protocentral_ADS1220::readRegister(uint8_t address)
{
  uint8_t data;

  digitalWrite(ads1220_cs_pin,LOW);
  delay(5);
  SPI.transfer(RREG|(address<<2));      	
  data = SPI.transfer(SPI_MASTER_DUMMY); 
  delay(5);
  digitalWrite(ads1220_cs_pin,HIGH);

  return data;
}  


void Protocentral_ADS1220::begin()
{
  static char data;
  
  // Configuration par défaut des connexions avec l'Arduino
  // Le Chip Select du convertisseur est positionné sur la broche 7 de l'arduino
  //ads1220_cs_pin = 7;
    
  // Par défaut DRDY est positionné sur la broche 6 de l'arduino
  //ads1220_drdy_pin = 6;
    
  //Serial.begin(115200);	        	//115200 57600
  SPI.begin();                           // wake up the SPI bus.
  //SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
	
  delay(100);
  SPI_Reset(ads1220_cs_pin);                                            
  delay(100);                                                    

  digitalWrite(ads1220_cs_pin,LOW);
  /*SPI.transfer(WREG);           //WREG command (43h, 08h, 04h, 10h, and 00h)
  SPI.transfer(0x01);      	
  SPI.transfer(0x04);     
  SPI.transfer(0x10);    
  SPI.transfer(0x00);   
  */
  Config_Reg0 = 0x01;
  Config_Reg1 = 0x04;
  Config_Reg2 = 0x10;
  Config_Reg3 = 0x00;
  


  writeRegister( CONFIG_REG0_ADDRESS , Config_Reg0);
  writeRegister( CONFIG_REG1_ADDRESS , Config_Reg1);
  writeRegister( CONFIG_REG2_ADDRESS , Config_Reg2);
  writeRegister( CONFIG_REG3_ADDRESS , Config_Reg3);
  delay(100);
  /*
  SPI.transfer(RREG);           //RREG
  data = SPI.transfer(SPI_MASTER_DUMMY);
  //Serial.println(data);
  data = SPI.transfer(SPI_MASTER_DUMMY); 
  //Serial.println(data);
  data = SPI.transfer(SPI_MASTER_DUMMY); 
  //Serial.println(data);
  data = SPI.transfer(SPI_MASTER_DUMMY); 
  //Serial.println(data);
  */

  Config_Reg0 = readRegister(CONFIG_REG0_ADDRESS);
  Config_Reg1 = readRegister(CONFIG_REG1_ADDRESS);
  Config_Reg2 = readRegister(CONFIG_REG2_ADDRESS);
  Config_Reg3 = readRegister(CONFIG_REG3_ADDRESS);

  /*
  Serial.println("Config_Reg : ");
  Serial.println(Config_Reg0,HEX);
  Serial.println(Config_Reg1,HEX);
  Serial.println(Config_Reg2,HEX);
  Serial.println(Config_Reg3,HEX);
  Serial.println(" ");
  */
  digitalWrite(ads1220_cs_pin,HIGH); //release chip, signal end transfer

  SPI_Start(ads1220_cs_pin);
  delay(100);

}

void Protocentral_ADS1220::SynchroRestart()
{
  //Serial.begin(115200);	        	//115200 57600
  SPI.begin();                           // wake up the SPI bus.
  //SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
}


void Protocentral_ADS1220::SPI_Command(unsigned char data_in, uint8_t ads_cs_pin)
{
    
  digitalWrite(ads_cs_pin, LOW);
  delay(2);
  digitalWrite(ads_cs_pin, HIGH);
  delay(2);
  digitalWrite(ads_cs_pin, LOW);
  delay(2);
  SPI.transfer(data_in);
  delay(2);
  digitalWrite(ads_cs_pin, HIGH);
}

void Protocentral_ADS1220::SPI_Reset(uint8_t cs_pin)
{
  SPI_Command(RESET,cs_pin);		                    			
}

void Protocentral_ADS1220::SPI_Start(uint8_t cs_pin)
{
  SPI_Command(START,cs_pin);
}


Protocentral_ADS1220::Protocentral_ADS1220() 								// Constructors 
{
  Serial.begin(115200);	        	//115200 57600
  Serial.print("ads1220 class declared");
  NewDataAvailable = false;
  ads1220_cs_pin = 7;
  ads1220_drdy_pin = 6;
}

void Protocentral_ADS1220::PGA_ON(void)
{	 
  Config_Reg0 &= ~_BV(0);
  writeRegister(CONFIG_REG0_ADDRESS,Config_Reg0);	
}

void Protocentral_ADS1220::PGA_OFF(void)
{	 
  Config_Reg0 |= _BV(0);
  writeRegister(CONFIG_REG0_ADDRESS,Config_Reg0);	
}

void Protocentral_ADS1220::Continuous_conversion_mode_ON(void)
{
  Config_Reg1 |= _BV(2);
  writeRegister(CONFIG_REG1_ADDRESS,Config_Reg1);
}

void Protocentral_ADS1220::Single_shot_mode_ON(void)
{
  Config_Reg1 &= ~_BV(2);
  writeRegister(CONFIG_REG1_ADDRESS,Config_Reg1);
}


void Protocentral_ADS1220::set_data_rate(int datarate)
{
  Config_Reg1 &= ~REG_CONFIG_DR_MASK;
  
  switch(datarate)
  {
    case(DR_20SPS):
      Config_Reg1 |= REG_CONFIG_DR_20SPS; 
      break;
    case(DR_45SPS):
      Config_Reg1 |= REG_CONFIG_DR_45SPS; 
      break;
    case(DR_90SPS):
      Config_Reg1 |= REG_CONFIG_DR_90SPS; 
      break;
    case(DR_175SPS):
      Config_Reg1 |= REG_CONFIG_DR_175SPS; 
      break;
    case(DR_330SPS):
      Config_Reg1 |= REG_CONFIG_DR_330SPS; 
      break;
    case(DR_600SPS):
      Config_Reg1 |= REG_CONFIG_DR_600SPS; 
      break;
    case(DR_1000SPS):
      Config_Reg1 |= REG_CONFIG_DR_1000SPS; 
      break;
  }

  writeRegister(CONFIG_REG1_ADDRESS,Config_Reg1);
}

// Ajout de la fonction permettant d'appliquer un filtrage 50-60Hz 
void Protocentral_ADS1220::set_filter(int filtertype)
{
	// Réinitialisation des valeurs de configuration par défaut du filtrage NO FILTER
	Config_Reg2 &= ~REG_CONFIG_FILTER_MASK;
	
	switch(filtertype)
	{
		case(NOFILTER):
		Config_Reg2 |= REG_CONFIG_NOFILTER;
		break;
		
		case(REJECTION_50_60HZ):
		Config_Reg2 |= REG_CONFIG_50_AND_60_REJECTION;
		break;
		
		case(REJECTION_50HZ):
		Config_Reg2 |= REG_CONFIG_50_REJECTION;
		break;
		
		case(REJECTION_60HZ):
		Config_Reg2 |= REG_CONFIG_60_REJECTION;
		break;
	}
 writeRegister(CONFIG_REG2_ADDRESS,Config_Reg2);
}

// Ajout de la fonction permettant de modifier le mode d'acquisition Single_Ended ou Differential
void Protocentral_ADS1220::set_mode(int mode)
{
  // Réinitialisation des valeurs de configuration des entrées
  Config_Reg0 &= ~REG_CONFIG_PGA_INPUT_MASK;

  switch(mode)
  {
    case(INPUT_DIFF_AIN0_AIN1):
      Config_Reg0 |= REG_CONFIG_MODE_DIFF_AIN0_AIN1 ; 
      break;
    case(INPUT_DIFF_AIN0_AIN2):
      Config_Reg0 |= REG_CONFIG_MODE_DIFF_AIN0_AIN2; 
      break;
    case(INPUT_DIFF_AIN0_AIN3):
      Config_Reg0 |= REG_CONFIG_MODE_DIFF_AIN0_AIN3; 
      break;
    case(INPUT_DIFF_AIN1_AIN2):
      Config_Reg0 |= REG_CONFIG_MODE_DIFF_AIN1_AIN2; 
      break;
    case(INPUT_DIFF_AIN1_AIN3):
      Config_Reg0 |= REG_CONFIG_MODE_DIFF_AIN1_AIN3; 
      break;
    case(INPUT_DIFF_AIN2_AIN3):
      Config_Reg0 |= REG_CONFIG_MODE_DIFF_AIN2_AIN3; 
      break;
    case(INPUT_DIFF_AIN1_AIN0):
      Config_Reg0 |= REG_CONFIG_MODE_DIFF_AIN1_AIN0; 
      break;
    case(INPUT_DIFF_AIN3_AIN2):
      Config_Reg0 |= REG_CONFIG_MODE_DIFF_AIN3_AIN2; 
      break;
	case(INPUT_SINGLE_ENDED_AIN0):
      Config_Reg0 |= REG_CONFIG_MODE_SINGLE_ENDED_AIN0; 
      break;
	case(INPUT_SINGLE_ENDED_AIN1):
      Config_Reg0 |= REG_CONFIG_MODE_SINGLE_ENDED_AIN1; 
      break;
	case(INPUT_SINGLE_ENDED_AIN2):
      Config_Reg0 |= REG_CONFIG_MODE_SINGLE_ENDED_AIN2; 
      break;
	case(INPUT_SINGLE_ENDED_AIN3):
      Config_Reg0 |= REG_CONFIG_MODE_SINGLE_ENDED_AIN3; 
      break;
  }
  
  writeRegister(CONFIG_REG0_ADDRESS,Config_Reg0);
}

// Ajout de la fonction permettant de configurer le mode de fonctionnement du convertisseur
// Normal, Turbo ou Duty cycle
void Protocentral_ADS1220::set_operating_mode(int operatingMode)
{
    // Réinitialisation des valeurs de configuration des entrées
    Config_Reg1 &= ~REG_CONFIG_DR_MASK;
    
    switch(operatingMode)
    {
      case(NORMAL):
        Config_Reg1 |= REG_CONFIG_MODE_NORMAL;
        break;
      case(DUTY_CYCLE):
        Config_Reg1 |= REG_CONFIG_MODE_DUTY_CYCLE;
        break;
      case(TURBO):
        Config_Reg1 |= REG_CONFIG_MODE_TURBO;
        break;       
    }
    writeRegister(CONFIG_REG1_ADDRESS,Config_Reg1);
    
}


void Protocentral_ADS1220::set_pga_gain(int pgagain)
{
  Config_Reg0 &= ~REG_CONFIG_PGA_GAIN_MASK;

  switch(pgagain)
  {
    case(PGA_GAIN_1):
      Config_Reg0 |= REG_CONFIG_PGA_GAIN_1 ; 
      break;
    case(PGA_GAIN_2):
      Config_Reg0 |= REG_CONFIG_PGA_GAIN_2; 
      break;
    case(PGA_GAIN_4):
      Config_Reg0 |= REG_CONFIG_PGA_GAIN_4; 
      break;
    case(PGA_GAIN_8):
      Config_Reg0 |= REG_CONFIG_PGA_GAIN_8; 
      break;
    case(PGA_GAIN_16):
      Config_Reg0 |= REG_CONFIG_PGA_GAIN_16; 
      break;
    case(PGA_GAIN_32):
      Config_Reg0 |= REG_CONFIG_PGA_GAIN_32; 
      break;
    case(PGA_GAIN_64):
      Config_Reg0 |= REG_CONFIG_PGA_GAIN_64; 
      break;
    case(PGA_GAIN_128):
      Config_Reg0 |= REG_CONFIG_PGA_GAIN_128; 
      break;
  }
  
  writeRegister(CONFIG_REG0_ADDRESS,Config_Reg0);
}


uint8_t * Protocentral_ADS1220::get_config_reg()
{
  static uint8_t config_Buff[4];

  Config_Reg0 = readRegister(CONFIG_REG0_ADDRESS);
  Config_Reg1 = readRegister(CONFIG_REG1_ADDRESS);
  Config_Reg2 = readRegister(CONFIG_REG2_ADDRESS);
  Config_Reg3 = readRegister(CONFIG_REG3_ADDRESS);

  config_Buff[0] = Config_Reg0 ; 
  config_Buff[1] = Config_Reg1 ;
  config_Buff[2] = Config_Reg2 ;
  config_Buff[3] = Config_Reg3 ;

  return config_Buff;
}


uint8_t * Protocentral_ADS1220::Read_Data()
{
  static byte SPI_Buff[3];

  if((digitalRead(ads1220_drdy_pin)) == LOW)             //        Wait for DRDY to transition low
  {
  	digitalWrite(ads1220_cs_pin,LOW);                         //Take CS low
  	delayMicroseconds(100);
  	for (int i = 0; i < 3; i++)
  	{ 
  	  SPI_Buff[i] = SPI.transfer(SPI_MASTER_DUMMY);
  	}
  	delayMicroseconds(100);
  	digitalWrite(ads1220_cs_pin,HIGH);                  //  Clear CS to high
  	NewDataAvailable = true;
  }
  	
  return SPI_Buff;
}

// Ajout de méthodes permettant de configurer las broches pour "Chip Select" et "Data Ready"

void Protocentral_ADS1220::set_CS_Pin(int cs_pin)
{
    ads1220_cs_pin = cs_pin;
}
void Protocentral_ADS1220::set_DRDY_Pin (int drdy_pin)
{
    ads1220_drdy_pin = drdy_pin;
}


void Protocentral_ADS1220::displayConfig()
{
    Serial.begin(115200);
    Serial.print ("Configuration :\n");
    Serial.print ("ChipSelectr Pin :\t");
    Serial.print (ads1220_cs_pin);
    Serial.print ("\n");
    
    
}