/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IteratorTest.cpp
 * Author: MBELE
 *
 * Created on 19 May 2017, 09:17
 */

#include <stdlib.h>
#include <iostream>

/*
 * Catch C++ Test Suite
 */
/*
 * Catch C++ Test Suite
 */
#include <queue>
#include <memory>

#include "catch.hpp"
#include "../headers/Image.h"

using namespace MBLLEB006;
using namespace std;

TEST_CASE( "Special member function test", "[Image]" ) {
    Image shrek;
    shrek << "shrek_test.pgm";
    
    Image::iterator it_begin = shrek.begin();
    Image::iterator it_end = shrek.end();
    
    SECTION("Copy constructor.[ Image(const Image& other) ]"){
        
        Image otherImage(shrek);
        
        /* This two images must have different iterators 
         * but pointing to the same data. 
         */
        Image::iterator other_it = otherImage.begin();
        for(it_begin = shrek.begin(); it_begin != it_end; it_begin++){
            REQUIRE(it_begin != other_it);
            REQUIRE(*it_begin == *other_it);
            other_it++;
        }
    } 
    SECTION("Copy assignment operator.[ Image& operator=(const Image& lhs) ]"){
        
        Image otherImage;
        otherImage = shrek;
        
        /* This two images must have different iterators 
         * but pointing to the same data. 
         */
        Image::iterator other_it = otherImage.begin();
        for(it_begin = shrek.begin(); it_begin != it_end; it_begin++){
            REQUIRE(it_begin != other_it);
            REQUIRE(*it_begin == *other_it);
            other_it++;
        }
    }
    
}
