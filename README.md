## Setting up robif2b

### Dependencies

 * Simple Open EtherCAT Master (SOEM) for communication via EtherCAT

### Building SOEM

Clone SOEM library in the src folder of robif2b (the current repository) [path is optional]

```bash
git clone https://github.com/OpenEtherCATsociety/SOEM
```

Replace `STATIC` with `SHARED` on line 72 in CMakeList.txt of SOEM,

```
add_library(soem SHARED
  ...
```

Build the SOEM library,

```
cd robif2b
mkdir install 
cd SOEM
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<path to created install folder>/install ..
make install
```

### Building robif2b

Before proceeding with the building, make sure the eternet port name is correct in the script. For example, in line 78 of freddy_2_example.c file, modify the value of `state.ecat.ethernet_if` if required.

Commands to build robif2b are,

```bash
cd robif2b
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=<path to created install folder>/install ..
cmake -DCMAKE_C_FLAGS="-I<path to created install folder>/install/include" -DENABLE_PACKAGE_REGISTRY=on -DENABLE_ETHERCAT=ON -DENABLE_KELO=ON ..
make install
```

### Running example script

```bash
cd robif2b
sudo ./build/src/example/freddy_2_example
```