/******************************************************************************
 * Title          : p2.cc
 * Author         : Renat Khalikov
 * Created on     : September 26, 2017
 * Description    : labeling program that segments a binary image into several
 *                  connected regions
 * Purpose        : 
 * Usage          : ./p2 many_objects_1_p1_out.pgm many_objects_1_p2_out.pgm
 * Build with     : make all
 */
#include "image.h"
#include "DisjSets.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv){  
  if (argc!=3) {
    printf("Usage: %s {input binary image} {output labeled image}\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string output_file(argv[2]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  RasterScan(&an_image);
  
  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
    return 0;
  }
}
