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
void saldo_cuenta(int cod[], float saldo[]);

// Implementar función transferencia
void transferencia(int cod[], float saldo[]);

// Implementar función saldo_global
float saldo_global(int cod[], float saldo[], int *imin, int *imax);

// Implementar función cobrar_intereses
void cobrar_intereses(int cod[], float saldo[]);

void SystemPause();
int pide_codigo();

/** Función main, llamada como primera función por el SO **/
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
          saldo_cuenta(cod, saldo);
        break;
        
        case 2:
          transferencia(cod, saldo);
        break;
        
        case 3:
          total = saldo_global(cod, saldo, &imin, &imax);
          printf("\nEn el banco hay ahora %.2f euros.\n", total);
          printf("La cuenta con menos dinero (%.2f euros) es la %03d y pertenece a %03d.\n",
            saldo[imin], imin, cod[imin]);
          printf("La cuenta con mas dinero (%.2f euros) es la %03d y pertenece a %03d.\n",
            saldo[imax], imax, cod[imax]);
          printf("\n");
        break;
        
        case 4:
          cobrar_intereses(cod, saldo);
        break;
        
      }
      
    } while ( opc != 0 );
    
    err = escribir_a_fichero(cod, saldo);
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
	int opcion;
	do{
		printf ("BANCO CO \n");
		printf ("----------\n");
		printf (" 0.Salir.\n");
		printf (" 1.Consultar saldo de una cuenta.\n");
		printf (" 2.Realizar transferencia\n");
		printf (" 3.Saldo global del banco.\n");
		printf (" 4.Cobrar intereses.\n\n");
		
		printf ("Elija opcion: ");
		scanf ("%d", &opcion);
		
		if (opcion>4 || opcion<0) {
			printf ("Opcion incorrecta. Por favor, indique una opcion válida.\n\n");
		}
	}while(opcion>4 || opcion<0);
	return opcion;
}

/* Funcion leer_de_fichero.
 * Esta función recoge los valores del archivo de cuentas y los devuelve por
 * referencia como vectores cod y saldo
 *	- Parámetros de entrada: Vectores de códigos de clientes y de saldos.
 *  - Valor de retorno: Error al leer un fichero.
 */
int leer_de_fichero(int cod[], float saldo[]){
  FILE *cuentas;
  cuentas = fopen (DATOS, "r" );
  if (cuentas == NULL) return 1;
  for(int i=0; fscanf(cuentas, "%d %f", &cod[i], &saldo[i]) != EOF; i++);
  fclose (cuentas);
  return 0;
}

/* Funcion escribir_a_fichero.
 * Esta función guarda los valores de los vectores en el archivo de cuentas
 *	- Parámetros de entrada: Vectores de códigos de clientes y de saldos.
 *  - Valor de retorno: Error al abrir un fichero.
 */
int escribir_a_fichero(int cod[], float saldo[]){
  FILE *cuentas;
  cuentas = fopen (DATOS, "w" );
  if (cuentas == NULL) return 1;
  for(int i=0; i < MAX_CUENTAS ; i++){
    fprintf(cuentas, "%d %.2f\r\n",cod[i], saldo[i]) ;
  }
  fclose (cuentas);
  return 0;
}


/* Funcion saldo_cuenta.
 * Saldo de una cuenta. Se pregunta al usuario por un código de cuenta, verificando
 * que está en el rango válido, y se muestra su saldo (indicando si es positivo o
 * negativo) o un aviso de que esa cuenta está sin usar.
 * - Parámetros de entrada: Vectores de códigos de clientes y de saldos.
 * - Valor de retorno: Ninguno.
 */
void saldo_cuenta(int cod[], float saldo[]){
  int indice;
  indice = pide_codigo();
  
  printf("\nLa cuenta con codigo %03d ", indice);
  if(cod[indice] == LIBRE){
    printf("esta actualmente sin usar.\n\n");
  }else{
    printf("pertenece al cliente con codigo %d.\n", cod[indice]);
    printf("Su saldo es de %.2f euros.\n\n", saldo[indice]);
  }
  //SystemPause();
}


