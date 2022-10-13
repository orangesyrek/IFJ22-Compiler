#include <stdlib.h>
#include <stdio.h>

#include "compiler.h"

extern struct compiler_ctx *ctx;

void print_ctx()
{
	printf("%d %d\n", ctx->last_read, ctx->current_row);
	ctx->current_row = 1;
}



void *
nc_realloc(void *ptr, size_t size)
{
    void *ret;

    ret = realloc(ptr, size);
    if (!ret) {
        free(ptr);
    }

    return ret;
}


void fun()
{
	struct lexeme lex1;
	c = getchar();
	if (c == '$') {
		while ((c = getchar()) != ' ') {

		nc_realloc(lex1.id, strlen(lex1.id) + 1);
		lex1.id[strlen(id)-1] = c;
		}
	}
}
