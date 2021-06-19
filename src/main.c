#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>

#include "../mpc/mpc.h"

/* Custom types */
typedef struct {
	int type;
	long num;
	int err;
} lval;

enum { LVAL_NUM, LVAL_ERR };
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

lval lval_num(long x) {
	lval v;
	v.type = LVAL_NUM;
	v.num = x;
	return v;
}

lval lval_err(int x) {
	lval v;
	v.type = LVAL_ERR;
	v.err = x;
	return v;
}


/* Function declarations */
int number_of_nodes(mpc_ast_t* t);
lval eval(mpc_ast_t* t);
lval eval_op(lval x, char* op, lval y);
void lval_print(lval v);
void lval_println(lval v);


/* NicoLISP Main Program */
int main(int argc, char** argv) {

	/* Create some parsers */
	mpc_parser_t* Number	= mpc_new("number");
	mpc_parser_t* Operator	= mpc_new("operator");
	mpc_parser_t* Expr		= mpc_new("expr");
	mpc_parser_t* NicoLISP	= mpc_new("nicolisp");


	/* Define them with the following language */
	mpca_lang(MPCA_LANG_DEFAULT,
	"															\
		number		: /-?[0-9]+/ 							;	\
		operator	: '+' | '-' | '*' | '/' 				;	\
		expr		: <number> | '(' <operator> <expr>+ ')' ;	\
		nicolisp	: /^/ <operator> <expr>+ /$/ 			;	\
	",
	Number, Operator, Expr, NicoLISP);

	/* Print version and Exit info */
	puts("NicoLISP Version 0.0.1");
	puts("Press Ctrl+c to Exit\n");
	
	/* REPL */
	while (1) {
	
		/* Prompt user for input */
		char* input = readline("nicoLSIP> ");
		
		/* Add input to history */
		add_history(input);
		
		/* Attempt to parse user input */
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, NicoLISP, &r)) {
			/* On success print the AST*/
			lval result = eval(r.output);
			lval_println(result);
			mpc_ast_delete(r.output);
		} else {
			/* Otherwise print error */
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
		
		
		/* Free retrieved input */
		free(input);
	
	}
	
	mpc_cleanup(4, Number, Operator, Expr, NicoLISP);
	return 0;
}

int number_of_nodes(mpc_ast_t* t) {
	if (t->children_num == 0) { return 1; }
	if (t->children_num >= 1) {
		int total = 1;
		for (int i = 0; i < t->children_num; i++) {
			total = total + number_of_nodes(t->children[i]);
		}
		return total;
	}
	return 0;
}

lval eval(mpc_ast_t* t) {

	/* Return immediately if it's a number */
	if (strstr(t->tag, "number")) {
		
		errno = 0;
		long x = strtol(t->contents, NULL, 10);
		/* `atoi` converts string to int */
		return errno != ERANGE
			? lval_num(x)
			: lval_err(LERR_BAD_NUM);
	}
	
	/* The operator is always second child.
	   If you consider an example: (+ 2 2)
	   the `(` is first, then `+` is second.
	   This is an assumption built into the
	   language grammar.
	*/
	char* op = t->children[1]->contents;
	
	/* Store the third child in `x` */
	lval x = eval(t->children[2]);
	
	/* Iterate the remaining children and combine */
	int i = 3;
	while (strstr(t->children[i]->tag, "expr")) {
		x = eval_op(x, op, eval(t->children[i]));
		i++;
	}
	
	return x;
}

/* Use the operator string to see which operation to perform */

lval eval_op(lval x, char* op, lval y) {

	if (x.type == LVAL_ERR) { return x; }
	if (y.type == LVAL_ERR) { return y; }

	if (strcmp(op, "+") == 0) { 
		return lval_num(x.num + y.num);
	}
	if (strcmp(op, "-") == 0) {
		return lval_num(x.num - y.num);
	}
	if (strcmp(op, "*") == 0) { 
		return lval_num(x.num * y.num);
	}
	if (strcmp(op, "/") == 0) {
		/* If second operand is zero, return error */
		return y.num ==0
			? lval_err(LERR_DIV_ZERO)
			: lval_num(x.num / y.num);
	}
	return lval_err(LERR_BAD_OP);
}

void lval_print(lval v) {
	switch (v.type) {
	case LVAL_NUM:
		printf("%li", v.num);
	break;
	case LVAL_ERR:
		if (v.err == LERR_DIV_ZERO) {
			printf("Error: Division By Zero");
		}
		if (v.err == LERR_BAD_OP) {
			printf("Error: Invalid Operator");
		}
		if (v.err == LERR_BAD_NUM) {
			printf("Error: Invalid Number");
		}
	break;	
	}
}

void lval_println(lval v) {
	lval_print(v); putchar('\n');
}





