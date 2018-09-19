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
    if (LMS_Open(&this->lime_device, device_list[0], NULL)){
        error();
    }
    cout << "LimeSDR-USB device opened successfully." << endl;

    if((n = LMS_GetNumChannels(this->lime_device, LMS_CH_RX)) < 0){
        error();
    }
    cout << "Number of RX Channels: " << n << endl;

    if((n = LMS_GetNumChannels(this->lime_device, LMS_CH_TX)) < 0){
        error();
    }
    cout << "Number of TX Channels: " << n << endl;

    
    if(LMS_GetLPFBWRange(this->lime_device, LMS_CH_RX, &bandwidth_range_rx) != 0){
        error();
    }
    cout << "RX bandwidth range: " << bandwidth_range_rx.min/(1e6)<<" MHz <--> "<<bandwidth_range_rx.max/(1e6) <<" MHz"<< endl;

    if(LMS_GetLPFBWRange(this->lime_device, LMS_CH_TX, &bandwidth_range_tx) != 0){
        error();
    }
    cout << "TX bandwidth range: " << bandwidth_range_tx.min/(1e6)<<" MHz <--> "<<bandwidth_range_tx.max/(1e6) <<" MHz"<< endl;

}

LimeFMCW::~LimeFMCW(){
    cout << "Lime device closing ..." << endl;
    if(this->lime_device != NULL){
        LMS_Close(this->lime_device);
    }
}

uint8_t  LimeFMCW::configLimeChannel(uint8_t pChannel, float pFrequency_center_rx, float pFrequency_center_tx, float pGain_rx, float pGain_tx,float pSampling_rate){
    this->frequency_center_tx = pFrequency_center_tx;
    this->frequency_center_rx = pFrequency_center_rx;
    this->sampling_rate = pSampling_rate;

    cout << "Initializing the Lime device..." << endl;
    if(LMS_Init(this->lime_device) != 0){
        error();
    }
    cout << "Initialization complete!" << endl;

    if((pChannel == LIMEFMCW_CH_TX) || (pChannel == LIMEFMCW_CH_RX)){

        pChannel = (pChannel == LIMEFMCW_CH_TX) ? LMS_CH_TX : LMS_CH_RX;

        cout << "Setting center frequency to -> "<< ((pChannel == LMS_CH_TX) ? this->frequency_center_tx : this->frequency_center_rx) << endl;
        for (int channel_index = 0; channel_index < NUMBER_OF_CHANNELS; channel_index++){
            if(LMS_EnableChannel(this->lime_device, pChannel,channel_index, true)){
                error();
            }

            if(LMS_SetLOFrequency(this->lime_device, pChannel,channel_index, ((pChannel == LMS_CH_TX) ? this->frequency_center_tx : this->frequency_center_rx))){
                error();
            }

            if(LMS_SetNormalizedGain(this->lime_device, pChannel,channel_index, pGain_rx)){
                error();
            }
        }
    }
    else{
        for (int channel_index = 0; channel_index < NUMBER_OF_CHANNELS; channel_index++){
            if(LMS_EnableChannel(this->lime_device, LMS_CH_RX,channel_index, true)){
                error();
            }
            if(LMS_EnableChannel(this->lime_device, LMS_CH_TX,channel_index, true)){
                error();
            }

            if(LMS_SetLOFrequency(this->lime_device, LMS_CH_RX,channel_index, frequency_center_rx)){
                error();
            }
            if(LMS_SetLOFrequency(this->lime_device, LMS_CH_TX,channel_index, frequency_center_tx)){
                error();
            }

            if(LMS_SetNormalizedGain(this->lime_device, LMS_CH_RX,channel_index, pGain_rx)){
                error();
            }
            if(LMS_SetNormalizedGain(this->lime_device, LMS_CH_TX,channel_index, pGain_tx)){
                error();
            }
        }
        cout << "Enabling RX and TX complete..."<< endl;
        cout << "Center frequency of RX is set to -> "<< this->frequency_center_rx << endl;
        cout << "Center frequency of TX is set to -> "<< this->frequency_center_tx << endl;
    }
    
    cout << "Setting Lime device sampling rate to -> "<< this->sampling_rate << endl;
    if(LMS_SetSampleRate(this->lime_device, this->sampling_rate,4)){
        error();
    }
    cout << "Channel configuration is complete." << endl;

    return 0;
}

uint8_t LimeFMCW::configTestSignal(lms_testsig_t pTestSignalOneType, lms_testsig_t pTestSignalTwoType){
    if(LMS_SetTestSignal(this->lime_device, LMS_CH_RX, 0, pTestSignalOneType, 0, 0) != 0){
        error();
    }
    if(LMS_SetTestSignal(this->lime_device, LMS_CH_RX, 1, pTestSignalTwoType, 0, 0) != 0){
        error();
    }
    return 0;
}

