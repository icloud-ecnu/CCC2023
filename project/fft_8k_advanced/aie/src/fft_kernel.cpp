#include "fft_kernel.hpp"
#include <cstdio>
#include <aie_api/utils.hpp>
#include <adf.h>

// void fft_1k_init(){
//     set_rounding(rounding_mode::symmetric_inf);
//     set_saturation(saturation_mode::saturate);
// }

void butterfly(unsigned l, cint16 *x, cint16 *y, cint16 *omg)
{
    unsigned m = l >> 1;
	auto iteromg=begin_vector<32>(omg);
    for (unsigned i = 0; i < m; i += 32)
    {
        for (cint16 *p = x, *p_out = y; p != x + N_POINT; p += l, p_out += l)
        {
            vector<cint16, 32> v_0 = load_v<32>(p + i);
            vector<cint16, 32> v_1 = load_v<32>(p + i + m);
            auto acc_t = mul(*iteromg, v_1);
            vector<cint16, 32> v_t = acc_t.to_vector<cint16>(OMG_SHIFT);
            v_1 = sub(v_0, v_t);
            v_0 = add(v_0, v_t);
            store_v(p_out + i, v_0);
            store_v(p_out + i + m, v_1);
        }
	    iteromg++;
    }
}

void butterfly_16(cint16 *x, cint16 *y)
{
	vector<cint16, 8> v_omg = load_v<8>(omg_16);
    for (cint16 *p = x, *p_out = y; p != x + N_POINT; p += 16, p_out += 16)
    {
        vector<cint16, 8> v_0 = load_v<8>(p);
        vector<cint16, 8> v_1 = load_v<8>(p + 8);
        auto acc_t = mul(v_omg, v_1);
        vector<cint16, 8> v_t = acc_t.to_vector<cint16>(OMG_SHIFT);
        v_1 = sub(v_0, v_t);
        v_0 = add(v_0, v_t);
        store_v(p_out, v_0);
        store_v(p_out + 8, v_1);
    }

    // vector<cint16, 8> v_omg = load_v<8>(omg_16);
    // auto iterx=begin_vector<32>(x);
    // auto itery=begin_vector<16>(y);
    // for (unsigned i=0;i<32;i++){
    //     vector<cint16,16> v_x=filter_even(*iterx,1);
    //     vector<cint16,8> v_0=v_x.extract<8>(0),v_1=v_x.extract<8>(1);
    //     auto acc_t = mul(v_omg, v_1);
    //     vector<cint16, 8> v_t = acc_t.to_vector<cint16>(OMG_SHIFT);
    //     v_1 = sub(v_0, v_t);
    //     v_0 = add(v_0, v_t);
    //     vector<cint16,16> v_cat=concat(v_0,v_1);

    //     v_x=filter_odd(*iterx,1);
    //     v_0=v_x.extract<8>(0);
    //     v_1=v_x.extract<8>(1);
    //     acc_t = mul(v_omg, v_1);
    //     v_t = acc_t.to_vector<cint16>(OMG_SHIFT);
    //     v_1 = sub(v_0, v_t);
    //     v_0 = add(v_0, v_t);

    //     auto p=interleave_zip(v_cat,concat(v_0,v_1),1);
    //     *itery++=p.first;
    //     *itery++=p.second;
    // }
}

void butterfly_32(cint16 *x, cint16 *y)
{
	vector<cint16, 16> v_omg = load_v<16>(omg_32);
    for (cint16 *p = x, *p_out = y; p != x + N_POINT; p += 32, p_out += 32)
    {
        vector<cint16, 16> v_0 = load_v<16>(p);
        vector<cint16, 16> v_1 = load_v<16>(p + 16);
        auto acc_t = mul(v_omg, v_1);
        vector<cint16, 16> v_t = acc_t.to_vector<cint16>(OMG_SHIFT);
        v_1 = sub(v_0, v_t);
        v_0 = add(v_0, v_t);
        store_v(p_out, v_0);
        store_v(p_out + 16, v_1);
    }
}

void butterfly_1024(cint16 *x, cint16 *y, cint16 *tf)
{
	auto iteromg=begin_vector<32>(omg_1024);
    auto iterx0=begin_vector<32>(x);
    auto iterx1=begin_vector<32>(x+512);
    auto itery0=begin_vector<32>(y);
    auto itery1=begin_vector<32>(y+512);
    auto itertf0=begin_vector<32>(tf);
    auto itertf1=begin_vector<32>(tf+512);
    for (unsigned i = 0; i < 16; i ++)
    {
        auto acc_t = mul(*iteromg++, *iterx1++);
        vector<cint16, 32> v_t = acc_t.to_vector<cint16>(OMG_SHIFT);
        *itery1++=mul(sub(*iterx0, v_t),*itertf1++).to_vector<cint16>(TF_SHIFT);
        *itery0++=mul(add(*iterx0++, v_t),*itertf0++).to_vector<cint16>(TF_SHIFT);
    }
}

