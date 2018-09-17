/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LimeFMCW.h
 * Author: MBELE Lebohang
 *
 * Created on 10 May 2017, 23:04
 */

#include <iostream>
#include <chrono>
#include <math.h>
#include <lime/LimeSuite.h>

#ifndef LIMEFMCW_H
#define LIMEFMCW_H

#define LIMEFMCW_CH_TX      0
#define LIMEFMCW_CH_RX      1
#define LIMEFMCW_CH_TX_RX   2

// Define the number of channels to use on both RX and TX channel
#define NUMBER_OF_CHANNELS  2

// Define the time(sec) to transmit and receive data
#define TRANSMISSION_RECEIVE_TIME   10

// Uncomment one or both of this micros to use that path
// #define USE_LIMEFMCW_CH_RX
// #define USE_LIMEFMCW_CH_TX



namespace MBLLEB006{

    class LimeFMCW {
    public:
        /******** public variables *******/
        lms_device_t* lime_device = NULL;
        /*********************************/

        /**
         * @Brief Default constructor
         */
        LimeFMCW();

        /**
         * @Brief Default destructor
         */
        ~LimeFMCW();

        /**
         * @Brief   Configure TX and RX Channel
         * @param   pChannel - LIMEFMCW_CH_TX or LIMEFMCW_CH_RX or LIMEFMCW_CH_TX_RX
         *          pNum_channels - Number of LimeUSB channels to enable in both RX and TX. Max is 2
         *          pFrequency_center_rx -RX Channel center frequency
         *          pFrequency_center_tx -TX Channel center frequency
         *          pGain_rx -RX Channel gain
         *          pGain_tx -TX Channel gain
         *          pSampling_rate - Sampling rate which will be doubled (x4)
         */
        uint8_t configLimeChannel(uint8_t pChannel, float pFrequency_center_rx, float pFrequency_center_tx, float pGain_rx, float pGain_tx,float pSampling_rate);
        
        /**
         * @Brief Enable or disable the test signal
         */
        uint8_t configTestSignal(lms_testsig_t pTestSignalOneType, lms_testsig_t pTestSignalTwoType);

        /**
         * @Brief Enable or disable the test signal
         */
        uint8_t configSystemStream(uint16_t pFIFOSize, float pThroughputVsLatency);

        /**
         * @Brief Start transmitting FMCW signal
         */
        uint8_t startFMCWTransmit();

        /**
         * @Brief Set the bandwidth of a single channel at the time
         *        define USE_LIMEFMCW_CH_TX and USE_LIMEFMCW_CH_RX to set the bandwidth of that specific path
         *
         * @param   pbandwidth - bandwidth value. Must not exceed the min-max of that channel
         *          
         */
        uint8_t setRFBandwidth(float pBandwidth);

        /**
         * @Brief Get the RX center frequency channel
         */
        float getRXCenterFrequency();

        /**
         * @Brief Get the TX center frequency channel
         */
        float getTXCenterFrequency();
        /**
         * @Brief Get the sampling rate channel
         */
        float getSamplingRate();

        /**
         * @Brief A helper function for reporting errors
         */
        uint8_t error();   
        
    private:
        /******** private variables *******/
        float       frequency_center_tx;
        float       frequency_center_rx;
        float       sampling_rate;
        float       *rx_buffers[NUMBER_OF_CHANNELS];
        float       *tx_buffers[NUMBER_OF_CHANNELS];
        int         buffer_size;

        lms_range_t bandwidth_range_tx;
        lms_range_t bandwidth_range_rx;
        lms_stream_meta_t rx_metadata;
        lms_stream_meta_t tx_metadata;
        lms_stream_t rx_streams[NUMBER_OF_CHANNELS];
        lms_stream_t tx_streams[NUMBER_OF_CHANNELS];
    };
}
#endif /* LIMEFMCW_H */

