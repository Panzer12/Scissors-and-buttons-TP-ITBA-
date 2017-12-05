#include <stdio.h>
#include "getnum/getnum.h"
#include "BackEnd.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#define NUMBER 0
#define LETTER 1
#define vs1 1
#define vsAI 2
#define Cargar_Partida 3
#define Terminar 4
#define DEL_BUFFER while( getchar()!='\n' );

int checkDato( char *cadena,juegoT *juego,Direcciontype *posInicial,Direcciontype *posFinal);
int menuOpcion();
char eleccion(char *cadena,juegoT *juego,Direcciontype *posInicial,Direcciontype *posFinal);
void ElegirDim(juegoT *juego);

void imprimirMatriz(juegoT* juego);
void imprimirLinea1(juegoT *juego);
void imprimirLinea(juegoT* juego,int fila);
void imprimirLineaInter(juegoT *juego);
void imprimirUltima(juegoT *juego);

int main(){
    int error=1;
    int opcion,i;
    int aumentoX;
    int aumentoY;
    int distancia;
    char cadena[42],c,siono;
    char cargarNombre[42];
    juegoT juego;
    Direcciontype posInicial;
    Direcciontype posFinal;
    srand (time(0));
    printf("Scissors and Buttons\n");
    juego.puntos.jugador1=0;
    juego.puntos.jugador2=0;
    opcion=menuOpcion();
    switch(opcion){
        case vs1  :  juego.TipoDeJuego=1;
                     ElegirDim(&juego);
                     juego.turno.jugador1=PrimerTurno(2);
                     if(juego.turno.jugador1==1){
                        juego.turno.jugador2=0;
                     }
                     else{
                        juego.turno.jugador2=1;
                     }
                     break;
        case vsAI :  juego.TipoDeJuego=2;
                     ElegirDim(&juego);
                     juego.turno.jugador1=1;
                     juego.turno.jugador2=0;
                     break;
        case Cargar_Partida :   printf("Escribir nombre del archivo a cargar: \n");

                                for (i = 0; (c = getchar()) != '\n' && i<35; i++)
                                    {
                                        cargarNombre[i]=c;
                                    }
                                cargarNombre[i]='\0';
                                //Chequea que el archivo ingresado exista.
                                error=cargarJuego (&juego, cargarNombre);
                                if ( error==0 )
                                {
                                    printf("No se pudo cargar el archivo\n");
                                    return 0;
                                }
            break;
        case Terminar : return 0;
    }
    do{
    if(error==1)
    imprimirMatriz( &juego );
    if(juego.turno.jugador1==1){
        printf("Turno Jugador1\n");
    }
    else if(juego.TipoDeJuego==2){
        printf("Turno Computadora\n");
    }else{
        printf("Turno Jugador2\n");
    }
    switch(eleccion(cadena,&juego,&posInicial,&posFinal)){
        case 'm':   if(juego.TipoDeJuego==2 && juego.turno.jugador2==1){
                        Computadora(&juego,&posInicial,&posFinal);
                        printf("[%d,%d]",posInicial.fildir,posInicial.coldir);
                        printf("[%d,%d]\n",posFinal.fildir,posFinal.coldir);
                    }
                        error=checkMove(&posInicial,&posFinal,&aumentoX,&aumentoY);
                        printf("%d\n",error);
                    if(error==1){
                        error=checkCorte(&juego,&posInicial,&posFinal,&aumentoX,&aumentoY,&distancia);
                     printf("%d\n",error);
                    }
                    if (error==1){
                        Corte(&juego,&posInicial,&posFinal,&aumentoX,&aumentoY,&distancia);
                        IntercambiarTurno(&juego);
                    }
                 break;
        case 't':   printf("\nEligio Terminar \n");
                    printf("Quiere guradar antes de salir y/n?");
                    scanf("%c",&siono);
                    //Verifica que la opcion elejida sea "y" o "n" unicamente
                    while ( (c=getchar())!= '\n' || (siono!='y' && siono!='n') ){
                        if (c!='\n'){
                            DEL_BUFFER
                        }
                        printf("Ingrese \"y\" o \"n\"\n");
                        scanf("%c",&siono);
                    }
                    if ( siono=='y' ){
                        printf("Ingrese el nombre del archivo: \n");
                        for (i = 0; (c = getchar()) != '\n' && i<35; i++){
                            cargarNombre[i]=c;
                        }
                        cargarNombre[i]='\0';
                        error=guardarJuego( &juego , cargarNombre );
                        if (error==0){
                            printf("no se pudo guardar el juego\n");
                        }
                        else{
                            printf("juego guardado con exito\n");
                        }
                    }
                    return 0;

        break;
                 break;
        case 'g':   error=guardarJuego( &juego , &(cadena[8]) );
                    //Verifica que haya memoria para guardar el archivo.
                    if (error==0)
                        printf("no se pudo guardar el juego\n");
                    else
                        printf("juego guardado con exito\n");
                 break;
        case 'e':   printf("Error al ingresar comando\n");
                    printf("Los comando que puede usar son:\n\n");
                    printf("* [num.fila,num.columna][num.fila,num.columna]\n");
                    printf("* guardar(minuscula) \"Nombre del archivo\" \n");
                    printf("* terminar(minuscula)\n");
                break;
    }
    }
    while(Ganador(&juego)==0);
    imprimirMatriz( &juego );
    if(juego.turno.jugador1==0){
        printf("Ganador Jugador1\n");
    }
    else if(juego.TipoDeJuego==1){
        printf("Ganador Jugador2\n");
    }
    else{
        printf("Mala suerte PERDISTE \n");
    }
return 1;
}

