//====================================================================
//        Copyright (c) 2015 Carsten Wulff Software, Norway
// ===================================================================
// Created       : wulff at 2015-11-29
// ===================================================================
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
//====================================================================

#ifndef RVR_SHIFTREG
#define RFR_SHIFTREG

#ifdef NRF52
#include "nrf_delay.h"
#include "nrf_gpio.h"
#endif

#define SCK 17
#define SLOAD 18
#define SI 19
#define ARST_N 20

uint32_t shiftreg[32];
uint32_t maxcount;
uint32_t digsel ;
uint32_t pwr_cv;
uint32_t ck_smpl;
uint32_t ck_digdiv ;
uint32_t highdrv;
uint32_t count;
uint32_t adcCount;
uint32_t arst_n;
uint32_t sck;
uint32_t si;
uint32_t sload;



void rvr_wait();
void makeShiftReg();
void programDevice();
void selectAdc();
void selectCkSmpl();
void selectCkDigDiv();
void pressed();
void initialize();
void setShiftReg(int v, int stop, int start);



#endif
