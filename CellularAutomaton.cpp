#include "CellularAutomaton.hpp"


QSet<char> CellularAutomaton::digits, CellularAutomaton::operators;



CellularAutomaton::SyntaxExrrorException::SyntaxExrrorException(QString msg, int pos)
{
    position = pos;
    message = msg.replace("%p", QString::number(pos));
}


CellularAutomaton::SyntaxExrrorException::SyntaxExrrorException(const SyntaxExrrorException *other)
{
    message = other->message;
    position = other->position;
}


CellularAutomaton::SyntaxExrrorException *CellularAutomaton::SyntaxExrrorException::clone() const
{
    return new SyntaxExrrorException(this);
}


void CellularAutomaton::SyntaxExrrorException::raise() const
{
    throw *this;
}


QString CellularAutomaton::SyntaxExrrorException::getMessage()
{
    return message;
}


int CellularAutomaton::SyntaxExrrorException::getPosition()
{
    return position;
}


CellularAutomaton::ScriptBrick::~ScriptBrick()
{

}


StatusT CellularAutomaton::ScriptBrick::exec(CellInfo *cell)
{
    std::cerr << "CellularAutomaton::ScriptBrick::exec: shouldn't be called!" << std::endl;
}


CellularAutomaton::ScriptBrickIf::ScriptBrickIf(QString condition) throw (SyntaxExrrorException *)
{
    comparisionOperator = NONE;

    switch (condition.count("=")) {
    case 0:
        break;
    case 1:
        if (comparisionOperator != NONE)
            SyntaxExrrorException(QObject::tr("Nadmiarowy operator porównania na pozycji %i."), condition.indexOf("=")).raise();
        comparisionOperator = EQUAL;
        break;
    }

    switch (condition.count("!=")) {
    case 0:
        break;
    case 1:
        if (comparisionOperator != NONE)
            SyntaxExrrorException(QObject::tr("Nadmiarowy operator porównania na pozycji %i."), condition.indexOf("!=")).raise();
        comparisionOperator = NOT_EQUAL;
        break;
    }

    switch (condition.count("<")) {
    case 0:
        break;
    case 1:
        if (comparisionOperator != NONE)
            SyntaxExrrorException(QObject::tr("Nadmiarowy operator porównania na pozycji %i."), condition.indexOf("<")).raise();
        comparisionOperator = LESS;
        break;
    }

    switch (condition.count("<=")) {
    case 0:
        break;
    case 1:
        if (comparisionOperator != NONE)
            SyntaxExrrorException(QObject::tr("Nadmiarowy operator porównania na pozycji %i."), condition.indexOf("<=")).raise();
        comparisionOperator = LESS_OR_EQUAL;
        break;
    }

    switch (condition.count(">")) {
    case 0:
        break;
    case 1:
        if (comparisionOperator != NONE)
            SyntaxExrrorException(QObject::tr("Nadmiarowy operator porównania na pozycji %i."), condition.indexOf(">")).raise();
        comparisionOperator = GREATER;
        break;
    }

    switch (condition.count(">=")) {
    case 0:
        break;
    case 1:
        if (comparisionOperator != NONE)
            SyntaxExrrorException(QObject::tr("Nadmiarowy operator porównania na pozycji %i."), condition.indexOf(">=")).raise();
        comparisionOperator = GREATER_OR_EQUAL;
        break;
    }

    QStringList list;
    switch (comparisionOperator) {
    case NONE:
        SyntaxExrrorException(QObject::tr("Brak operatora porównania."), condition.length()).raise();
        break;
    case EQUAL:
        list = condition.split("=");
        break;
    case NOT_EQUAL:
        list = condition.split("!=");
        break;
    case LESS:
        list = condition.split("<");
        break;
    case LESS_OR_EQUAL:
        list = condition.split("<=");
        break;
    case GREATER:
        list = condition.split(">");
        break;
    case GREATER_OR_EQUAL:
        list = condition.split(">=");
        break;
    }

    leftExpression = list[0];
    try {
        checkSyntaxOfMathexpr(leftExpression.toUtf8().constData());
    } catch (SyntaxExrrorException *e) {
        SyntaxExrrorException(QObject::tr("Błąd przetwarzania lewego wyrażenia: ") + e->getMessage(), e->getPosition()).raise();
    }

    rightExpression = list[1];
    try {
        checkSyntaxOfMathexpr(rightExpression.toUtf8().constData());
    } catch (SyntaxExrrorException *e) {
        SyntaxExrrorException(QObject::tr("Błąd przetwarzania prawego wyrażenia: ") + e->getMessage(), e->getPosition()).raise();
    }

    then = nullptr;
    next = nullptr;
}


