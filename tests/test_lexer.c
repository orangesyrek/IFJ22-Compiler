#include <stdio.h>

#include "../src/compiler.h"
#include "../src/lexical_analysis.h"

struct compiler_ctx *ctx = NULL;

int
main(void)
{
	struct lexeme lex;
	int stop = 1;

	if (compiler_ctx_new(&ctx)) {
		return 1;
	}

	while(stop){
		lex = getToken();
		printToken(lex);

		if(lex.type == LEX_EOF){
			printf("%s\n", "stop");
			stop = 0;
		}
	}

	compiler_ctx_destroy(ctx);
	return 0;
}