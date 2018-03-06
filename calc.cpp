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
    ADD,
    SUB,
    MULTI,
    DIV
} OPERATION;

typedef union
{
    OPERATION oper;
    int iValue;
} VALUE;

class Token
{
public:
    Token(TYPE t, VALUE v = VALUE()): type(t), value(v) {}
    TYPE getType()
    {
        return type;
    }
    VALUE getValue()
    {
        return value;
    }
private:
    TYPE type;
    VALUE value;
};

class Interpreter
{
public:
    Interpreter(string str): orgTest(str), pos(0) {}
    int expr()
    {
        Token left = getNextToken();
        Token op = getNextToken();
        Token right = getNextToken();
        return compute(left, right, op);
    }
    int test(string str)
    {
        this->setString(str);
        return this->expr();
    }
    void setString(string str)
    {
        orgTest = str;
        pos = 0;
    }
    Token getNextToken()
    {
        if(pos >= orgTest.length())
        {
            return Token(END);
        }
        VALUE v;
        switch(orgTest[pos])
        {
            case '+':
                v.oper = ADD;
                pos++;
                return Token(OP, v);
                break;
            case '-':
                v.oper = SUB;
                pos++;
                return Token(OP, v);
                break;
            case '*':
                v.oper = MULTI;
                pos++;
                return Token(OP, v);
                break;
            case '/':
                v.oper = DIV;
                pos++;
                return Token(OP, v);
                break;
            default:
                break;
        }
        int tmp = 0;
        while((48 <= orgTest[pos] && orgTest[pos] <= 57) || ' ' == orgTest[pos])
        {
            if(' ' != orgTest[pos])
            {
                tmp *= 10;
                tmp += orgTest[pos] - 48;
            }
            pos++;
        }
        v.iValue = tmp;
        return Token(INTEGER, v);
    }
    int compute(Token &left, Token &right, Token &op)
    {
        switch(op.getValue().oper)
        {
            case ADD:
                return left.getValue().iValue + right.getValue().iValue;
            case SUB:
                return left.getValue().iValue - right.getValue().iValue;
            case MULTI:
                return left.getValue().iValue * right.getValue().iValue;
            case DIV:
                return left.getValue().iValue / right.getValue().iValue;
            default:
                break;
        }
        return 0;
    }
private:
    string orgTest;
    size_t pos;
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
    Interpreter test("1 +2");
    EXPECT_EQ_INT(3, test.expr());
    EXPECT_EQ_INT(10, test.test("12 - 2"));
    EXPECT_EQ_INT(20, test.test("4* 5"));
    EXPECT_EQ_INT(4, test.test("8/2"));
}

int main()
{
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass*100.0/test_count);

    return  0;
}
