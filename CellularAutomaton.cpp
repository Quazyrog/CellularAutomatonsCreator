#include "CellularAutomaton.hpp"
#include "CellInfo.hpp"


QSet<char> CellularAutomaton::digits, CellularAutomaton::operators;


CellularAutomaton::CellularAutomaton()
{

}


void CellularAutomaton::initialize()
{
    if (digits.empty()) {
        digits.insert('0');
        digits.insert('1');
        digits.insert('2');
        digits.insert('3');
        digits.insert('4');
        digits.insert('5');
        digits.insert('6');
        digits.insert('7');
        digits.insert('8');
        digits.insert('9');
    }

    if (operators.empty()) {
        operators.insert('+');
        operators.insert('-');
        operators.insert('*');
        operators.insert('/');
    }
}


long long int CellularAutomaton::calculateMathexpr(const char *expr, size_t length)
{
    int result = 0;

    if (expr[0] == '(' && expr[length - 1] == ')') {
        return calculateMathexpr(expr + 1, length - 2);
    }

    int level = 0;
    const char *op = nullptr;
    for (const char *c = expr; c < expr + length; c++) {
        switch (*c) {
        case ')':
            level--;
            break;
        case '(':
            level++;
            break;
        case '+':
        case '-':
            if (level == 0) {
                op = c;
                c = expr + length;
                break;
            }
        case '*':
        case '/':
            if (level == 0 && op == nullptr) {
                op = c;
            }
        }
    }

    if (op == nullptr) {
        std::cerr << std::string(expr).substr(0, length) << " = " << atoll(expr) << std::endl;
        return atoll(expr);
    }

    const char *left, *right = op + 1;
    size_t leftLength, rightLength = 0;
    left = expr;
    leftLength = op - left;
    result = calculateMathexpr(left, leftLength);

    char expectedOperator[2];
    if (*op == '+' || *op == '-') {
        expectedOperator[0] = '+';
        expectedOperator[1] = '-';
    } else if (*op == '*' || *op == '/') {
        expectedOperator[0] = '*';
        expectedOperator[1] = '/';
    }

    while (right + rightLength < expr + length) {
        right = op + 1;
        const char *c;
        for (c = right; c < expr + length ; c++) {
            if (*c == ')') {
                level--;
            } else if (*c == '(') {
                level++;
            } else if ((*c == expectedOperator[0]) || (*c == expectedOperator[1])) {
                if (level == 0) {
                    break;
                }
            }
        }

        rightLength = c - right;

        switch (*op) {
        case '+':
            result += calculateMathexpr(right, rightLength);
            break;
        case '-':
            result -= calculateMathexpr(right, rightLength);
            break;
        case '*':
            result *= calculateMathexpr(right, rightLength);
            break;
        case '/':
            result /= calculateMathexpr(right, rightLength);
            break;
        }

        op = c;
    }
    std::cerr << std::string(expr).substr(0, length) << " = " << result << std::endl;
    return result;
}


int CellularAutomaton::checkSyntaxOfMathexpr(const char *expr)
{
    MathexprCharType previous = NOTHING;
    int bracketsLevel = 0;
    for (const char *cp = expr; *cp != 0; cp++) {
        std::cerr << *cp;
        switch (previous) {
        case DIGIT:
            if (digits.contains(*cp)) {
                previous = DIGIT;
            } else if (operators.contains(*cp)) {
                previous = OPERATOR;
            } else if (*cp == ')') {
                previous = BRACKET_CLOSE;
                bracketsLevel--;
            } else if (*cp == ' ') {
            } else {
                return cp - expr + 1;
            }
            break;
        case OPERATOR:
            if (digits.contains(*cp)) {
                previous = DIGIT;
            } else if (*cp == '(') {
                previous = BRACKET_OPEN;
                bracketsLevel++;
            } else if (*cp == ')') {
                previous = BRACKET_CLOSE;
                bracketsLevel--;
            } else if (*cp == ' ') {
            } else {
                return cp - expr + 1;
            }
            break;
        case BRACKET_OPEN:
            if (digits.contains(*cp)) {
                previous = DIGIT;
            }else if (*cp == '(') {
                previous = BRACKET_OPEN;
                bracketsLevel++;
            } else if (*cp == '-') {
                previous = MINUS;
            } else if (*cp == ' ') {
            } else {
                return cp - expr + 1;
            }
            break;
        case BRACKET_CLOSE:
            if (operators.contains(*cp)) {
                previous = OPERATOR;
            } else if (*cp == ')') {
                previous = BRACKET_CLOSE;
                bracketsLevel--;
            } else if (*cp == ' ') {
            } else {
                return cp - expr + 1;
            }
            break;
        case MINUS:
            if (digits.contains(*cp)) {
                previous = DIGIT;
            } else if (*cp == '(') {
                previous = BRACKET_OPEN;
                bracketsLevel++;
            } else if (*cp == ' ') {
            } else {
                return cp - expr + 1;
            }
            break;
        case NOTHING:
            if (digits.contains(*cp)) {
                previous = DIGIT;
            } else if (*cp == '(') {
                previous = BRACKET_OPEN;
                bracketsLevel++;
            } else if (*cp == ')') {
                previous = BRACKET_CLOSE;
                bracketsLevel--;
            } else if (*cp == '-') {
                previous = MINUS;
            } else if (*cp == ' ') {
                previous = NOTHING;
            } else {
                return cp - expr + 1;
            }
            break;
        }

        if (bracketsLevel < 0) {
            return cp - expr;
        }
    }

    if (bracketsLevel != 0)
        return -1;

    return 0;
}


long long int CellularAutomaton::calculateMathexpr(const char *expr)
{
    int spaces = 0;
    for (const char *c = expr; *c != 0; c++) {
        if (*c == ' ')
            spaces++;
    }

    char *buffer = new char [strlen(expr) - spaces];
    char *dest = buffer;
    for (const char *src = expr; *src != 0; src++) {
        if (*src != ' ') {
            *dest = *src;
            dest++;
        }
    }

    return calculateMathexpr(buffer, strlen(expr) - spaces);
}
