#ifndef JPEG_H
#define JPEG_H
#include"payloadextraction/payload_extraction.h"
#include"colorspaceconversion/colorspace_conversion.h"

class Jpeg{
    private:
    PayloadExtraction payload_extractor;
    ColorSpaceConversion colorspace_converter; 
};

#endif
