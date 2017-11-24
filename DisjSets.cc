#include "DisjSets.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;
/**
 * Construct the disjoint sets object.
 * numElements is the initial number of disjoint sets.
 */
DisjSets::DisjSets( int numElements ) : s( numElements )
{
  for( int i = 0; i < s.size( ); i++ )
    s[ i ] = -1;
}

/**
 * Union two disjoint sets.
 * For simplicity, we assume root1 and root2 are distinct
 * and represent set names.
 * root1 is the root of set 1.
 * root2 is the root of set 2.
 */
void DisjSets::unionSets( int root1, int root2 )
{
  if (root1 != root2)
  {
    if( s[ root2 ] < s[root1] ) { // root2 is deeper
      s[root2] += s[root1];
      s[root1] = root2;
      //return root2;
    }
    else {
      s[root1] += s[root2];
      s[root2] = root1; // make root1 new root
    }
  }
}

/**
 * Perform a find.
 * Error checks omitted again for simplicity.
 * Return the set containing x.
 */
int DisjSets::find( int x ) const {
  if( s[ x ] < 0 )
   return x;
  else
   return find( s[ x ] );
}

void DisjSets::Print( ) {
  for( int i = 0; i < s.size( ); i++ )
    cout << s[i] << " ";
}

