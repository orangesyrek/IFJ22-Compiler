#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "lexical_analysis.h"

#define DEBUGLEX

struct compiler_ctx *ctx = NULL;

#ifdef DEBUGLEX

int
main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	if (compiler_ctx_new(&ctx)) {
		return 1;
	}

	print_ctx();
	int stop = 1;
	while(stop){

		struct lexeme lex = get_token();

		//printf("%s\n", "before condition EOF");
		printToken(lex);
		if(lex.type == EOF){
			printf("%s\n", "stop");
			stop = 0;
		}

	}

	compiler_ctx_destroy(ctx);
	return 0;
}
#else

int
main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	if (compiler_ctx_new(&ctx)) {
		return 1;
	}
	print_ctx();
	compiler_ctx_destroy(ctx);
	return 0;
}

#endif
