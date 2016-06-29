#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

#include <vector>
//#include <fstream>
//#include <sstream>
#include <string.h>

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;


/** @function main */
int main(int argc, char** argv)
{
  //Mat src, src_gray;

  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
      return -1;

  Mat edges;
  namedWindow("edges",1);
  for(;;)
  {
      Mat frame;
      cap >> frame; // get a new frame from camera

      /// Read the image
      //src = imread( argv[1], 1 );

      /// Convert it to gray
      cvtColor(frame, edges, COLOR_BGR2GRAY);

      /// Reduce the noise so we avoid false circle detection
      GaussianBlur(edges, edges, Size(9, 9), 2, 2 );
      //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);

      Canny(edges, edges, 0, 30, 3);

      vector<Vec3f> circles;

      /// Apply the Hough Transform to find the circles
      HoughCircles( edges, circles, CV_HOUGH_GRADIENT, 1, edges.rows/8, 200, 100, 0, 0 );

      /// Draw the circles detected
      for( size_t i = 0; i < circles.size(); i++ )
      {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         // circle center
         circle( frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
         // circle outline
         circle( frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
      }

      /// Show your results
      namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
      imshow( "Hough Circle Transform Demo", frame );

      waitKey(0);
      //return 0;

      //imshow("edges", edges);
      if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
