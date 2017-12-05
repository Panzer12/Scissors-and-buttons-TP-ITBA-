#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "BackEnd.h"
/*Genero espacio para la matriz*/

char **GenMatriz(juegoT *juego){
    int col;
    juego->matriz = calloc( juego->Dim,sizeof(*juego->matriz));
      if (juego->matriz==NULL){
        free ( juego->matriz );
            return NULL;
      }
    for (col=0; col<juego->Dim ; col++){
        juego->matriz[col]=calloc( juego->Dim,sizeof(*juego->matriz));
        if (juego->matriz[col]==NULL){
             borrarmatriz(juego->matriz, col);
             return NULL;
        }
    }
    return juego->matriz;
}
//----------------------------------------------------------------------------------//
/*Borra matriz en caso de ser null*/
void borrarmatriz ( char **matriz, int col ){
    int fil;
    for (fil=0 ; fil<col ; fil++){
        free ( matriz[fil] );
    }
    free ( matriz );
}
//----------------------------------------------------------------------------------//
/*Carga la matriz de un archivo .txt*/
int CargaMatriz(juegoT *juego){
    int NumeroTablero,fil,i,j,z,Tablero;
    char NombreArchivo[10],c;
    int error=1;
    FILE *pArchivo;
    sprintf(NombreArchivo,"%dx%d",juego->Dim,juego->Dim);
    pArchivo=fopen(NombreArchivo,"r");
    if(pArchivo==NULL){
    error=0;   
    return error;
    }
    NumeroTablero=fgetc(pArchivo);
    NumeroTablero-='0';
    Tablero=randint(NumeroTablero);
    
    for(i=0;i<Tablero;i++){
            fgetc(pArchivo);
            fgetc(pArchivo);
            for(j=0;j<juego->Dim;j++){
                    for(z=0;z<juego->Dim;z++){
                        c=fgetc(pArchivo);
                        if (!(isalpha(c) || c==' ')){
                            error=0;
                            
                        }
                    }
                    c=fgetc(pArchivo);
                    c=fgetc(pArchivo); /*Consumo los enter*/
                    if(c!='\n'){
                      error=0;
                      
                    }
            }
            c=fgetc(pArchivo);
            if(c!='-'){  /*Comprueba que el texto este bien separado y los elementos sean validos*/
                error=0;

            }
    }
    if (error==0){
        return error;
    }
    i=(juego->Dim*(juego->Dim+2))*(Tablero)+(3*(Tablero+1));
    fseek ( pArchivo , i, SEEK_SET );
    for (fil=0;fil<juego->Dim;fil++){
        
          fgets(juego->matriz[fil],30,pArchivo); 
        }
    fgetc(pArchivo);
    fgetc(pArchivo);
 
   return error;
}
/* Revisa que la direccion a cortar sean  validas*/
int checkMove(Direcciontype *posInicial,Direcciontype *posFinal,int *aumentoX,int *aumentoY){
    int y,x;
    y=(posFinal->fildir)-(posInicial->fildir);
    x=(posFinal->coldir)-(posInicial->coldir);
    if(x==y && x>0){
        *aumentoX=1;
        *aumentoY=1;
    }
    else if(x==y && x<0){
        *aumentoX=-1;
        *aumentoY=-1;
    }
    else if(x==0 && y>0){   /* Asigno la direccion en caso que pertenezcan*/
        *aumentoX=0;        /* a alguna de las 8 coordenadas cartesianas*/
        *aumentoY=1;
    }
    else if(x==0 && y<0){
        *aumentoX=0;
        *aumentoY=-1;
    }
    else if(y==0 && x>0){
        *aumentoX=1;
        *aumentoY=0;
    }
    else if(y==0 && x<0){
        *aumentoX=-1;
        *aumentoY=0;
    }
    else if(x==-y && x>0){
        *aumentoX=1;
        *aumentoY=-1;
    }
    else if(x==-y && x<0){
        *aumentoX=-1;
        *aumentoY=1;
    }
    else{
        return 0;
    }
    return 1;
}
/* Se fija que en el corte sea de un mismo elemento */
int checkCorte(juegoT *juego,Direcciontype *posInicial,Direcciontype *posFinal,int *aumentoX,int *aumentoY,int *distancia){
    char primerLetra,letraSig;
    int i,error=1,y,x;
    y=(posFinal->fildir)-(posInicial->fildir);
    x=(posFinal->coldir)-(posInicial->coldir);
    if(x==0){
        *distancia=abs(y);
    }
    else{
        *distancia=abs(x);
    }
    primerLetra=juego->matriz[posInicial->fildir][posInicial->coldir];
    
    for(i=0;(i<(*distancia)+1 && error==1);i++){   /* Se fija que en una direccion los elementos sean iguales*/
        letraSig=juego->matriz[posInicial->fildir+((*aumentoY)*i)][posInicial->coldir+((*aumentoX)*i)]; /* o que el elemento sea un espacio  */
        
        if((letraSig!=primerLetra && letraSig!='0') || primerLetra=='0'){
            error=0;
        }
    }
    return error;
}
void Corte(juegoT *juego,Direcciontype *posInicial,Direcciontype *posFinal,int *aumentoX,int *aumentoY,int *distancia){
    int i,primerLetra,letraSig;
    primerLetra=juego->matriz[posInicial->fildir][posInicial->coldir];
    for(i=0;i<*distancia+1;i++){
        letraSig=juego->matriz[posInicial->fildir+((*aumentoY)*i)][posInicial->coldir+((*aumentoX)*i)];
        if(letraSig==primerLetra && juego->turno.jugador1==1){ /*Suma la cantidad de botones cortados al jugador que corresponda */
            juego->puntos.jugador1++;
        }
        else if(letraSig==primerLetra && juego->turno.jugador2==1){
            juego->puntos.jugador2++;
        }
        juego->matriz[posInicial->fildir+((*aumentoY)*i)][posInicial->coldir+((*aumentoX)*i)]='0';
    } /*Cambia los elementos cortados por un '0'*/
}
/*Intercambia los turnos de los jugadores*/
void IntercambiarTurno(juegoT *juego){
    int aux;
    aux=juego->turno.jugador1;
    juego->turno.jugador1=juego->turno.jugador2;
    juego->turno.jugador2= aux;
}
int PrimerTurno(int turnos){
        double random;
        random=rand()/(RAND_MAX+1.0);
        return random*turnos;
  }
