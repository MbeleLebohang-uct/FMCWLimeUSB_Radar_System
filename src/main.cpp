/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: MBELE
 *
 * Created on 10 May 2017, 22:36
 */

#include <iostream>
#include <lime/LimeSuite.h>
#include <thread>
#include <chrono>
#include <math.h>

#include "LimeFMCW.h"

using namespace std;
using namespace MBLLEB006;
/*
 * 
 */


lms_device_t* device;
const double sampleRate = 5e6;
bool running;
const int tx_size = 1024*16;
float tx_buffer[2*tx_size];
float start_freq = 10.0;
float bandwidth = 50000000.0;

int main(int argc, char** argv){
	cout << "-------Raptor Watch FMCW Radar System-------" << endl;
	LimeFMCW RaptorWatch;

	RaptorWatch.stopFMCWTransmit();
	cout << "------------------D O N E------------------" << endl;
    return 0;
}
