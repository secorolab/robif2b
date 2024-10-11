# robif2b - building blocks for robotic interfaces

## Dependencies

* Simple Open EtherCAT Master ([SOEM](https://github.com/OpenEtherCATsociety/SOEM))  - for communication via EtherCAT
* Kortex API 2.6.0 - for communication with Kinova Gen3 arms

## Workspace

1. Create a workspace folder and clone the repository

    ```bash
    mkdir ~/<ws> && cd ~/<ws>

    mkdir src build install
    cd src
    ```

## Building dependencies

### SOEM

1. Clone the SOEM repository

    ```bash

    git clone https://github.com/OpenEtherCATsociety/SOEM.git
    ```

2. Replace `STATIC` with `SHARED` on line 72 in CMakeList.txt of SOEM

    ``` cmake
    add_library(soem SHARED
      ...
    ```

3. Build the SOEM library

    ``` bash
    cd ~/<ws>/build
    # create a build folder
    mkdir soem && cd soem

    # build the library
    cmake -DCMAKE_INSTALL_PREFIX=~/<ws>/install -DCMAKE_BUILD_TYPE=Release ../../src/SOEM

    # install the library
    make install
    ```

### robif2b

1. Building robif2b

    Before proceeding with the building, make sure the eternet port name is correct in the script. For example, in line 78 of freddy_2_example.c file, modify the value of `state.ecat.ethernet_if` if necessary.

2. Build robif2b library

    ```bash
    cd ~/<ws>/build
    # create a build folder
    mkdir robif2b && cd robif2b

    # build the library
    cmake -DCMAKE_INSTALL_PREFIX=~/<ws>/install -DCMAKE_PREFIX_PATH=/home/<abs_path_to_ws>/build/ -DCMAKE_C_FLAGS="-I/home/<abs_path_to_ws>/install/include" -DENABLE_ETHERCAT=ON -DENABLE_KELO=ON -DCMAKE_BUILD_TYPE=Release ../../src/robif2b

    # `ENABLE_ETHERCAT=ON` to enable EtherCAT communication
    # `ENABLE_KELO=ON` to enable Kelo drive communication
    # `ENABLE_ENABLE_KORTEX=ON` to enable Kinova Kortex drive communication
    # `ENABLE_KORTEX_API_AUTO_DOWNLOAD=ON` to download the Kortex API automatically

    # install the library
    make install
    ```

### Running example script

```bash
cd ~/<ws>/build
sudo ./robif2b/src/example/freddy_2_example
```
