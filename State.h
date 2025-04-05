#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <stack>
#include <queue>
#include "Action.h"
#include "Goal.h"
#include <unordered_set>

using namespace std;

class State
{
    int size, boardNumber;
    vector<vector<int> > board;
    vector<State> previousMoves;

public:
    State() : board(3, vector<int>(3, 0))
    {
        size = 3;
        boardNumber = 6;
        random(size, boardNumber);
        pushdown();
    }
    State(int boardSize, int amountOfNumbers) : board(boardSize, vector<int>(boardSize, 0))
    {
        size = boardSize;
        boardNumber = amountOfNumbers;
        random(size, boardNumber);
        pushdown();
    }

    bool operator==(const State &state) const
    {
        for (int i = 0; i < state.board.size(); i++)
        {
            for (int j = 0; j < state.board[i].size(); j++)
            {
                if (board[i][j] != state.board[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }
    // Task 1.2
    int getBoardSize();
    int getNumberOfBlocks();
    void random(int size, int boardrange); // Input size to be size of the board, board range are the numbers that are put on the board
    void pushdown();                       // Pushed down numbers until they are at the bottom of the board
    void printBoard();                     // Prints board
    // Task 1.3.1
    bool isFull(int col);                    // If column is full returns true
    bool isEmpty(int col);                   // If column is empty return true
    int removeBlock(int column);             // If column is not empty, return block
    bool insertBlock(int column, int value); // If column is full return false, otherwise place block
    bool moveBlock(Action a);                // Read source and destination from action object
    // Task 1.3.2
    void possibleMoves(queue<Action> &actions, Goal goal);
    bool isGoal(Goal goal);
    int isValue(int col);
    // Task 2
    double heuristic(Goal goal);
    void possibleMoves(queue<Action> &actions);
    void possibleMoves(priority_queue<Action> &actions, Goal goal);
    // Task 3
    void possibleMoves(priority_queue<Action> &actions, vector<Goal> goals);
    // Task 4
    void randomMove(priority_queue<Action> &actions);
};
int State::getBoardSize()
{
    return size;
}
int State::getNumberOfBlocks()
{
    return boardNumber;
}
void State::random(int size, int boardNumber)
{
    srand(time(0));
    for (int i = 1; i <= boardNumber; i++)
    {
        int row = rand() % size;
        int col = rand() % size;
        while (board[row][col] != 0)
        {
            row++;
            if (row % size == 0)
            {
                col++;
                row = 0;
                if (col % size == 0)
                {
                    col = 0;
                    row = 0;
                }
            }
        }
        board[row][col] = i;
    }
}
void State::pushdown()
{
    stack<int> dropdown;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[j][i] != 0)
            {
                dropdown.push(board[j][i]);
            }
        }
        for (int j = size - 1; j >= 0; j--)
        {
            if (!dropdown.empty())
            {
                board[j][i] = dropdown.top();
                dropdown.pop();
            }
            else
            {
                board[j][i] = 0;
            }
        }
    }
}
void State::printBoard()
{
    for (int i = 0; i < size; i++)
    {
        cout << " ----";
    }
    cout << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {

            cout << "| " << setw(2) << board[i][j] << " ";
        }
        cout << "|" << endl;
    }
    for (int i = 0; i < size; i++)
    {
        cout << " ----";
    }
    cout << endl;
}
bool State::isFull(int col)
{
    if (board[0][col] != 0)
    {
        return true;
    }
    return false;
}
bool State::isEmpty(int col)
{
    if (board[size - 1][col] == 0)
    {
        return true;
    }
    return false;
}
int State::removeBlock(int column)
{
    if (!isEmpty(column))
    {
        //int takenColumn = column;
        for (int i = 0; i < size; i++)
        {
            if (board[i][column] != 0)
            {
                int value = board[i][column];
                board[i][column] = 0;
                return value;
            }
        }
    }
    return -1;
}
bool State::insertBlock(int column, int value)
{
    if (value == -1)
    {
        return false;
    }
    else if (!isFull(column))
    {
        for (int i = size - 1; i >= 0; i--)
        {
            if (board[i][column] == 0)
            {
                board[i][column] = value;
                return true;
            }
        }
    }
    return false;
}
bool State::moveBlock(Action a)
{
    if (a.destination == a.source)
    {
        return false;
    }
    else if (insertBlock(a.destination, removeBlock(a.source)))
    {
        return true;
    }
    return false;
}
int State::isValue(int col)
{
    for (int i = 0; i < size; i++)
    {
        if (board[i][col] != 0)
        {
            return board[i][col];
        }
    }
    return -1;
}
void State::possibleMoves(queue<Action> &actions)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            Action action;
            action.destination = j;
            action.source = i;
            State temp = State(*this);
            if (temp.moveBlock(action))
            {
                actions.push(action);
            }
        }
    }
    return;
}
bool State::isGoal(Goal goal)
{
    if (board[goal.row][goal.col] == goal.value)
    {
        return true;
    }
    return false;
}
double State::heuristic(Goal goal) // Need to calibrate for multiple goals
{
    double distance = 0;
    double cost = 100 / ((size-1)*3)+2;
    int row, col;
    for (row = 0; row < board.size(); row++)
    {
        for (col = 0; col < board[row].size(); col++)
        {
            if (board[row][col] == goal.value)
            {
                goto check;
            }
        }
    }
check:
    // Change so that is adjusts with boardsize
    if (board[goal.row][goal.col] == goal.value) // If is goal, return 0
    {
        return distance;
    }
    if (board[goal.row][goal.col] != goal.value) // If goal row/col is not value ((n-1)*3) + 2
    {
        distance += cost;
    }
    if (board[goal.row][goal.col] != 0) // If goal row/col is not 0
    {
        distance += cost;
    }
    int above = 0;
    while ((row - above) > -1) // number of blocks above target not equal to 0
    {
        if (board[row - above][col] != 0)
            distance += cost;
        above++;
    }
    above = 0;
    while ((goal.row - above) > -1) // number of blocks above location
    {
        if (board[goal.row - above][goal.col] != 0)
            distance += cost;
        above++;
    }
    int below = 1;
    while ((goal.row + below) < size) // number of blocks below target is 0
    {
        if (board[goal.row + below][goal.col] == 0)
            distance += cost;
        below++;
    }

    return distance;
}

