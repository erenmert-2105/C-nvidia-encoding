#include <iostream>
#include <fstream>
#include "nvEncodeAPI.h"

// Input video file and output file paths
const char* inputFilePath = "C:\\Users\\erenm\\OneDrive\\Masaüstü\\c++ task\\rabbit.mp4";
const char* outputFilePath = "C:\\Users\\erenm\\OneDrive\\Masaüstü\\c++ task";

int main() {
    NVENCSTATUS nvStatus;
    NV_ENCODE_API_FUNCTION_LIST nvEncodeAPI;
    nvStatus = NvEncodeAPICreateInstance(&nvEncodeAPI);

    if (nvStatus != NV_ENC_SUCCESS) {
        std::cerr << "Failed to create NVENC instance" << std::endl;
        return 1;
    }
    nvStatus = nvEncodeAPI.nvEncOpenEncodeSession(nullptr, NV_ENC_DEVICE_TYPE_CUDA);
    if (nvStatus != NV_ENC_SUCCESS) {
        std::cerr << "Failed to open encode session" << std::endl;
        return 1;
    }

    // Setting up encoding parameters
    NV_ENC_INITIALIZE_PARAMS initializeParams = {};
    initializeParams.encodeConfig = new NV_ENC_CONFIG();
    initializeParams.encodeWidth = 1920; // Width of 1920x720
    initializeParams.encodeHeight = 720; // Height of 1920x720
    initializeParams.encodeGUID = NV_ENC_CODEC_H264_GUID; // H.264 codec
    initializeParams.presetGUID = NV_ENC_PRESET_P7_GUID; // Encoding preset
    initializeParams.enablePTD = 1;

    // Bitrate
    initializeParams.encodeConfig->rcParams.averageBitRate = 5000000; // 5 Mbps (adjust as needed)

    // Output settings for a H.264 format video file
    FILE* outputFile = fopen(outputFilePath, "wb");
    if (!outputFile) {
        std::cerr << "Failed to open output file for writing" << std::endl;
        return 1;
    }

    nvStatus = nvEncodeAPI.nvEncInitializeEncoder(nullptr, &initializeParams);
    if (nvStatus != NV_ENC_SUCCESS) {
        std::cerr << "Failed to initialize encoder" << std::endl;
        return 1;
    }

    // Loading the input video file
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input video file" << std::endl;
        return 1;
    }

    // Performing encoding and writing the encoded output to file
    uint32_t encodedBufferSize = initializeParams.encodeWidth * initializeParams.encodeHeight * 3 / 2; // Eklenen kısım: Kodun işlem yapacağı buffer boyutu
    uint8_t* encodedBuffer = new uint8_t[encodedBufferSize];
    while (!inputFile.eof()) {
        // Reading data from the input video file
        const int bufferSize = 5 * 1024 * 1024; // 5 MB buffer size
        uint8_t* inputBuffer = new uint8_t[bufferSize];
        inputFile.read(reinterpret_cast<char*>(inputBuffer), bufferSize);
        const uint32_t inputSize = static_cast<uint32_t>(inputFile.gcount());

        // Performing encoding
        NV_ENC_PIC_PARAMS encodePicParams = {};
        encodePicParams.version = NV_ENC_PIC_PARAMS_VER;
        encodePicParams.inputBuffer = inputBuffer;
        encodePicParams.inputWidth = initializeParams.encodeWidth;
        encodePicParams.inputHeight = initializeParams.encodeHeight;
        encodePicParams.inputPitch = initializeParams.encodeWidth;
        encodePicParams.inputTimeStamp = 0;
        encodePicParams.inputBufferParams = nullptr;
        encodePicParams.outputBitstream = encodedBuffer; 
        encodePicParams.completionEvent = nullptr;

        nvStatus = nvEncodeAPI.nvEncEncodePicture(nullptr, &encodePicParams);
        if (nvStatus != NV_ENC_SUCCESS) {
            std::cerr << "Failed to encode picture" << std::endl;
            return 1;
        }

        // Writing encoded data to the output file
        fwrite(encodedBuffer, 1, encodePicParams.bitstreamSizeInBytes, outputFile); 
    }

    // Cleanup
    delete[] encodedBuffer;
    delete[] initializeParams.encodeConfig;
    fclose(outputFile);
    inputFile.close();
    nvEncodeAPI.nvEncDestroyEncoder(); 
    nvEncodeAPI.nvEncCloseEncodeSession(); 

    return 0;
}
