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
int menu();

// Implementar función leer_de_fichero
int leer_de_fichero(int cod[], float saldo[]);

// Implementar función escribir_a_fichero
int escribir_a_fichero(int cod[], float saldo[]);

// Implementar función saldo_cuenta


// Implementar función transferencia


// Implementar función saldo_global


// Implementar función cobrar_intereses

void SystemPause();

int main()
{
  int opc, err, imin, imax;
  int cod[MAX_CUENTAS]; // códigos clientes
  float saldo[MAX_CUENTAS]; //saldos clientes
  float total;

  err = leer_de_fichero(cod, saldo);
  if ( err != 0 ) {
    printf("\nError en la lectura del archivo de datos.\n\n");
    SystemPause();
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
      SystemPause();
    }
  }
  return 0;
}


/** Al final implementamos las funciones prototipieadas al principio**/

/* Funcion SystemPause.
 * Esta función emula un system("pause") haciendolo compatible con otros SO
 *	Argumentos:
 *  Retorno:
 */
void SystemPause(){
  char ch;
  while ((ch = getchar()) != '\n' && ch != EOF); //Vaciamos el buffer existente.
  printf ("Presione Intro para continuar...");
  getchar ();
}

/* Funcion menu.
 * Esta función muestra un menú y pide la introducción de una opción correcta
 *	Argumentos:
 *  Retorno: Valor introducido por el ususario.
 */
int menu(){
	int opcion = -1;
	while (opcion>4 || opcion<0) {
		printf ("BANCO CO \n");
		printf ("----------\n");
		printf (" 0.Salir.\n");
		printf (" 1.Consultar saldo de una cuenta.\n");
		printf (" 2.Realizar transferencia\n");
		printf (" 3.Saldo global del banco.\n");
		printf (" 4.Cobrar intereses.\n");
		
		printf ("Elija opcion:\n");
		scanf ("%d", &opcion);
		
		if (opcion>4 || opcion<0) {
			printf ("Opcion incorrecta. Por favor, indique una opcion válida.");
		}
	}
	return opcion;
}

/* Funcion leer_de_fichero.
 * Esta función recoge los valores del archivo de cuentas y los devuelve por
 * referencia como vectores cod y saldo
 *	Argumentos: vector de ints cod, vector de ints saldo
 *  Retorno: Error al leer un fichero.
 */
int leer_de_fichero(int cod[], float saldo[]){
  FILE *cuentas;
  cuentas = fopen (DATOS, "r" );
  if (cuentas == NULL) return -1;
  for(int i=0; fscanf(cuentas, "%d %f", &cod[i], &saldo[i]) != EOF; i++);
  fclose (cuentas);
  return 0;
}

/* Funcion escribir_a_fichero.
 * Esta función guarda los valores de los vectores en el archivo de cuentas
 *	Argumentos: vector de ints cod, vector de ints saldo
 *  Retorno: Error al abrir un fichero.
 */
int escribir_a_fichero(int cod[], float saldo[]){
  FILE *cuentas;
  cuentas = fopen (DATOS, "w" );
  if (cuentas == NULL) return -1;
  for(int i=0; i < MAX_CUENTAS ; i++){
    fprintf(cuentas, "%d %.2f\r\n",cod[i], saldo[i]) ;
  }
  fclose (cuentas);
  return 0;
}