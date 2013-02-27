/*
https://www.codeeval.com/open_challenges/60/

Description:
There is a monkey which can walk around on a planar grid.
The monkey can move one space at a time left, right, up or down. That is,
from (x, y) the monkey can go to (x+1, y), (x-1, y), (x, y+1), and (x, y-1).
Points where the sum of the digits of the absolute value of the x coordinate
plus the sum of the digits of the absolute value of the y coordinate are lesser
than or equal to 19 are accessible to the monkey. For example,
the point (59, 79) is inaccessible because 5 + 9 + 7 + 9 = 30, which is greater
than 19. Another example: the point (-5, -7) is accessible because
abs(-5) + abs(-7) = 5 + 7 = 12, which is less than 19. How many points can the
monkey access if it starts at (0, 0), including (0, 0) itself?

Answer: 102485
*/

inline int sum_of_digits(int num) {
  if (num < 0) return sum_of_digits(-num);
  int sum = 0;
  while(num > 0){
    sum += num%10;
    num = num/10;
  }
  return sum;
}

#include <cmath>
#include <queue>
#include <bitset>

typedef std::pair<int, int> coord_type;  // (y, x)
typedef std::queue<coord_type> queue_type;

// Since MAX_N is the smallest number with sum of digits greater than 19,
// the original problem can be safely reduced to the problem of walking
// in the grid starting from (0,0) but restricted by the bounding box
// from (-MAX_N, -MAX_N) to (MAX_N, MAX_N).
//
// The grid can be further converted to a graph of (MAX_N*2+1)*(2*MAX_N+1)
// nodes represented by a binary matrix with those inaccessible points
// labeled as 0 otherwise 1. Hence the problem is essentially asking for
// the number of points in the connected components containing the origin
// point (0,0).

// Below is a simple region growing algorithm starting from the seed point
// (0,0) using BFS (breadth-first-search).

inline unsigned int sum_of_digits(coord_type& coord) {
  return sum_of_digits(coord.first) + sum_of_digits(coord.second);
}

const int MAX_N = 299;

inline unsigned long index_from_coord(int y, int x) {
  return (y+MAX_N)*(2*MAX_N+1) + (x+MAX_N);
}

unsigned long count_reachable_points_by_bfs() {
  const long total = (MAX_N*2 + 1) * (MAX_N*2 + 1);
  std::bitset<total> mask;
  mask[total/2] = 1; // set the center

  int steps[4][3] = {
    // dy, dx, d_index
    {0,  1, 1},
    {1,  0, MAX_N*2 + 1},
    {0, -1, -1},
    {-1, 0, -(MAX_N*2 + 1)},
  };

  queue_type q;
  q.push(coord_type(0, 0));
  unsigned long cnt = 0;
  while(!q.empty()) {
    coord_type coord = q.front();
    int y = coord.first;
    int x = coord.second;
    q.pop();  ++cnt;
    unsigned long index = index_from_coord(y, x);

    for (int k = 0; k < 4; ++k) { // check 4-neighbors
      coord_type neighbor(y + steps[k][0], x + steps[k][1]);
      if (!mask[index + steps[k][2]] && sum_of_digits(neighbor) <= 19) {
        q.push(neighbor);
        mask[index + steps[k][2]] = 1;
      }
    }
  }
  return cnt;
}

#include <iostream>
int main() {
  unsigned long cnt = count_reachable_points_by_bfs();
  std::cout << cnt << std::endl;
}
