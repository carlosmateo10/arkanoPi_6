/** arkanoPi_5.c
* Programa principal del juego.
*/

// Mensaje para el inicio del juego. "-ARKANOPI-"
int mensaje[57][7] = {
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},

{0,1,1,1,1,1,0},
{0,1,0,1,0,0,0},//A
{0,1,0,1,0,0,0},
{0,1,1,1,1,1,0},

{0,0,0,0,0,0,0},
{0,1,1,1,1,1,0},//R
{0,1,0,1,0,0,0},
{0,1,0,1,1,0,0},
{0,1,1,1,0,1,0},

{0,0,0,0,0,0,0},
{0,1,1,1,1,1,0},//K
{0,0,0,1,0,0,0},
{0,0,1,0,1,0,0},
{0,1,0,0,0,1,0},

{0,0,0,0,0,0,0},
{0,1,1,1,1,1,0},
{0,1,0,1,0,0,0},//A
{0,1,0,1,0,0,0},
{0,1,1,1,1,1,0},

{0,0,0,0,0,0,0},
{0,1,1,1,1,1,0},//N
{0,0,1,0,0,0,0},
{0,0,0,1,0,0,0},
{0,1,1,1,1,1,0},

{0,0,0,0,0,0,0},
{0,1,1,1,1,1,0},
{0,1,0,0,0,1,0},//O
{0,1,0,0,0,1,0},
{0,1,1,1,1,1,0},

{0,0,0,0,0,0,0},
{0,1,1,1,1,1,0},//P
{0,1,0,1,0,0,0},
{0,1,0,1,0,0,0},
{0,0,1,0,0,0,0},

{0,0,0,0,0,0,0},
{0,1,0,0,0,1,0},//I
{0,1,1,1,1,1,0},
{0,1,0,0,0,1,0},
{0,0,0,0,0,0,0},

{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{0,0,0,0,0,0,0},
{0,0,0,0,0,0,0},
{0,0,0,0,0,0,0},
{0,0,0,0,0,0,0},
{0,0,0,0,0,0,0},
{0,0,0,0,0,0,0},
{0,0,0,0,0,0,0},
{0,0,0,0,0,0,0},
{0,0,0,0,0,0,0},
{0,0,0,0,0,0,0}
};

int delayMusica=0; // Retardo asociado al tiempo de espera entre notas musicales de la melodía

//Arrays que contienen notas musicales.
int c[5]={131,262,523,1046,2093};       // frecuencias 4 octavas de Do
int cs[5]={139,277,554,1108,2217};      // Do#
int d[5]={147,294,587,1175,2349};       // Re
int ds[5]={156,311,622,1244,2489};    // Re#
int e[5]={165,330,659,1319,2637};      // Mi
int f[5]={175,349,698,1397,2794};       // Fa
int fs[5]={185,370,740,1480,2960};     // Fa#
int g[5]={196,392,784,1568,3136};     // Sol
int gs[5]={208,415,831,1661,3322};   // Sol#
int a[5]={220,440,880,1760,3520};      // La
int as[5]={233,466,932,1866,3729};    // La#
int b[5]={247,494,988,1976,3951};      // Si


int debounceTime; //variable asociada a evitar los rebotes de los pulsadores.
int lcd; //variable que se inicializará para el uso del lcd
#include <wiringPi.h>
#include <lcd.h> //libreria para implementar el LCD.
#include "arkanoPi_6.h"
#include "tmr.h"
#include <softTone.h> //libreria para implementar el zumbador.

//Definición de los estados
enum fsm_state{
	WAIT_START=0,
	WAIT_PUSH=1,
	WAIT_END=2,
	PAUSE=3};

static tmr_t* tmr;
static tmr_t* tmr_pelota;
static tmr_t* tmr_pausa;
static tmr_t* tmr_bso;
volatile int flags = 0;
int columnaActual=0;
int fila=0;

int bso=0;

void delay_until (unsigned int next) {  					   // espera hasta la proxima activacion del reloj asociado a la fsm
	unsigned int now = millis();

	if (next > now) {
		delay (next - now);
    }
}

