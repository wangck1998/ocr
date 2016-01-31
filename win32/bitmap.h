#include <windows.h>
/*
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

typedef struct tagBITMAPFILEHEADER {
  WORD   bfType;            // 0x424D, "BM"
  DWORD  bfSize;
  WORD   bfReserved1;
  WORD   bfReserved2;
  DWORD  bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
  DWORD  biSize;            // 40
  LONG   biWidth;
  LONG   biHeight;
  WORD   biPlanes;          // 1
  WORD   biBitCount;        // 1, 4, 8, 24
  DWORD  biCompression;     // BI_RGB£¬BI_RLE8£¬BI_RLE4£¬BI_BITFIELDS
  DWORD  biSizeImage;
  LONG   biXPelsPerMeter;
  LONG   biYPelsPerMeter;
  DWORD  biClrUsed;         // 0: 2^biBitCount
  DWORD  biClrImportant;
} BITMAPINFOHEADER;
*/

class Bitmap {
private:
  void init();
  void compute();
  BITMAPFILEHEADER bmFileHeader;
  BITMAPINFOHEADER bmInfoHeader;
  int lineSize; // used to locate the index (Get/Set Pixel)
  int width, height;
  unsigned char *vtData;

public:
  Bitmap();
  Bitmap(const char* const file);
  Bitmap(int width, int height, int *data[]);
  ~Bitmap();
  void SaveImage(const char* const file);

  int GetWidth();
  int GetHeight();
  int GetPixel(int r, int c);
  void SetPixel(int r, int c, int col);
  void Gray();
  void Binarizate(); // must be grayed at first
  void BinarizateBlock(int r1, int r2, int c1, int c2); 
  int ComputeThresholdValue(int r1, int r2, int c1, int c2); // for binarizate the image
};

#define GETR(x) ((x) & 255)
#define GETG(x) ((x) >> 8 & 255)
#define GETB(x) ((x) >> 16 & 255)
#define JOIN(r, g, b) ((int) (r) | ((int) (g) << 8) | ((int) (b) << 16))
