#include <stdio.h>

#include "../src/compiler.h"
#include "../src/lexical_analysis.h"

struct compiler_ctx *ctx = NULL;

int
main(void)
{
	int count = 0;

	if (compiler_ctx_new(&ctx)) {
		return 1;
	}

	print_ctx();
	int stop = 1;

	while(stop){
		if (count > 300) {
			break;
		}
		struct lexeme lex = getToken();
		printToken(lex);
		if(lex.type == LEX_EOF){
			printf("%s\n", "stop");
			stop = 0;
		}
		count++;
	}

	compiler_ctx_destroy(ctx);
	return 0;
}