static volatile tipo_juego juego;
//-------------------------------------------------------------------------
// Método que se encarga de mostrar por la pantalla el mensaje
// inicial definido anteriormente. Se muestra además un mensaje por el LCD.
// Primero se muestra el mensaje con todo lo que quepa en el display, posteriormente
// este se desplaza a la izquierda con un efecto de scroll.
//--------------------------------------------------------------------------
void mensajeInicial(){
	int x=1;
	int i,j=0;
	while(x){
		if(FLAG_TECLA){
			x=0;
		}
		lcdClear(lcd);
		lcdPosition(lcd, 0, 0);
		lcdPuts(lcd, "ArkanoPi");
		lcdPosition(lcd, 0, 1);
		lcdPuts(lcd, "Pulse tecla");
		for(i=0;i<57;i++) {
			for(j=0;j<MATRIZ_ALTO;j++) {
				if (i<10){
	juego.arkanoPi.pantalla.matriz[i][j]=mensaje[i][j];
	}
	else if(i>=10){
	juego.arkanoPi.pantalla.matriz[0][j]=mensaje[i-9][j];
	juego.arkanoPi.pantalla.matriz[1][j]=mensaje[i-8][j];
	juego.arkanoPi.pantalla.matriz[2][j]=mensaje[i-7][j];
	juego.arkanoPi.pantalla.matriz[3][j]=mensaje[i-6][j];
	juego.arkanoPi.pantalla.matriz[4][j]=mensaje[i-5][j];
	juego.arkanoPi.pantalla.matriz[5][j]=mensaje[i-4][j];
	juego.arkanoPi.pantalla.matriz[6][j]=mensaje[i-3][j];
	juego.arkanoPi.pantalla.matriz[7][j]=mensaje[i-2][j];
	juego.arkanoPi.pantalla.matriz[8][j]=mensaje[i-1][j];
	juego.arkanoPi.pantalla.matriz[9][j]=mensaje[i][j];
	}
	}
	delay(120);
	}
	}
	return;
}
//-----------------------------------------------------------
// Método encargado de atender la interrupción producida por 
// la pulsación del pulsador derecho. Activa los flags correspodientes
// y además se incluye un método para evitar rebotes teniendo en
// cuenta el tiempo que necesita el usuario para pulsarlos cada vez
// que lo hace.
//-----------------------------------------------------------
void pulsDerecha (void) {

	if (millis () < debounceTime)
	{
	debounceTime = millis () + DEBOUNCE_TIME ;
	return;
	}
	// Atención a la interrupción
	piLock (FLAGS_KEY);
	flags |= FLAG_RAQUETA_DERECHA;
	flags |= FLAG_PAUSA1;
	flags |= FLAG_TECLA;
	piUnlock (FLAGS_KEY);
	// Wait for key to be released
	while (digitalRead (16) == HIGH) {
	delay (1) ;
	}
	debounceTime = millis () + DEBOUNCE_TIME ;


}

//-----------------------------------------------------------
// Método encargado de atender la interrupción producida por 
// la pulsación del pulsador izquierda. Activa los flags correspodientes
// y además se incluye un método para evitar rebotes teniendo en
// cuenta el tiempo que necesita el usuario para pulsarlos cada vez
// que lo hace.
//-----------------------------------------------------------
void pulsIzquierda (void) {
	if (millis () < debounceTime)
	{
	debounceTime = millis () + DEBOUNCE_TIME ;
	return;
	}
	// Atención a la interrupción
	piLock (FLAGS_KEY);
	flags |= FLAG_RAQUETA_IZQUIERDA;
	flags |= FLAG_TECLA;
	flags |= FLAG_PAUSA2;
	piUnlock (FLAGS_KEY);
	// Wait for key to be released
	while (digitalRead (19) == HIGH) {
	delay (1) ;
	}
	debounceTime = millis () + DEBOUNCE_TIME ;
}


//------------------------------------------------------
// Funcion que se ejecuta cada 1ms. Se encarga de mandar 
// la información correspondiente por cada puerto GPIO.
// Lo hace de forma que se comprueban las posiciones a
// encender, y únicamente enciende las mismas.
// De esta forma se reduce el tiempo de ejecución.
//------------------------------------------------------
void timer_isr (union sigval value) {
//leds
	
	while(juego.arkanoPi.pantalla.matriz[columnaActual][fila]==0){
		fila++;
		if(fila==7){
			fila=0;
			columnaActual++;
		}
		if(columnaActual>=10){
			columnaActual=0;
		}
	}
	digitalWrite(FILA0, fila & 0x01);
	digitalWrite(FILA1, fila & 0x02);
	digitalWrite(FILA2, fila & 0x04);

	digitalWrite(DEMUX0, columnaActual & 0x01);
	digitalWrite(DEMUX1, columnaActual & 0x02);
	digitalWrite(DEMUX2, columnaActual & 0x04);
	digitalWrite(DEMUX3, columnaActual & 0x08);

	fila++;
	if(fila==7){
		fila=0;
		columnaActual++;
	}
	if(columnaActual>=10){
		columnaActual=0;
	}
	tmr_startms(tmr, 1);
}
//------------------------------------------------------
// Funcion que se ejecuta cada 75 ms. Es la encargada de
//desactivar los flags asociados a la pausa y la pulsación
//de cada uno de los pulsadores. Si estos no se pulsan a la
//vez en menos de 75 ms, se desactivan sus flags, para evitar
//entrar en la pausa de forma indeseada.
//------------------------------------------------------
void pausa_isr (union sigval value) {
	piLock (FLAGS_KEY);
	flags &=~FLAG_PAUSA1;
	flags &=~FLAG_PAUSA2;
	piUnlock (FLAGS_KEY);
	tmr_startms((tmr_pausa), 75);
}
//------------------------------------------------------
// Funcion que se ejecuta cada 500ms. Es la encargada de
// activar el flag correspondiente a la pelota, para que
// así esta se actualice de forma automática, sin que sea
// necesario pulsar la tecla P.
//------------------------------------------------------
void temporizadorPelota (union sigval value) {
	piLock (FLAGS_KEY);
	flags |=FLAG_PELOTA;
	piUnlock (FLAGS_KEY);
	tmr_startms((tmr_pelota), CLK_PELOTA);
}

