/******************************************************************************
 * Title          : p4.cc
 * Author         : Renat Khalikov
 * Created on     : September 26, 2017
 * Description    : recognizes objects from the database
 * Purpose        : 
 * Usage          : ./p4 many_objects_1_p2_out.pgm 
 *                  two_objects_database.txt 
 *                  many_objects_1_p4_out.pgm
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
    printf("Usage: %s {input labeled image} {input database} {output image}\n", argv[0]);
    return 0;
  }
  const string input_image(argv[1]);
  string input_database(argv[2]);
  const string output_image(argv[3]);

  Image an_image;
  if (!ReadImage(input_image, &an_image)) {
    cout <<"Can't open file " << input_image << endl;
    return 0;
  }

  ifstream database_file(input_database);
  if (database_file.fail()) {
    cerr << "Could not open: {input database}\n";
    exit(1); // 1 indicates an error occurred
  }
  CompareObjectAttributes(database_file, &an_image);
  database_file.close();
  
  if (!WriteImage(output_image, an_image)){
    cout << "Can't write to file " << output_image << endl;
    return 0;
  }
}
