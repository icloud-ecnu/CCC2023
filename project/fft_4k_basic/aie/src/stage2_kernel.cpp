#include "stage2_kernel.hpp"
#include <aie_api/utils.hpp>
#include <cstdio>

#define LEN_LOAD_X 8

using sliding_mul=sliding_mul_ops<8,4,1,LEN_LOAD_X,1,cint16,cint16,cacc48>;

void fft_stage2(input_window<cint16> *x_in0,input_window<cint16> *x_in1,input_window<cint16> *x_in2,input_window<cint16> *x_in3,
                output_window<cint16> *y_out0,output_window<cint16> *y_out1,output_window<cint16> *y_out2,output_window<cint16> *y_out3)
{
    // aie::tile tile = aie::tile::current();
    // printf("before stage2: %llu\n", tile.cycles());

    cint16 *x0=(cint16*)x_in0->ptr;
    cint16 *x1=(cint16*)x_in1->ptr;
    cint16 *x2=(cint16*)x_in2->ptr;
    cint16 *x3=(cint16*)x_in3->ptr;
    cint16 *y0=(cint16*)y_out0->ptr;
    cint16 *y1=(cint16*)y_out1->ptr;
    cint16 *y2=(cint16*)y_out2->ptr;
    cint16 *y3=(cint16*)y_out3->ptr;

    auto iterx0=begin_vector<LEN_LOAD_X>(x0);
    auto iterx1=begin_vector<LEN_LOAD_X>(x1);
    auto iterx2=begin_vector<LEN_LOAD_X>(x2);
    auto iterx3=begin_vector<LEN_LOAD_X>(x3);
    auto itery0=begin_vector<LEN_LOAD_X>(y0);
    auto itery1=begin_vector<LEN_LOAD_X>(y1);
    auto itery2=begin_vector<LEN_LOAD_X>(y2);
    auto itery3=begin_vector<LEN_LOAD_X>(y3);
    for (unsigned i=0;i<N_POINT/LEN_LOAD_X;i++){
        vector<cint16,LEN_LOAD_X*4> x;
        x.insert(0,*iterx0++);
        x.insert(1,*iterx1++);
        x.insert(2,*iterx2++);
        x.insert(3,*iterx3++);
        auto iteromg=begin_vector<4>((cint16*)omg4);
        auto res=sliding_mul::mul(*iteromg++,0,x,0);
        *itery0++=res.to_vector<cint16>(0);
        res=sliding_mul::mul(*iteromg++,0,x,0);
        *itery1++=res.to_vector<cint16>(0);
        res=sliding_mul::mul(*iteromg++,0,x,0);
        *itery2++=res.to_vector<cint16>(0);
        res=sliding_mul::mul(*iteromg++,0,x,0);
        *itery3++=res.to_vector<cint16>(0);
    }

    // printf("stage2: %llu\n", tile.cycles());
}