uint8_t LimeFMCW::configSystemStream(uint16_t pFIFOSize, float pThroughputVsLatency, float pF_start, float pF_sweep, float pT_cpi){
    // Set the bufferSize
    this->buffer_size = pFIFOSize * 16;
    
    // ThroughputVsLatency is a vallue between 0..1
    pThroughputVsLatency = ((pThroughputVsLatency > 1.0)&&(pThroughputVsLatency < 0)) ? 0.5 : pThroughputVsLatency;

    for (int channel_index = 0; channel_index < NUMBER_OF_CHANNELS; ++channel_index){
#ifdef USE_LIMEFMCW_CH_TX
        this->tx_streams->channel = channel_index;
        this->tx_streams->fifoSize = pFIFOSize * pFIFOSize;
        this->tx_streams->throughputVsLatency = pThroughputVsLatency;
        this->tx_streams->isTx = false;
        this->tx_streams->dataFmt = lms_stream_t::LMS_FMT_F32;

        if (LMS_SetupStream(this->lime_device, &this->tx_streams[channel_index])){
            error();
        }
        cout << "TX Channels Streams has been setup successfully ..." << endl;
#endif
#ifdef USE_LIMEFMCW_CH_RX
        this->rx_streams->channel = channel_index;
        this->rx_streams->fifoSize = pFIFOSize * pFIFOSize;
        this->rx_streams->throughputVsLatency = pThroughputVsLatency;
        this->rx_streams->isTx = true;
        this->rx_streams->dataFmt = lms_stream_t::LMS_FMT_F32;

        if (LMS_SetupStream(this->lime_device, &rx_streams[channel_index])){
            error();
        }
        cout << "RX Channels Streams has been setup successfully ..." << endl;
#endif
    }

    // Setup data buffers
    // Initialize the stream
    for (int channel_index = 0; channel_index < NUMBER_OF_CHANNELS; ++channel_index){
#ifdef USE_LIMEFMCW_CH_TX
        // Fill the TX buffer with transmit IQ data
        generateLinearChirpSignal(pF_start,pF_sweep,pT_cpi);
        LMS_StartStream(&this->tx_streams[channel_index]);
#endif
#ifdef USE_LIMEFMCW_CH_RX
        this->rx_buffers[channel_index] = new float[this->buffer_size*2];       
        LMS_StartStream(&this->rx_streams[channel_index]);
#endif       
    }

    return 0;
}

void LimeFMCW::startFMCWTransmit(){
    
#ifdef USE_LIMEFMCW_CH_RX
    this->rx_metadata.flushPartialPacket = false;
    this->rx_metadata.waitForTimestamp   = false;
    this->rx_metadata.timestamp = 0;
#endif
#ifdef USE_LIMEFMCW_CH_TX
    this->tx_metadata.flushPartialPacket = false;
    this->tx_metadata.waitForTimestamp   = true;
    this->tx_metadata.timestamp = 0;
#endif
    auto t1 = chrono::high_resolution_clock::now();
    auto t2 = t1; 

    while((chrono::high_resolution_clock::now() - t1) < chrono::seconds(TRANSMISSION_RECEIVE_TIME)){
        for (int channel_index = 0; channel_index < NUMBER_OF_CHANNELS; ++channel_index){
#ifdef USE_LIMEFMCW_CH_TX           
        int sent_samples = LMS_SendStream(&this->tx_streams[channel_index], this->tx_buffers[channel_index], this->buffer_size,&this->tx_metadata,1000);
        if(sent_samples != this->buffer_size){
            cout << "WARNING: Samples sent: " << sent_samples << "/" << this->buffer_size << endl;
        }
#endif   
#ifdef USE_LIMEFMCW_CH_RX           
        int samplesRead;
        samplesRead = LMS_RecvStream(&this->rx_streams[channel_index], this->rx_buffers[channel_index], this->buffer_size,&this->tx_metadata,1000);
#endif          
        }
    }

    // Print RF statistics
    if(chrono::high_resolution_clock::now() - t2 > chrono::seconds(1)){
        lms_stream_status_t status;
#ifdef USE_LIMEFMCW_CH_RX         
        LMS_GetStreamStatus(this->rx_streams,&status);
        cout << "RX rate: " << status.linkRate/(1e6) << "MB/s on all channels" << endl;
        cout << "RX 0 FIFO: " << 100*status.fifoFilledCount/status.fifoSize << "%" << endl;
#endif   
#ifdef USE_LIMEFMCW_CH_TX 
        LMS_GetStreamStatus(this->tx_streams,&status);
        cout << "TX rate: " << status.linkRate/(1e6) << "MB/s on all channels" << endl;
        cout << "TX 0 FIFO: " << 100*status.fifoFilledCount/status.fifoSize << "%" << endl;
#endif        
    }
}

