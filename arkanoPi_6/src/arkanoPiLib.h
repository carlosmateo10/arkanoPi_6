/** arkanoPiLib.h
*/

#ifndef _ARKANOPILIB_H_
#define _ARKANOPILIB_H_

#include <stdio.h>

// CONSTANTES DEL JUEGO
#define MATRIZ_ANCHO 	10
#define MATRIZ_ALTO 	7
#define LADRILLOS_ANCHO 10
#define LADRILLOS_ALTO 	2
#define RAQUETA_ANCHO 		3
#define RAQUETA_ALTO 		1

typedef struct {
	// Posicion
	int x;
	int y;
	// Forma
	int ancho;
	int alto;
} volatile tipo_raqueta;

typedef struct {
	// Posicion
	int x;
	int y;
	// Trayectoria
	int xv;
	int yv;
} volatile tipo_pelota;

typedef struct {
	// Matriz de ocupación de las distintas posiciones que conforman el display
	// (correspondiente al estado encendido/apagado de cada uno de los leds)
	int matriz[MATRIZ_ANCHO][MATRIZ_ALTO];
} volatile tipo_pantalla;

typedef struct {
  tipo_pantalla ladrillos; // Notese que, por simplicidad, los ladrillos comparten tipo con la pantalla
  tipo_pantalla pantalla;
  tipo_raqueta raqueta;
  tipo_pelota pelota;
} volatile tipo_arkanoPi;

extern tipo_pantalla pantalla_inicial;

//------------------------------------------------------
// FUNCIONES DE INICIALIZACION / RESET
//------------------------------------------------------
void ReseteaMatriz(tipo_pantalla *p_pantalla);
void ReseteaLadrillos(tipo_pantalla *p_ladrillos);
void ReseteaPelota(tipo_pelota *p_pelota);
void ReseteaRaqueta(tipo_raqueta *p_raqueta);

//------------------------------------------------------
// FUNCIONES DE VISUALIZACION (ACTUALIZACION DEL OBJETO PANTALLA QUE LUEGO USARA EL DISPLAY)
//------------------------------------------------------
void PintaMensajeInicialPantalla (tipo_pantalla *p_pantalla);
void PintaMensajePausa (tipo_pantalla *p_pantalla);
void PintaPantallaPorTerminal (tipo_pantalla *p_pantalla);
void PintaLadrillos(volatile tipo_pantalla *p_ladrillos, volatile tipo_pantalla *p_pantalla);
void PintaRaqueta(volatile tipo_raqueta *p_raqueta, volatile tipo_pantalla *p_pantalla);
void PintaPelota(volatile tipo_pelota *p_pelota, volatile tipo_pantalla *p_pantalla);
void ActualizaPantalla(volatile tipo_arkanoPi* p_arkanoPi);
void CopiarPantallaAMatriz (tipo_pantalla *p, int matrizDes[10][7]);

void InicializaArkanoPi(tipo_arkanoPi *p_arkanoPi);
int CalculaLadrillosRestantes(tipo_pantalla *p_ladrillos);

#endif /* _ARKANOPILIB_H_ */
