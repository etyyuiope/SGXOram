# SGXOram by Nankai University
SGX ORAM implementation and evaluation

## Usage
* GCC >= v5.0.
* Make sure that your compiler supports C++11 standards.
```shell
make all CXX=<your-compiler>;
```
Then run the executable file in the command line by:
```shell
Usage:
  Simulator [OPTION...]

  -c, --constant arg  The constant multiplicated with the slot size. 
                      (default: 1)
  -f, --file arg      The file path of the data you want to load into the 
                      SGX. (default: ./input.data)
  -n, --number arg    The number of the total blocks. (default: 100000)
  -v, --verbose       Enable verbose mode
  -w, --way arg       The number of ways in the SGX tree. (default: 8)
  -h, --help          Print usage information.
  ```