#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int fildir;
    int coldir;
} Direcciontype;

typedef struct{
    int jugador1;
    int jugador2;
}TurnoT;

typedef struct{
    int jugador1;
    int jugador2;
}puntajeT;

 typedef struct{
        char **matriz;
        int jugador;
        int TipoDeJuego;
        int Dim;
        TurnoT turno;
        puntajeT puntos;
        char *DatosMatriz;
    } juegoT;
int PrimerTurno();
void IntercambiarTurno(juegoT *juego);
char **GenMatriz(juegoT *juego);
void borrarmatriz ( char **matriz, int col );
int CargaMatriz(juegoT *juego);
int checkMove(Direcciontype *posInicial,Direcciontype *posFinal,int *aumentoX,int *aumentoY);
int checkCorte(juegoT *juego,Direcciontype *posInicial,Direcciontype *posFinal,int *aumentoX,int *aumentoY,int *distancia);
void Corte(juegoT *juego,Direcciontype *posInicial,Direcciontype *posFinal,int *aumentoX,int *aumentoY,int *distancia);
int Ganador(juegoT *juego);
int PosibleCorte(juegoT *juego,int fil,int col);
void Computadora(juegoT *juego,Direcciontype *posInicial,Direcciontype *posFinal);
void CorteComputadora(juegoT *juego,int fil,int col,Direcciontype *posInicial,Direcciontype *posFinal,int TipoCorte,int *corteMax,int *corteMin,int *corte);
int guardarJuego (juegoT * juego, const char  *nombrearchivo);
int cargarJuego(juegoT *juego, const char  *cargarNombre);
