#include "CellularAutomaton.hpp"


QSet<char> CellularAutomaton::digits, CellularAutomaton::operators;


SyntaxCheckResult::SyntaxCheckResult(QString m, int p)
{
    msg = m.replace("%i", QString::number(p + 1));
    invalid = p + 1;
}


ScriptBrick::~ScriptBrick()
{

}


StatusT ScriptBrick::exec(CellInfo *cell)
{
    std::cerr << "ScriptBrick::exec: shouldn't be called!" << std::endl;
    return 0;
}


ScriptBrickIf::ScriptBrickIf(QString l, ComparisionOperators c, QString r)
{
    leftExpression = l;
    comparisionOperator = c;
    rightExpression = r;

    then = nullptr;
    next = nullptr;
}


ScriptBrickIf::~ScriptBrickIf()
{
    if (next != nullptr)
        delete next;
    if (then != nullptr)
        delete then;
}


StatusT ScriptBrickIf::exec(CellInfo *cell)
{
    QString left = leftExpression.replace("$$", QString::number(cell->me()));
    left = left.replace("$$", QString::number(cell->me()));
    for (size_t i = 0; i < 32; i++) {
        left = left.replace(QString("$") + QString::number(i), QString::number(cell->get(i)));
    }

    QString right = rightExpression.replace("$$", QString::number(cell->me()));
    right = right.replace("$$", QString::number(cell->me()));
    for (size_t i = 0; i < 32; i++) {
        right = right.replace(QString("$") + QString::number(i), QString::number(cell->get(i)));
    }

    bool result = false;
    long long int l, r;
    r = CellularAutomaton::calculateMathexpr(right.toUtf8().constData());
    l = CellularAutomaton::calculateMathexpr(left.toUtf8().constData());
    switch (comparisionOperator) {
    case EQUAL:
        result = (l == r);
        break;
    case NOT_EQUAL:
        result = (l != r);
        break;
    case LESS:
        result = (l < r);
        break;
    case LESS_OR_EQUAL:
        result = (l <= r);
        break;
    case GREATER:
        result = (l > r);
        break;
    case GREATER_OR_EQUAL:
        result = (l >= r);
        break;
    }

    if (result) {
        if (then != nullptr) {
            return then->exec(cell);
        } else {
            std::cerr << "ScriptBrickIf::exec: then == nullptr!" << std::endl;
        }
    } else {
        if (next != nullptr) {
            return next->exec(cell);
        } else {
            std::cerr << "ScriptBrickIf::exec: next == nullptr!" << std::endl;
        }
    }

    return 0;
}


ScriptBrickReturn::ScriptBrickReturn(StatusT v)
{
    value = v;
}


ScriptBrickReturn::~ScriptBrickReturn()
{

}


StatusT ScriptBrickReturn::exec(CellInfo *cell)
{
    return value;
}


CellularAutomaton::CellularAutomaton()
{
    grid = nullptr;
    oldGrid = nullptr;
    startScript = new ScriptBrickReturn(0);

    gridWidth = 0;
    gridHeight = 0;
    info = new CellInfo;
}


CellularAutomaton::~CellularAutomaton()
{
    if (startScript != nullptr) {
        delete startScript;
    }

    deleteGrid();
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
        digits.insert('$');
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
    return result;
}


SyntaxCheckResult CellularAutomaton::checkSyntaxOfMathexpr(const char *expr)
{
    MathexprCharType previous = NOTHING;
    int bracketsLevel = 0;
    for (const char *cp = expr; *cp != 0; cp++) {
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
                return SyntaxCheckResult(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1);
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
                return SyntaxCheckResult(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1);
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
                return SyntaxCheckResult(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1);
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
                return SyntaxCheckResult(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1);
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
                return SyntaxCheckResult(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1);
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
                return SyntaxCheckResult(QObject::tr("Nie prawidłowy znak %i."), cp - expr + 1);
            }
            break;
        }

        if (bracketsLevel < 0) {
            return SyntaxCheckResult(QObject::tr("Nie prawidłowy znak %i - nadmiarowy \")\"."), cp - expr + 1);
        }
    }

    if (bracketsLevel > 0)
        return SyntaxCheckResult(QObject::tr("Nie prawidłowy znak %i - oczekiwano \")\"."), strlen(expr));

    return SyntaxCheckResult("OK", -1);
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


void CellularAutomaton::setScript(ScriptBrick *start)
{
    if (startScript != nullptr) {
        delete startScript;
    }
    startScript = start;
}


void CellularAutomaton::nextGeneration()
{
    StatusT **g;
    g = oldGrid;
    oldGrid = grid;
    grid = g;

    for (size_t x = 0; x < gridWidth; x++) {
        for (size_t y = 0; y < gridHeight; y++) {
            StatusT s = startScript->exec(collectCellInfo(x, y));
            std::cerr << "[" << (int) s << ": " << (int) collectCellInfo(x, y)->get(0) << " " << (int) collectCellInfo(x, y)->get(1) << "]  ";
            grid[x][y] = s;
        }
        std::cerr << std::endl;
    }
    std::cerr << std::endl;
}


CellInfo *CellularAutomaton::collectCellInfo(size_t x, size_t y)
{
    info = new(info) CellInfo;
    info->me() = oldGrid[x][y];
    for (int xx = x - 1; xx <= (int) x + 1; xx++) {
        for (int yy = y - 1; yy <= (int) y + 1; yy++) {
            if (0 <= yy && yy <= (int) gridHeight - 1 && 0 <= xx && xx <= (int) gridWidth - 1 && !(xx == (int) x && yy == (int) y))
                info->get(oldGrid[xx][yy])++;
        }
    }
    return info;
}


void CellularAutomaton::createGrid(size_t w, size_t h)
{
    deleteGrid();

    grid = new StatusT * [w];
    for (size_t x = 0; x < w; x++) {
        grid[x] = new StatusT [h];
    }

    oldGrid = new StatusT * [w];
    for (size_t x = 0; x < w; x++) {
        oldGrid[x] = new StatusT [h];
    }

    gridWidth = w;
    gridHeight = h;

    clear();
}


void CellularAutomaton::deleteGrid()
{
    if (grid != nullptr) {
        for (size_t r = 0; r < gridHeight; r++) {
            delete [] grid[r];
        }
        delete [] grid;
    }

    if (oldGrid != nullptr) {
        for (size_t r = 0; r < gridHeight; r++) {
            delete [] oldGrid[r];
        }
        delete [] oldGrid;
    }
}


StatusT CellularAutomaton::get(size_t x, size_t y)
{
    return grid[x][y];
}


void CellularAutomaton::set(size_t x, size_t y, StatusT stat)
{
    grid[x][y] = stat % STATUS_NUMBER;
}


void CellularAutomaton::clear()
{
    for (size_t x = 0; x < gridWidth; x++)
        memset(grid[x], 0, gridHeight);
}
