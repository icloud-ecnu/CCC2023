// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "config.hpp"

extern "C" {

void s2mm(ap_int<DWIDTH>* mem, hls::stream<data >& s, int size) {
data_mover:
    for (int i = 0; i < size; i++) {
        #pragma HLS PIPELINE II=1 // pipeline
        data x = s.read();
        mem[i] = x.data;
    }
}
}