/* Se fija si quedaron cortes disponibles */
int Ganador(juegoT *juego){
    int fil,col,hayCorte=1;
    for(fil=0;fil<juego->Dim && hayCorte;fil++){
        for(col=0;col<juego->Dim && hayCorte;col++){
            if(isalpha(juego->matriz[fil][col])){
                hayCorte=PosibleCorte(juego,fil,col);
            }
        }
    }
  return hayCorte;
}
/* Se fija en todas las direcciones si hay algun corte disponible */
int PosibleCorte(juegoT *juego,int fil,int col){
    int i,j,z,flag=1,haycorte=1;
    char primerLetra,letraSig='Z';
    primerLetra=juego->matriz[fil][col];
    for(i=-1;i<2&&haycorte;i++){
        for(j=-1;j<2&&haycorte;j++){
            if(j!=0 || i!=0){
                flag=1;
                for(z=1;z<juego->Dim && flag;z++){
                    if(fil+(i*z)>=0 && fil+(i*z)<juego->Dim && col+(j*z)>=0 && col+(j*z)<juego->Dim){
                        letraSig=juego->matriz[fil+(i*z)][col+(j*z)];
                        if(letraSig!=primerLetra && letraSig!='0'){
                            flag=0;
                        }
                        if(letraSig==primerLetra){
                            haycorte=0;
                            flag=0;
                        }
                    }
                }
            }
        }
    }
    return haycorte;
}
//*****************************************************************************************
/*se fija elemento por elemento los cortes maximos y minimos*/
void Computadora(juegoT *juego,Direcciontype *posInicial,Direcciontype *posFinal){
    int corteMax=0,corteMin;
    int fil,col,TipoCorte,corte;
    corte=randint(juego->Dim*juego->Dim);
    corteMin=juego->Dim+1;
    TipoCorte=randint(2);       /*Elijo el tipo de corte 1->corteMaximo 0->corteMinimo*/
    while(corte>0){
    for(fil=0;fil<juego->Dim;fil++){
        for(col=0;col<juego->Dim;col++){
            if(isalpha(juego->matriz[fil][col])){
                CorteComputadora(juego,fil,col,posInicial,posFinal,TipoCorte,&corteMax,&corteMin,&corte);
            }
        }
    }
}
}
void CorteComputadora(juegoT *juego,int fil,int col,Direcciontype *posInicial,Direcciontype *posFinal,int TipoCorte,int *corteMax,int *corteMin,int* corte){
    int i,j,z,flag=1,cantLetra=1,filFin,ColFin,filAux,ColAux;
    char primerLetra,letraSig='Z';
    primerLetra=juego->matriz[fil][col];
    for(i=-1;i<2;i++){
        for(j=-1;j<2;j++){
            if(j!=0 || i!=0){
                flag=1;
                cantLetra=1;
                for(z=1;z<juego->Dim && flag;z++){
                    filAux=fil+(i*z);
                    ColAux=col+(j*z);
                    if(filAux>=0 && filAux<juego->Dim && ColAux>=0 && ColAux<juego->Dim){
                        letraSig=juego->matriz[filAux][ColAux];
                        if(letraSig!=primerLetra && letraSig!='0'){
                            flag=0;
                        }
                        if(letraSig==primerLetra){
                            cantLetra++;
                            filFin=fil+(i*z);
                            ColFin=col+(j*z);
                        }
                    }else{
                        flag=0;
                    }
                }
                if(TipoCorte==1){
                    if(cantLetra>=*corteMax && cantLetra>=2){
                        (*corte)--;
                        *corteMax=cantLetra;
                        if(*corte>0){
                        posInicial->coldir=col; // Cargo las dos posiciones para cortar
                        posInicial->fildir=fil;
                        posFinal->coldir=ColFin;
                        posFinal->fildir=filFin;}
                    }
                }
                else{
                    if(cantLetra<=*corteMin && cantLetra>=2){
                        *corteMin=cantLetra;
                        (*corte)--;
                        if(*corte>0){
                        posInicial->coldir=col;
                        posInicial->fildir=fil;
                        posFinal->coldir=ColFin;
                        posFinal->fildir=filFin;}
                    }
                }
            }
        }
    }
    return ;
}
/*Guarda los datos del juego*/
int guardarJuego(juegoT *juego, const char *nombrearchivo)
{
  int turno,i,j;
  FILE *archivo;
  archivo=fopen(nombrearchivo,"wb");
  if(archivo==NULL)
    return 0;
else{
   fseek(archivo,0,SEEK_SET);
   fwrite(&juego->TipoDeJuego,sizeof(int),1,archivo);
   if(juego->turno.jugador1==1)
   turno=juego->turno.jugador1==1;
   else
   turno=2;
   fwrite(&turno,sizeof(int),1,archivo);
   fwrite(&juego->Dim,sizeof(int),1,archivo);
   for(i = 0; i < juego->Dim; i++){
            for(j = 0;j<juego->Dim; j++){
                fwrite(&(juego->matriz[i][j]), sizeof(char), 1, archivo);
            }
     }

   fclose(archivo);
  }
  return 1;
}
/*Carga los datos del juego*/
int cargarJuego(juegoT *juego,const char *nombrearchivo)
{
    int turno,i,j;
    FILE *archivo;
    archivo=fopen(nombrearchivo,"rb");
    char aux;
    if (archivo==NULL)
    {
        return 0;
    }
   else
   {
    fread(&juego->TipoDeJuego,sizeof(int),1,archivo);
    fread(&turno,sizeof(int),1,archivo);
    if (turno==1){
        juego->turno.jugador1=1;
        juego->turno.jugador2=0;
      }
    else {
        juego->turno.jugador2=1;
        juego->turno.jugador1=0;
      }
    fread(&juego->Dim,sizeof(int),1,archivo);
    juego->matriz=GenMatriz(juego);
    for(i=0;i<juego->Dim;i++){
        for(j=0;j<juego->Dim;j++){
        fread(&aux,sizeof(char),1,archivo);
        if (aux=='0')
            juego->matriz[i][j]='0';
        else juego->matriz[i][j]=aux;
        }
    }
   }
   return 1;
}

int randint(int a){
    return rand()%a;
}


/* Checkea que los datos sean validos */

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
           
        }
    }
    if (status == NUMBER || i < 4){
        error = 0;
   
    }
    posInicial->fildir=number[0];
    posInicial->coldir=number[1];
    posFinal->fildir=number[2];
    posFinal->coldir=number[3];
    
    return error;
}