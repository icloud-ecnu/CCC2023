#pragma once

#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include "definition.hpp"

using namespace aie;

void fft_stage2(input_window<cint16> *x_in0,input_window<cint16> *x_in1,input_window<cint16> *x_in2,input_window<cint16> *x_in3,
                input_window<cint16> *x_in4,input_window<cint16> *x_in5,input_window<cint16> *x_in6,input_window<cint16> *x_in7,
                output_stream<cint16> *y_out);