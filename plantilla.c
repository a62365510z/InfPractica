#include <stdio.h>
#include <stdlib.h>

/** Primero definimos las constantes o variables globales **/
// Número máximo de cuentas
#define MAX_CUENTAS 1000

// Código de la cuenta del banco
#define COD_BANCO 0

// Las cuentas sin usar tienen en código de cliente este valor:
#define LIBRE -1

// Ruta al archivo de datos
#define DATOS "cuentas.txt"

// Comisión a cobrar por cada transferencia (excepto entre cuentas propias)
#define COMISION 0.20

// Límite máximo de descubierto permitido al realizar una transferencia
// (sin tener en cuenta la comisión por la transferencia)
#define MAX_DESCUBIERTO 100.00

// Porcentaje (en tanto por cien) a cobrar sobre descubiertos
#define INTERES 10.0


/** Luego definimos los prototipos de las funciones **/
// Implementar función menu


// Implementar función leer_de_fichero


// Implementar función escribir_a_fichero


// Implementar función saldo_cuenta


// Implementar función transferencia


// Implementar función saldo_global


// Implementar función cobrar_intereses



int main()
{
  int opc,err;
  int cod[MAX_CUENTAS]; // código cliente
  float saldo[MAX_CUENTAS], total;
  int imin,imax;

  err = leer_de_fichero(cod,saldo);
  if ( err != 0 ) {

    printf("\nError en la lectura del archivo de datos.\n\n");
    system("pause");

  } else {

    do {

      opc = menu();
      
      switch ( opc ) {
        
        case 1:
          saldo_cuenta(cod,saldo);
        break;
        
        case 2:
          transferencia(cod,saldo);
        break;
        
        case 3:
          total = saldo_global(cod,saldo,&imin,&imax);
          printf("\nEn el banco hay ahora %.2f euros.\n",total);
          printf("La cuenta con menos dinero (%.2f euros) es la %03d y pertenece a %03d.\n",
            saldo[imin],imin,cod[imin]);
          printf("La cuenta con mas dinero (%.2f euros) es la %03d y pertenece a %03d.\n",
            saldo[imax],imax,cod[imax]);
        break;
        
        case 4:
          cobrar_intereses(cod,saldo);
        break;
        
      }
      
    } while ( opc != 0 );
    
    err = escribir_a_fichero(cod,saldo);
    if ( err != 0 ) {
      printf("\nError en la escritura del archivo de datos.\n\n");
      system("pause");
    }
  }
  return 0;
}


/** Al final implementamos las funciones prototipieadas al principio**/

