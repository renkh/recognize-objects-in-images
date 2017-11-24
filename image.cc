
// Class for representing a 2D gray-scale image,
// with support for reading/writing pgm images.
// To be used in Computer Vision class.

#include "image.h"
#include "DisjSets.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <array>
#include <string>   // getline()

using namespace std;

namespace ComputerVisionProjects {

Image::Image(const Image &an_image){
  AllocateSpaceAndSetSize(an_image.num_rows(), an_image.num_columns());
  SetNumberGrayLevels(an_image.num_gray_levels());

  for (size_t i = 0; i < num_rows(); ++i)
    for (size_t j = 0; j < num_columns(); ++j){
      SetPixel(i,j, an_image.GetPixel(i,j));
    }
}

Image::~Image(){
  DeallocateSpace();
}

void
Image::AllocateSpaceAndSetSize(size_t num_rows, size_t num_columns) {
  if (pixels_ != nullptr) DeallocateSpace();
  pixels_ = new int*[num_rows];
  for (size_t i = 0; i < num_rows; ++i)
    pixels_[i] = new int[num_columns];

  num_rows_ = num_rows;
  num_columns_ = num_columns;
}

void
Image::DeallocateSpace() {
  for (size_t i = 0; i < num_rows_; i++)
    delete pixels_[i];
  delete pixels_;
  pixels_ = nullptr;
  num_rows_ = 0;
  num_columns_ = 0;
}

bool ReadImage(const string &filename, Image *an_image) {  
  if (an_image == nullptr) abort();
  FILE *input = fopen(filename.c_str(),"rb");
  if (input == 0) {
    cout << "ReadImage: Cannot open file" << endl;
    return false;
  }
  
  // Check for the right "magic number".
  char line[1024];
  if (fread(line, 1, 3, input) != 3 || strncmp(line,"P5\n",3)) {
    fclose(input);
    cout << "ReadImage: Expected .pgm file" << endl;
    return false;
  }
  
  // Skip comments.
  do
    fgets(line, sizeof line, input);
  while(*line == '#');
  
  // Read the width and height.
  int num_columns,num_rows;
  sscanf(line,"%d %d\n", &num_columns, &num_rows);
  an_image->AllocateSpaceAndSetSize(num_rows, num_columns);
  

  // Read # of gray levels.
  fgets(line, sizeof line, input);
  int levels;
  sscanf(line,"%d\n", &levels);
  an_image->SetNumberGrayLevels(levels);

  // read pixel row by row.
  for (int i = 0; i < num_rows; ++i) {
    for (int j = 0;j < num_columns; ++j) {
      const int byte=fgetc(input);
      if (byte == EOF) {
        fclose(input);
        cout << "ReadImage: short file" << endl;
        return false;
      }
      an_image->SetPixel(i, j, byte);
    }
  }
  
  fclose(input);
  return true; 
}

bool WriteImage(const string &filename, const Image &an_image) {  
  FILE *output = fopen(filename.c_str(), "w");
  if (output == 0) {
    cout << "WriteImage: cannot open file" << endl;
    return false;
  }
  const int num_rows = an_image.num_rows();
  const int num_columns = an_image.num_columns();
  const int colors = an_image.num_gray_levels();

  // Write the header.
  fprintf(output, "P5\n"); // Magic number.
  fprintf(output, "#\n");  // Empty comment.
  fprintf(output, "%d %d\n%03d\n", num_columns, num_rows, colors);

  for (int i = 0; i < num_rows; ++i) {
    for (int j = 0; j < num_columns; ++j) {
      const int byte = an_image.GetPixel(i , j);
      if (fputc(byte,output) == EOF) {
	    fclose(output);
            cout << "WriteImage: could not write" << endl;
	    return false;
      }
    }
  }

  fclose(output);
  return true; 
}

// Implements the Bresenham's incremental midpoint algorithm;
// (adapted from J.D.Foley, A. van Dam, S.K.Feiner, J.F.Hughes
// "Computer Graphics. Principles and practice", 
// 2nd ed., 1990, section 3.2.2);  
void
DrawLine(int x0, int y0, int x1, int y1, int color,
  Image *an_image) {  
  if (an_image == nullptr) abort();

  #ifdef SWAP
  #undef SWAP
  #endif
  #define SWAP(a,b) {a^=b; b^=a; a^=b;}

  const int DIR_X = 0;
  const int DIR_Y = 1;
  
  // Increments: East, North-East, South, South-East, North.
  int incrE, incrNE, incrS, incrSE, incrN;     
  int d;         /* the D */
  int x,y;       /* running coordinates */
  int mpCase;    /* midpoint algorithm's case */
  int done;      /* set to 1 when done */
  
  int xmin = x0;
  int xmax = x1;
  int ymin = y0;
  int ymax = y1;
  
  int dx = xmax - xmin;
  int dy = ymax - ymin;
  int dir;

  if (dx * dx > dy * dy) {  // Horizontal scan.
    dir=DIR_X;
    if (xmax < xmin) {
      SWAP(xmin, xmax);
      SWAP(ymin , ymax);
    } 
    dx = xmax - xmin;
    dy = ymax - ymin;

    if (dy >= 0) {
      mpCase = 1;
      d = 2 * dy - dx;      
    } else {
      mpCase = 2;
      d = 2 * dy + dx;      
    }

    incrNE = 2 * (dy - dx);
    incrE = 2 * dy;
    incrSE = 2 * (dy + dx);
  } else {// vertical scan.
    dir = DIR_Y;
    if (ymax < ymin) {
      SWAP(xmin, xmax);
      SWAP(ymin, ymax);
    }
    dx = xmax - xmin;
    dy = ymax-ymin;    

    if (dx >=0 ) {
      mpCase = 1;
      d = 2 * dx - dy;      
    } else {
      mpCase = 2;
      d = 2 * dx + dy;      
    }

    incrNE = 2 * (dx - dy);
    incrE = 2 * dx;
    incrSE = 2 * (dx + dy);
  }
  
  /// Start the scan.
  x = xmin;
  y = ymin;
  done = 0;

  while (!done) {
    an_image->SetPixel(x,y,color);

    // Move to the next point.
    switch(dir) {
      case DIR_X: 
      if (x < xmax) {
       switch(mpCase) {
         case 1:
         if (d <= 0) {
          d += incrE;  
          x++;
        } else {
          d += incrNE; 
          x++; 
          y++;
        }
        break;

        case 2:
        if (d <= 0) {
          d += incrSE; 
          x++; 
          y--;
        } else {
          d += incrE;  
          x++;
        }
        break;
      } 
    } else {
     done=1;
   }     
   break;

   case DIR_Y: 
   if (y < ymax) {
    switch(mpCase) {
     case 1:
     if (d <= 0) {
       d += incrE;  
       y++;
     } else {
       d += incrNE; 
       y++; 
       x++;
     }
     break;

     case 2:
     if (d <= 0) {
      d += incrSE; 
      y++; 
      x--;
    } else {
      d += incrE;  
      y++;
    }
    break;
	  } // mpCase
        } // y < ymin 
        else {
         done=1;
       }
       break;    
     }
   }
 }

/**
 * ConvertToBinary( ) sets image pixels to 0 if its value is below threshold
 * and 1 if its value is above threshold
 * 
 * @param {int} threshold_value: the threshold value
 * @param {Image} an_image: input image
 */
void ConvertToBinary(const int threshold_value, Image *an_image) {
  if (an_image == nullptr) abort();
  int pixel;
  int row = an_image->GetNumberOfRows();
  int column = an_image->GetNumberOfColumns();
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      pixel = an_image->GetPixel(i,j);
      if (pixel <= threshold_value)
        an_image->SetPixel(i,j,0);
      else
        an_image->SetPixel(i,j,1);
    }
  }
  an_image->SetNumberGrayLevels(1);
}

