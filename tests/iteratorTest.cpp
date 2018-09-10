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

TEST_CASE( "Overload operators of Image object", "[Image]" ) {
    Image shrek;
    shrek << "shrek_test.pgm";
    
    Image::iterator it_begin = shrek.begin();
    Image::iterator it_end = shrek.end();
    
    SECTION("Dereference the Iterators.[*]"){
        REQUIRE(*it_begin == 'd');
        *it_begin = 'e';
        REQUIRE(*it_begin == 'e');
    } 
    SECTION("Iterator should point to the next data element of the Image.[++]"){
        /* Since we have tested that dereference operator work, we can use it */
        REQUIRE(*it_begin == 'd');
        *it_begin = 'e';
        it_begin++;
        REQUIRE(*it_begin != 'e');
        REQUIRE(*it_begin == 'd');
    }  
    SECTION("Iterator should point to the previous data element of the Image.[--]"){
        /* Since we have tested that ++ operator work, we can use it */
        REQUIRE(*it_begin == 'd');
        *it_begin = 'e';
        ++it_begin;
        it_begin--;
        REQUIRE(*it_begin == 'e');
        REQUIRE(*it_begin != 'd');
    }  
    SECTION("Check the equality of the two Iterators.[==]"){
        REQUIRE(!(it_begin == it_end));
        
        Image::iterator it_temp = shrek.begin();
        REQUIRE(it_begin == it_temp);
    }
    SECTION("Left Iterator should be equal to the right Iterator.[=]"){
        /* Since we have tested that == operator work, we can use it */
        it_end = it_begin;
        
        REQUIRE(it_end == it_begin);
    }
}