//------------------------------------------------------
// Funcion que se ejecuta cada tiempo variable. Se encarga
// de hacer sonar música mientras jugamos a través del zumbador.
// Se ejecuta utilizando la librería softTone de wiringPi.
// La música reproducida es la marcha imperial de Star Wars.
//------------------------------------------------------
void superMusica (union sigval value) {
	if(bso==0){
		softToneWrite(17,g[2]);
		delayMusica=500;
	}else if(bso==1){
		softToneWrite(17,0);
		delayMusica=100;
	}else if(bso==2){
		softToneWrite(17,g[2]);
		delayMusica=500;
	}else if(bso==3){
		softToneWrite(17,0);
		delayMusica=100;
	}else if(bso==4){
		softToneWrite(17,g[2]);
		delayMusica=500;
	}else if(bso==5){
		softToneWrite(17,0);
		delayMusica=100;
	}else if(bso==6){
		softToneWrite(17,ds[2]);
		delayMusica=500;
	}else if(bso==7){
		softToneWrite(17,0);
		delayMusica=1;
	}else if(bso==8){
		softToneWrite(17,as[2]);
		delayMusica=125;
	}else if(bso==9){
		softToneWrite(17,0);
		delayMusica=25;
	}else if(bso==10){
		softToneWrite(17,g[2]);
		delayMusica=500;
	}else if(bso==11){
		softToneWrite(17,0);
		delayMusica=100;
	}else if(bso==12){
		softToneWrite(17,ds[2]);
		delayMusica=500;
	}else if(bso==13){
		softToneWrite(17,0);
		delayMusica=1;
	}else if(bso==14){
		softToneWrite(17,as[2]);
		delayMusica=125;
	}else if(bso==15){
		softToneWrite(17,0);
		delayMusica=25;
	}else if(bso==16){
		softToneWrite(17,g[2]);
		delayMusica=500;
	}else if(bso==17){
		softToneWrite(17,0);
		delayMusica=2000;
	}
	bso++;
	if(bso>17){
		bso=0;
	}

	tmr_startms((tmr_bso), delayMusica);
}

//--------------------------------------------------------------------------------
// FUNCIONES DE COMPROBACION DE LA MAQUINA DE ESTADOS.
// Se ejecutan cada vez que se cumple el temporizador de la fsm y 
//comprueban la activación de los flags correspondientes a cada función: pelota
// o movimiento de la raqueta. El ultimo de ellos comprueba que el juego ha acabado.
// También se ha incluído la comprobación de los flags de la Pausa. Se activará tanto
// si pulsamos la tecla "y" en modo depuración, o si pulsamos ambos pulsadores a la vez.
//---------------------------------------------------------------------------------
int CompruebaTeclaPulsada (fsm_t* this) {
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_TECLA);
	flags &= ~FLAG_TECLA;
	piUnlock (FLAGS_KEY);

	return result;
}
int CompruebaTeclaPausa (fsm_t* this) {
	int result1,result2,result3, result4;

	piLock (FLAGS_KEY);
	result1 = (flags & FLAG_PAUSA);
	result2 = (flags & FLAG_PAUSA1);
	result3 = (flags & FLAG_PAUSA2);
	result4 = result2 && result3; //result4 activada si ambos pulsadores estaban activos.
	flags &= ~FLAG_PAUSA;

	piUnlock (FLAGS_KEY);

	return (result1 || result4); // Activaremos la pausa si o bien estaban ambos pulsadores activos
									// o bien si se pulsó la tecla "y" (solo en depuración)
}

int CompruebaTeclaPelota (fsm_t* this) {
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_PELOTA);
	piUnlock (FLAGS_KEY);

	return result;
}

int CompruebaTeclaRaquetaIzquierda (fsm_t* this) {
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_RAQUETA_IZQUIERDA);
	piUnlock (FLAGS_KEY);

	return result;
}

int CompruebaTeclaRaquetaDerecha (fsm_t* this) {
	int result;
	piLock (FLAGS_KEY);
	result = (flags & FLAG_RAQUETA_DERECHA);
	piUnlock (FLAGS_KEY);

	return result;
}

