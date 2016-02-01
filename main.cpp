#include <bits/stdc++.h>

unsigned char scanChar() {
  int t = getchar();
  assert(t != -1);
  putchar(t);
  return t;
}

int scanInt() {
  int ret = 0;
  int c = ' ';
  while (! isdigit(c)) c = scanChar();
  do {
    ret = ret * 10 + (c & 15);
    c = scanChar();
  }while (isdigit(c));
  return ret;
}

int w, h;
int s;
// current line
double* g0;
// last line
double* g1;

unsigned char* p; 

double fix;

int main(int argc, char* argv[]) {
  if (argc != 4) return -1;
  if (! freopen(argv[1], "r", stdin)) return -2;
  if (! freopen(argv[2], "w", stdout)) return -2;
  if (! (fix = atof(argv[3]))) return -2;
  assert(scanChar() == 'P');
  assert(scanChar() == '6');
  w = scanInt();
  h = scanInt();
  assert(scanInt() == 255);
  s = w / 8;
  
  p = new unsigned char[w];
  g0 = new double[w];
  g1 = new double[w];

  int i, j;
  int r, g, b, c;
  double t;
  
  g1[w - 1] = 127 * s;
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      r = getchar(); g = getchar(); b = getchar();
      t = .3 * r + .59 * g + .11 * b;
      p[(i & 1) ? w - j - 1: j] = (int) round(t);
    }
    
    for (j = 0; j < w; j++) {
      g0[j] = (j ? g0[j - 1] : g1[w - 1]) * ((double) (s - 1) / s) + p[j];
      t = i ? (g0[j] + g1[w - j - 1]) / 2 : g0[i];
      p[j] = (unsigned char) (p[j] < t / s * fix);
    }
    
    for (j = 0; j < w; j++) {
      c = p[(i & 1) ? w - j - 1 : j] ? 0 : 255;
      putchar(c); putchar(c); putchar(c);
    }
    std::swap(g0, g1);
  }
  
  delete[] p;
  delete[] g0;
  delete[] g1;
  
  return 0;
}
