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
    fft_1k_graph<4> fft4;
    fft_1k_graph<5> fft5;
    fft_1k_graph<6> fft6;
    fft_1k_graph<7> fft7;
    stage2_graph s2;
public:
    input_plio in[8];
    output_plio out;
    
    fft_4k_graph(){
        in[0]=input_plio::create("DataInFFT0",plio_128_bits,"data/DataInFFT0.txt");
        in[1]=input_plio::create("DataInFFT1",plio_128_bits,"data/DataInFFT1.txt");
        in[2]=input_plio::create("DataInFFT2",plio_128_bits,"data/DataInFFT2.txt");
        in[3]=input_plio::create("DataInFFT3",plio_128_bits,"data/DataInFFT3.txt");
        in[4]=input_plio::create("DataInFFT4",plio_128_bits,"data/DataInFFT4.txt");
        in[5]=input_plio::create("DataInFFT5",plio_128_bits,"data/DataInFFT5.txt");
        in[6]=input_plio::create("DataInFFT6",plio_128_bits,"data/DataInFFT6.txt");
        in[7]=input_plio::create("DataInFFT7",plio_128_bits,"data/DataInFFT7.txt");
        out=output_plio::create("DataOutFFT0",plio_128_bits,"data/DataOutFFT0.txt");

        connect<>(in[0].out[0],fft0.in);
        connect<>(in[1].out[0],fft1.in);
        connect<>(in[2].out[0],fft2.in);
        connect<>(in[3].out[0],fft3.in);
        connect<>(in[4].out[0],fft4.in);
        connect<>(in[5].out[0],fft5.in);
        connect<>(in[6].out[0],fft6.in);
        connect<>(in[7].out[0],fft7.in);
        connect<>(fft0.out,s2.in[0]);
        connect<>(fft1.out,s2.in[1]);
        connect<>(fft2.out,s2.in[2]);
        connect<>(fft3.out,s2.in[3]);
        connect<>(fft4.out,s2.in[4]);
        connect<>(fft5.out,s2.in[5]);
        connect<>(fft6.out,s2.in[6]);
        connect<>(fft7.out,s2.in[7]);
        connect<>(s2.out,out.in[0]);
    }
};

