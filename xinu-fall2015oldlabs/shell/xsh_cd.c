#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 *
 * xsh_cd ~ my soccer dream team
 *
 */
shellcmd xsh_cd(int nargs,char * args[]){
	if(nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tDisplays my dream team\n");
	}

	/* Check argument count */

	if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
			args[0]);
		return 0;
	}

		if(nargs == 1){
			if(strncmp(args[0],"dreamteam",9) == 0){
				printf("\n-------------------Buffon------------------\n");
				printf("\n----Lahm----Maldini----Vidic----Carlos\n");
				printf("\n--------Pirlo------Schweinsteiger---------\n");
				printf("\n-----Robben--------Zidane--------Beckham--\n");
				printf("\n----------------Ronaldinho--------------\n");
				return 1;
			}
		}
}

