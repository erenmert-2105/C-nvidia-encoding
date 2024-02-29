# NVIDIA NVENC Video Encoding Example

This example demonstrates video encoding using NVIDIA's NVENC API. NVENC is an API that provides video encoding and acceleration on NVIDIA GPUs. In this example, there is a simple code snippet for encoding a H.264 video file.

## Installation

1. **NVIDIA GPU**: You need to have an NVIDIA GPU to run this example.
2. **NVIDIA Video Codec SDK**: Download and install the [NVIDIA Video Codec SDK](https://developer.nvidia.com/nvidia-video-codec-sdk).
3. **C++ Compiler**: You will need a C++ compiler to compile the code.
4. **nvEncodeAPI.h and nvEncodeAPI64.dll**: These header file and library are part of the NVIDIA Video Codec SDK.

## Usage

1. Place `nvEncodeAPI.h` header file and `nvEncodeAPI64.dll` library in the directory where your code resides.
2. Specify the video file you want to encode, such as `rabbit.mp4`, and specify where the output file should be saved.
3. Compile and run your code.

