// Recursive descent parser used from Wikipedia:
// https://en.wikipedia.org/wiki/Recursive_descent_parser
#include <calc/lang/parser.h>
#include <calc/error.h>
#include <stdbool.h>


// Following operations implemented (priority top to bottom):
// 1. Multiplication, division, remainder (modulo)
// 2. Plus, minus, including unary ones
// 3. Assignment


typedef struct {
	TokenList list;
	size_t index;
} TokenIter;


static inline AST* assignment(TokenIter *);

#define _(code, token) calc_mkError((code), (token).slice)


static inline AST* treeError(error_t e) {
	AST *node = langP_newTree();
	node->type = ERROR;
	node->error = e;
	return node;
}


static inline AST* treeOperation(AST* lhs, AST* rhs, Token op) {
	AST* node = langP_newTree();
	node->type = OPERATION;
	node->lop = lhs;
	node->rop = rhs;
	node->token = op;
	return node;
}


static inline AST* treeValue(NodeType type, Token t) {
	AST* node = langP_newTree();
	node->type = type;
	node->token = t;
	return node;
}


static inline bool hasNext(TokenIter* it) {
	return it->index < it->list.size;
}


static inline Token lastToken(TokenIter *iter) {
	return iter->list.tokens[iter->list.size - 1];
}


static inline Token peekToken(TokenIter *iter) {
	if (hasNext(iter)) {
		return iter->list.tokens[iter->index];
	} else {
		Token t = lastToken(iter);
		t.type = END;
		// Token t = { END };
		return t;
	}
}


static inline void backToken(TokenIter *iter) {
	(iter->index)--;
}


static inline void nextToken(TokenIter *iter) {
	(iter->index)++;
}


static Token accept(TokenIter *iter, TokenType t) {
	Token token = peekToken(iter);
	if (token.type == t) {
		nextToken(iter);
		return token;
	}

	token.type = FALSE;
	return token;
}


static error_t expect(TokenIter *iter, TokenType t) {
	Token token = accept(iter, t);
	if (token.type != FALSE)
		return NO_ERROR;

	error_t e = _(SYNTAX, token);
	if (token.type == END) {
		e.where = lastToken(iter).slice;
		e.code = END_OF_FILE;
	}
	return e;
}


// Something to be multiplied
static AST* factor(TokenIter *iter) {
	// <FACTOR> := <IDENTIFIER> | <NUMBER> | ('(' <EXPRESSION> ')')
	Token t;
	if ((t = accept(iter, T_IDENT)).type != FALSE)
		return treeValue(IDENTIFIER, t);
	else if ((t = accept(iter, T_NIL)).type != FALSE)
		return treeValue(VALUE, t);
	else if ((t = accept(iter, T_REAL)).type != FALSE)
		return treeValue(VALUE, t);
	else if ((t = accept(iter, T_INTEGER)).type != FALSE)
		return treeValue(VALUE, t);
	else if (accept(iter, LPAREN).type != FALSE) {
		AST* val = assignment(iter);
		
		error_t e = expect(iter, RPAREN);
		if (e.code != OK) {
			lang_freeTree(val);
			return treeError(e);
		} else {
			return val;
		}

	} else {
		Token invalid = peekToken(iter);
		if (invalid.type == END)
			invalid = lastToken(iter);
		nextToken(iter);
		return treeError(_(SYNTAX, invalid));
	}
}


// multiplication, division and remainder (C operator prec = 3)
// Something to then be added/subtracted
static AST* term(TokenIter *iter) {
	// <TERM> := <FACTOR> ( ('*' | '/') <FACTOR>)*
	AST* lhs = factor(iter);
	if (lhs->type == ERROR)
		return lhs;

	Token op = peekToken(iter);
	while (op.type == MUL || op.type == DIV || op.type == MOD) {
		nextToken(iter);
		
		AST* rhs = factor(iter);
		if (rhs->type == ERROR) {
			lang_freeTree(lhs);
			return rhs;
		}

		AST* joined = treeOperation(lhs, rhs, op);
		if (joined->type == ERROR) {
			lang_freeTree(lhs);
			lang_freeTree(rhs);
			return joined;
		} else
			lhs = joined;

		op = peekToken(iter);
	}
	return lhs;
}


// Addition and subtraction (C operator prec = 4)
// Something that can be assigned
static AST* expression(TokenIter *iter) {
	// <EXPRESSION> := ('+' | '-' | '') <TERM> (('+' | '-') <TERM>)*
	Token op = peekToken(iter);
	bool unary = false;
	if (op.type == ADD || op.type == SUB) 
	{
		unary = true;
		// unary plus/minus
		nextToken(iter);
	}

	AST* lhs = term(iter);
	if (lhs->type == ERROR)
		return lhs;

	if (unary) {
		Token zeroToken = { T_INTEGER, { "0", 1 } };
		AST* zero = treeValue(VALUE, zeroToken);
		lhs = treeOperation(zero, lhs, op);
	}

	op = peekToken(iter);
	while (op.type == ADD || op.type == SUB) {
		nextToken(iter);
		
		AST* rhs = term(iter);
		if (rhs->type == ERROR) {
			lang_freeTree(lhs);
			return rhs;
		}

		AST* joined = treeOperation(lhs, rhs, op);
		if (joined->type == ERROR) {
			lang_freeTree(lhs);
			lang_freeTree(rhs);
			return joined;
		} else
			lhs = joined;

		op = peekToken(iter);
	}

	return lhs;
}


static AST* assignment(TokenIter *iter) {
	Token var, op;
	AST *left = NULL, *right = NULL;
	while ((var = accept(iter, T_IDENT)).type != FALSE) {
		if ((op = accept(iter, EQU)).type != FALSE) {
			if (right != NULL)
				right = right->rop = langP_newTree();
			else
				left = right = langP_newTree();
			right->type = OPERATION;
			right->lop = treeValue(IDENTIFIER, var);
			right->token = op;
		} else {
			backToken(iter); // return before identifier
			break;
		}
	}

	AST* expr = expression(iter);
	if (expr->type == ERROR) {
		lang_freeTree(left);
		return expr;
	} else if (left == NULL)
		return expr;

	right->rop = expr;
	return left;
}


// factor = number | var | ('(' assign ')')
// term   = factor (('*' | '/' | '%') factor)*
// expr   = ('+' | '-') term (('+' | '-') term)*
// assign = (var '=')* expr


// How line is parsed starts from here
static AST* line(TokenIter *iter) {
	return assignment(iter);
}



AST* lang_parseTokens(TokenList list) {
	TokenIter it = { list, 0 };
	return line(&it);
}


#undef _