/**
 * RasterScan( ) performs a double pass sequential search using
 * 4-way connectivity
 * 
 * @param {Image} an_image: input image
 */
void RasterScan(Image *an_image) {
  // matrix dimensions
  int row = an_image->GetNumberOfRows();
  int column = an_image->GetNumberOfColumns();

  DisjSets equivalence_table(1000);

  // create a region counter
  int region_counter = 0;
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      // pixel is the current pixel being looked at
      int pixel = an_image->GetPixel(i,j);
      // found first foreground (non-zero) object
      if (pixel != 0) {
        // For every pixel check the north and west pixel
        // (when considering 4-connectivity)
        // condition ? value_if_true : value_if_false 
        int north_pixel = (i == 0) ? 0 : an_image->GetPixel(i-1, j);  
        int west_pixel = (j == 0) ? 0 : an_image->GetPixel(i, j-1); 
        // If none of the neighbors fit the criterion 
        // then assign pixel to region value of the region counter. 
        // Increment region counter.
        if (north_pixel == 0 && west_pixel == 0) {
          pixel = ++region_counter;
          //std::cout << pixel << " ";
        }

        // If only one neighbor fits the criterion assign pixel to that region.
        else if (west_pixel != 0 && north_pixel == 0) {
          pixel = west_pixel;
        } 
        else if (west_pixel == 0 && north_pixel != 0) {
          pixel = north_pixel;
        }

        // If multiple neighbors match and are all members of the same region, 
        // assign pixel to their region.
        else if (west_pixel != 0 && north_pixel != 0)
        {
          if (west_pixel == north_pixel) {
            pixel = west_pixel;
          }
          // If multiple neighbors match and are members of different regions, 
          // assign pixel to one of the regions (it doesn't matter which one).
          // Indicate that all of these regions are equivalent.
          else {
            int west = equivalence_table.find(west_pixel);
            int north = equivalence_table.find(north_pixel);
            equivalence_table.unionSets(west, north); 
            pixel = west;
          }
        }
        an_image->SetPixel(i,j,pixel);
        //std::cout << pixel << " ";
      }
    }
  }

  // scan image again, assigning all equivalent regions the same region value.
  int region_value[255];
  for (int i = 0; i < 256; ++i)
  {
    region_value[i] = -1;
  }
  int new_region = 0;
  //int new_value;
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      int pixel = equivalence_table.find(an_image->GetPixel(i,j));
      if (pixel != 0) {
        if (region_value[pixel] != -1) {
          an_image->SetPixel(i,j,region_value[pixel]);
        }
        else if (region_value[pixel] == -1)
        {
          region_value[pixel] = ++new_region;
          an_image->SetPixel(i,j,new_region);
        }
      }
    }
  }
  //PrintImageToCout(an_image);
  //equivalence_table.Print();
  //cout << "Number of objects: " << new_region << endl;
  an_image->SetNumberGrayLevels(new_region);
}