template<unsigned id>
void radix2_dit(input_window<cint16> *x_in, output_window<cint16> *y_out)
{
    cint16 *x = (cint16 *)x_in->ptr;
    cint16 *y = (cint16 *)y_out->ptr;

    // --------------------------------shuffle--------------------------------

    // aie::tile tile = aie::tile::current();
    // // printf("before shuffle: %llu\n", tile.cycles());

    for (unsigned i = 0; i < N_SWAP_2; i += 2)
        chess_unroll_loop(8)
    {
        int16 a=swap2[i],b=swap2[i+1];
        y[a] = x[b];
        y[b] = x[a];
    }
    for (unsigned i = 0; i < N_SWAP_4; i += 4)
        chess_unroll_loop(8)
    {
        int16 i0=swap4[i],i1=swap4[i+1],i2=swap4[i+2],i3=swap4[i+3];
        y[i3] = x[i0];
        y[i0] = x[i1];
        y[i1] = x[i2];
        y[i2] = x[i3];
    }
    // bit_mat = 3
    y[0] = x[0];
    y[48] = x[48];
    y[72] = x[72];
    y[120] = x[120];
    y[258] = x[258];
    y[306] = x[306];
    y[330] = x[330];
    y[378] = x[378];
    y[645] = x[645];
    y[693] = x[693];
    y[717] = x[717];
    y[765] = x[765];
    y[903] = x[903];
    y[951] = x[951];
    y[975] = x[975];
    y[1023] = x[1023];

    // bit_mat = 5
    // y[0] = x[0];
    // y[132] = x[132];
    // y[330] = x[330];
    // y[462] = x[462];
    // y[561] = x[561];
    // y[693] = x[693];
    // y[891] = x[891];
    // y[1023] = x[1023];

    // printf("shuffled: %llu\n", tile.cycles());

    // ----------------------------------dit----------------------------------

    auto iterout=begin_vector<MAX_VEC_LEN>(x);
    for (cint16* p=y;p!=y+N_POINT;p+=MAX_VEC_LEN)
    {
        auto iter=begin_vector<MAX_VEC_LEN>(mat_omg_8);
        auto m=mul(*iter++,*p);

        for (unsigned i=1;i<MAX_VEC_LEN;i++){
            m=mac(m,*iter++,*(p+i));
        }
        *iterout++=m.to_vector<cint16>(MAT_OMG_SHIFT);
    }

    // auto iterin=begin_vector<32>(y);
    // auto iterout=begin_vector<4>(x);
    // for (unsigned i=0;i<32;i++){
    //     auto itermat=begin_vector<8>(mat_omg_8);
    //     for (unsigned j=0;j<8;j++){
    //         auto res=sliding_mul_ops<4,8,1,4,1,cint16,cint16,cacc48>::mul(*itermat++,0,*iterin,0);
    //         *iterout++=res.to_vector<cint16>(MAT_OMG_SHIFT);
    //     }
    //     iterin++;
    // }

    // printf("l<=MAX_VEC_LEN: %llu\n", tile.cycles());
    
    butterfly_16(x, y);
    // printf("btf l=16: %llu\n", tile.cycles());
    butterfly_32(y, x);
    // printf("btf l=32: %llu\n", tile.cycles());
    butterfly(64, x, y,omg_64);
    // printf("btf l=64: %llu\n", tile.cycles());
    butterfly(128, y, x,omg_128);
    // printf("btf l=128: %llu\n", tile.cycles());
    butterfly(256, x, y,omg_256);
    // printf("btf l=256: %llu\n", tile.cycles());
    butterfly(512, y, x,omg_512);

    switch (id)
    {
    case 0:
        butterfly(1024, x, y,omg_1024);
        break;
    case 1:
        butterfly_1024(x, y,(cint16*)tf1);
        break;
    case 2:
        butterfly_1024(x, y,(cint16*)tf2);
        break;
    case 3:
        butterfly_1024(x, y,(cint16*)tf3);
        break;
    case 4:
        butterfly_1024(x, y,(cint16*)tf4);
        break;
    case 5:
        butterfly_1024(x, y,(cint16*)tf5);
        break;
    case 6:
        butterfly_1024(x, y,(cint16*)tf6);
        break;
    case 7:
        butterfly_1024(x, y,(cint16*)tf7);
        break;
    }

    // printf("dit: %llu\n", tile.cycles());

    return;
}