int CompruebaFinalJuego (fsm_t* this) {
	int result;

	piLock (FLAGS_KEY);
	result = (flags & FLAG_FINAL_JUEGO);
	piUnlock (FLAGS_KEY);

	return result;
}


// FUNCIONES DE SALIDA O ACCION DE LA MAQUINA DE ESTADOS

// void InicializaJuego (void): funcion encargada de llevar a cabo
// la oportuna inicialización de toda variable o estructura de datos
// que resulte necesaria para el desarrollo del juego.
void InicializaJuego (fsm_t* fsm) {
	piLock (FLAGS_KEY);
	InicializaArkanoPi(&(juego.arkanoPi));
	piUnlock (FLAGS_KEY);

}

// void MueveRaquetaIzquierda (void): funcion encargada de ejecutar
// el movimiento hacia la izquierda contemplado para la raqueta.
// Debe garantizar la viabilidad del mismo mediante la comprobaciÃ³n
// de que la nueva posiciÃ³n correspondiente a la raqueta no suponga
// que Ã©sta rebase o exceda los lÃ­mites definidos para el Ã¡rea de juego
// (i.e. al menos uno de los leds que componen la raqueta debe permanecer
// visible durante todo el transcurso de la partida).
void MueveRaquetaIzquierda (fsm_t* fsm) {
	piLock (FLAGS_KEY);
	flags &= ~FLAG_RAQUETA_IZQUIERDA;
	if(juego.arkanoPi.raqueta.x==-2){
		juego.arkanoPi.raqueta.x= juego.arkanoPi.raqueta.x+1-1;
	}
	else juego.arkanoPi.raqueta.x= juego.arkanoPi.raqueta.x-1;

	ActualizaPantalla(&(juego.arkanoPi));
	#ifdef DEBUG
	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal((&juego.arkanoPi.pantalla));
	piUnlock (STD_IO_BUFFER_KEY);
	#endif
	piUnlock (FLAGS_KEY);
}

// void MueveRaquetaDerecha (void): funciÃ³n similar a la anterior
// encargada del movimiento hacia la derecha.
void MueveRaquetaDerecha (fsm_t* fsm) {
	piLock (FLAGS_KEY);
	if(juego.arkanoPi.raqueta.x==9){
		juego.arkanoPi.raqueta.x= juego.arkanoPi.raqueta.x+1-1;
		}
		else juego.arkanoPi.raqueta.x= juego.arkanoPi.raqueta.x+1; 
	flags &= ~FLAG_RAQUETA_DERECHA;
	ActualizaPantalla(&(juego.arkanoPi));
	#ifdef DEBUG
	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal((&juego.arkanoPi.pantalla));
	piUnlock (STD_IO_BUFFER_KEY);
	#endif
	piUnlock (FLAGS_KEY);
}

