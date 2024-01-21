#include <stdio.h>
#define ARRAY_LEN(x)(sizeof(x) / sizeof(x[0]))

int main(){
	int x[] = {3,6,2,7,45};

	for(int i = 0; i < ARRAY_LEN(x); i++){
		printf("%d\n", x[i]);
	}

	return 0;
}