CellularAutomaton::ScriptBrickIf::~ScriptBrickIf()
{
    if (next != nullptr)
        delete next;
    if (then != nullptr)
        delete then;
}


StatusT CellularAutomaton::ScriptBrickIf::exec(CellInfo *cell)
{
    QString left = leftExpression;
    for (size_t i = 0; i < 32; i++) {
        left = left.replace(QString("$") + QString::number(i), QString::number(cell->get(i)));
    }

    QString right = rightExpression;
    for (size_t i = 0; i < 32; i++) {
        right = right.replace(QString("$") + QString::number(i), QString::number(cell->get(i)));
    }

    bool result = false;
    switch (comparisionOperator) {
    case EQUAL:
        result = (CellularAutomaton::calculateMathexpr(right.toUtf8().constData()) ==
                  CellularAutomaton::calculateMathexpr(left.toUtf8().constData()));
        break;
    case NOT_EQUAL:
        result = (CellularAutomaton::calculateMathexpr(right.toUtf8().constData()) !=
                  CellularAutomaton::calculateMathexpr(left.toUtf8().constData()));
        break;
    case LESS:
        result = (CellularAutomaton::calculateMathexpr(right.toUtf8().constData()) <
                  CellularAutomaton::calculateMathexpr(left.toUtf8().constData()));
        break;
    case LESS_OR_EQUAL:
        result = (CellularAutomaton::calculateMathexpr(right.toUtf8().constData()) <=
                  CellularAutomaton::calculateMathexpr(left.toUtf8().constData()));
        break;
    case GREATER:
        result = (CellularAutomaton::calculateMathexpr(right.toUtf8().constData()) >
                  CellularAutomaton::calculateMathexpr(left.toUtf8().constData()));
        break;
    case GREATER_OR_EQUAL:
        result = (CellularAutomaton::calculateMathexpr(right.toUtf8().constData()) >=
                  CellularAutomaton::calculateMathexpr(left.toUtf8().constData()));
        break;
    }

    if (result) {
        if (then != nullptr) {
            return then->exec(cell);
        } else {
            std::cerr << "CellularAutomaton::ScriptBrickIf::exec: then == nullptr!" << std::endl;
        }
    } else {
        if (next != nullptr) {
            return next->exec(cell);
        } else {
            std::cerr << "CellularAutomaton::ScriptBrickIf::exec: next == nullptr!" << std::endl;
        }
    }
}


CellularAutomaton::ScriptBrickReturn::ScriptBrickReturn(StatusT v)
{
    value = v;
}


CellularAutomaton::ScriptBrickReturn::~ScriptBrickReturn()
{

}


StatusT CellularAutomaton::ScriptBrickReturn::exec(CellInfo *cell)
{
    return value;
}


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


int CellularAutomaton::checkSyntaxOfMathexpr(const char *expr) throw (SyntaxExrrorException *)
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
                SyntaxExrrorException(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1).raise();
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
                SyntaxExrrorException(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1).raise();
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
                SyntaxExrrorException(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1).raise();
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
                SyntaxExrrorException(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1).raise();
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
                SyntaxExrrorException(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1).raise();
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
                SyntaxExrrorException(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1).raise();
            }
            break;
        }

        if (bracketsLevel < 0) {
            SyntaxExrrorException(QObject::tr("Nie prawidłowy znak %i - nadmiarowy \")\"."), cp - expr + 1).raise();
        }
    }

    if (bracketsLevel > 0)
        SyntaxExrrorException(QObject::tr("Nie prawidłowy znak %i - oczekiwano \")\"."), strlen(expr)).raise();

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
