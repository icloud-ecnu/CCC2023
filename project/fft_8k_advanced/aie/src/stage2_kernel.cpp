#include "stage2_kernel.hpp"
#include <aie_api/utils.hpp>
#include <cstdio>

#define LEN_LOAD_X 4

using sliding_mul=sliding_mul_ops<LEN_LOAD_X,8,1,LEN_LOAD_X,1,cint16,cint16,cacc48>;

void fft_stage2(input_window<cint16> *x_in0,input_window<cint16> *x_in1,input_window<cint16> *x_in2,input_window<cint16> *x_in3,
                input_window<cint16> *x_in4,input_window<cint16> *x_in5,input_window<cint16> *x_in6,input_window<cint16> *x_in7,
                output_stream<cint16> *y_out)
{
    // aie::tile tile = aie::tile::current();
    // printf("before stage2: %llu\n", tile.cycles());

    cint16 *x0=(cint16*)x_in0->ptr;
    cint16 *x1=(cint16*)x_in1->ptr;
    cint16 *x2=(cint16*)x_in2->ptr;
    cint16 *x3=(cint16*)x_in3->ptr;
    cint16 *x4=(cint16*)x_in4->ptr;
    cint16 *x5=(cint16*)x_in5->ptr;
    cint16 *x6=(cint16*)x_in6->ptr;
    cint16 *x7=(cint16*)x_in7->ptr;

    auto iterx0=begin_vector<LEN_LOAD_X>(x0);
    auto iterx1=begin_vector<LEN_LOAD_X>(x1);
    auto iterx2=begin_vector<LEN_LOAD_X>(x2);
    auto iterx3=begin_vector<LEN_LOAD_X>(x3);
    auto iterx4=begin_vector<LEN_LOAD_X>(x4);
    auto iterx5=begin_vector<LEN_LOAD_X>(x5);
    auto iterx6=begin_vector<LEN_LOAD_X>(x6);
    auto iterx7=begin_vector<LEN_LOAD_X>(x7);
    for (unsigned i=0;i<N_POINT/LEN_LOAD_X;i++){
        vector<cint16,LEN_LOAD_X*8> x;
        x.insert(0,*iterx0++);
        x.insert(1,*iterx1++);
        x.insert(2,*iterx2++);
        x.insert(3,*iterx3++);
        x.insert(4,*iterx4++);
        x.insert(5,*iterx5++);
        x.insert(6,*iterx6++);
        x.insert(7,*iterx7++);
        auto iteromg=begin_vector<8>((cint16*)mat_omg_8);
        auto res=sliding_mul::mul(*iteromg++,0,x,0);
        writeincr(y_out,res.to_vector<cint16>(MAT_OMG_SHIFT));
        res=sliding_mul::mul(*iteromg++,0,x,0);
        writeincr(y_out,res.to_vector<cint16>(MAT_OMG_SHIFT));
        res=sliding_mul::mul(*iteromg++,0,x,0);
        writeincr(y_out,res.to_vector<cint16>(MAT_OMG_SHIFT));
        res=sliding_mul::mul(*iteromg++,0,x,0);
        writeincr(y_out,res.to_vector<cint16>(MAT_OMG_SHIFT));
        res=sliding_mul::mul(*iteromg++,0,x,0);
        writeincr(y_out,res.to_vector<cint16>(MAT_OMG_SHIFT));
        res=sliding_mul::mul(*iteromg++,0,x,0);
        writeincr(y_out,res.to_vector<cint16>(MAT_OMG_SHIFT));
        res=sliding_mul::mul(*iteromg++,0,x,0);
        writeincr(y_out,res.to_vector<cint16>(MAT_OMG_SHIFT));
        res=sliding_mul::mul(*iteromg++,0,x,0);
        writeincr(y_out,res.to_vector<cint16>(MAT_OMG_SHIFT));

    }

    // printf("stage2: %llu\n", tile.cycles());
}
