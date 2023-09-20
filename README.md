# CCFSys Custom Computing Challenge 2023: Finals

> Participating School: East China Normal University (飞天小女警队)
>
> Team Members: Zhuoyan Bai （白卓岩）, Yixuan Du（堵仪萱）, Longshan Xu（徐珑珊）
> 
> Advisor: Dr. Fei Xu
>
> Result: Our Team was awarded the **first prize** in [CCFSys Customized Computing Challenge (CCC) 2023](https://ccfsys-ccc.github.io/2023/results/). 

## Project Overview

This branch represents the **final round design** submission. Based on the preliminary design, optimizations were made for 1K, 4K, and 8K-point AIE designs, as well as the connection between 8K-point PL and AIE.

The objective of this project is the FFT challenge. The overall progress from the preliminary round to the final round is as follows:

- Accomplished the AIE design, optimization, and simulation of small-scale FFT algorithms for 1024-point `cint16` and `cfloat` types;
- Explored the connection between AIE kernels, extended to larger point numbers (4k, 8k) of `cint16` type FFT;
- Established the data connection between PL and AIE, which can be invoked from the host side and runs on VCK5000 hardware.

## Code Execution
The code for the final round is in the `project` directory under the submitted `fft_8k_advanced` project, allowing for AIE simulation and hardware execution.

1. AIE Simulation

Execute the following code to run AIE simulation individually.
> Note: Before running `make`, ensure the `XPFM` path in the `Makefile`.

```shell
# Compile and execute AIE simulation
cd ./CCC2023/project/fft_8k_advanced/aie
make
make aieemu
```

2. Hardware Execution

The `sources/fft_8k/execution` folder contains the necessary `fft.xclbin` file, `host.exe` file, and input file `DataInFFTO.txt` for invoking PL and AIE through the host, as well as the output file `DataOutFFT0.txt` produced after running. To run on VCK5000, execute the following code.

```shell
# Clone the hacc_demo repository
git clone https://github.com/Xtra-Computing/hacc_demo.git

# Acquire VCK5000 compute node (modify the command based on the hacc_demo storage path)
./hacc_demo/env/vck5000_alloc 3
source ./hacc_demo/env/vck5000_env

# Run the executable in the execution folder of this project
./CCC2023/project/fft_8k_advanced/execution/host.exe

# Exit the node
./hacc_demo/env/vck5000_exit
```

After execution, the `.ipynb` file in the `sources/fft_8k_advanced/notebook` folder can be used to visualize the output results and verify them.

## Directory Description
The main directory structure for the final submission is as follows:

```
CCC2023
├── project
│   ├── fft_8k_advanced      Complete code for 8K-point FFT
│   │   ├── aie              AIE Design
│   │   ├── execution        Executable files
│   │   ├── host             Host-side code
│   │   ├── hw_link          Hardware linkage
│   │   ├── pl               PL Design
│   │   └── Makefile
│   │
│   └── fft_4k_basic         4K-point FFT AIE code
│
├── notebook
│   └── fft_8k.ipynb
└── README.md
```

## Design Document

Zhuoyan Bai, Yixuan Du, Longshan Xu, Fei Xu, "[基于AIE的快速傅里叶变换的算法优化与实现](https://github.com/icloud-ecnu/CCC2023/blob/main/pdf/CCFSys_CCC_2023.pdf)," in: CCFSys CCC 2023. Nanchang, China, August 4-5, 2023. [[PPT](https://github.com/icloud-ecnu/CCC2023/blob/main/pdf/CCC2023-ECNU-EN.pdf)]
