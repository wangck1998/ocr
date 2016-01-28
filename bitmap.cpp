#include "bitmap.h"
#include <cmath>
#include <cstdio>
#include <algorithm>
using namespace std;

void Bitmap::init() {
  vtData = NULL;
}

int Bitmap::GetWidth() {
  return width;
}

int Bitmap::GetHeight() {
  return height;
}

void Bitmap::compute() {
  lineSize = bmInfoHeader.biWidth * 3 
    + (bmInfoHeader.biWidth & 3);
  width = bmInfoHeader.biWidth;
  height = bmInfoHeader.biHeight;
}

int Bitmap::GetPixel(int i, int j) {
#ifndef NDEBUG
  if (i < 0 || i >= height || j < 0 || j >= width) {
    fprintf(stderr, "INDEX ERROR.\n");
    return -1;
  }
#endif
  i = height - 1 - i;
  int idx = lineSize * i + j * 3;
  return JOIN(vtData[idx + 2], 
      vtData[idx + 1],
      vtData[idx]);
}

void Bitmap::SetPixel(int i, int j, int color) {
#ifndef NDEBUG
  if (i < 0 || i >= height || j < 0 || j >= width) {
    fprintf(stderr, "INDEX ERROR.\n");
    return;
  }
#endif
  i = height - 1 - i;
  int idx = lineSize * i + j * 3;
  vtData[idx + 2] = (unsigned char) GETR(color);
  vtData[idx + 1] = (unsigned char) GETG(color);
  vtData[idx]     = (unsigned char) GETB(color);
}

Bitmap::Bitmap() {
}

Bitmap::Bitmap(const char* const file) {
  init();
  FILE *f = fopen(file, "rb");
  if (! f) {
    fprintf(stderr, "can't open %s\n", file);
    return;
  }
  fread(&bmFileHeader, 1, sizeof(bmFileHeader), f);
  fread(&bmInfoHeader, 1, sizeof(bmInfoHeader), f);
  vtData = new unsigned char[bmFileHeader.bfSize];
  fread(vtData, 1, bmFileHeader.bfSize, f);
  fclose(f);

  compute();
}

void Bitmap::SaveImage(const char* const file) {
  FILE *f = fopen(file, "wb");
  if (! f) {
    fprintf(stderr, "can't open %s\n", file);
    return;
  }
  fwrite(&bmFileHeader, 1, sizeof(bmFileHeader), f);
  fwrite(&bmInfoHeader, 1, sizeof(bmInfoHeader), f);
  fwrite(vtData, 1, bmFileHeader.bfSize, f);
  fclose(f);
}

Bitmap::~Bitmap() {
  if (vtData) delete[] vtData;
}

void Bitmap::Gray() {
  for (int i = height - 1; i >= 0; i--)
    for (int j = 0; j < width; j++) {
      int color = GetPixel(i, j);
      int gray = (int) round((0.3 * GETR(color)) + (0.59 * GETG(color)) + (0.11 * GETB(color)));
#ifndef NDEBUG
      if (gray < 0 || gray > 255) {
        fprintf(stderr, "VALUE ERROR\n");
        return;
      }
#endif
      SetPixel(i, j, JOIN(gray, gray, gray));
    }
}

// divide into 5 blocks to handle
void Bitmap::Binarizate() {
  int blockRSize = height / 5 + 1;
  int blockCSize = width / 5 + 1;
  for (int i = 0; i < height; i += blockRSize)
    for (int j = 0; j < width; j += blockCSize)
      BinarizateBlock(i, min(height, i + blockRSize), j, min(width, j + blockCSize));
}

void Bitmap::BinarizateBlock(int r1, int r2, int c1, int c2) {
  int temp = ComputeThresholdValue(r1, r2, c1, c2);
#ifndef NDEBUG
  if (temp < 0 || temp > 256) {
    fprintf(stderr, "VALUE ERROR\n");
    return;
  }
#endif
  for (int i = r1; i < r2; i++)
    for (int j = c1; j < c2; j++) {
      int color = GETR(GetPixel(i, j)) >= temp ? 255 : 0;
      SetPixel(i, j, JOIN(color, color, color));
    }
}

int Bitmap::ComputeThresholdValue(int r1, int r2, int c1, int c2) {
  int bestK = 0;
  double bestValue = 0;

  int *count = new int[256];
  double *countValue = new double[256];
  //memset(count, 0, 256 * sizeof(int)); 
  fill(count, count + 256, 0);

  for (int i = r1; i < r2; i++)
    for (int j = c1; j < c2; j++)
      count[GETR(GetPixel(i, j))]++;
  countValue[0] = 0;
  for (int i = 1; i < 256; i++) {
    countValue[i] = countValue[i - 1] + (double) count[i] * i;
    count[i] += count[i - 1];
  }

  double muT = countValue[255] / ((r2 - r1) * (c2 - c1));

  /* if variance smaller than 100, we suppose it's a white block. */ 
  double variance = 0;
  for (int i = 0; i < 256; i++) 
    variance += (muT - i) * (muT - i) * (count[i] - (i ? count[i - 1] : 0));
  variance /= (r2 - r1) * (c2 - c1);
  if (variance < 100.) return 0;

  for (int k = 1; k <= 256; k++) {
    int omega1 = count[k - 1];
    double mu1 = countValue[k - 1] / omega1;
    int omega2 = count[255] - count[k - 1];
    double mu2 = (countValue[255] - countValue[k - 1]) / omega2;
    double temp = (mu1 - muT) * (mu1 - muT) * omega1;
    temp +=       (mu2 - muT) * (mu2 - muT) * omega2;
    if (temp > bestValue) {
      bestValue = temp;
      bestK = k;
    }
  }

  delete[] count;
  delete[] countValue;
  return bestK;
}