void LimeFMCW::pauseFMCWTransmit(){
    cout << "Pausing lime stream..." << endl;
    for (int channel_index = 0; channel_index < NUMBER_OF_CHANNELS; ++channel_index){
#ifdef USE_LIMEFMCW_CH_TX
        LMS_StopStream(&this->tx_streams[channel_index]);
#endif
#ifdef USE_LIMEFMCW_CH_RX
        LMS_StopStream(&this->rx_streams[channel_index]);
#endif       
    }
}

void LimeFMCW::resumeFMCWTransmit(){
    cout << "Resuming lime stream..." << endl;
    for (int channel_index = 0; channel_index < NUMBER_OF_CHANNELS; ++channel_index){
#ifdef USE_LIMEFMCW_CH_TX
        LMS_StartStream(&this->tx_streams[channel_index]);
#endif
#ifdef USE_LIMEFMCW_CH_RX
        LMS_StartStream(&this->rx_streams[channel_index]);
#endif       
    }
}

void LimeFMCW::stopFMCWTransmit(){
    for (int channel_index = 0; channel_index < NUMBER_OF_CHANNELS; ++channel_index){
#ifdef USE_LIMEFMCW_CH_TX
        LMS_StopStream(&this->tx_streams[channel_index]);
        LMS_DestroyStream(this->lime_device,&this->tx_streams[channel_index]);
#endif
#ifdef USE_LIMEFMCW_CH_RX
        LMS_StopStream(&this->rx_streams[channel_index]);
        LMS_DestroyStream(this->lime_device,&this->rx_streams[channel_index]);
#endif  
    cout << "Lime stream stopped and destroyed..." << endl;
    }
}

void LimeFMCW::generateLinearChirpSignal(float f_start, float f_sweep, float t_cpi){
#ifdef USE_LIMEFMCW_CH_TX    
    for (int channel_index = 0; channel_index < NUMBER_OF_CHANNELS; ++channel_index){
        
        this->tx_buffers[channel_index] = new float[this->buffer_size*2];

        for (int x = 0; x < buffer_size; ++x){
            float delta = x / (float)buffer_size;
            float t = t_cpi * delta;
            float phase = 2 * M_PI * t * (f_start + (f_sweep - f_start) * delta / 2);

            this->tx_buffers[channel_index][2*x] = cos(phase);
            this->tx_buffers[channel_index][2*x + 1] = sin(phase);
        }
    }
#else
    cout << "USE_LIMEFMCW_CH_TX is not defined. Therefore chirp signal cannot be generated..." << endl;
#endif
}

uint8_t LimeFMCW::setRFBandwidth(float pBandwidth){
    cout << "NOTE: If the bandwidth value is out of bounds, it will be adjusted accordingly." << endl;
    
    // Ensure that pBandwidth is not out of bounds
#ifdef USE_LIMEFMCW_CH_TX
    pBandwidth = ((pBandwidth > this->bandwidth_range_tx.max) ? this->bandwidth_range_tx.max : pBandwidth);
    pBandwidth = ((pBandwidth < this->bandwidth_range_tx.min) ? this->bandwidth_range_tx.min : pBandwidth);
    cout << "Setting bandwidth of TX channel to -> "<< pBandwidth << endl;
#endif        
#ifdef USE_LIMEFMCW_CH_RX
    pBandwidth = ((pBandwidth > this->bandwidth_range_rx.max) ? this->bandwidth_range_rx.max : pBandwidth);
    pBandwidth = ((pBandwidth < this->bandwidth_range_rx.min) ? this->bandwidth_range_rx.min : pBandwidth);
    cout << "Setting bandwidth of RX channel to -> "<< pBandwidth << endl;
#endif
    
    for (int channel_index = 0; channel_index < NUMBER_OF_CHANNELS; channel_index++){
#ifdef USE_LIMEFMCW_CH_TX
        if(LMS_SetLPFBW(this->lime_device, LMS_CH_TX, channel_index, pBandwidth) != 0){
            error();
        }
#endif        
#ifdef USE_LIMEFMCW_CH_RX
        if(LMS_SetLPFBW(this->lime_device, LMS_CH_RX, channel_index, pBandwidth) != 0){
            error();
        }
#endif
    }
    return 0;
}


float LimeFMCW::getRXCenterFrequency(){
    return this->frequency_center_rx;
}

float LimeFMCW::getTXCenterFrequency(){
    return this->frequency_center_tx;
}

float LimeFMCW::getSamplingRate(){
    return this->sampling_rate;
}

uint8_t LimeFMCW::error(){
    cout << "ERROR: "<< LMS_GetLastErrorMessage() << endl;
    if(this->lime_device != NULL){
        LMS_Close(this->lime_device);
    }
    exit(-1);
}