/* Funcion transferencia.
 * Realizar transferencia. Se pregunta por dos códigos de cuenta, que deberán ser
 * válidos y pertenecientes a cuentas en uso, y una cantidad que se tratará de
 * transferir de una cuenta a la otra. El banco permite pequeños descubiertos en las
 * cuentas, de como máximo el valor de la constante MAX_DESCUBIERTO (que por
 * defecto vale 100). Si se va a superar ese descubierto, no se realiza la transferencia,
 * informando al usuario. Si no se supera, se realiza la transferencia restando la
 * cantidad indicada de la primera cuenta y sumándosela a la segunda. Si el dueño de
 * ambas cuentas no coincide, se cobrará una comisión de 20 céntimos (el valor de la
 * constante COMISION ) a la cuenta origen de la transferencia, ingresando esa
 * cantidad en la cuenta del banco (código COD_BANCO ). Si ambas cuentas son del
 * mismo cliente, se considera un traspaso entre sus cuentas y no se cobra comisión.
 *
 * - Parámetros de entrada/salida: Vectores de códigos de clientes y de saldos.
 * - Valor de retorno: Ninguno.
 *
 */
void transferencia(int cod[], float saldo[]){
  int indice_o, indice_d;
  float cantidad = 0;
  indice_o = pide_codigo();
  printf("ORIGEN: Cuenta %03d de cliente %d. Saldo actual: %.2f\n", indice_o, cod[indice_o], saldo[indice_o]);
  indice_d = pide_codigo();
  printf("Destino: Cuenta %03d de cliente %d. Saldo actual: %.2f\n", indice_d, cod[indice_d], saldo[indice_d]);
  
  do{
    printf ("Cantidad a transferir: ");
    scanf ("%f", &cantidad);
    if(cantidad <= 0){
      printf("Debe indicar una cantidad positiva.\n\n");
    }
  }while(cantidad <= 0);
  
  if( (saldo[indice_o] - cantidad) < (-MAX_DESCUBIERTO) ){
    printf("La transferencia no se realiza por falta de fondos.\n\n");
  }else{
    saldo[indice_o]  -= cantidad;
    saldo[indice_d]  += cantidad;
    
    if(cod[indice_o] != cod[indice_d]){
      saldo[indice_o]  -= COMISION;
      saldo[COD_BANCO] += COMISION;
    }
    
    printf("Transferencia realizada satisfactoriamente.\n\n");
  }
}


/* Funcion saldo_global.
 * Saldo global del banco. Se mostrará por pantalla la suma de todos los saldos de
 * las cuentas en uso, que es el total de dinero que hay en el banco. También se
 * indicará el número y saldo de la cuenta con menos dinero y de la cuenta con más
 * dinero.
 *
 * - Parámetros de entrada: Vectores de códigos de clientes y de saldos.
 * - Parámetros de entrada/salida: Los códigos de las cuentas con menor y
 * mayor saldo.
 * - Valor de retorno: Número real. Saldo total sumando los saldos de todas las
 * cuentas. Es conveniente no incluir las cuentas no usadas, por si alguna
 * tiene un saldo no nulo.
 */
float saldo_global(int cod[], float saldo[], int *imin, int *imax){
  float min, max = 0, total = 0;
  
  for(int i=0; i < MAX_CUENTAS; i++){
    if(cod[i] != LIBRE){
      total += saldo[i];
      if(saldo[i] > max){
        max = saldo[i];
        *imax = i;
      }
    }
  }
  min = max;
  for(int i=0; i < MAX_CUENTAS; i++){
    if(cod[i] != LIBRE){
      if(saldo[i] < min){
        min = saldo[i];
        *imin = i;
      }
    }
  }
  return total;
}


/* Funcion cobrar_intereses.
 * Cobro de intereses en las cuentas deudoras. Se cargará en todas las cuentas con
 * saldo negativo un interés del 10% (definido en la constante INTERES )
 * proporcional a la cantidad adeudada, ingresando estos valores en la cuenta del
 * banco.

 * - Parámetros de entrada/salida: Vectores de códigos de clientes y de saldos.
 * - Valor de retorno: Ninguno.
 */
void cobrar_intereses(int cod[], float saldo[]){
  float recaudado = 0, recaudo = 0;
  
  for(int i=1; i < MAX_CUENTAS; i++){
    if(cod[i] != LIBRE){
      if(saldo[i] < 0){
        recaudo = (-saldo[i]) / INTERES;
        saldo[i] -= recaudo;
        recaudado += recaudo;
        saldo[COD_BANCO] += recaudo;
      }
    }
  }
  printf("El banco ha recaudado %.2f euros.\n\n", recaudado);
}

int pide_codigo(){
  int indice;
  do{
    printf("Codigo de la cuenta a consultar (0-%d): ", MAX_CUENTAS-1);
    scanf ("%d", &indice);
    if(indice < 0 || indice >= MAX_CUENTAS){
      printf("\nCodigo de cuenta no aceptado. Repita, por favor.\n\n");
    }
  }while(indice < 0 || indice >= MAX_CUENTAS);
  return indice;
}
