#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main(){
	
	srand(time(NULL));
	int rand_a = (rand() % 100) + 1;
	int rand_b = (rand() % 100) + 1;

	int input;
	int ans = rand_a * rand_b;

	printf("%d * %d  = ?\n", rand_a, rand_b);
	printf("\n");
	
	while(1){
		printf(">> Input : ");
		scanf("%d",&input);

		if(input == ans){
			printf("[!] Great Job~!\n");
			exit(1);
		}
		else{
			printf("[?] %d is not answer\n", input);
			printf("\n");
		}
	}
}