// void MovimientoPelota (void): funciÃ³n encargada de actualizar la
// posiciÃ³n de la pelota conforme a la trayectoria definida para Ã©sta.
// Para ello deberÃ¡ identificar los posibles rebotes de la pelota para,
// en ese caso, modificar su correspondiente trayectoria (los rebotes
// detectados contra alguno de los ladrillos implicarÃ¡n adicionalmente
// la eliminaciÃ³n del ladrillo). Del mismo modo, deberÃ¡ tambiÃ©n
// identificar las situaciones en las que se dÃ© por finalizada la partida:
// bien porque el jugador no consiga devolver la pelota, y por tanto Ã©sta
// rebase el lÃ­mite inferior del Ã¡rea de juego, bien porque se agoten
// los ladrillos visibles en el Ã¡rea de juego.
void MovimientoPelota (fsm_t* fsm) {
	if(flags & FLAG_PAUSA){
		return;
	}else{
	piLock (FLAGS_KEY);
	flags &= ~FLAG_PELOTA;
	piUnlock (FLAGS_KEY);
	int tmpX=juego.arkanoPi.pelota.x;					//posiciones actuales de la pelota
	int auxY=juego.arkanoPi.pelota.y+juego.arkanoPi.pelota.yv;  //posicion siguiente de la pelota
	if(auxY>6)
				flags |= FLAG_FINAL_JUEGO;
	if(juego.arkanoPi.pelota.yv==1){ //ABAJO
			if(juego.arkanoPi.pelota.y>6){
				flags |= FLAG_FINAL_JUEGO;
			}
			else if(juego.arkanoPi.pelota.xv==-1){ //ABAJO IZQUIERDA
				if(tmpX<1){
					juego.arkanoPi.pelota.xv=1;
					if(juego.arkanoPi.raqueta.x==tmpX || juego.arkanoPi.raqueta.x+1==tmpX || juego.arkanoPi.raqueta.x+2==tmpX){
						juego.arkanoPi.pelota.yv=-1;
					}
				}

				else if(juego.arkanoPi.pantalla.matriz[juego.arkanoPi.pelota.x-1][juego.arkanoPi.pelota.y+1]==1){
					if(juego.arkanoPi.pelota.y==0){
						juego.arkanoPi.ladrillos.matriz[juego.arkanoPi.pelota.x-1][juego.arkanoPi.pelota.y+1]=0;
						juego.arkanoPi.pelota.yv=-1;
						juego.arkanoPi.pelota.xv=+1;
						if(CalculaLadrillosRestantes((tipo_pantalla*)(&(juego.arkanoPi.ladrillos)))==0){
							piLock (FLAGS_KEY);
							flags |= FLAG_FINAL_JUEGO;
							piUnlock (FLAGS_KEY);
						}
					}
					else if(tmpX==juego.arkanoPi.raqueta.x+1){
						juego.arkanoPi.pelota.yv=-1;
						juego.arkanoPi.pelota.xv=-1;
					}
					else if(tmpX==juego.arkanoPi.raqueta.x+2){
						juego.arkanoPi.pelota.yv=-1;
						juego.arkanoPi.pelota.xv=0;
					}
					else if(tmpX==juego.arkanoPi.raqueta.x+3){
						juego.arkanoPi.pelota.yv=-1;
						juego.arkanoPi.pelota.xv=+1;
					}
				}
			}

			else if(juego.arkanoPi.pelota.xv==0){ //ABAJO ABAJO
				if(juego.arkanoPi.pantalla.matriz[juego.arkanoPi.pelota.x][juego.arkanoPi.pelota.y+1]==1){
					if(tmpX==juego.arkanoPi.raqueta.x){
						juego.arkanoPi.pelota.yv=-1;
						juego.arkanoPi.pelota.xv=-1;
					}
					else if(tmpX==juego.arkanoPi.raqueta.x+1){
						juego.arkanoPi.pelota.yv=-1;
					}
					else if(tmpX==juego.arkanoPi.raqueta.x+2){
						juego.arkanoPi.pelota.yv=-1;
						juego.arkanoPi.pelota.xv=+1;
					}
				}
			}
			else if(juego.arkanoPi.pelota.xv==1){ //ABAJO DERECHA
				if(tmpX>8){
					juego.arkanoPi.pelota.xv=-1;
					if(juego.arkanoPi.raqueta.x==tmpX || juego.arkanoPi.raqueta.x+1==tmpX || juego.arkanoPi.raqueta.x+2==tmpX){
						juego.arkanoPi.pelota.yv=-1;
					}
				}
				else if(juego.arkanoPi.pantalla.matriz[juego.arkanoPi.pelota.x+1][juego.arkanoPi.pelota.y+1]==1){
					if(juego.arkanoPi.pelota.y==0){
						juego.arkanoPi.ladrillos.matriz[juego.arkanoPi.pelota.x+1][juego.arkanoPi.pelota.y+1]=0;
						juego.arkanoPi.pelota.yv=-1;
						juego.arkanoPi.pelota.xv=-1;
						if(CalculaLadrillosRestantes((tipo_pantalla*)(&(juego.arkanoPi.ladrillos)))==0){
							piLock (FLAGS_KEY);
							flags |= FLAG_FINAL_JUEGO;
							piUnlock (FLAGS_KEY);
						}
					}
					else if(tmpX==juego.arkanoPi.raqueta.x-1){
						juego.arkanoPi.pelota.yv=-1;
						juego.arkanoPi.pelota.xv=-1;
					}
					else if(tmpX==juego.arkanoPi.raqueta.x){
						juego.arkanoPi.pelota.yv=-1;
						juego.arkanoPi.pelota.xv=0;
					}
					else if(tmpX==juego.arkanoPi.raqueta.x+1){
						juego.arkanoPi.pelota.xv=+1;
					}
				}
			}
		}
	else if(juego.arkanoPi.pelota.yv==-1){ //ARRIBA
			if(juego.arkanoPi.pelota.y==0){
				juego.arkanoPi.pelota.yv=1;
			}
			else if(juego.arkanoPi.pelota.xv==-1){ //ARRIBA IZQUIERDA
				if(juego.arkanoPi.pelota.x==0){
					if(juego.arkanoPi.pelota.y!=0){
						juego.arkanoPi.pelota.xv=1;
					}
					else if (juego.arkanoPi.pelota.y==0){
						juego.arkanoPi.pelota.xv=1;
						juego.arkanoPi.pelota.yv=1;
					}
				}
				else if(juego.arkanoPi.pantalla.matriz[juego.arkanoPi.pelota.x-1][juego.arkanoPi.pelota.y-1]==1){
					juego.arkanoPi.ladrillos.matriz[juego.arkanoPi.pelota.x-1][juego.arkanoPi.pelota.y-1]=0;
					juego.arkanoPi.pelota.x=juego.arkanoPi.pelota.x-1;
					juego.arkanoPi.pelota.y=juego.arkanoPi.pelota.y-1;
					juego.arkanoPi.pelota.yv=1;
					if(CalculaLadrillosRestantes((tipo_pantalla*)(&(juego.arkanoPi.ladrillos)))==0){
						piLock (FLAGS_KEY);
						flags |= FLAG_FINAL_JUEGO;
						piUnlock (FLAGS_KEY);
					}
				}
			}//ARRIBA IZQUIERDA
			else if(juego.arkanoPi.pelota.xv==0){ //ARRIBA ARRIBA
				if(juego.arkanoPi.pantalla.matriz[juego.arkanoPi.pelota.x][juego.arkanoPi.pelota.y-1]==1){
					juego.arkanoPi.ladrillos.matriz[juego.arkanoPi.pelota.x][juego.arkanoPi.pelota.y-1]=0;
					juego.arkanoPi.pelota.y=juego.arkanoPi.pelota.y-1;
					juego.arkanoPi.pelota.yv=1;
					if(CalculaLadrillosRestantes((tipo_pantalla*)(&(juego.arkanoPi.ladrillos)))==0){
						piLock (FLAGS_KEY);
						flags |= FLAG_FINAL_JUEGO;
						piUnlock (FLAGS_KEY);
					}
				}
			}//ARRIBA ARRIBA
			else if(juego.arkanoPi.pelota.xv==1){ //ARRIBA DERECHA
				if(juego.arkanoPi.pelota.x==9){
					if(juego.arkanoPi.pelota.y!=0){
						juego.arkanoPi.pelota.xv=-1;
					}
					else if (juego.arkanoPi.pelota.y==0){
						juego.arkanoPi.pelota.xv=-1;
						juego.arkanoPi.pelota.yv=1;
					}
				}
				else if(juego.arkanoPi.pantalla.matriz[juego.arkanoPi.pelota.x+1][juego.arkanoPi.pelota.y-1]==1){
					juego.arkanoPi.ladrillos.matriz[juego.arkanoPi.pelota.x+1][juego.arkanoPi.pelota.y-1]=0;
					juego.arkanoPi.pelota.yv=1;
					if(CalculaLadrillosRestantes((tipo_pantalla*)(&(juego.arkanoPi.ladrillos)))==0){
						piLock (FLAGS_KEY);
						flags |= FLAG_FINAL_JUEGO;
						piUnlock (FLAGS_KEY);
					}
				}
			} //ARRIBA DERECHA
		} //ARRIBA
	if(auxY>6)
			flags |= FLAG_FINAL_JUEGO;
	else{
	juego.arkanoPi.pelota.x += juego.arkanoPi.pelota.xv;
	juego.arkanoPi.pelota.y += juego.arkanoPi.pelota.yv;
	}
	ActualizaPantalla(&(juego.arkanoPi)); //Tras cada ejecución, se actualiza la pantalla.
	#ifdef DEBUG
	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal((&juego.arkanoPi.pantalla));
	piUnlock (STD_IO_BUFFER_KEY);
	#endif
	piUnlock (FLAGS_KEY);

	int ladrillos_juego=CalculaLadrillosRestantes(&(juego.arkanoPi.ladrillos));
	lcdClear(lcd);						//Mostramos por el LCD los ladrillos restantes
	lcdPosition(lcd, 0, 0);
	lcdPuts(lcd, "Ladrillos:");
	lcdPosition(lcd, 0, 1);
	lcdPrintf(lcd, "%d", ladrillos_juego);

	}

}
//--------------------------------------------------------------
// Este método está activa el modo Pausa. Consistirá en pintar 
// por pantalla una "P" además de indicar por el LCD que estamos en 
// el modo Pausa.
//--------------------------------------------------------------
void Pausa(fsm_t* fsm){
	piLock (FLAGS_KEY);
	flags &= ~FLAG_PAUSA1;
	flags &= ~FLAG_PAUSA2;
	piUnlock (FLAGS_KEY);

	lcdClear(lcd);
	lcdPosition(lcd, 0, 0);
	lcdPuts(lcd, "PAUSA");
	lcdPosition(lcd, 0, 1);

	PintaMensajePausa(&(juego.arkanoPi.pantalla));
	#ifdef DEBUG
	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal((&juego.arkanoPi.pantalla));
	piUnlock (STD_IO_BUFFER_KEY);
	#endif
}

