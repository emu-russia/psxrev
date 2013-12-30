#include "Image.h"

//File information header
//provides general information about the file
struct BitmapFileHeader { 
  WORD    bfType; 
  DWORD   bfSize; 
  WORD    bfReserved1; 
  WORD    bfReserved2; 
  DWORD   bfOffBits; 
};

//Bitmap information header
//provides information specific to the image data
struct BitmapInfoHeader {
  DWORD  biSize; 
  long   biWidth; 
  long   biHeight; 
  WORD   biPlanes; 
  WORD   biBitCount; 
  DWORD  biCompression; 
  DWORD  biSizeImage; 
  LONG   biXPelsPerMeter; 
  LONG   biYPelsPerMeter; 
  DWORD  biClrUsed; 
  DWORD  biClrImportant; 
};

//Colour palette
struct RGBQuad {
  BYTE    rgbBlue; 
  BYTE    rgbGreen; 
  BYTE    rgbRed; 
  BYTE    rgbReserved; 
};

bool Image::loadBMP(const std::string &fname)
{
	clear();

	// read bitmap header
	BitmapFileHeader fheader;
	BitmapInfoHeader iheader;
	for (int i=0; i<14; ++i) ((char*)&fheader)[i] = 3;

	FILE *fp = fopen(fname.c_str(), "rb");
	if (!fp)
		return false;

	fread(&fheader.bfType, 2, 1, fp);
	fread(&fheader.bfSize, 4, 1, fp);
	fread(&fheader.bfReserved1, 2, 1, fp);
	fread(&fheader.bfReserved2, 2, 1, fp);
	fread(&fheader.bfOffBits, 4, 1, fp);

	fread(&iheader, 40, 1, fp);
/*	fread(&iheader.biSize, 4, 1, fp);
	fread(&iheader.biWidth, 4, 1, fp);
	fread(&iheader.biHeight, 4, 1, fp);
	fread(&iheader.biPlanes, 2, 1, fp);
	fread(&iheader.biBitCount, 2, 1, fp);*/

	m_width = iheader.biWidth;
	m_height = abs(iheader.biHeight);
	ASSERT(iheader.biPlanes == 1);

	if (iheader.biCompression != 0) {
		Console::error("cannot handle compressed bitmaps\n");
		fclose(fp);
		return false;
	}

	// TEMP!
	if (iheader.biBitCount != 24) {
		Console::error("bitmap doesnt have 24bit color. Only 24-bit bitmaps can be handled\n");
		fclose(fp);
		return false;
	}
	m_nChannels = 3;
	m_bytesPerPixel = iheader.biBitCount / 8;
	m_bytesPerChannel = m_bytesPerPixel / m_nChannels;
	m_format = Image::I8BITS;

	// load the palette, if any
	RGBQuad *palette = 0;
	if (iheader.biBitCount < 24) {
		LONG numCols = 1 << iheader.biBitCount;
		palette = new RGBQuad[numCols];
		fread(palette, 4, numCols, fp);
		// TEMP: palette not used!
	}

	// allocate storage for bitmap
	m_data.resize(m_width*m_height*m_bytesPerPixel);

	// load bitmap data to temporary storage
	long size = fheader.bfSize - fheader.bfOffBits;
	if (size <= 0)
		Console::error("bad bitmap size\n");
	BYTE *tmpData = new BYTE[size];
	fread(tmpData, size, 1, fp);

	// get the width of each line in bytes with and without padding to a DWORD boundary
	LONG byteWidth, padWidth;
	byteWidth = padWidth = (LONG)(m_width * (double)iheader.biBitCount / 8.0);
	while (padWidth % 4)
		padWidth++;

	if (iheader.biHeight > 0) {
		// bitmap is stored inverted
		for (int i=0; i<(int)m_height; ++i) {
			LONG lineStart = (m_height - i - 1)*padWidth;
			for (int j=0; j<(int)m_width; ++j) {
				m_data[(i*m_width + j)*m_bytesPerPixel    ] = *(tmpData + lineStart + j*m_bytesPerPixel + 2);
				m_data[(i*m_width + j)*m_bytesPerPixel + 1] = *(tmpData + lineStart + j*m_bytesPerPixel + 1);
				m_data[(i*m_width + j)*m_bytesPerPixel + 2] = *(tmpData + lineStart + j*m_bytesPerPixel);
			}
		}
	}
	else {
		// bitmap doesnt need to be inverted
		for (size_t i=0; i<m_height; ++i) {
			memcpy(&m_data[i*byteWidth], tmpData + i*padWidth, byteWidth);
		}
	}

	// clean-up
	delete[] palette;
	delete[] tmpData;

	Console::print("loaded BMP image %d x %d\n", m_width, m_height);

	return true;
}