#include <stdio.h>

int main(){

	int num_star = 10;
	
	for(int i = 0; i < num_star; i++){
		for(int k = 0; k < num_star - i; k++){
			printf(" ");
		}
		for(int j = 0; j < i; j++){
			printf("* ");
		}
		printf("* ");
		printf("\n");
	}

	int len_stem = 3;

	for(int i = 0; i < len_stem; i++){
		for(int j = 0; j < num_star; j++){
			printf(" ");
		}
		printf("*");
		printf("\n");
	}

	return 0;
}