// void FinalJuego (void): funciÃ³n encargada de mostrar en la ventana de
// terminal los mensajes necesarios para informar acerca del resultado del juego.
void FinalJuego (fsm_t* fsm) {
	//Se imprime por pantalla los resultados y el motivo por el que ha acabado la partida
	int ladrillos_juego=CalculaLadrillosRestantes(&(juego.arkanoPi.ladrillos));
	piLock (FLAGS_KEY);
	flags = 0;
	piUnlock (FLAGS_KEY);
	if(ladrillos_juego==0){
		#ifdef DEBUG
		piLock (STD_IO_BUFFER_KEY);
		printf("¡Has ganado!\n" );
		printf("Has roto todos los ladrillos.\n");
		fflush(stdout);
		piUnlock (STD_IO_BUFFER_KEY);
		#endif
		lcdClear(lcd);
		lcdPosition(lcd, 0, 0);
		lcdPuts(lcd, "Felicidades");
		lcdPosition(lcd, 0, 1);
		lcdPuts(lcd, "Has ganado");
	}
	else {
		#ifdef DEBUG
		piLock (STD_IO_BUFFER_KEY);
		printf("Game over\n");
		printf("Te han faltado %d ladrillos por romper.\n", ladrillos_juego);
		fflush(stdout);
		piUnlock (STD_IO_BUFFER_KEY);
		#endif
		lcdClear(lcd);
		lcdPosition(lcd, 0, 0);
		lcdPuts(lcd, "Game over");
		lcdPosition(lcd, 0, 1);
		lcdPuts(lcd, "Repetimos?");
	}
}

