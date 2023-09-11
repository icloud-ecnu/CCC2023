// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "config.hpp"

extern "C" {

void mm2s(
    ap_int<DWIDTH>* mem, 
    hls::stream<data >& s0, 
    hls::stream<data >& s1, 
    hls::stream<data >& s2, 
    hls::stream<data >& s3, 
    hls::stream<data >& s4, 
    hls::stream<data >& s5, 
    hls::stream<data >& s6, 
    hls::stream<data >& s7,
    int size) {
#pragma HLS interface axis port=s0
#pragma HLS interface axis port=s1
#pragma HLS interface axis port=s2
#pragma HLS interface axis port=s3
#pragma HLS interface axis port=s4
#pragma HLS interface axis port=s5
#pragma HLS interface axis port=s6
#pragma HLS interface axis port=s7
    data x;

    for (int i = 0; i < size; ++ i) {
#pragma HLS PIPELINE II=1
        x.data = mem[i];
        x.keep_all();

        switch(i / 256) {
        case 0:s0.write(x);break;
        case 1:s1.write(x);break;
        case 2:s2.write(x);break;
        case 3:s3.write(x);break;
        case 4:s4.write(x);break;
        case 5:s5.write(x);break;
        case 6:s6.write(x);break;
        case 7:s7.write(x);break;
        }
    }
}
}