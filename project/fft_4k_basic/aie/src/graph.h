#pragma once

#include <adf.h>
#include "fft.hpp"
#include "fft_kernel.hpp"

using namespace adf;

class fft_4k_graph: public graph{
private:
    fft_1k_graph<0> fft0;
    fft_1k_graph<1> fft1;
    fft_1k_graph<2> fft2;
    fft_1k_graph<3> fft3;
    stage2_graph s2;
public:
    input_plio in[4];
    output_plio out[4];
    
    fft_4k_graph(){
        in[0]=input_plio::create("DataInFFT0",plio_128_bits,"data/DataInFFT0.txt");
        in[1]=input_plio::create("DataInFFT1",plio_128_bits,"data/DataInFFT1.txt");
        in[2]=input_plio::create("DataInFFT2",plio_128_bits,"data/DataInFFT2.txt");
        in[3]=input_plio::create("DataInFFT3",plio_128_bits,"data/DataInFFT3.txt");
        out[0]=output_plio::create("DataOutFFT0",plio_128_bits,"data/DataOutFFT0.txt");
        out[1]=output_plio::create("DataOutFFT1",plio_128_bits,"data/DataOutFFT1.txt");
        out[2]=output_plio::create("DataOutFFT2",plio_128_bits,"data/DataOutFFT2.txt");
        out[3]=output_plio::create("DataOutFFT3",plio_128_bits,"data/DataOutFFT3.txt");

        connect<>(in[0].out[0],fft0.in);
        connect<>(in[1].out[0],fft1.in);
        connect<>(in[2].out[0],fft2.in);
        connect<>(in[3].out[0],fft3.in);
        connect<>(fft0.out,s2.in[0]);
        connect<>(fft1.out,s2.in[1]);
        connect<>(fft2.out,s2.in[2]);
        connect<>(fft3.out,s2.in[3]);
        connect<>(s2.out[0],out[0].in[0]);
        connect<>(s2.out[1],out[1].in[0]);
        connect<>(s2.out[2],out[2].in[0]);
        connect<>(s2.out[3],out[3].in[0]);
    }
};

