#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Name : Kim Dong Seong
// ID : cs20150112

int cnt = 0;

typedef struct{
	int hit;
	int miss;
	int eviction;
}target;

typedef struct{
	unsigned long long tag;
	
	int valid;
	int used;
}line;

typedef struct{
	line *lines;
}set;

typedef struct{
	set *sets;
}cache;


int powof2(int x){
	int result = 2;

	result = result << x;

	return result;
}


cache make_cache(int s, int E){											// make data structure of cache simulator
	cache Cache;
	set Set;
	line Line;
	
	int i,j;
	int s_pow = powof2(s);

	Cache.sets = (set *)calloc(sizeof(line)*E,s_pow);
//	if (Cache.sets != NULL){
	for(i = 0; i < s_pow; i++){
		Set.lines = (line *)calloc(sizeof(line),E);
		Cache.sets[i]= Set;
//			if (Set.lines != NULL){
		for(j = 0; j < E; j++){
			Line.tag = 0;
			Line.valid = 0;
			Line.used = 0;
			Set.lines[j] = Line;
		}
//			}
	}
//	}

	return Cache;
}

target run(cache c, unsigned long long address, int s, int b, int E){			// run cache simulator with given traces
	cache Cache = c;
	target Target;

	unsigned long long t;
	unsigned long long set_i;
	
	int i,j,k;
	int invalid_count = 0;
	int empty_line_index = -1;
	int minimum_count = 0;
	int minimum_index = 0;
	
	t = address >> (s + b);
	set_i = (address << (sizeof(unsigned long long)*8 - (s + b)) ) >> (sizeof(unsigned long long)*8 - s);
	
	Target.hit = 0;
	Target.miss = 0;
	Target.eviction = 0;
	
	for(i = 0; i < E ; i++){
		if (Cache.sets[set_i].lines[i].valid == 1){
			if (Cache.sets[set_i].lines[i].tag == t){
				Target.hit = 1;
				Cache.sets[set_i].lines[i].used = cnt;
				cnt++;
				return Target;
			}
			else
				continue;
		}
		else{
			invalid_count += 1;
			continue;
		}
	}
	
	if (invalid_count != 0){
		Target.miss += 1;
		for(k = 0; k < E ; k++){
			if (Cache.sets[set_i].lines[k].valid == 0){
				empty_line_index = k;
				break;
			}
		}
		Cache.sets[set_i].lines[empty_line_index].valid = 1;
		Cache.sets[set_i].lines[empty_line_index].tag = t;
		Cache.sets[set_i].lines[empty_line_index].used = cnt;
		cnt++;
	}
	else{
		Target.miss += 1;
		Target.eviction += 1;
		minimum_count = Cache.sets[set_i].lines[0].used;
		for(j = 1; j < E; j++){
			if (Cache.sets[set_i].lines[j].used < minimum_count){
				minimum_count = Cache.sets[set_i].lines[j].used;
				minimum_index = j;
			}
		}
		
		Cache.sets[set_i].lines[minimum_index].tag = t;
		Cache.sets[set_i].lines[minimum_index].used = cnt;
		cnt++;
	}
	return Target;
}

int main(int argc, char **argv){
	cache Cache;
	target result;

	int hit_count = 0;
	int miss_count = 0;
	int eviction_count = 0;

	char c;
	int set_bits_number;
	int line_number;
	int block_bits_number;
	FILE *trace_file;
	
	char trace_str[20];
	char operation;
	unsigned long long address = 0;
	int i,j;
	int s_pow;

	while( (c = getopt(argc, argv, "s:E:b:t:")) != -1){			// parsing command line
    	switch(c){
			case 's':
				set_bits_number = atoi(optarg);
				break;

			case 'E':
				line_number = atoi(optarg);
				break;

			case 'b':
				block_bits_number = atoi(optarg);
				break;

			case 't':
				trace_file = fopen(optarg, "r");
				break;
				
			default:
				break;
		}
	}

	Cache = make_cache(set_bits_number, line_number);

	while (fgets(trace_str, 20, trace_file) != NULL){
		if (trace_str[0] == 'I')
			continue;
		else{
			operation = trace_str[1];
			for (i = 3; trace_str[i] != ','; i++){
				switch(trace_str[i]){
					case 'a':
						address = address*16 + 10;
						break;
					case 'b':
						address = address*16 + 11;
						break;
					case 'c':
						address = address*16 + 12;
						break;
					case 'd':
						address = address*16 + 13;
						break;
					case 'e':
						address = address*16 + 14;
						break;
					case 'f':
						address = address*16 + 15;
						break;
					case '0':
						address = address*16;
						break;
					case '1':
						address = address*16 + 1;
						break;
					case '2':
						address = address*16 + 2;
						break;
					case '3':
						address = address*16 + 3;
						break;
					case '4':
						address = address*16 + 4;
						break;
					case '5':
						address = address*16 + 5;
						break;
					case '6':
						address = address*16 + 6;
						break;
					case '7':
						address = address*16 + 7;
						break;
					case '8':
						address = address*16 + 8;
						break;
					case '9':
						address = address*16 + 9;
						break;
					default:
						break;
				}
			}
			switch(operation){
				case 'L':
				case 'S':
					result = run(Cache, address, set_bits_number, block_bits_number, line_number);
					hit_count += result.hit;						// result.hit
					miss_count += result.miss;						// result.miss
					eviction_count += result.eviction;				// result.eviction
					break;

				case 'M':
					result = run(Cache, address, set_bits_number, block_bits_number, line_number);
					hit_count += result.hit;
					miss_count += result.miss;
					eviction_count += result.eviction;
					
					result = run(Cache, address, set_bits_number, block_bits_number, line_number);
					hit_count += result.hit;
					miss_count += result.miss;
					eviction_count += result.eviction;
					break;

				default:
					break;
			}
			address = 0;
		}
	}

    
	printSummary(hit_count, miss_count, eviction_count);
	fclose(trace_file);

	s_pow = powof2(set_bits_number);
	for(j = 0; j < s_pow; j++){
		if (Cache.sets[j].lines != NULL)
			free(Cache.sets[j].lines);
	}
	if (Cache.sets != NULL)
		free(Cache.sets);
	return 0;
}
