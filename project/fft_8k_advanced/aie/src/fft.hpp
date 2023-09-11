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
        // initialization_function(fft_kernel) = "fft_1k_init";

        runtime<ratio>(fft_kernel)=0.8;

        if (id==6) location<kernel>(fft_kernel)=tile(22,2);
        if (id==1) location<kernel>(fft_kernel)=tile(23,2);
        if (id==2) location<kernel>(fft_kernel)=tile(24,2);
        if (id==3) location<kernel>(fft_kernel)=tile(22,1);
        if (id==4) location<kernel>(fft_kernel)=tile(24,1);
        if (id==5) location<kernel>(fft_kernel)=tile(22,0);
        if (id==0) location<kernel>(fft_kernel)=tile(23,0);
        if (id==7) location<kernel>(fft_kernel)=tile(24,0);
    }
};

class stage2_graph :public graph{
private:
    kernel stage2_kernel;
public:
    port<input> in[8];
    port<output> out;
    stage2_graph(){
        stage2_kernel=kernel::create(fft_stage2);

        for (unsigned i=0;i<8;i++){
            connect<window<N_POINT*sizeof(cint16)> >(in[i],stage2_kernel.in[i]);
        }
        connect<stream>(stage2_kernel.out[0],out);

        source(stage2_kernel)="stage2_kernel.cpp";

        runtime<ratio>(stage2_kernel)=0.8;

        location<kernel>(stage2_kernel)=tile(23,1);
    }
};