//void ReseteaJuego (void): funciÃ³n encargada de llevar a cabo la
// reinicializaciÃ³n de cuantas variables o estructuras resulten
// necesarias para dar comienzo a una nueva partida.
void ReseteaJuego (fsm_t* fsm) {
	//AQUI
	#ifdef DEBUG
	piLock (STD_IO_BUFFER_KEY);
	printf("Bienvenido a ArkanoPi, pulse para continuar\n");
	piUnlock (STD_IO_BUFFER_KEY);
	#endif

	mensajeInicial();
	lcdClear(lcd);
	lcdPosition(lcd, 0, 0);
	lcdPuts(lcd, "ArkanoPi");
	lcdPosition(lcd, 0, 1);
	lcdPuts(lcd, "Pulse tecla");

}

//------------------------------------------------------
// FUNCIONES DE INICIALIZACION
//------------------------------------------------------

// int systemSetup (void): procedimiento de configuracion del sistema.
// RealizarÃ¡, entra otras, todas las operaciones necesarias para:
// configurar el uso de posibles librerÃ­as (e.g. Wiring Pi),
// configurar las interrupciones externas asociadas a los pines GPIO,
// configurar las interrupciones periÃ³dicas y sus correspondientes temporizadores,
// crear, si fuese necesario, los threads adicionales que pueda requerir el sistema
int systemSetup (void) {

	// sets up the wiringPi library
	if (wiringPiSetupGpio () < 0) {
		#ifdef DEBUG
		piLock (STD_IO_BUFFER_KEY);
		printf ("Unable to setup wiringPi\n");
		piUnlock (STD_IO_BUFFER_KEY);
		#endif
		return -1;
    }


	#ifdef DEBUG
	// Lanzamos thread para exploracion del teclado convencional del PC
	int x=0;
	x = piThreadCreate (thread_explora_teclado);

	if (x != 0) {
		piLock (STD_IO_BUFFER_KEY);
		printf ("it didn't start!!!\n");
		
		return -1;
    }
	#endif
	return 1;
}

//InicializaciÃ³n del autÃ³mata. Ponemos los flags a 0.
void fsm_setup(fsm_t* luz_fsm) {
	piLock (FLAGS_KEY);
	flags = 0;
	piUnlock (FLAGS_KEY);
	#ifdef DEBUG
	piLock (STD_IO_BUFFER_KEY);
	printf("Bienvenido a ArkanoPi, pulse para continuar\n");
	piUnlock (STD_IO_BUFFER_KEY);
	#endif
	mensajeInicial();
	#ifdef DEBUG
	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal((&juego.arkanoPi.pantalla));
	piUnlock (STD_IO_BUFFER_KEY);
	#endif

}

//------------------------------------------------------
// PI_THREAD (thread_explora_teclado): Thread function for keystrokes detection and interpretation
//------------------------------------------------------

