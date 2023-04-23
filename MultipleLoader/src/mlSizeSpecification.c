#include "SizeSpec.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
SizeSpec_t mlSizeSpecification1D(size_t x, size_t sub_x){
  SizeSpec_t s; 
  s.global_size = (size_t*)malloc(sizeof(size_t));
  s.local_size = (size_t*)malloc(sizeof(size_t));
  s.global_size[0] = x;
  s.local_size[0] = sub_x;
  s.dim = 1;
  return s;
}

SizeSpec_t mlSizeSpecification2D(size_t x, size_t y, size_t sub_x, size_t sub_y){
  SizeSpec_t s; 
  s.global_size = (size_t*)malloc(sizeof(size_t)*2);
  s.global_size[0] = x;  //1st dim parameter size
  s.global_size[1] = y;  //2nd dim parameter size
  s.local_size = (size_t*)malloc(sizeof(size_t)*2);
  s.local_size[0] = sub_x;
  s.local_size[1] = sub_y;
  s.dim = 2;
  return s;
}