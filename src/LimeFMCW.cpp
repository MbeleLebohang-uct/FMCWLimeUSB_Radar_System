/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LimeFMCW.cpp
 * Author: MBELE Lebohang
 * 
 * Created on 10 September 2018, 23:04
 */
#include <iostream>
#include "LimeFMCW.h"

using namespace std;
using namespace MBLLEB006;

LimeFMCW::LimeFMCW(){
    cout << "\nConnecting LimeSDR-USB ..." << endl;

    lms_info_str_t device_list[8];
    int n = LMS_GetDeviceList(device_list);
    
    cout << "Devices found: "<< n << endl;
    if(n < 0){
        error();    
    }else if(n == 0){
        cout << "No device found. Check if LimeSDR-USB is connected correctly to VM." << endl;
    }

    cout << "Opening the first device found ..." <<endl;
    if (LMS_Open(&lime_device, device_list[0], NULL)){
        error();
    }
    cout << "LimeSDR-USB device opened successfully." << endl;
}

LimeFMCW::~LimeFMCW(){
    cout << "Lime device closing ..." << endl;
    if(lime_device != NULL){
        LMS_Close(lime_device);
    }
}

uint8_t  LimeFMCW::LimeChannelConfig(bool Channel, float pFrequency_center,float pSampling_rate){
    this->frequency_center = pFrequency_center;
    this->sampling_rate = pSampling_rate;

    cout << "Initializing the Lime device..." << endl;
    if(LMS_Init(lime_device) != 0){
        error();
    }
    cout << "Initialization complete!" << endl;

    cout << "Setting center frequency to -> "<< this->frequency_center << endl;
    if(LMS_EnableChannel(lime_device, Channel,0, true)){
        error();
    }

    if(LMS_SetLOFrequency(lime_device, Channel,0, this->frequency_center)){
        error();
    }
    
    cout << "Setting sampling frequency to -> "<< this->sampling_rate << endl;
    if(LMS_SetSampleRate(lime_device, this->sampling_rate,2)){
        error();
    }
    cout << "Channel configuration is complete." << endl;

    return 0;
}

float LimeFMCW::getCenterFrequency(){
    return this->frequency_center;
}

float LimeFMCW::getSamplingRate(){
    return this->sampling_rate;
}

uint8_t LimeFMCW::error(){
    cout << "ERROR: "<< LMS_GetLastErrorMessage() << endl;
    if(lime_device != NULL){
        LMS_Close(lime_device);
    }
    exit(-1);
}