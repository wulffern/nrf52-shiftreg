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

#include "shiftreg.h"


void rvr_wait(){
#ifdef NRF52
	nrf_gpio_pin_write(SCK,sck);
	nrf_gpio_pin_write(SI,si);
	nrf_gpio_pin_write(SLOAD,sload);
	nrf_gpio_pin_write(ARST_N,arst_n);
    nrf_delay_ms(30);
#else
    sleep(1);
#endif
}



void programDevice(){

    //All outputs are inverted through the opto coupler!
    arst_n = 1; //reset device
    rvr_wait();

    arst_n = 0;
    sload = 1;

    for(int i=0;i<24;i++){
        sck = 1;
        rvr_wait();
        si = !shiftreg[i];
        rvr_wait();
        sck = 0;
        rvr_wait();
    }

    sck = 1;
    sload = 0;
    rvr_wait();
    sload = 1;
    rvr_wait();
}

void selectAdc(){
    adcCount++;
    if(adcCount > 8)
        adcCount = 0;
//PWR_CV
//0000_0000( ADC(0(is(enabled,(bit(0(is(inverted(((
//0000_0001( All(ADCs(disabled(
//XXXX_XX1X( ADC(1(is(enabled(
//XXXX_X1XX( ADC(2(is(enabled(
//XXXX_1XXX( ADC(3(is(enabled(
//XXX1_XXXX( ADC(4(is(enabled(
//XX1X_XXXX( ADC(5(is(enabled(
//X1XX_XXXX( ADC(6(is(enabled(
//1XXX_XXXX( ADC(7(is(enabled(
    if(adcCount == 0){
        pwr_cv = 0;
    }else if(adcCount == 1){
        pwr_cv = 3;
    }else if(adcCount == 2){
        pwr_cv = 5;
    }else if(adcCount == 3){
        pwr_cv = 9;
    }else if(adcCount == 4){
        pwr_cv = 17;
    }else if(adcCount == 5){
        pwr_cv = 33;
    }else if(adcCount == 6){
        pwr_cv = 65;
    }else if(adcCount == 7){
        pwr_cv = 129;
    }else if(adcCount == 8){
        pwr_cv = 1;
    }

//DIGSEL
//000( Digital(output(from(ADC(0(is(selected(
//001( Digital(output(from(ADC(1(is(selected(
//010( Digital(output(from(ADC(2(is(selected(
//011( Digital(output(from(ADC(3(is(selected(
//100( Digital(output(from(ADC(4(is(selected(
//101( Digital(output(from(ADC(5(is(selected(
//110( Digital(output(from(ADC(6(is(selected(
//111( Digital(output(from(ADC(7(is(selected(
    if(adcCount <=7)
        digsel = adcCount;
    else
        digsel = 0;
	
}



void selectCkSmpl() {
	ck_smpl++;
	if(ck_smpl > 7){
		ck_smpl = 0;
	}
//CK_SMPL
//000( ADC(clock(is(CK_SIN/2
//001( ADC(clock(is(CK_SIN/4
//010( ADC(clock(is(CK_SIN/8
//011( ADC(clock(is(CK_SIN/16
//100( ADC(clock(is(CK_SIN/32
//101( ADC(clock(is(CK_SIN/64
//110( ADC(clock(is(CK_SIN/128
//111( ADC(clock(is(CK_SIN/256
}

void selectDigDiv() {
	ck_digdiv++;
	if(ck_digdiv > 7)
		ck_digdiv = 0;

//CK_DIGDIV
//000( Digital(output(clock(is(CK_SIN/2
//001( Digital(output(clock(is(CK_SIN/4
//010( Digital(output(clock(is(CK_SIN/8
//011( Digital(output(clock(is(CK_SIN/16
//100( Digital(output(clock(is(CK_SIN/32
//101( Digital(output(clock(is(CK_SIN/64
//110( Digital(output(clock(is(CK_SIN/128
//111( Digital(output(clock(is(CK_SIN/256
}


void initialize(){

#ifdef NRF52
	nrf_gpio_cfg_output(SCK);
	nrf_gpio_cfg_output(SLOAD);
	nrf_gpio_cfg_output(SI);
	nrf_gpio_cfg_output(ARST_N);

#endif
	
    maxcount = 23;

    for(int i=0;i<23;i++){
        shiftreg[i] = 1;
    }
    adcCount = 0;

    //Parameters
    digsel = 0;
    pwr_cv = 0;
    ck_smpl = 0;
    ck_digdiv = 0;
    highdrv = 0;
    si =1;
    sload =1;
    arst_n = 0;
    sck = 1;
}

void setShiftReg(int v, int stop, int start){
    int count = stop - start;
    int tmp = v;
    for(int i=stop;i>=start;i=i-1){
        if(tmp >= (1 << count)){
            shiftreg[i] = 1;
            tmp = tmp - (1 << count);
        }else{
            shiftreg[i] = 0;
        }
        count = count -1;
    }

}

void makeShiftReg(){

    setShiftReg(0,3,0); //NC<3:0>

    shiftreg[4] = 1; //Enable modulator

    setShiftReg(digsel,7,5); //DIGSEL<2:0>

    setShiftReg(pwr_cv,15,8); //PWR_CV<7:0>

    shiftreg[16] = 1;

    setShiftReg(ck_smpl,19,17); //CK_SMPL<2:0>

    setShiftReg(ck_digdiv,22,20); //CK_DIGDIV<2:0>

    shiftreg[23] = highdrv;
}




