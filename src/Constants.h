#pragma once

extern int WHITE_THRESHOLD;


// structure in which all necessary things could be saved
struct pos
{
  // initially default values are zero
  //_pos() : x(), y(), b() {}
  int x=0;
  int y=0;
  // brightness of the pixel at this position
  float b=0;
  // is set to true, if the pixel is a border pixel
  bool border = false;
};