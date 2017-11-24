/******************************************************************************
 * Title          : p3.cc
 * Author         : Renat Khalikov
 * Created on     : September 26, 2017
 * Description    : takes a labeled image as input, computes object attributes,
 *                  and generates the database of the objects
 * Purpose        : 
 * Usage          : ./p3 many_objects_1_p2_out.pgm 
 *                       many_objects_1_database.txt 
 *                       many_objects_1_p3_out.pgm
 * Build with     : make all
 */
#include "image.h"
#include "DisjSets.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv){  
  if (argc!=4) {
    printf("Usage: %s {input labeled image} {output database} {output image}\n", argv[0]);
    return 0;
  }
  const string input_image(argv[1]);
  const string output_database(argv[2]);
  const string output_image(argv[3]);

  Image an_image;
  if (!ReadImage(input_image, &an_image)) {
    cout <<"Can't open file " << input_image << endl;
    return 0;
  }

  ofstream output_filename(output_database);
  if (output_filename.fail()) {
    cerr << "Could not open: {output database}\n";
    exit(1); // 1 indicates an error occurred
  }
  ComputeObjectAttributes(output_filename, &an_image);
  output_filename.close();
  
  if (!WriteImage(output_image, an_image)){
    cout << "Can't write to file " << output_image << endl;
    return 0;
  }
}
