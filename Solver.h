#include "State.h"
#include <unistd.h>
using namespace std;
class Solver
{
public:
    State startingState;
    queue<Action> actions;
    priority_queue<Action> pq;
    stack<Goal> disjunctiveGoals;
    vector<Goal> goals;
    vector<State> previousMoves;
    Goal goal;
    Solver(){};
    Solver(int boardSize, int amountOfNumbers)
    {
        startingState = State(boardSize, amountOfNumbers);
    }
    bool search()
    {
        Action move;
        cout << "Initial state: \n";
        startingState.printBoard();
        setGoal(goal);
        startingState.possibleMoves(actions);
        previousMoves.push_back(startingState);
        int counter = 0;
        do
        {
            if (actions.empty())
            {
                cout << "No possible moves left!\n";
                return false;
            }
            move = actions.front(); // Take first action from possible moves
            actions.pop();
            State temp = State(startingState); // Create a copy of current State
            temp.moveBlock(move);              // Copied state completes first move
            if (existingState(temp, previousMoves))           // If it is an existing move, get ne
            {
                continue;
            }
            startingState.moveBlock(move);
            cout << "\nStep: " << counter << " Move " << startingState.isValue(move.destination) << " from " << move.source << " to " << move.destination << ".\n";
            startingState.printBoard();
            if (startingState.isGoal(goal))
            {
                cout << "Found ";
                goal.print();
                cout << "!\n";
                return true;
            }
            previousMoves.push_back(startingState);
            while (!actions.empty())
            {
                actions.pop();
            }
            startingState.possibleMoves(actions);

            counter++;
            if (actions.empty())
            {
                cout << "No possible moves left!";
                return false;
            }
        } while (!(startingState.isGoal(goal)) && (counter < 100));
        cout << "Could not find Goal State";
        return false;
    }
    bool heuristicSearch()
    {
        Action move;
        cout << "Initial state: \n";
        startingState.printBoard();
        setGoal(goal);
        startingState.possibleMoves(pq, goal);
        previousMoves.push_back(startingState);
        int counter = 0;
        do
        {
            if (pq.empty())
            {
                cout << "No possible moves left!\n";
                return false;
            }
            move = pq.top();
            pq.pop();
            State temp = State(startingState);
            temp.moveBlock(move);
            if (existingState(temp, previousMoves))
            {
                continue;
            }
            startingState.moveBlock(move);
            cout << "\nStep: " << counter << " Move " << startingState.isValue(move.destination) << " from " << move.source << " to " << move.destination << ".\n";
            startingState.printBoard();
            if (startingState.isGoal(goal))
            {
                cout << "Found ";
                goal.print();
                cout << "!\n";
                return true;
            }
            previousMoves.push_back(startingState);
            while (!pq.empty())
            {
                pq.pop();
            }
            startingState.possibleMoves(pq, goal);
            counter++;
            if (pq.empty())
            {
                cout << "No possible moves left!";
                return false;
            }
        } while (!(startingState.isGoal(goal)) && (counter < 100));
        cout << "Could not find Goal State";
        return false;
    }
    bool heuristicDisjunctiveSearch()
    {
        Action move;
        cout << "Initial state: \n";
        startingState.printBoard();
        int again = 0;
        while (again != -1 || disjunctiveGoals.size() >= startingState.getNumberOfBlocks())
        {
            setGoal(goal);
            disjunctiveGoals.push(goal);
            cout << "\nWould you like to add another goal?: ";
            cin >> again;
        }
        goal = disjunctiveGoals.top();
        disjunctiveGoals.pop();
        startingState.possibleMoves(pq, goal);
        previousMoves.push_back(startingState);
        int counter = 0;
        State base = State(startingState);
        do
        {

            if (pq.empty())
            {
                cout << "No possible moves left!\n";
                if (!disjunctiveGoals.empty())
                {
                    counter = 0;
                    startingState = base;
                    while (!pq.empty())
                    {
                        pq.pop();
                    }
                    startingState.possibleMoves(pq, goal);
                    previousMoves.push_back(startingState);
                    previousMoves.clear();
                    cout << "\nInitial state: \n";
                    startingState.printBoard();
                    goal = disjunctiveGoals.top();
                    disjunctiveGoals.pop();
                }
                else
                {
                    return false;
                }
            }
            move = pq.top(); // Take first action from possible moves
            pq.pop();
            State temp = State(startingState); // Create a copy of current State
            temp.moveBlock(move);              // Copied state completes first move
            if (existingState(temp, previousMoves))           // If it is an existing move, get new
            {
                continue;
            }
            startingState.moveBlock(move);
            cout << "\nStep: " << counter << " Move " << startingState.isValue(move.destination) << " from " << move.source << " to " << move.destination << ".\n"; // Did not make
            startingState.printBoard();
            if (startingState.isGoal(goal))
            {
                cout << "Found ";
                goal.print();
                cout << "!\n";
                counter = 0;
                startingState = base;
                while (!pq.empty())
                {
                    pq.pop();
                }
                startingState.possibleMoves(pq, goal);
                previousMoves.push_back(startingState);
                if (disjunctiveGoals.empty())
                {
                    return true;
                }
                previousMoves.clear();
                cout << "\nInitial state: \n";
                startingState.printBoard();
                goal = disjunctiveGoals.top();
                disjunctiveGoals.pop();
                continue;
            }
            previousMoves.push_back(startingState);
            while (!pq.empty())
            {
                pq.pop();
            }
            startingState.possibleMoves(pq, goal);
            counter++;
            if ((counter == 100) && (!disjunctiveGoals.empty()))
            {
                cout << "Could not find Goal State\n";
                counter = 0;
                startingState = base;
                while (!pq.empty())
                {
                    pq.pop();
                }
                startingState.possibleMoves(pq, goal);
                previousMoves.push_back(startingState);
                previousMoves.clear();
                cout << "Initial state: \n";
                startingState.printBoard();
                goal = disjunctiveGoals.top();
                disjunctiveGoals.pop();
            }
        } while (counter < 100);
        cout << "Could not find Goal State";
        return false;
    }
    bool heuristicConjunctiveSearch()
    {
        Action move;
        cout << "Initial state: \n";
        startingState.printBoard();
        int again = 0;
        while (again != -1 || goals.size() >= startingState.getNumberOfBlocks()) // Add goal states
        {
            setConjunctiveGoal(goal); //Done
            goals.push_back(goal);
            cout << "\nPress -1 to search; else type any other number for more goals: ";
            cin >> again;
        }
        startingState.possibleMoves(pq, goals); //Up to here
        previousMoves.push_back(startingState);
        int counter = 0;
        do
        {
            if (pq.empty())
            {
                cout << "No possible moves left!\n";
                return false;
            }
            move = pq.top();
            pq.pop();
            State temp = State(startingState);
            temp.moveBlock(move);

            if (existingState(temp, previousMoves))
            {
                continue;
            }
            startingState = temp;
            cout << "\nStep: " << counter << " Move " << startingState.isValue(move.destination) << " from " << move.source << " to " << move.destination << ".\n";
            startingState.printBoard();

            for (int i = 0; i < goals.size(); i++)
            {
                if (!startingState.isGoal(goals[i]))
                {
                    break;
                }
                else if (i == goals.size() - 1)
                {
                    cout << "Found goal states!\n";
                    return true;
                }
            }
            previousMoves.push_back(startingState);
            while (!pq.empty())
            {
                pq.pop();
            }
            startingState.possibleMoves(pq, goals);
            counter++;
        } while (counter < 100);
        cout << "Could not find Goal State";
        return false;
    }
    bool existingState(State currentState, vector<State> previousMoves)
    {
        for (int i = 0; i < previousMoves.size(); i++)
        {
            if (previousMoves[i] == currentState)
            {
                return true;
            }
        }
        return false;
    }
    bool existingState(State currentState, vector<SearchNode> openSet)
    {
        for (int i = 0; i < openSet.size(); i++)
        {
            if (openSet[i].state == currentState)
            {
                return true;
            }
        }
        return false;
    }
    bool valueInGoals(int value) // Could be combined with below
    {
        for (int i = 0; i < goals.size(); i++)
        {
            if (goals[i].value == value)
            {
                return true;
            }
        }
        return false;
    }
    bool targetInGoals(int row, int col) // Could be combined wiht below
    {
        for (int i = 0; i < goals.size(); i++)
        {
            if ((goals[i].row == row) && (goals[i].col == col))
            {
                return true;
            }
        }
        return false;
    }
    void setConjunctiveGoal(Goal &goal)
    {
        int value = 0;
        int row = -1;
        int column = -1;
        cout << "Input a goal in the format (block, row, col):\n";
        while ((value < 1) || (value > startingState.getNumberOfBlocks()))
        {
            cout << "Block (1 - " << startingState.getNumberOfBlocks() << "): ";
            cin >> value;
            if (value < 1 || (value > startingState.getNumberOfBlocks()))
                cout << "Number not in range\n";
        }
        if (valueInGoals(value))
        {
            cout << "Value is already in a goal\n";
            return;
        }
        goal.value = value;
        while ((row < 0) || (row >= startingState.getBoardSize()))
        {
            cout << "Row (0 - " << startingState.getBoardSize() - 1 << " bottom - up): ";
            cin >> row;
            if ((row < 0) || (row >= startingState.getBoardSize()))
                cout << "Number not in range\n";
        }

        while ((column < 0) || (column >= startingState.getBoardSize()))
        {
            cout << "Column (0 - " << startingState.getBoardSize() - 1 << " left - right): ";
            cin >> column;
            if ((column < 0) || (column >= startingState.getBoardSize()))
                cout << "Number not in range\n";
        }
        if (targetInGoals(row, column))
        {
            cout << "Location is already occupied\n";
            return;
        }
        goal.row = row;
        goal.col = column;
        goal.print();
    }
    void setGoal(Goal &goal)
    {
        int value = 0;
        int row = -1;
        int column = -1;
        cout << "Input a goal in the format (block, row, col):\n";
        while ((value < 1) || (value > startingState.getNumberOfBlocks()))
        {
            cout << "Block (1 - " << startingState.getNumberOfBlocks() << "): ";
            cin >> value;
            if (value < 1)
                cout << "Number not in range\n";
        }
        goal.value = value;
        while ((row < 0) || (row >= startingState.getBoardSize()))
        {
            cout << "Row (0 - " << startingState.getBoardSize() - 1 << " bottom - up): ";
            cin >> row;
            if ((row < 0) || (row >= startingState.getBoardSize()))
                cout << "Number not in range\n";
        }

        while ((column < 0) || (column >= startingState.getBoardSize()))
        {
            cout << "Column (0 - " << startingState.getBoardSize() - 1 << " left - right): ";
            cin >> column;
            if ((column < 0) || (column >= startingState.getBoardSize()))
                cout << "Number not in range\n";
        }
        goal.row = row;
        goal.col = column;
        goal.print();
    }
    bool AStar()
    {
        priority_queue<SearchNode *, vector<SearchNode *>, decltype(&SearchNode::compare)> openSet(&SearchNode::compare);
        stack<State> correctMoves;
        SearchNode startingNode(startingState, goal, 0, nullptr); // Initialised with it's own starting state and goal
        int statesVisited = 1;
        vector<SearchNode> previousMoves;
        cout << "Initial state: \n";
        startingState.printBoard();
        setGoal(startingNode.goal);
        cout << startingState.heuristic(startingNode.goal) << endl;
        startingNode.cost = 0;       // Set initial node to 0 + heuristic
        openSet.push(&startingNode); // Push the starting Node into a priority Queue
        while (!openSet.empty())
        {
            SearchNode *currentNode = openSet.top();
            if (currentNode->state.isGoal(currentNode->goal))
            {
                cout << "Found Goal State!\n\n";
                cout << "Starting state: \n";
                startingState.printBoard();
                previousMoves.clear();
                int steps = reconstructPath(currentNode, &previousMoves);
                for (int i = previousMoves.size() - 1; i >= 0; i--)
                {
                    previousMoves[i].state.printBoard();
                }
                cout << "Total steps: " << steps;
                return true;
            }
            openSet.pop();
            previousMoves.push_back(*currentNode);
            for (State neighbourState : currentNode->possibleMoves(currentNode->state))
            {
                // double tempScore = currentNode->cost; //(100 / ((currentNode->state.getBoardSize() - 1) * 3) + 2); // Add one to cost, as a move is occurring
                SearchNode *neighbourNode = new SearchNode(neighbourState, currentNode->goal, currentNode->fScore, currentNode);
                neighbourNode->fScore = neighbourNode->state.heuristic(goal) + neighbourNode->cost; // neighbourNode assigns cost as the heuristic plus the number of moves in parent
                if (existingState(neighbourState, previousMoves))                                   // If it is an existing move, get a new Node
                {
                    continue;
                }
                cout << "Number of states visited " << statesVisited << ". Current heuristic value " << neighbourNode->fScore << endl;
                neighbourNode->state.printBoard();
                statesVisited++;
                openSet.push(neighbourNode);
            }
        }
        return false;
    }

