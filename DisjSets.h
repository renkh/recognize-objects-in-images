#ifndef COMPUTER_VISION_DISJSETS_H_
#define COMPUTER_VISION_DISJSETS_H_

#include <cstdlib>
#include <string>
#include <vector>

class DisjSets {
  public:
    explicit DisjSets( int numElements );
    int find( int x ) const;
    //int find( int x );
    void unionSets( int root1, int root2 );
    void Print( );
  private:
    std::vector<int> s;
};


#endif  // COMPUTER_VISION_DISJSETS_H_