/**
 * ComputeObjectAttributes( ) computes attributes that serve as object model
 * database. Atrributes include object label, row position of the center,
 * column position of the center, the minimum moment of inertia,
 * and the orientation
 * 
 * @param {ostream} output_file: the output database file containing attributes
 * @param {Image} an_image: input image
 */
void ComputeObjectAttributes(std::ostream &output_file, Image *an_image) {
  size_t number_of_obects = an_image->GetNumberGrayLevels();
  std::vector<int> area(number_of_obects+1, 0);
  std::vector<int> x_axis(number_of_obects+1, 0);
  std::vector<int> y_axis(number_of_obects+1, 0);
  std::vector<int> x_axis_squared(number_of_obects+1, 0);
  std::vector<int> y_axis_squared(number_of_obects+1, 0);
  std::vector<int> xy(number_of_obects+1, 0);

  int row = an_image->GetNumberOfRows();
  int column = an_image->GetNumberOfColumns();
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      int pixel = an_image->GetPixel(i,j);
      if (pixel != 0) {
        area[pixel] += 1;
        x_axis[pixel] += i;
        y_axis[pixel] += j;
        x_axis_squared[pixel] += i*i;
        y_axis_squared[pixel] += j*j;
        xy[pixel] += i*j;
      }
    }
  }
  // header
  output_file << "object label | " 
              << "row position of the center | " 
              << "column position of the center | " 
              << "minimum moment of inertia | " 
              << "orientation" << endl;

  for (int i = 1; i < number_of_obects+1; ++i) {
    // get the area and center of objects
    // center of the object (x, y)
    // x = (1/A)∑∑i bij
    // y = (1/A)∑∑j bij
    double x_pos_of_center = x_axis[i] / area[i];
    double y_pos_of_center = y_axis[i] / area[i];

    // calculate a, b and c using the formulas from the slides
    // a = ∫∫(x')^2 b(x, y)dx'dy'
    double a = x_axis_squared[i] - (area[i] * x_pos_of_center * x_pos_of_center);
    // 2∫∫(x'y') b(x, y)dx'dy'
    double b = 2 * (xy[i] - (area[i] * x_pos_of_center * y_pos_of_center));
    // c = ∫∫(y')2b(x, y)dx'dy'
    double c = y_axis_squared[i] - (area[i] * y_pos_of_center * y_pos_of_center);
    
    // Arc tangent of two numbers
    // The result is an angle expressed in radians. 
    // To convert from radians to degrees, divide by 2
    // 2 x Pi radians = 360 degrees.
    double theta = atan2(b,a-c) / 2;
    // calculate E by using all the information obtained above
    // E = a sin^2(θ) − b sin(θ) cos(θ) + c cos^2(θ)
    double min_moment_of_inertia = a*sin(theta)*sin(theta) - b*sin(theta)*cos(theta) + c*cos(theta)*cos(theta);

    int endpoint_x = x_pos_of_center + cos(theta)*50;
    int endpoint_y = y_pos_of_center + sin(theta)*50; 
    DrawLine(x_pos_of_center, y_pos_of_center, endpoint_x, endpoint_y, 200, an_image);

    output_file << i << " " << x_pos_of_center << " " << y_pos_of_center << " " << min_moment_of_inertia << " " << theta << endl;
  }
}

