// #include <iostream>
// #include <vector>
#include <Rcpp.h>
using namespace Rcpp;

List actor_nbhd_1(IntegerMatrix el, int q) {
  
  int m = el.nrow();
  
  // Identify the sphere of radius 1 about q
  std::vector<int> n1;
  int i;
  for (i = 0; i < m; i++) {
    if (el(i, 0) == q) {
      if (std::find(n1.begin(), n1.end(), el(i, 1)) != n1.end()) {
        continue;
      } else {
        n1.push_back(el(i, 1));
      }
    }
  }
  
  return List::create(Named("d0") = q,
                      Named("d1") = n1);
}

List event_nbhd_1(IntegerMatrix el, int a) {
  
  int m = el.nrow();
  
  // Identify the sphere of radius 1 about a
  std::vector<int> n1;
  int i;
  for (i = 0; i < m; i++) {
    if (el(i, 1) == a) {
      if (std::find(n1.begin(), n1.end(), el(i, 0)) != n1.end()) {
        continue;
      } else {
        n1.push_back(el(i, 0));
      }
    }
  }
  
  return List::create(Named("d0") = a,
                      Named("d1") = n1);
}

List actor_nbhd_2(IntegerMatrix el, int q) {
  
  int m = el.nrow();
  
  // Identify the spheres of radius 1 and 2 about q
  std::vector<int> n1;
  std::vector<int> n2;
  int i, j;
  for (i = 0; i < m; i++) {
    if (el(i, 0) == q) {
      if (std::find(n1.begin(), n1.end(), el(i, 1)) != n1.end()) {
        continue;
      } else {
        n1.push_back(el(i, 1));
        for (j = 0; j < m; j++) {
          if (j == i) {
            continue;
          }
          if (el(j, 1) == el(i, 1)) {
            if (std::find(n2.begin(), n2.end(), el(j, 0)) != n2.end()) {
              continue;
            } else {
              n2.push_back(el(j, 0));
            }
          }
        }
      }
    }
  }
  
  return List::create(Named("d0") = q,
                      Named("d1") = n1,
                      Named("d2") = n2);
}

unsigned tetrahedral_C(unsigned n) {
  return choose_C(n + 2, 3);
}

// Triad census for affiliation networks

// algorithm adapted from Batagelj and Mrvar (2001)
// performed on an edgelist
// [[Rcpp::export]]
IntegerMatrix triad_census_edgelist(IntegerMatrix el, int actors) {
  
  // Initialize vector of indices
  int is[choose_C(actors, 3)];
  
  // Loop over actors
  
}