void State::possibleMoves(priority_queue<Action> &actions, Goal goal)
{
    int row, col;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            Action action;
            action.destination = j;
            action.source = i;
            State temp = State(*this);
            if (temp.moveBlock(action))
            {
                action.heuristic = temp.heuristic(goal);
                actions.push(action);
            }
        }
    }
}
void State::randomMove(priority_queue<Action> &actions)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            Action action;
            action.destination = j;
            action.source = i;
            State temp = State(*this);
            if (temp.moveBlock(action))
            {
                action.heuristic = rand() % 10;
                actions.push(action);
            }
        }
    }
    return;
}
void State::possibleMoves(priority_queue<Action> &actions, vector<Goal> goals)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            Action action;
            action.destination = j;
            action.source = i;
            State temp = State(*this);
            if (temp.moveBlock(action))
            {
                for (int i = 0; i < goals.size(); i++)
                {
                    action.heuristic += temp.heuristic(goals[i]);
                }
                action.heuristic /= goals.size();
                actions.push(action);
                action.heuristic = 0;
            }
        }
    }
}

class SearchNode
{
public:
    State state;
    Goal goal;
    double cost;
    double fScore;
    SearchNode *parent;
    SearchNode() {}
    SearchNode(State newState, Goal sameGoal, double newCost, SearchNode *parentNode)
    {
        state = newState;
        goal = sameGoal;
        cost = newCost;
        parent = parentNode;
    }
    bool operator==(const SearchNode &other) const
    {
        return state == other.state;
    }

    static bool compare(SearchNode* n1, SearchNode* n2)
    {
        return n1->fScore > n2->fScore;
    }
    
    vector<State> possibleMoves(State node)
    {
        vector<State> moves;
        for (int i = 0; i < node.getBoardSize(); i++)
        {
            for (int j = 0; j < node.getBoardSize(); j++)
            {
                Action action;
                action.destination = j;
                action.source = i;
                State temp = State(node);
                if (temp.moveBlock(action))
                {
                    // action.heuristic = temp.heuristic(goal);
                    moves.push_back(temp);
                }
            }
        }
        return moves;
    }
};