//--------------------------------------------------------------------------------------//

 int checkDato(char *cadena,juegoT *juego,Direcciontype *posInicial,Direcciontype *posFinal){      //Checkea los datos del movimiento devuelve 1 si esta OK
    int c;
    int number[4] = { 0, 0, 0, 0 };
    int i = 0, status = NUMBER, numberflag = 0, error = 1;

 /* Se´permite agregar tantos espacios como se desee antes del primer corchete */

    /* Máquina de estados para chequear el comando, empieza en NUMBER y
	** sigue en LETTER para volver a NUMBER y así sucesivamente */
    for(c=1;cadena[c]!='\0' && error == 1;c++) {
        switch (status){
            case NUMBER:    if (isdigit(cadena[c]) && i < 4){
                                number[i] = number[i] * 10 + cadena[c] - '0';
                                numberflag = 1;
                            }else if(( (cadena[c]==',' && (i%2)==0) || (cadena[c]==']' && ((i%2)==1 || i==4) ))&& numberflag == 1){
                                i++;
                                numberflag = 0;
                                if (cadena[c] == ']')
                                    status = LETTER;
                            }else{
                                error = 0;
                                
                            }
                            break;
            case LETTER:    if ( cadena[c]== '[')
                                status = NUMBER;
                            else{
                                error = 0;
                               
                            }
                            break;
        }
        if(number[i]>juego->Dim-1 && i<4){
            error=0;
            printf("%d\n",i);
            printf("%c\n",number[i]);
        }
    }
    if (status == NUMBER || i < 4){
        error = 0;
    printf("aa2\n");
    }
    posInicial->fildir=number[0];
    posInicial->coldir=number[1];
    posFinal->fildir=number[2];
    posFinal->coldir=number[3];
    
    return error;
}
//-----------------------------------------------------------------------------------------------//
int menuOpcion(){
     int menu;
     printf("\n1. Juego de dos jugadores\n");
     printf("2. Juego contra computadora\n");
     printf("3. Recuperar un juego guardado\n");
     printf("4. Terminar\n\n");

        menu=getint("Elija una opcion: ");     //Pide que se ingrese por pantalla una opcion.

     while ( menu > 4 || menu < 1 )
        {
         printf("Dato Incorrecto\n");
         menu=getint("Elija una opcion: ");
        }

     return menu;

    }
//----------------------------------------------------------------------------------------//
void imprimirMatriz(juegoT* juego){
int fil=0,col=0;
printf("\n\tPuntaje\t\tJugador1:%d",juego->puntos.jugador1);
if(juego->TipoDeJuego==1){
    printf("\tJugador2:%d\n",juego->puntos.jugador2);
}else{
    printf("\tComputadora:%d\n",juego->puntos.jugador2);
}
printf("\n     ");
for(col=0;col<(juego->Dim);col++){
    printf(" %d  ",col);
}
imprimirLinea1(juego);
for(fil=0;fil<(juego->Dim)-1;fil++)
    {
        imprimirLinea(juego,fil);
        imprimirLineaInter(juego);
    }

imprimirLinea(juego,fil);
imprimirUltima(juego);
}
//------------------------------------------------------------------------------------------//
void imprimirLinea1(juegoT *juego){
int fil;
printf("\n    ┌");
for(fil=0;fil<(juego->Dim)-1;fil++)
    {
        printf("───┬");
}
printf("───┐\n");
}
//*******************************************************************************************************
void imprimirLinea(juegoT* juego,int fila){
int col;
printf("  %d ",fila);
for(col=0;col<juego->Dim;col++){
        printf("│");
        if ( juego->matriz[fila][col]=='0' ){
                printf("   ");
            }
        else
            printf(" %c ", juego->matriz[fila][col]);

    }
printf("│\n");
}
//*******************************************************************************************************
void imprimirLineaInter(juegoT *juego){
    int fil;
    printf("    ├");
    for(fil=0;fil<(juego->Dim)-1;fil++){
        printf("───┼");
    }
printf("───┤\n");
}
//*******************************************************************************************************
void imprimirUltima(juegoT *juego){
    int fil;
    printf("    └");
    for(fil=0;fil<(juego->Dim)-1;fil++)
    {
        printf("───┴");
}
printf("───┘\n");
}
//-----------------------------------------------------------------------------------------------------//
void ElegirDim(juegoT *juego){
    int i;
    printf("Seleccione la dimencion de la matriz\n");
    do{
        juego->Dim=getint("Recuerde que debe ser mayor a 5 y menor a 30: ");
    }while(juego->Dim < 5 || juego->Dim > 30);
    juego->matriz=GenMatriz(juego);
    if ((i=(CargaMatriz(juego))==0)){
        printf("error al cargar la matriz no existe juego con esa dimencion\n");
        ElegirDim(juego);
    }
}
char eleccion(char *cadena,juegoT *juego,Direcciontype *posInicial,Direcciontype *posFinal){
    
    char c;
    int i=0;
    char *opciones[2]={"guardar ","terminar"}; 
       
    if(juego->TipoDeJuego==2&&juego->turno.jugador2==1){
        return 'm';
    }
    for(i=0; (c=getchar()) != '\n' && i<41 ;i++ ){
        cadena[i]=c;
       
    }
    cadena[i]='\0';
    if (cadena[0]=='['){
        if(checkDato(cadena,juego,posInicial,posFinal)==1){
            return 'm';
        }
    }
    if (cadena[0]=='g'){
        if ( strncmp(cadena,opciones[0],8)==0 && cadena[8]!='\0' ){  //compara si la cadena es guardar.
            return 'g';
        }
    }
    if(cadena[0]=='t'){
        if( strcmp(cadena,opciones[1])==0 ){  //chequea que la cadena sea igual a una de las opciones
            return 't';
        }

    }
    return 'e';
}

