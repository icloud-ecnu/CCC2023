#pragma once

#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include "definition.hpp"

using namespace aie;

static cint16 omg4[]={{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {0, -1}, {-1, 0}, {0, 1}, {1, 0}, {-1, 0}, {1, 0}, {-1, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}};

void fft_stage2(input_window<cint16> *x_in0,input_window<cint16> *x_in1,input_window<cint16> *x_in2,input_window<cint16> *x_in3,
                output_window<cint16> *y_out0,output_window<cint16> *y_out1,output_window<cint16> *y_out2,output_window<cint16> *y_out3);