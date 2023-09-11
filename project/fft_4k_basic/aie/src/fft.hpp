#pragma once

#include <adf.h>
#include "fft_kernel.hpp"
#include "stage2_kernel.hpp"

using namespace adf;

template<unsigned id>
class fft_1k_graph : public graph {
private:
    kernel fft_kernel;
public:
    port<input> in;
    port<output> out;

    fft_1k_graph(){
        fft_kernel=kernel::create(radix2_dit<id>);

        connect<window<N_POINT*sizeof(cint16)> >(in,fft_kernel.in[0]);
        connect<window<N_POINT*sizeof(cint16)> >(fft_kernel.out[0],out);

        source(fft_kernel)="fft_kernel.cpp";

        runtime<ratio>(fft_kernel)=0.75;
    }
};

class stage2_graph :public graph{
private:
    kernel stage2_kernel;
public:
    port<input> in[4];
    port<output> out[4];
    stage2_graph(){
        stage2_kernel=kernel::create(fft_stage2);

        for (unsigned i=0;i<4;i++){
            connect<window<N_POINT*sizeof(cint16)> >(in[i],stage2_kernel.in[i]);
            connect<window<N_POINT*sizeof(cint16)> >(stage2_kernel.out[i],out[i]);
        }

        source(stage2_kernel)="stage2_kernel.cpp";

        runtime<ratio>(stage2_kernel)=1;
    }
};