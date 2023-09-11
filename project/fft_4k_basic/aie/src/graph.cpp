#include "graph.h"

fft_4k_graph g;

#if defined(__AIESIM__) || defined(__X86SIM__)

int main(int argc,char** argv){
    g.init();
    g.run(1);
    g.end();
    return 0;
}

#endif