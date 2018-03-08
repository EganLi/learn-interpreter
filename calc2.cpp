#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

enum TYPE
{
    END,
    OP,
    INTEGER,
};

typedef enum
{
    ADD = 0,
    SUB = 1,
    MUL,
    DIV
} OPERATION;

typedef union
{
    OPERATION oper;
    int iValue;
    bool isUsed;
} VALUE;

class Token
{
    public:
        Token(TYPE t, OPERATION op): type(t), value() { value.oper = op; }
        Token(TYPE t, int v): type(t), value() { value.iValue = v; }
        Token(TYPE t, bool u): type(t), value() { value.isUsed = u; }
        Token& operator=(const Token& t)
        {
            this->type = t.type;
            if(OP == this->type)
            {
                this->value.oper = t.value.oper;
            }
            else if(INTEGER == t.type)
            {
                this->value.iValue = t.value.iValue;
            }
            else
            {
                this->value.isUsed = false;
            }
            return *this;
        }
        TYPE getType()
        {
            return type;
        }
        VALUE getValue()
        {
            return value;
        }
        void setValue(TYPE t, int v)
        {
            switch (t)
            {
                case END:       value.isUsed = (bool)v;     break;
                case OP:        value.oper = (OPERATION)v;  break;
                case INTEGER:   value.iValue = v;           break;
            }
        }
    private:
        TYPE type;
        VALUE value;
};

class Lexer
{
    public:
        Lexer(string str): orgStr(str), pos(0) {}
        Lexer(const Lexer& lex)
        {
            orgStr = lex.orgStr;
            pos = lex.pos;
        }
        void test(string str)
        {
            orgStr = str;
            pos = 0;
        }
        Token getNextToken()
        {
            while (pos < orgStr.length())
            {
                if (' ' == orgStr[pos])
                {
                    pos++;
                    continue;
                }

                if (48 <= orgStr[pos] && orgStr[pos] <= 57)
                {
                    int interger = orgStr[pos] - 48;
                    pos++;
                    while (48 <= orgStr[pos] && orgStr[pos] <= 57)
                    {
                        interger *= 10;
                        interger += orgStr[pos] - 48;
                        pos++;
                    }
                    return Token(INTEGER, interger);
                }

                int op = 0;
                switch(orgStr[pos])
                {
                    case '/':   op++;
                    case '*':   op++;
                    case '-':   op++;
                    case '+':   pos++;
                                return Token(OP, (OPERATION)op);
                }
                break;
            }

            return Token(END, false);
        }
    private:
        string orgStr;
        size_t pos;
};

class Parser
{
    public:
        Parser(string str): lexer(str), token(lexer.getNextToken()) {}
        Parser(Lexer lex): lexer(lex), token(lex.getNextToken()) {}
        int test(string str)
        {
            lexer.test(str);
            token = lexer.getNextToken();
            return expr();
        }
        void eat(TYPE t)
        {
            if (token.getType() == t)
            {
                token = lexer.getNextToken();
            }
        }
        int factor()
        {
            Token tmp = token;
            eat(INTEGER);
            return tmp.getValue().iValue;
        }
        int expr()
        {
            int ret = factor();

            while (END != token.getType())
            {
                /*
                if (INTEGER == token.getType())
                {
                    break;
                }
                */
                switch (token.getValue().oper)
                {
                    case ADD:
                        eat(OP);
                        ret += factor();
                        break;
                    case SUB:
                        eat(OP);
                        ret -= factor();
                        break;
                    case MUL:
                        eat(OP);
                        ret *= factor();
                        break;
                    case DIV:
                        eat(OP);
                        ret /= factor();
                        break;
                    default:
                        break;
                }
            }

            return ret;
        }
    private:
        Lexer lexer;
        Token token;
};

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do { \
        test_count++; \
        if(equality) \
            test_pass++; \
        else { \
            fprintf(stderr, "%s:%d: expect: " format "actual: " format "\n", __FILE__, __LINE__, expect, actual); \
            main_ret = 1; \
        } \
    } while(0)

#define EXPECT_EQ_INT(expect, actual)   EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_parse()
{
    Parser test("1 +2");
    EXPECT_EQ_INT(3, test.expr());
    EXPECT_EQ_INT(10, test.test("12 - 2"));
    EXPECT_EQ_INT(20, test.test("4* 5"));
    EXPECT_EQ_INT(4, test.test("8/2"));
    EXPECT_EQ_INT(15, test.test("1 +2 +3+4 + 5"));
    EXPECT_EQ_INT(6, test.test("1 *2 +3-4 + 5"));
    EXPECT_EQ_INT(2, test.test("4/2 *3 +1 - 5"));
}

int main()
{
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass*100.0/test_count);

    return 0;
}






























