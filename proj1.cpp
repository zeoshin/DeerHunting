/*===========================================================================
#
# EECS281 at the University of Michigan
# Project #1 Deer Hunting WINTER 2014
#
# Last Modified February 5, 2014 by Hongshen Zhao
#
===========================================================================*/
#include "proj1func.h"

using namespace std;

////////////////////////////MAIN FUNCTION///////////////////////////////
int main(int argc, char* argv[])
{
  string line;
  string ex_type;
  string input_type;
  string output_type = "M";
  int size = 0, farm_num = 0, read_counter = 0;
  int p_x = 0, p_y = 0;
  int got = 0;
////////////////////////////READ COMMAND LINE///////////////////////////
  extern char *optarg;
  int arg;
  const char* const short_options = "sqo:h";
  const struct option long_options[] = {
	{"stack",	0,	0,	's'},
	{"queue",	0,	0,	'q'},
	{"output",	1,	0,	'o'},
	{"help",	0,	0,	'h'},
	{0,			0,	0,	 0 }
  };

  do{
	arg = getopt_long(argc, argv, short_options, long_options, NULL);
	switch (arg){
	case 's':{
	  if(!strcmp(ex_type.c_str(), "queue") || !strcmp(ex_type.c_str(), "stack")){
	    fprintf(stderr, "More than one --stack or --queue on the command line.\n");
		exit(1);
	  }
	  ex_type = "stack";
	  break;
	}
	case 'q':{
	  if(!strcmp(ex_type.c_str(), "queue") || !strcmp(ex_type.c_str(), "stack")){
	    fprintf(stderr, "More than one --stack or --queue on the command line.\n");
		exit(1);
	  }
	  ex_type = "queue";
	  break;
	}
	case 'o':{
	  if(*optarg == 76) output_type = "L";
	  else if(*optarg == 77) output_type = "M";
	  else exit(1);
	  break;
	}
	case 'h':
	  exit(0);
	  break;
	case '?':
	  exit(1);
	  break;
	case -1:
	  break;
	default:
	  abort();
	}
  }while(arg != -1);
  if(strcmp(ex_type.c_str(), "queue") && strcmp(ex_type.c_str(), "stack")){
	fprintf(stderr, "Less than one --stack or --queue on the command line.\n");
	exit(1);
  }
////////////////////////////READ MAP DIMENSIONS/////////////////////////
  getline(cin, line);
  input_type = line;
  getline(cin, line);
  size = atoi(line.c_str());
  if(size <= 1) exit(1);
  getline(cin, line);
  farm_num = atoi(line.c_str());
  if(farm_num < 1) exit(1);
//////////////////////////////INITIALIZE ARRAYS/////////////////////////
  char** map = new char*[size * farm_num];
  char** dir = new char*[size * farm_num];
  for(int i = 0; i < size * farm_num; i++){
    map[i] = new char[size];
	dir[i] = new char[size];
  }
  read_counter = 3;
  for(int i = 0; i < farm_num * size; i ++){
	for(int j = 0; j < size; j ++){
	  map[i][j] = '.';
	}
  }
/////////////////////////////READ MAP CHARACTERS////////////////////////
  while(getline(cin, line) && !line.empty()) {
	if(line.c_str()[0] == 47 && line.c_str()[1] != 47){
	  fprintf(stderr, "Illegal map characters.\n");
	  exit(1);
	}
	if(line.c_str()[0] != 47 && line.c_str()[1] != 47){
	  read_counter++;
	  if(!strcmp(input_type.c_str(), "M") && read_counter >= 4){
		if(read_counter - 4 > size * farm_num - 1) exit(1);
		if(line.length() != (unsigned) size){
	      fprintf(stderr, "Illegal map characters.\n");
		  exit(1);
	    }
        for(int i = 0; i < size; i++){
		  if(line.c_str()[i] == 83){
			map[read_counter - 4][i] = 'S';
		  	p_x = read_counter - 4;
			p_y = i;
		  }
		  else if(line.c_str()[i] == 62)
			map[read_counter - 4][i] = '>';
		  else if(line.c_str()[i] == 60)
			map[read_counter - 4][i] = '<';
		  else if(line.c_str()[i] == 46)
			map[read_counter - 4][i] = '.';
		  else if(line.c_str()[i] == 35)
			map[read_counter - 4][i] = '#';
		  else if(line.c_str()[i] == 66)
			map[read_counter - 4][i] = 'B';
		  else{
			fprintf(stderr, "Illegal map characters.\n");
			exit(1);
		  }
		}
	  }
	  else if(!strcmp(input_type.c_str(), "L") && read_counter >= 4){
		string str;
		int row, col, farm;
		string character;
		int p1 = 0;
		int p2 = 0;
        int p3 = 0;
		for(unsigned int i = 0; i < line.length() && p1 == 0; i++){
		  if(line.c_str()[i] == 44) p1 = i;
		}
		for(unsigned int i = p1 + 1; i < line.length() && p2 == 0; i++){
		  if(line.c_str()[i] == 44) p2 = i;
		}
		for(unsigned int i = p2 + 1; i < line.length() && p3 == 0; i++){
		  if(line.c_str()[i] == 44) p3 = i;
		}
		row = atoi(str.assign(line, 1, p1 - 1).c_str());
		col = atoi(str.assign(line, p1 + 1, p2 - p1 - 1).c_str());
		farm = atoi(str.assign(line, p2 + 1, p3 - p2 - 1).c_str());
		if(row > size - 1 || col > size - 1 || farm > farm_num - 1){
	      fprintf(stderr, "Illegal positions.\n");
		  exit(1);
		}
		if(row < 0 || col < 0 || farm < 0){
	      fprintf(stderr, "Illegal positions.\n");
		  exit(1);
	    }
		character.assign(line, p3 + 1, line.length() - p3 - 2);
		if(!strcmp(character.c_str(), "S")){
		  map[row + farm * size][col] = 'S';
		  p_x = row + farm * size;
		  p_y = col;
		}
		else if(!strcmp(character.c_str(), ">"))
		  map[row + farm * size][col] = '>';
		else if(!strcmp(character.c_str(), "<"))
		  map[row + farm * size][col] = '<';
		else if(!strcmp(character.c_str(), "."))
		  map[row + farm * size][col] = '.';
		else if(!strcmp(character.c_str(), "#"))
		  map[row + farm * size][col] = '#';
		else if(!strcmp(character.c_str(), "B"))
		  map[row + farm * size][col] = 'B';
		else{
		  fprintf(stderr, "Illegal map characters.\n");
		  exit(1);
		}
	  }
	  else exit(1);
	}
  }
/////////////////////////////////MODE STACK/////////////////////////////
  if(!strcmp(ex_type.c_str(), "stack")){
    stack<int> mystack, route;
    mystack.push(p_x * size + p_y);
    dir[p_x][p_y] = 'S';
    while(!got && !mystack.empty()){
///////////////push in the order of n, e, s, w, d, u////////////////////
      route.push(mystack.top());
      mystack.pop();
	  if(map[p_x][p_y] == '>'){
	    if(p_x / size < farm_num - 1 && map[p_x + size][p_y] != '#')
		  if(dir[p_x + size][p_y] != 'u' && dir[p_x + size][p_y] != 'd' 
		    && dir[p_x + size][p_y] != 'n' && dir[p_x + size][p_y] != 'e'
			&& dir[p_x + size][p_y] != 's' && dir[p_x + size][p_y] != 'w'
			&& dir[p_x + size][p_y] != 'S'){
		    mystack.push((p_x + size) * size + p_y);
		    dir[p_x + size][p_y] = 'u';
		  }
	  }
	  else if(map[p_x][p_y] == '<'){
	    if(p_x / size > 0 && map[p_x - size][p_y] != '#')
		  if(dir[p_x - size][p_y] != 'u' && dir[p_x - size][p_y] != 'd' 
		    && dir[p_x - size][p_y] != 'n' && dir[p_x - size][p_y] != 'e'
			&& dir[p_x - size][p_y] != 's' && dir[p_x - size][p_y] != 'w'
			&& dir[p_x - size][p_y] != 'S'){
		    mystack.push((p_x - size) * size + p_y);
		    dir[p_x - size][p_y] = 'd';
		  }
	  }
	  else{
        if(p_x % size > 0  && map[p_x - 1][p_y] != '#'){
		  if(dir[p_x - 1][p_y] != 'u' && dir[p_x - 1][p_y] != 'd' 
		    && dir[p_x - 1][p_y] != 'n' && dir[p_x - 1][p_y] != 'e'
			&& dir[p_x - 1][p_y] != 's' && dir[p_x - 1][p_y] != 'w'
			&& dir[p_x - 1][p_y] != 'S'){
		    mystack.push((p_x - 1) * size + p_y);
		    dir[p_x - 1][p_y] = 'n';
		  }
        }
        if(p_y < size - 1 && map[p_x][p_y + 1] != '#'){
		  if(dir[p_x][p_y + 1] != 'u' && dir[p_x][p_y + 1] != 'd' 
		    && dir[p_x][p_y + 1] != 'n' && dir[p_x][p_y + 1] != 'e'
			&& dir[p_x][p_y + 1] != 's' && dir[p_x][p_y + 1] != 'w'
			&& dir[p_x][p_y + 1] != 'S'){
		    mystack.push(p_x * size + p_y + 1);
		    dir[p_x][p_y + 1] = 'e';
		  }
        }
        if((p_x) % size < size - 1 && map[p_x + 1][p_y] != '#'){
		  if(dir[p_x + 1][p_y] != 'u' && dir[p_x + 1][p_y] != 'd' 
		    && dir[p_x + 1][p_y] != 'n' && dir[p_x + 1][p_y] != 'e'
			&& dir[p_x + 1][p_y] != 's' && dir[p_x + 1][p_y] != 'w'
			&& dir[p_x + 1][p_y] != 'S'){
		    mystack.push((p_x + 1) * size + p_y);
		    dir[p_x + 1][p_y] = 's';
		  }
        }
        if(p_y > 0 && map[p_x][p_y - 1] != '#'){
		  if(dir[p_x][p_y - 1] != 'u' && dir[p_x][p_y - 1] != 'd' 
		    && dir[p_x][p_y - 1] != 'n' && dir[p_x][p_y - 1] != 'e'
			&& dir[p_x][p_y - 1] != 's' && dir[p_x][p_y - 1] != 'w'
			&& dir[p_x][p_y - 1] != 'S'){
		    mystack.push(p_x * size + p_y - 1);
		    dir[p_x][p_y - 1] = 'w';
		  }
        }
	  }
      if(!mystack.empty()) p_x = (mystack.top()- mystack.top() % size) / size;
      if(!mystack.empty()) p_y = mystack.top() % size;
      if(map[p_x][p_y] == 'B' && !mystack.empty()){
	    got = 1;
		route.push(mystack.top());
	  }
    }
    printf("%d\n%d\n", size, farm_num);
///////////////////////////////CREATE ROUTE/////////////////////////////
	int next_index = -1, index;
	stack<int> temp;
	char prev = 'B';
	if(!route.empty()) next_index = route.top();
	do{
	  index = next_index;
	  if (!route.empty() && next_index == route.top()){
		temp.push(next_index);
		if(prev != 'B')
	      map[(route.top()- route.top() % size) / size][route.top() % size] 
		  = prev;
	  }
	  if(!route.empty() 
		&& dir[(route.top()- route.top() % size) / size][route.top() % size] == 'n' 
		&& next_index == route.top())
		next_index = next_index + size;
	  else if(!route.empty() 
		&& dir[(route.top()- route.top() % size) / size][route.top() % size] == 'e' 
		&& next_index == route.top())
		next_index = next_index - 1;
	  else if(!route.empty() 
		&& dir[(route.top()- route.top() % size) / size][route.top() % size] == 's' 
		&& next_index == route.top())
		next_index = next_index - size;
	  else if(!route.empty() 
		&& dir[(route.top()- route.top() % size) / size][route.top() % size] == 'w' 
		&& next_index == route.top())
		next_index = next_index + 1;
	  else if(!route.empty() 
		&& dir[(route.top()- route.top() % size) / size][route.top() % size] == 'd' 
		&& next_index == route.top())
		next_index = next_index + size * size;
	  else if(!route.empty() 
		&& dir[(route.top()- route.top() % size) / size][route.top() % size] == 'u' 
		&& next_index == route.top())
		next_index = next_index - size * size;
	  else {};
	  if(!route.empty() && index == route.top()){
	    prev = dir[(route.top()- route.top() % size) / size][route.top() % size];
	  }
      if(!route.empty()) route.pop();
	}
	while(!route.empty() 
	&& dir[(route.top() - route.top() % size) / size][route.top() % size] != 'S' && got);
	if(!strcmp(output_type.c_str(), "L")) printf("//path taken\n");
	if(!route.empty() 
	&& map[(route.top() - route.top() % size) / size][route.top() % size] == 'S'){
	  temp.push(next_index);
	  map[(route.top() - route.top() % size) / size][route.top() % size] = prev;
	}
	while(!temp.empty() && !strcmp(output_type.c_str(), "L") && !temp.empty() 
	&& got && map[(temp.top()- temp.top() % size) / size][temp.top() % size] != 'B'){
	  printf("(%d,%d,%d,%c)\n", (temp.top()- temp.top() % size) / size % size, 
	  temp.top() % size, temp.top() / size / size, 
	  map[(temp.top()- temp.top() % size) / size][temp.top() % size]);
	  if(!temp.empty()) temp.pop();
	}
  }
/////////////////////////////////MODE STACK/////////////////////////////
  else if(!strcmp(ex_type.c_str(), "queue")){
    deque<int> mystack, route;
    mystack.push_back(p_x * size + p_y);
    dir[p_x][p_y] = 'S';
    while(!got && !mystack.empty()){
///////////////push in the order of n, e, s, w, d, u////////////////////
      route.push_back(mystack.front());
      mystack.pop_front();
	  if(map[p_x][p_y] == '>'){
	    if(p_x / size < farm_num - 1 && map[p_x + size][p_y] != '#')
		  if(dir[p_x + size][p_y] != 'u' && dir[p_x + size][p_y] != 'd'
		    && dir[p_x + size][p_y] != 'n' && dir[p_x + size][p_y] != 'e'
			&& dir[p_x + size][p_y] != 's' && dir[p_x + size][p_y] != 'w'
			&& dir[p_x + size][p_y] != 'S'){
		    mystack.push_back((p_x + size) * size + p_y);
		    dir[p_x + size][p_y] = 'u';
		  }
	  }
	  else if(map[p_x][p_y] == '<'){
	    if(p_x / size > 0 && map[p_x - size][p_y] != '#')
		  if(dir[p_x - size][p_y] != 'u' && dir[p_x - size][p_y] != 'd'
		    && dir[p_x - size][p_y] != 'n' && dir[p_x - size][p_y] != 'e'
			&& dir[p_x - size][p_y] != 's' && dir[p_x - size][p_y] != 'w'
			&& dir[p_x - size][p_y] != 'S'){
		    mystack.push_back((p_x - size) * size + p_y);
			dir[p_x - size][p_y] = 'd';
		  }
	  }
	  else{
        if(p_x % size > 0  && map[p_x - 1][p_y] != '#'){
		  if(dir[p_x - 1][p_y] != 'u' && dir[p_x - 1][p_y] != 'd'
		    && dir[p_x - 1][p_y] != 'n' && dir[p_x - 1][p_y] != 'e'
			&& dir[p_x - 1][p_y] != 's' && dir[p_x - 1][p_y] != 'w'
			&& dir[p_x - 1][p_y] != 'S'){
		    mystack.push_back((p_x - 1) * size + p_y);
		    dir[p_x - 1][p_y] = 'n';
		  }
        }
        if(p_y < size - 1 && map[p_x][p_y + 1] != '#'){
		  if(dir[p_x][p_y + 1] != 'u' && dir[p_x][p_y + 1] != 'd'
		    && dir[p_x][p_y + 1] != 'n' && dir[p_x][p_y + 1] != 'e'
			&& dir[p_x][p_y + 1] != 's' && dir[p_x][p_y + 1] != 'w'
			&& dir[p_x][p_y + 1] != 'S'){
		    mystack.push_back(p_x * size + p_y + 1);
		    dir[p_x][p_y + 1] = 'e';
		  }
        }
        if((p_x) % size < size - 1 && map[p_x + 1][p_y] != '#'){
		  if(dir[p_x + 1][p_y] != 'u' && dir[p_x + 1][p_y] != 'd'
		    && dir[p_x + 1][p_y] != 'n' && dir[p_x + 1][p_y] != 'e'
			&& dir[p_x + 1][p_y] != 's' && dir[p_x + 1][p_y] != 'w'
			&& dir[p_x + 1][p_y] != 'S'){
		    mystack.push_back((p_x + 1) * size + p_y);
		    dir[p_x + 1][p_y] = 's';
		  }
        }
        if(p_y > 0 && map[p_x][p_y - 1] != '#'){
		  if(dir[p_x][p_y - 1] != 'u' && dir[p_x][p_y - 1] != 'd'
		    && dir[p_x][p_y - 1] != 'n' && dir[p_x][p_y - 1] != 'e'
			&& dir[p_x][p_y - 1] != 's' && dir[p_x][p_y - 1] != 'w'
			&& dir[p_x][p_y - 1] != 'S'){
		    mystack.push_back(p_x * size + p_y - 1);
		    dir[p_x][p_y - 1] = 'w';
		  }
        }
	  }
      if(!mystack.empty())
		p_x = (mystack.front()- mystack.front() % size) / size;
      if(!mystack.empty())
		p_y = mystack.front() % size;
      if(map[p_x][p_y] == 'B' && !mystack.empty()){
	    got = 1;
        route.push_back(mystack.front());
      }
    }
    printf("%d\n%d\n", size, farm_num);
///////////////////////////////CREATE ROUTE/////////////////////////////
	int next_index = -1, index;
	stack<int> temp;
	char prev = 'B';
	if(!route.empty()) next_index = route.back();
	do{
	  index = next_index;
	  if (!route.empty() && next_index == route.back()){
		temp.push(next_index);
		if(prev != 'B')
	      map[(route.back()- route.back() % size) / size][route.back() % size] 
		  = prev;
	  }
	  if(!route.empty() 
		&& dir[(route.back()- route.back() % size) / size][route.back() % size] == 'n' 
		&& next_index == route.back())
		next_index = next_index + size;
	  else if(!route.empty() 
		&& dir[(route.back()- route.back() % size) / size][route.back() % size] == 'e' 
		&& next_index == route.back())
		next_index = next_index - 1;
	  else if(!route.empty() 
		&& dir[(route.back()- route.back() % size) / size][route.back() % size] == 's' 
		&& next_index == route.back())
		next_index = next_index - size;
	  else if(!route.empty() 
		&& dir[(route.back()- route.back() % size) / size][route.back() % size] == 'w' 
		&& next_index == route.back())
		next_index = next_index + 1;
	  else if(!route.empty() 
		&& dir[(route.back()- route.back() % size) / size][route.back() % size] == 'd' 
		&& next_index == route.back())
		next_index = next_index + size * size;
	  else if(!route.empty() 
		&& dir[(route.back()- route.back() % size) / size][route.back() % size] == 'u' 
		&& next_index == route.back())
		next_index = next_index - size * size;
	  else {};
	  if(!route.empty() && index == route.back()){
	    prev = dir[(route.back()- route.back() % size) / size][route.back() % size];
	  }
      if(!route.empty()) route.pop_back();
	}
	while(!route.empty() 
	&& dir[(route.back() - route.back() % size) / size][route.back() % size] != 'S' && got);
	if(!strcmp(output_type.c_str(), "L")) printf("//path taken\n");
	if(!route.empty() 
	&& map[(route.back() - route.back() % size) / size][route.back() % size] == 'S'){
	  temp.push(next_index);
	  map[(route.back() - route.back() % size) / size][route.back() % size] = prev;
	}
	while(!temp.empty() && !strcmp(output_type.c_str(), "L") && !temp.empty()
	&& got && map[(temp.top()- temp.top() % size) / size][temp.top() % size] != 'B'){
	  printf("(%d,%d,%d,%c)\n", (temp.top()- temp.top() % size) / size % size, 
	  temp.top() % size, temp.top() / size / size, 
	  map[(temp.top()- temp.top() % size) / size][temp.top() % size]);
	  if(!temp.empty()) temp.pop();
	}
  }
  else exit(1);
  if(!strcmp(output_type.c_str(), "M")) printmap(size * farm_num ,size, &map[0]);
/////////////////////Don't forget to delete!!!!!!!!!!!!!!!!!!!!////////
  for(int i = 0; i < size * farm_num; i++){
    delete[] map[i];
	delete[] dir[i];
  }
  delete[] map;
  delete[] dir;
  return 0;
}
