//Worked on by Logan McCAul and Yiyang Qi
#include <stdio.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include "Filter.h"

using namespace std;

#include "rtdsc.h"

//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);

int
main(int argc, char **argv)
{

  if ( argc < 2) {
    fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
  }

  //
  // Convert to C++ strings to simplify manipulation
  //
  string filtername = argv[1];

  //
  // remove any ".filter" in the filtername
  //
  string filterOutputName = filtername;
  string::size_type loc = filterOutputName.find(".filter");
  if (loc != string::npos) {
    //
    // Remove the ".filter" name, which should occur on all the provided filters
    //
    filterOutputName = filtername.substr(0, loc);
  }

  Filter *filter = readFilter(filtername);

  double sum = 0.0;
  int samples = 0;
  for (int inNum = 2; inNum < argc; inNum++) {
    string inputFilename = argv[inNum];
    string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
    struct cs1300bmp *input = new struct cs1300bmp;
    struct cs1300bmp *output = new struct cs1300bmp;
    int ok = cs1300bmp_readfile( (char *) inputFilename.c_str(), input);

    if ( ok ) {
      double sample = applyFilter(filter, input, output);
      sum += sample;
      samples++;
      cs1300bmp_writefile((char *) outputFilename.c_str(), output);
    }
    delete input;
    delete output;
  }
  fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);

}

struct Filter *
readFilter(string filename)
{
  ifstream input(filename.c_str());

  if ( ! input.bad() ) {
    int size = 0;
    input >> size;
    Filter *filter = new Filter(size);
    int div;
    input >> div;
    int value;
    filter -> setDivisor(div);
    for (int i=0; i < size; i++) {
      for (int j=0; j < size; j++) {
	input >> value;
	filter -> set(i,j,value);
      }
    }
    return filter;
  }
}


double
applyFilter(struct Filter *filter, cs1300bmp *input, cs1300bmp *output)
{

  long long cycStart, cycStop;

  cycStart = rdtscll();

  output -> width = input -> width;
  output -> height = input -> height;
  
	short int fil = filter -> getSize();
	short int fildiv = filter -> getDivisor();
	short int temp1=0,temp2=0,temp3=0,temp4=0;
	short int filarray[fil][fil];
	short int h = (input -> height) -1;
	short int w = (input -> width) -1;
	
	/*for (short int i = 0; i < fil; i++) {
	for (short int j = 0; j < fil; j++) {
		  filarray[i][j] = filter -> get(i, j);
	  }
  }*/
  		  filarray[0][0] = filter -> get(0, 0);
		  filarray[0][1] = filter -> get(0, 1);
		  filarray[0][2] = filter -> get(0, 2);
		  filarray[1][0] = filter -> get(1, 0);
		  filarray[1][1] = filter -> get(1, 1);
		  filarray[1][2] = filter -> get(1, 2);
		  filarray[2][0] = filter -> get(2, 0);
		  filarray[2][1] = filter -> get(2, 1);
		  filarray[2][2] = filter -> get(2, 2);

  
for(short int plane = 0; plane < 3; plane++) {
 for(short int row = h; row > 0; row--) {
  for(short int col = w; col > 0; col--) {
	temp2 = 
		(input -> color[plane][row - 1][col - 1] 
		 * filarray[0][0])
		 + (input -> color[plane][row][col - 1] 
		 * filarray[0][1])
		 + (input -> color[plane][row + 1][col - 1] 
		 * filarray[0][2]);
	temp3 =
		 (input -> color[plane][row -1][col] 
		 * filarray[1][0])
		 + (input -> color[plane][row][col] 
		 * filarray[1][1])
		 + (input -> color[plane][row + 1][col] 
		 * filarray[1][2]);
	temp4 =
		 (input -> color[plane][row - 1][col + 1] 
		 * filarray[2][0])
		 + (input -> color[plane][row][col + 1] 
		 * filarray[2][1])
		 +  (input -> color[plane][row + 1][col + 1] 
		 * filarray[2][2]);

	temp1 = temp2 + temp3 + temp4;
	temp1 = temp1 / fildiv;

	if ( temp1  < 0 ) {
	  temp1 = 0;
	}

	if ( temp1  > 255 ) { 
	  temp1 = 255;
	}
	      	output -> color[plane][row][col] = temp1;

      }
    }

  }


  cycStop = rdtscll();
  double diff = cycStop - cycStart;
  double diffPerPixel = diff / (output -> width * output -> height);
  fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
	  diff, diff / (output -> width * output -> height));
  return diffPerPixel;
}
