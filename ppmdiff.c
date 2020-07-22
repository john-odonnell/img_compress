#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "assert.h"
#include "pnm.h"
#include "uarray2.h"
#include "a2plain.h"

static bool opened_stdin = false;

static FILE *openread(const char *path) {
  if (path == NULL || !strcmp(path, "-")) {
    assert(!opened_stdin);
    opened_stdin = true;
    return stdin;
  } else {
    FILE *input = fopen(path, "rb");
    if (!input) {
      perror(path);
      exit(1);
    }
    return input;
  }
}

static inline double square(double x) {
  return x * x;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s file1.ppm file1.ppm\n\n"
                    "where one file may be - for standard input\n",
            argv[0]);
    exit(1);
  }
  FILE *in1 = openread(argv[1]);
  FILE *in2 = openread(argv[2]);
  Pnm_ppm ppm1 = Pnm_ppmread(in1, array2_methods_plain);
  Pnm_ppm ppm2 = Pnm_ppmread(in2, array2_methods_plain);
  fclose(in1);
  fclose(in2);
  if (abs((int)ppm1->width  - (int)ppm2->width)  > 1 ||
      abs((int)ppm1->height - (int)ppm2->height) > 1
      ) {
    fprintf(stderr, "Images differ too much in width or height\n");
    printf("1.0\n"); // maximum RMS error
    exit(1);
  }

  unsigned w = ppm1->width  < ppm2->width  ? ppm1->width  : ppm2->width;
  unsigned h = ppm1->height < ppm2->height ? ppm1->height : ppm2->height;

  double sumsq = 0.0;
  double d1 = ppm1->denominator;
  double d2 = ppm2->denominator;
  for (unsigned j = 0; j < h; j++) {
    for (unsigned i = 0; i < w; i++) {
      Pnm_rgb p1 = UArray2_at(ppm1->pixels, i, j);
      Pnm_rgb p2 = UArray2_at(ppm2->pixels, i, j);
      sumsq += square(p1->red/d1   - p2->red/d2)   +
               square(p1->green/d1 - p2->green/d2) + 
               square(p1->blue/d1  - p2->blue/d2);
    }
  }
  printf("%.4f\n", sqrt(sumsq / (w * h * 3)));
}
