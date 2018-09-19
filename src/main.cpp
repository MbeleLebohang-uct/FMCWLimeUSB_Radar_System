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

int error()
{
  cout << "ERROR: " << LMS_GetLastErrorMessage() << endl;
  LMS_Close(device);
  exit(-1);
}

void StreamTest(){
    lms_stream_t tx_stream;
    tx_stream.channel = 0;
    tx_stream.fifoSize = 256*1024;
    tx_stream.throughputVsLatency = 0.5;
    tx_stream.dataFmt = lms_stream_t::LMS_FMT_F32;
    tx_stream.isTx = true;

    lms_stream_meta_t meta_tx;   
    meta_tx.waitForTimestamp = false;
    meta_tx.flushPartialPacket = false;
    meta_tx.timestamp = 0;

    for (int i = 0; i <tx_size; i++)
    {
        tx_buffer[2*i] = cos(2*M_PI*i/16.0);
        tx_buffer[2*i+1] = sin(2*M_PI*i/16.0);
    }

    LMS_SetupStream(device, &tx_stream);
    LMS_StartStream(&tx_stream);

    while (running)
    {
        int ret = LMS_SendStream(&tx_stream,tx_buffer,tx_size,&meta_tx,1000);
        if (ret != tx_size)
            cout << "error: samples sent: " << ret << "/" << tx_size << endl;;
    }

    LMS_StopStream(&tx_stream);
    LMS_DestroyStream(device,&tx_stream); 
}

int main(int argc, char** argv){
	cout << "Raptor Watch FMCW Radar System" << endl;
    int n= LMS_GetDeviceList(nullptr);

    if (n > 0)
    {
        if (LMS_Open(&device,NULL,NULL)!=0){ //open first device
            error();
        }

        if (LMS_Init(device)!=0){
            error();
        }

        if (LMS_EnableChannel(device,LMS_CH_TX,0,true)!=0){ 
            error();
        }

        if (LMS_SetSampleRate(device,sampleRate,0)!=0){
            error();
        }

        if (LMS_SetLOFrequency(device,LMS_CH_TX, 0, 500e6)!=0){
            error();
        }
    
        if (LMS_SetAntenna(device, LMS_CH_TX, 0, LMS_PATH_TX1)!=0){   //TX1_1        
            error();
		}

        running = true;
        std::thread thread = std::thread(StreamTest);
        cout << "Press <Enter> to stop transmitting..." << endl;
    	cin.ignore();
        running = false;
        thread.join();

        if (LMS_Close(device)==0){
            cout << "Closed" << endl;
        }
		cout << "Done" << endl;
    }
    else{
    	cout <<  "No device found." << endl;
    }
    return 0;
}
