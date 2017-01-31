#include <iostream>
#include <string.h>

using namespace std;

void printmap(int r, int c, char** map){
  for(int i = 0; i < r; i++){
	if(i % c == 0){
	  printf("//farm %d\n", i / c);
	}
    printf("%s\n", map[i]);
  }
}
