/*
 Hugues Rime 2016
 *
 * http://electronics.stackexchange.com/questions/157270/pic12f683-or-pic16f688-assembly-or-c-blinking-led-example-with-xc8-compiler-mpa
*/
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic12f683.h>

#pragma config FOSC = INTOSCCLK // Oscillator Selection bits (INTOSC oscillator:
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Detect (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

#define _XTAL_FREQ 8000000



void interrupt TX();
unsigned int ADC(unsigned char voie);

void main(void) {
  OSCCON = 0x70;                 // switch to 8MHz system clock
  ANSEL = 0x00;                  // digital IO
  GPIO = 0x00;                   // tous les gpio à 0
  CMCON0 = 7;

  TRISIO2 = 1;                   // pi en entree/sortie
  TRISIO3 = 1;
  TRISIO4 = 0;
  TRISIO5 = 0;
                                 // analogic input
  ADCS0 = 0b010;                 // Fréquence d'échantillonnage fixée à Fosc/32
  ANSELbits.ANS0=1;              // Gp1 en analogic in
  ADCS1 = 0b010;
  ANSELbits.ANS1=1;

  ADCON0bits.ADFM = 0;           // Format du résultat avec bit de signe
  ADCON0bits.VCFG = 0;           // référence positive = VDD
  ADCON0bits.ADON = 1;           // Mise en route du convertisseur analogique-numérique


                                 //Interruption
    GIE = 1;                     //Activation du bit d'interruption générale
    T0IE = 1;                    //Validation interruption sur TIMER0

//    //TIMER0
//    T0CS = 0 ;  // horloge du timer = fréq osc divisée par 4
//    PSA = 0 ;   // utilisation du prédiviseur
//    PS2 = PS1 = PS0 = 1 ; // prédivision par 256
//    TMR0 = 196; //Pré-chargement du compteur pour avoir un temps de 10 ms




  GP4=0;
  while(1)
    {

    unsigned int an_value = ADC(1);

    int temp = an_value;

     GP5 = 1;

    if (temp < 128)
            __delay_ms(10); // wait 500 milliseconds
    else if ((temp > 127) && (temp < 256))
            __delay_ms(100); // wait 500 milliseconds
    else if ((temp > 255) && (temp < 384))
            __delay_ms(200); // wait 500 milliseconds
    else if ((temp > 383) && (temp < 512))
            __delay_ms(500); // wait 500 milliseconds
    else if ((temp > 511) && (temp < 640))
            __delay_ms(2000); // wait 500 milliseconds
    else if ((temp > 639) && (temp < 768))
            __delay_ms(300); // wait 500 milliseconds
    else if ((temp > 767) && (temp < 896))
            __delay_ms(100); // wait 500 milliseconds
    else
            __delay_ms(0); // wait 500 milliseconds


    GP5 = 0;
    __delay_ms(100); // wait 500 milliseconds



    }
}

unsigned int ADC(unsigned char voie){

    unsigned int resultat;
    ADCON0bits.CHS0 = voie;                  // Sélection de la voie
    __delay_us(5);      // Attente d'un délai nécessaire à l'acquisition lors de l'échantillonnage
    ADCON0bits.GO_nDONE = 1;                // Lancement de la conversion
    while (ADCON0bits.GO_nDONE);            // Attente que la conversion soit terminée
    // Mise en forme du résultat sur 10 bits (0-1023)
    resultat = (ADRESH <<8) + ADRESL;
    resultat = resultat >> 6;
    return (resultat);
}

/*
void interrupt TX()
 {
     if((T0IF) && (T0IE))
     {
         TMR0 = 0x37;      // Reset timer
         T0IF = 0;           // mark the interrupt as handled ( Corrected)

     	}
 }
*/