PI_THREAD (thread_explora_teclado) {
	int teclaPulsada;

	while(1) {
		delay(60); // Wiring Pi function: pauses program execution for at least 10 ms

		piLock (STD_IO_BUFFER_KEY);

		if(kbhit()) {
			teclaPulsada = kbread();
			piLock (FLAGS_KEY);
			flags |= FLAG_TECLA; 		// Se activa el flag para indicar que se ha pulsado una tecla.
			piUnlock (FLAGS_KEY);
			switch(teclaPulsada) {		//Dependiendo de la tecla, realizaremos una función u otra.
			piLock (FLAGS_KEY);
			flags &= ~FLAG_TECLA;
			piUnlock (FLAGS_KEY);
			case 'i':
				piLock (FLAGS_KEY);
				flags |= FLAG_RAQUETA_IZQUIERDA;
				piUnlock (FLAGS_KEY);
				break;

			case 'o':
				piLock (FLAGS_KEY);
				flags |= FLAG_RAQUETA_DERECHA;
				piUnlock (FLAGS_KEY);
				break;

			case 'p':
				piLock (FLAGS_KEY);
				flags |=FLAG_PELOTA;
				piUnlock (FLAGS_KEY);
				break;

			case 'q':
				exit(0);
				break;

			case 'y':
				piLock (FLAGS_KEY);
				if(flags & FLAG_PAUSA){
					flags &= ~FLAG_PAUSA;

				}else{
					flags |=FLAG_PAUSA;
				}

				piUnlock (FLAGS_KEY);
				break;

			default:
				printf("INVALID KEY!!!\n");
				break;
			}
		}

		piUnlock (STD_IO_BUFFER_KEY);
	}
}

int main (){

	wiringPiSetupGpio ();  			//Configuración de los puertos GPIO necesarios

		// Interrupts setup
		wiringPiISR (PUL_DER, INT_EDGE_RISING, &pulsDerecha) ;
		wiringPiISR (PUL_IZQ, INT_EDGE_RISING, &pulsIzquierda);

		pinMode(0, OUTPUT);
		pinMode(1, OUTPUT);
		pinMode(2, OUTPUT);
		pinMode(3, OUTPUT);
		pinMode(4, OUTPUT);
		pinMode(7, OUTPUT);
		pinMode(14, OUTPUT);
		pinMode(17, OUTPUT);
		pinMode(18, OUTPUT);
		pinMode(22, OUTPUT);
		pinMode(23, OUTPUT);
		pinMode(24, OUTPUT);
		pinMode(25, OUTPUT);

		pinMode(16, INPUT);
		pinMode(19, INPUT);
		pinMode(17, OUTPUT);

		wiringPiSetup();
		//Aquí configuramos el display LCD. Utilizamos los pines definidos, además de 
		// utilizar el modo de 4 bits.
		lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
		lcdPosition(lcd, 0, 0);
		lcdPosition(lcd, 0, 1);
		lcdPuts(lcd, "-ArkanoPi-");

		//Aquí inicializamos la librería SoftTone. Con ella generaremos los tonos musicales
		// para que haya musica mientras jugamos. Utiliza el pin GPIO 17.
		softToneCreate(17);



	unsigned int next;
	//En este apartado inicializamos los 4 temporizadores necesarios para las interrupciones
	// periódicas: el del refresco de la matriz, el del temporizador de la pelota, el de evitar
	// la pulsación inncesaria de la pausa, y el de la reproducción de música.

	tmr = tmr_new (timer_isr);  //Inicialización y creación del temporizador de 1ms
	tmr_startms((tmr), CLK_1MS);

	tmr_pausa = tmr_new (pausa_isr);
	tmr_startms(tmr_pausa, 75);

	tmr_pelota= tmr_new (temporizadorPelota);
	tmr_startms((tmr_pelota), CLK_PELOTA);

	tmr_bso= tmr_new (superMusica);
	tmr_startms((tmr_bso), 200);


	// Maquina de estados: lista de transiciones
	// {EstadoOrigen,FunciÃ³nDeEntrada,EstadoDestino,FunciÃ³nDeSalida}
	fsm_trans_t estados_tabla[] = {
			{WAIT_START, CompruebaTeclaPulsada,  WAIT_PUSH, InicializaJuego},
			{WAIT_PUSH,   CompruebaTeclaPelota,  WAIT_PUSH, MovimientoPelota},
			{WAIT_PUSH,   CompruebaTeclaRaquetaIzquierda, WAIT_PUSH, MueveRaquetaIzquierda },
			{ WAIT_PUSH,   CompruebaTeclaRaquetaDerecha,  WAIT_PUSH, MueveRaquetaDerecha },
			{ WAIT_PUSH,   CompruebaTeclaPausa,  PAUSE, Pausa},
			{PAUSE, CompruebaTeclaPausa, WAIT_PUSH, Pausa    },
			{WAIT_PUSH,   CompruebaFinalJuego,  WAIT_END, FinalJuego },
			{WAIT_END,   CompruebaTeclaPulsada,  WAIT_START, ReseteaJuego },
			{ -1, NULL, -1, NULL },
	};
	fsm_t* arkanoPi_fsm = fsm_new (WAIT_START, estados_tabla, NULL);

	// Configuracion e inicializacion del sistema
	systemSetup();
	fsm_setup ((arkanoPi_fsm));

	next = millis();     		//Control de la máquina de estados.
	while (1) {
		fsm_fire (arkanoPi_fsm);
		next += CLK_MS;
		delay_until (next);
	}

	fsm_destroy (arkanoPi_fsm);

}

