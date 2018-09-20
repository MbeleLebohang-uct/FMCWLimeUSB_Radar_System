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

//Channel config params
float center_freq_rx = 500e6;		// 500 MHz
float center_freq_tx = 500e6;		// 500 MHz
float gain_rx		 = 1;			// 1 = max gain = 73 dB
float gain_tx		 = 1;
double sampling_rate = 5e6;

uint16_t fifosize = 1024;
float throughputVsLatency = 0.5;
float f_start = 20e6;
float f_sweep = f_start + bandwidth;
float t_cpi = 10;
	

int main(int argc, char** argv){
	cout << "-------Raptor Watch FMCW Radar System-------" << endl;
	LimeFMCW RaptorWatch;
        
	RaptorWatch.configLimeChannels(center_freq_rx,center_freq_tx, gain_rx,gain_tx,sampling_rate);

	/*RaptorWatch.setRFBandwidth(bandwidth);

	RaptorWatch.configSystemStreams(fifosize,throughputVsLatency, f_start, f_sweep, t_cpi);*/
	

	// Run a transmit thread separately
    RaptorWatch.lime_device_running = true;
    std::thread thread = std::thread(RaptorWatch.threadTest);
    
    cout << "Press <enter> to stop stransmitting..." << endl;
    cin.ignore();

    RaptorWatch.lime_device_running = false;
    thread.join();
	//RaptorWatch.stopFMCWTransmit();
	cout << "------------------D O N E------------------" << endl;
    return 0;
}