/**
 * CompareObjectAttributes( ) compares the attributes of each object in a
 * labeled image file with those from the object model database.
 * 
 * @param {istream} database_file: database file containing attributes
 * @param {Image} an_image: input image
 */
void CompareObjectAttributes(istream &database_file, Image *an_image) {
  size_t number_of_obects = an_image->GetNumberGrayLevels();
  std::vector<int> area(number_of_obects+1, 0);
  std::vector<int> x_axis(number_of_obects+1, 0);
  std::vector<int> y_axis(number_of_obects+1, 0);
  std::vector<int> x_axis_squared(number_of_obects+1, 0);
  std::vector<int> y_axis_squared(number_of_obects+1, 0);
  std::vector<int> xy(number_of_obects+1, 0);

  int row = an_image->GetNumberOfRows();
  int column = an_image->GetNumberOfColumns();
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      int pixel = an_image->GetPixel(i,j);
      if (pixel != 0) {
        area[pixel] += 1;
        x_axis[pixel] += i;
        y_axis[pixel] += j;
        x_axis_squared[pixel] += i*i;
        y_axis_squared[pixel] += j*j;
        xy[pixel] += i*j;
      }
    }
  }

  string header;
  getline(database_file, header);
  int database_object_label, database_row_center, database_column_center;
  double database_inertia, database_orientation;
  while(database_file.good()) {
    database_file >> database_object_label
                  >> database_row_center 
                  >> database_column_center
                  >> database_inertia
                  >> database_orientation;

    for (int i = 1; i < number_of_obects+1; ++i) {
      // get the area and center of objects
      // center of the object (x, y)
      // x = (1/A)∑∑i bij
      // y = (1/A)∑∑j bij
      double x_pos_of_center = x_axis[i] / area[i];
      double y_pos_of_center = y_axis[i] / area[i];

      // calculate a, b and c using the formulas from the slides
      // a = ∫∫(x')^2 b(x, y)dx'dy'
      double a = x_axis_squared[i] - (area[i] * x_pos_of_center * x_pos_of_center);
      // 2∫∫(x'y') b(x, y)dx'dy'
      double b = 2 * (xy[i] - (area[i] * x_pos_of_center * y_pos_of_center));
      // c = ∫∫(y')2b(x, y)dx'dy'
      double c = y_axis_squared[i] - (area[i] * y_pos_of_center * y_pos_of_center);
      
      // Arc tangent of two numbers
      // The result is an angle expressed in radians. 
      // To convert from radians to degrees, divide by 2
      // 2 x Pi radians = 360 degrees.
      double theta = atan2(b,a-c) / 2;
      // calculate E by using all the information obtained above
      // E = a sin^2(θ) − b sin(θ) cos(θ) + c cos^2(θ)
      double min_moment_of_inertia = a*sin(theta)*sin(theta) - b*sin(theta)*cos(theta) + c*cos(theta)*cos(theta);

      double smaller_number = min(min_moment_of_inertia, database_inertia);
      double larger_number = max(min_moment_of_inertia, database_inertia);
      double compare_inertia = smaller_number / larger_number;
      double threshold = 0.8;

      if (compare_inertia > 0.8) {
        int endpoint_x = x_pos_of_center + cos(theta)*50;
        int endpoint_y = y_pos_of_center + sin(theta)*50; 
        DrawLine(x_pos_of_center, y_pos_of_center, endpoint_x, endpoint_y, 200, an_image);
      }
    }
  }
}

void PrintImageToCout(Image *an_image) {
  // matrix dimensions
  int row = an_image->GetNumberOfRows();
  int column = an_image->GetNumberOfColumns();
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      std::cout << an_image->GetPixel(i,j) << " ";
    }
  }
}

}  // namespace ComputerVisionProjects
