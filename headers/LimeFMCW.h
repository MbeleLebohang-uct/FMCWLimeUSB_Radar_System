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
#include <lime/LimeSuite.h>

#ifndef LIMEFMCW_H
#define LIMEFMCW_H

#define LIMEFMCW_CH_TX      0
#define LIMEFMCW_CH_RX      1
#define LIMEFMCW_CH_TX_RX   2

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
        uint8_t configLimeChannel(uint8_t pChannel, uint8_t pNum_channels, float pFrequency_center_rx, float pFrequency_center_tx, float pGain_rx, float pGain_tx,float pSampling_rate);
        
        /**
         * @Brief Enable or disable the test signal
         */
        uint8_t configTestSignal(lms_testsig_t pTestSignalOneType, lms_testsig_t pTestSignalTwoType);

        /**
         * @Brief Set the bandwidth of a single channel at the time
         * @param   pChannel - LIMEFMCW_CH_TX or LIMEFMCW_CH_RX
         *          pNum_channels - Number of LimeUSB channels to enable in specfied channel. Max is 2
         *          pbandwidth - bandwidth value. Must not exceed the min-max of that channel
         *          
         */
        uint8_t setRFBandwidth(uint8_t pChannel, uint8_t pNum_channels, float pBandwidth);

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
        lms_range_t bandwidth_range_tx;
        lms_range_t bandwidth_range_rx;
        float       frequency_center_tx;
        float       frequency_center_rx;
        float       sampling_rate;
    };
}
#endif /* LIMEFMCW_H */

