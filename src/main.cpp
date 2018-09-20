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
#include <chrono>
#include <thread>

#include "LimeFMCW.h"

using namespace std;
using namespace MBLLEB006;
/*
 * 
 */


lms_device_t* device;
const double sampleRate = 10e6;
bool running;
const int tx_size = 1024*2;
float tx_buffer[2*tx_size];
double gain_tx = 1;
float bandwidth = 100e6;

float f_start = 10e6;
float f_sweep = f_start + bandwidth;
float t_cpi = 10;
lms_range_t bandwidth_range_tx;

int error()
{
  cout << "ERROR: " << LMS_GetLastErrorMessage() << endl;
  LMS_Close(device);
  exit(-1);
}

void FMCWTransmitter(){
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

    // Fill the tx_buffer with fmcw data.
    for (int x = 0; x <tx_size; x++)
    {
        float delta = x / (float)tx_size;
        float t = t_cpi * delta;
        float phase = 2 * M_PI * t * (f_start + (f_sweep - f_start) * delta / 2);
        tx_buffer[2*x] = cos(phase);
        tx_buffer[2*x+1] = sin(phase);
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

        if (LMS_SetLOFrequency(device,LMS_CH_TX, 0, 2400e6)!=0){
            error();
        }
        if(LMS_SetNormalizedGain(device, LMS_CH_TX,0, 1)){
            error();
        }
        if(LMS_GetLPFBWRange(device, LMS_CH_TX, &bandwidth_range_tx) != 0){
            error();
        }
        cout << "Setting the RF bandwidth ..." << endl;
        if(LMS_SetLPFBW(device, LMS_CH_TX, 0, 100e6)!= 0){
            error();
        }
        cout << "TX bandwidth range: " << bandwidth_range_tx.min/(1e6)<<" MHz <--> "<<bandwidth_range_tx.max/(1e6) <<" MHz"<< endl;
        if(LMS_SetLPF(device, LMS_CH_TX,0,true) != 0){
            error();
        }
        
        if (LMS_SetAntenna(device, LMS_CH_TX, 0, LMS_PATH_TX1)!=0){   //TX1_1        
            error();
        }

        if (LMS_Calibrate(device, LMS_CH_TX, 0, 100e6,0)!=0){         
            error();
        }

        running = true;
        std::thread thread = std::thread(FMCWTransmitter);
        cout << "Press <enter> to stop the stream ..." << endl;
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
