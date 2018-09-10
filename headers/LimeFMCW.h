/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Image.h
 * Author: MBELE
 *
 * Created on 10 May 2017, 23:04
 */

#include <iostream>
#include <lime/LimeSuite.h>

#ifndef LIMEFMCW_H
#define LIMEFMCW_H

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
         * @Brief   Configure TX/RX Channel
         * @param   Channel - LMS_CH_TX or LMS_CH_RX
         *          pFrequency_center - Channel center frequency
         *          pSampling_rate - Sampling rate which will be doubled (x2)
         */
        uint8_t LimeChannelConfig(bool Channel, float pFrequency_center,float pSampling_rate);
        
        /**
         * @Brief Get the center frequency channel
         */
        float getCenterFrequency();

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
        float    frequency_center;
        float    sampling_rate;
    };
}
#endif /* LIMEFMCW_H */

