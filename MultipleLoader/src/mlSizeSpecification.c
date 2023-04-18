#include "SizeSpec.h"

SizeSpec_t mlSizeSpecification(int nr_of_computations){
  SizeSpec_t s;
  s.local_work_size = 256; // Local id 0-255 ig
  s.n_work_groups = (nr_of_computations + s.local_work_size + 1) / s.local_work_size;
  s.global_work_size = s.n_work_groups * s.local_work_size;
  return s;
}