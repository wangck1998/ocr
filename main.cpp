#include "Bitmap.h"
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
  string fileName = "a.bmp";
  if (argc == 2) fileName = string(argv[1]);
  Bitmap *example = new Bitmap(fileName.c_str());
  int width = example->GetWidth();
  int height = example->GetHeight();
  /*
  printf("%d * %d\n", width, height);
  for (int i = 0; i < width; i++) {
    int color = example->GetPixel(height - 1, i);
    printf("%d %d %d\n", GETR(color), GETG(color), GETB(color));
  }
  */
  example->Gray();
  example->SaveImage(("gray_" + fileName).c_str());
  example->Binarizate();
  example->SaveImage(("bin_" + fileName).c_str());
  delete example;
  return 0;
}
