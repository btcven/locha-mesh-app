#ifndef SIMULACION_H
#define SIMULACION_H

#include <algorithm>
#include <random>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ctime>    

using namespace std;






//-------------------------------------------------------------------
// funciones necesarias para la simulacion, no requeridas en locha
//-------------------------------------------------------------------

std::string get_current_datetime(){

//	errno_t err;
struct tm time_info;
time_t time_create = time(NULL);
localtime_s(&time_info, &time_create);
//char timebuf[26];
//err = asctime_s(timebuf, 26, &time_info);
std::stringstream buffer;

buffer << std::put_time(&time_info, "%d-%m-%Y %H:%M:%S");



    std::string  str;
	str = buffer.str(); 
	
	return str;
}

uint8_t escribe_archivo(std::string file_name, std::string mensaje){
	
ofstream outfile;
   outfile.open(file_name);
   if(outfile.fail()){
	    return 1;
   }
   if(!outfile.is_open()){
	    return 1;
   }

   outfile  << mensaje << endl;
    outfile.close();
    return 0;
}

std::string leer_archivo(std::string file_name){
 ifstream infile; 
 std::string data; 
   infile.open(file_name); 
   if(infile.fail()){
	    return "";
   }
   infile >> data; 
    infile.close();
    return data;
}


#endif // SIMULACION_H