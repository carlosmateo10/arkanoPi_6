/** arkanoPi_6.h
* Libreria del programa principal del juego.
*/


#ifndef _ARKANOPI_H_
#define _ARKANOPI_H_

#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <wiringPi.h>
#include <wiringPi.h>
#include <lcd.h>


#include "kbhit.h" // para poder detectar teclas pulsadas sin bloqueo y leer las teclas pulsadas

#include "arkanoPiLib.h"

#include "fsm.h" // para poder crear y ejecutar la máquina de estados

#define CLK_MS 10 // Periodo de actualizacion de la maquina de estados (10 ms)
#define CLK_1MS 1
#define CLK_PELOTA 500

#define DEBOUNCE_TIME 50

// FLAGS DEL SISTEMA
#define FLAG_RAQUETA_DERECHA 			0x01
#define FLAG_RAQUETA_IZQUIERDA 			0x02
#define FLAG_TECLA			 			0x04
#define FLAG_PELOTA						0x08
#define FLAG_FINAL_JUEGO	 			0x10
#define FLAG_PAUSA						0x20
#define FLAG_JOYSTICK					0x40
#define FLAG_PAUSA1						0X80
#define FLAG_PAUSA2						0X100

#define DEMUX0 3
#define DEMUX1 4
#define DEMUX2 7
#define DEMUX3 14

#define FILA0 0
#define FILA1 1
#define FILA2 2


//USE WIRINGPI PIN NUMBERS
#define LCD_RS  8               //Register select pin
#define LCD_E   10               //Enable Pin
#define LCD_D0  0               //Data pin D0
#define LCD_D1  0               //Data pin D1
#define LCD_D2  0               //Data pin D2
#define LCD_D3  0               //Data pin D3
#define LCD_D4  22				//Data pin D4
#define LCD_D5  23				//Data pin D5
#define LCD_D6  24				//Data pin D6
#define LCD_D7  25				//Data pin D7

#define PUL_DER 16				//Define la posición de los pulsadores en los GPIO
#define PUL_IZQ	19



typedef struct {
	tipo_arkanoPi arkanoPi;
	char teclaPulsada;
} tipo_juego;

// A 'key' which we can lock and unlock - values are 0 through 3
//	This is interpreted internally as a pthread_mutex by wiringPi
//	which is hiding some of that to make life simple.
#define	FLAGS_KEY	1			//Mutex de flags
#define	STD_IO_BUFFER_KEY	2	//Mutex de la pantalla

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
// Prototipos de funciones de entrada
int CompruebaTeclaPulsada (fsm_t* this);
int CompruebaTeclaPelota (fsm_t* this);
int CompruebaTeclaPausa (fsm_t* this);
int CompruebaTeclaRaquetaIzquierda (fsm_t* this);
int CompruebaTeclaRaquetaDerecha (fsm_t* this);
int CompruebaFinalJuego (fsm_t* this);
int CompruebaJoystick (fsm_t* this);


//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void InicializaJuego (fsm_t*);
void MueveRaquetaIzquierda (fsm_t*);
void MueveRaquetaDerecha (fsm_t*);
void MovimientoPelota (fsm_t*);
void FinalJuego (fsm_t*);
void ReseteaJuego (fsm_t*);
void Pausa (fsm_t*);
void ControlJoystick (fsm_t*);

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------
PI_THREAD (thread_explora_teclado);

//------------------------------------------------------
// FUNCIONES DE INICIALIZACION
//------------------------------------------------------
int systemSetup (void);

#endif /* ARKANOPI_H_ */
