class Goal
{
    public: 
    int value, row, col;
    Goal(){
        value = 1;
        row = 1;
        col = 1;
    }
    void print() const;
};

void Goal::print() const
{
    cout << "Goal (" << value << ", " << row << ", " << col << ")";
}