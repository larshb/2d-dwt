# 2D-DWT

5/3 2D Discrete Wavelet Transform reference implementation tweaked from [openJPEG](https://github.com/uclouvain/openjpeg).

Currently only works with 512x512 image tiles.

## Usage

Use the provided MATLAB script (`run_test.m`) to test the 2D-DWT softcore:

```
dwt <n_resolutions> <input-file>.bin <output-file>.bin
```

## Building

The source code in `/src` is provided for reference and can be compiled with any standard C compiler:

```
gcc src/main.c src/dwt.c -o dwt
```
