//#include "fann.h"
//#include "parroto.h"
//extern ParrotObserver parroto;

/*
 * encoder.c
 *
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include "datatype.h"
#include "jpegconfig.h"
#include "prototype.h"

#include "rgbimage.h"

#include <time.h>

#include "tf.hpp"

UINT8	Lqt [BLOCK_SIZE];
UINT8	Cqt [BLOCK_SIZE];
UINT16	ILqt [BLOCK_SIZE];
UINT16	ICqt [BLOCK_SIZE];

INT16	Y1 [BLOCK_SIZE];
INT16	Y2 [BLOCK_SIZE];
INT16	Y3 [BLOCK_SIZE];
INT16	Y4 [BLOCK_SIZE];
INT16	CB [BLOCK_SIZE];
INT16	CR [BLOCK_SIZE];
INT16	Temp [BLOCK_SIZE];
UINT32 lcode = 0;
UINT16 bitindex = 0;

INT16 global_ldc1;
INT16 global_ldc2;
INT16 global_ldc3;



UINT8* encodeImage(
	RgbImage* srcImage,
	UINT8 *outputBuffer,
	UINT32 qualityFactor,
	UINT32 imageFormat
) {


	global_ldc1 = 0;
	global_ldc2 = 0;
	global_ldc3 = 0;




	/** Quantization Table Initialization */
	initQuantizationTables(qualityFactor);

	srand(time(NULL));


	UINT16 i, j;
	/* Writing Marker Data */
	outputBuffer = writeMarkers(outputBuffer, imageFormat, srcImage->w, srcImage->h);
	for (i = 0; i < srcImage->h; i += 8) {
		for (j = 0; j < srcImage->w; j += 8) {
			readMcuFromRgbImage(srcImage, j, i, Y1);
			/* Encode the data in MCU */
			//outputBuffer = encodeMcu(imageFormat, outputBuffer);
			outputBuffer = encodeMcu(imageFormat, outputBuffer);
		}
	}

	/* Close Routine */
	closeBitstream(outputBuffer);

	return outputBuffer;
}

UINT8* encodeMcu(
	UINT32 imageFormat,
	UINT8 *outputBuffer
) {

	levelShift(Y1);

	float dataIn [BLOCK_SIZE];
	float dataOut[BLOCK_SIZE];

	for (int i = 0; i < BLOCK_SIZE; ++i)
	{
		dataIn[i] = Y1[i] / 256.;
	}

	bool isNN = true;

// #pragma parrot(input, "jpeg", [64]dataIn)
// parroto.write("jpeg", 0, dataIn, 64, 0, 0);
// 
// 	dct(Y1);
// 	quantization(Y1, ILqt);
// 
// 	for (int i = 0; i < BLOCK_SIZE; ++i)
// 	{
// 		dataOut[i] = Temp[i] / 256.;
// 	}
// 	isNN = false;
// 
// parroto.write("jpeg", 1, dataOut, 64, -0.9, 0.9);
// #pragma parrot(output, "jpeg", [64]<-0.9; 0.9>dataOut)
	tfInfer(64,dataIn,64,dataOut);

	for(int i = 0; i < BLOCK_SIZE; ++i)
	{
		Temp[i] = dataOut[i] * 256.0;
	}
	if(isNN)
	{
		for(int i = 8; i < BLOCK_SIZE; ++i)
		{
			Temp[i] = 0.0;
		}
	}

	outputBuffer = huffman(1, outputBuffer);

	return outputBuffer;
}
