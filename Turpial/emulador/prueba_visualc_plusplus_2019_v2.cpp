// prueba_visualc_plusplus_2019_v2.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <thread>
#include <iomanip>
#include <ctime>

#include "routing.h"
#include "lora.h"

#include <windows.h>  // solo si va a correr en Windows, para linux deve comentarlo

using namespace std;

#define RAD_ENABLED true
#define RADIOTYPE "LORA" 


// definiciones de variables globales

std::string latitud;
	std::string longitud;
	std::string potencia;
	std::string RSSI;
	std::string SNR;

packet_t Buffer_packet; // Packet struct for buffering
// tables
neighbor_entry_t neighborTable[255];
int neighborEntry = 0;
route_to_node_t routeTable[255];
int routeEntry = 0;
std::string listado_nodos = "nodos.txt";
std::string listado_mensajes = "mensajes.txt";

// clock since reset
clock_t start = clock();

std::string id_nodo;

//-------------------------------------------------------------------
// funciones necesarias para la simulacion, no requeridas en locha
//-------------------------------------------------------------------
void pos(short C, short R)
{
    COORD xy ;
    xy.X = C ;
    xy.Y = R ;
    SetConsoleCursorPosition( 
    GetStdHandle(STD_OUTPUT_HANDLE), xy);
}
void cls( )
{
    pos(0,0);
    for(int j=0;j<30;j++)
    cout << string(100, ' ');
    pos(0,0);
} 

std::string randStr( size_t length ) {
      srand(time(NULL));  //generate a seed by using the current time
      char str2[16];

      str2[length-1] = '\0';
      size_t i = 0;
      int r;

      for(i = 0; i < length-1; ++i) {
        for(;;) {
          r = rand() % 57 + 65; //interval between 65 ('A') and 65+57=122 ('z')
          if((r >= 65 && r <= 90) || (r >= 97 && r <= 122)) { // exclude '[' to '`'
            str2[i] = (char)r;
            break;
          }
        }
      }
	  
	 
	// string str(str2);
	 
      return std::string(str2);
    }


// imprime la tabla de vecinos
void printNeighborTable(){
    std::cout <<  "\n";
    std::cout <<  "Neighbor Table:";
	
	 int i;
    for( i = 0 ; i < neighborEntry ; i++){
        std::cout <<  neighborTable[i].id;
        std::cout << neighborTable[i].hops_away;
        std::cout <<  neighborTable[i].age;
        // TODO poder visualizar los servicios del vecino y la parte de calidad del vinculo
        std::cout << "\n";
    }
    std::cout << "\n total Neighbors: ";
	std::cout << neighborEntry;
    std::cout << "\n";
}

// imprime la tabla de rutas
void printRoutingTable(){
    std::cout << "\n";
    std::cout << "Routing Table:";
	
    for( int i = 0 ; i < routeEntry ; i++){
        std::cout << "% destination: ";
		std::cout << routeTable[i].receiver.id;
        std::cout << " via ";
        std::cout << routeTable[i].next_neighbor.id;
        std::cout << " age ";
		std::cout << routeTable[i].age;
        std::cout << "\n";
    }
	std::cout << "\n total routes: ";
	std::cout << routeEntry;
    std::cout << "\n";
}



std::string create_unique_id()
{
	return randStr(16);
}

void show_data(std::string id_nodo){
	 pos(0,0);
	std::cout << "ID de nodo:\n";

std::cout << id_nodo;
std::cout << "\n";

printNeighborTable();

std::cout << "\n";

 printRoutingTable();
std::cout << "\n";

std::cout << "Espacio ocupado en memoria:\n";
std::cout << "neighborTable: ";
std::cout << sizeof(neighborTable);
std::cout << " bytes\n";
std::cout << "routeTable: ";
std::cout << sizeof(routeTable);
std::cout << " bytes\n";
	



}

void clear_console(){
	printf("\033c");
	// clean the screen
    cls();  // solo para windows
}

void fn()
{
	int xx = 0;
    for(;;)
   {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        xx++;
		if (xx=30){
			scan_radio(id_nodo);
		}
   }
}

// para simular la seccion setup de arduino
void setup_inicial(){
	  id_nodo=create_unique_id();
	   startup_radio();
      start_radio();
	  scan_radio(id_nodo);
	  // emular los task asincronicos cada x segundos de la librerya TaskManager de arduino
	   std::thread(fn).detach();
}

int main(int nNumberofArgs, char* pszArgs[])
{
	std::string nMonth;


	// se verifican los parametros recibidos
	
	if (nNumberofArgs <1) {
		std::cout << " No se recibieron parametros por la linea de comando\n";
		return 0;
	}


	std::string destination_mcu;
	std::string memory_mcu;
    for (int i = 1; i < nNumberofArgs; ++i) {
        if (std::string(pszArgs[i]) == "-d") {
            if (i + 1 <= nNumberofArgs) { // Make sure we aren't at the end of argv!
                destination_mcu = pszArgs[i+1]; // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "-d (destination_mcu) option requires one argument." << std::endl;
				  system ("PAUSE");
                return 1;
            }  
        } 
		if (std::string(pszArgs[i]) == "-m") {
            if (i + 1 <= nNumberofArgs) { // Make sure we aren't at the end of argv!
                memory_mcu = pszArgs[i+1]; // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "-m (memory_mcu) option requires one argument." << std::endl;
				  system ("PAUSE");
                return 1;
            }  
        }
    }

	// fin de los parametros recibidos
	if (destination_mcu==""){
		// no se recibio un mcu destino
		std::cout << " No se recibio el mcu destino , debe colocar el paremetro -d NOMBRE_DEL_MCU en la linea de comandos\n";
		system ("PAUSE");
		return 0;
	} else {
		std::cout << " MCU:";
		std::cout << destination_mcu;
	}

	if (memory_mcu==""){
		// no se recibio un mcu destino
		std::cout << " No se recibio la cantidad de memoria del mcu destino , debe colocar el paremetro -m ESPACIO_de_MEMORIA_EN_KB en la linea de comandos\n";
		system ("PAUSE");
		return 0;
	} else {
		std::cout << " Memory MCU:";
		std::cout << memory_mcu;
	}

	setup_inicial();
	id_nodo = create_unique_id();
	
	while(true) {

		//Sleep(2000);
		std::this_thread::sleep_for(chrono::seconds(2));
	//	clear_console();
		show_data(id_nodo);

//getline(cin, nMonth);

   
    if (nMonth=="exit"){
	break;
}
	}
	//system ("PAUSE");
return 0;
}
