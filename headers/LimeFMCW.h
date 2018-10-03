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
#include <thread>
#include <lime/LimeSuite.h>

#ifndef LIMEFMCW_H
#define LIMEFMCW_H

#define LIMEFMCW_CH_TX      0
#define LIMEFMCW_CH_RX      1
#define LIMEFMCW_CH_TX_RX   2

// Define the number of channels to use on both RX and TX channel
#define NUMBER_OF_CHANNELS  1

// Define the time(sec) to transmit and receive data
#define TRANSMISSION_RECEIVE_TIME   60
#define BUFFER_SIZE   2048*8

// Uncomment one or both of this micros to use that path
//#define USE_LIMEFMCW_CH_RX
#define USE_LIMEFMCW_CH_TX



namespace MBLLEB006{

    class LimeFMCW {
    public:
        /*************** public variables ************/
        lms_device_t* lime_device = NULL;
        bool          lime_device_running = false;
        /*********************************************/

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
        void configLimeChannels(float pFrequency_center_rx, float pFrequency_center_tx, float pGain_rx, float pGain_tx,float pSampling_rate);
        
        /**
         * @Brief Enable or disable the test signal
         */
        uint8_t configTestSignal(lms_testsig_t pTestSignalOneType, lms_testsig_t pTestSignalTwoType);

        /**
         * @Brief Configure the signal and start the stream
         * @param pThroughputVsLatency - look at limesuite.h for more info
         *         pF_start - The initial frequency to start transmitting from
         *        pF_sweep  - Sweep frequency
         *        pT_cpi  - coherent processing interval
         */
        void configSystemStreams(float pThroughputVsLatency, float pF_start, float pF_sweep, float pT_cpi);

        /**
         * @Brief Start transmitting FMCW signal
         */
        void startFMCWTransmit();

        /**
         * @Brief Pause transmitting FMCW signal
         */
        void pauseFMCWTransmit();

        /**
         * @Brief Resume transmitting FMCW signal
         */
        void resumeFMCWTransmit();

        /**
         * @Brief stop transmitting FMCW signal. This also closes the device for you.
         */
        void stopFMCWTransmit();

        /**
         * @Brief Generate a linear chirp signal
         *
         * @param f_start - The initial frequency to start transmitting from
         *        f_sweep  - Sweep frequency
         *        t_cpi  - coherent processing interval
         */
        void generateLinearChirpSignal(float pF_start, float pF_sweep, float pT_cpi);
        
        /**
         * @Brief Generate a linear chirp signal as specified in Radar Principles by Peeble Z. 
         *        page 340
         *
         * @param f_start - The initial frequency to start transmitting from
         *        f_sweep  - Sweep frequency
         *        t_cpi  - coherent processing interval
         */
        void generateLinearDigitalFM(float pF_start, float pF_sweep, float pT_cpi);

        /**
         * @Brief print the generate a linear chirp signal
         *
         * @param None
         */
        void printChirpSignal();

        /**
         * @Brief Set the bandwidth of a single channel at the time
         *        define USE_LIMEFMCW_CH_TX and USE_LIMEFMCW_CH_RX to set the bandwidth of that specific path
         *
         * @param   pbandwidth - bandwidth value. Must not exceed the min-max of that channel
         *          
         */
        void setRFBandwidth(float pBandwidth);

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
        float       rx_buffers[NUMBER_OF_CHANNELS][BUFFER_SIZE*2];
        float       tx_buffers[NUMBER_OF_CHANNELS][BUFFER_SIZE*2];

        lms_range_t bandwidth_range_tx;
        lms_range_t bandwidth_range_rx;
        lms_range_t samplingrate_range_tx;
        lms_range_t samplingrate_range_rx;
        lms_stream_meta_t rx_metadata;
        lms_stream_meta_t tx_metadata;
        lms_stream_t rx_streams[NUMBER_OF_CHANNELS];
        lms_stream_t tx_streams[NUMBER_OF_CHANNELS];
    };
}
#endif /* LIMEFMCW_H */