    int reconstructPath(SearchNode *goalNode, vector<SearchNode> *history)
    {
        int steps = 0;
        while (goalNode->parent != nullptr)
        {
            history->push_back(*goalNode);
            if (goalNode->parent == nullptr)
            {
                cout << "Parent Found!";
                break;
            }
            steps++;
            goalNode = goalNode->parent;
        }
        return steps;
    }
    bool MonteCarlo() // Can make a recursive function
    {
        priority_queue<Action, vector<Action>, decltype(&Action::compare)> montePQ(&Action::compare);
        Action move;
        State temp;
        int steps = 0;
        setGoal(goal);
        cout << "Starting State: \n";
        while (!startingState.isGoal(goal))
        {
            startingState.possibleMoves(actions); // Puts the base moves
            previousMoves.push_back(startingState); // Pushes starting state into previous moves
            int size = actions.size();
            startingState.printBoard();
            for (int i = 0; i < size; i++)
            {
                temp = startingState;
                move = actions.front();
                actions.pop();
                temp.moveBlock(move);
                move.heuristic = simulation(temp);
                montePQ.push(move);
            }
            move = montePQ.top();
            temp = startingState;
            temp.moveBlock(move);
            montePQ.pop();
            while(existingState(temp, previousMoves))
            {
                if(montePQ.empty())
                {
                    cout << "Ran out of moves\n";
                    return false;
                }
                temp = startingState;
                move = montePQ.top();
                temp.moveBlock(move);
                montePQ.pop();
            }
            startingState.moveBlock(move);
            while(!montePQ.empty())
            {
                montePQ.pop();
            }
            steps++;
            cout << "\nStep: " << steps << " Move " << startingState.isValue(move.destination) << " from " << move.source << " to " << move.destination << ". With heuristic: " << move.heuristic <<"\n";
        }
        cout << "Found Goal" << endl;
        startingState.printBoard();
    }
    double expansion(State state)
    {
        vector<State> MontePre;
        Action move;
        state.randomMove(pq);
        MontePre.push_back(state);
        int counter = 0;
        do
        {
            if (pq.empty())
            {
                MontePre.clear();
                return 1;
            }
            move = pq.top(); // Take first action from possible moves
            pq.pop();
            State temp = State(state); // Create a copy of current State
            temp.moveBlock(move);      // Copied state completes first move
            if (existingState(temp, MontePre))   // If it is an existing move, get ne
            {
                continue;
            }
            state.moveBlock(move);
            if (state.isGoal(goal))
            {
                MontePre.clear();
                return 0;
            }
            MontePre.push_back(state);
            while (!pq.empty())
            {
                pq.pop();
            }
            state.randomMove(pq);
            counter++;
            if (pq.empty())
            {
                MontePre.clear();
                return 1;
            }
        } while (counter < 1000);
        MontePre.clear();
        return 1;
    }
    double simulation(State state)
    {
        double win = 0.0;
        for (int i = 0; i < 2000; i++) // 100 * iterations of 50
        {
            State tempBoard = state;
            win += expansion(tempBoard);
        }
        return (win / 20);
    }
};