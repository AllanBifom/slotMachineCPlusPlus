#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <tuple>
#include <utility>

using namespace std;

class SlotMachine
{
public:
    int MAX_LINES;
    int MIN_BET;
    int MAX_BET;
    int ROWS;
    int COLUMNS;

    map<string, int> symbolCount;
    map<string, int> symbolMultiplier;

    /**
     * Prompts user to deposit Money into their account
     * Check for invalid input and constraint.
     */
    int getDeposit()
    {
        int amount;
        while (true)
        {
            cout << "How much would you like to deposit (min $10)? $";
            cin >> amount;
            if (cin.fail() || amount < 10)
            {
                cout << "Must be a number at least $10." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
            }
            else
            {
                break;
            }
        }
        return amount;
    }

    /**
     * Prompts the user to enter the number of lines they want to put money on
     * Checks for invalid input and constraint.
     */
    int getNumberOfLines()
    {
        int lines;
        while (true)
        {
            cout << "Enter the number of lines to bet (1-" << MAX_LINES << ")? ";
            cin >> lines;
            if (cin.fail() || lines < 1 || lines > MAX_LINES)
            {
                cout << "Enter a valid number of lines (1 - " << MAX_LINES << ")" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
            }
            else
            {
                break;
            }
        }
        return lines;
    }

    /**
     * Prompts the user to enter how much they want to place on each line.
     * Checks for invalid input and constraint.
     */
    int getPlayersBet()
    {
        int amount;
        while (true)
        {
            cout << "How much would you like to bet on each line (min $" << MIN_BET << " - max $" << MAX_BET << ")? ";
            cin >> amount;
            if (cin.fail() || amount < MIN_BET || amount > MAX_BET)
            {
                cout << "Amount must be between $" << MIN_BET << " - $" << MAX_BET << endl;
                cin.clear();
                cin.ignore(10000, '\n');
            }
            else
            {
                break;
            }
        }
        return amount;
    }

    /**
     * Organizes our slot Machine into row * columns matrix of elements
     * rows: an integer representing the number of horizontal rows in our machine
     * column: an integer representing the number of vertical columns in our machine.
     * symbols: a dictionary that stores the symbols in our machine, with thier respective count in each row
     *          meaning in row 1, symbol there are n number of symbol x,y and z and we randomly select which one goes into each column
     *          it resets in each row.
     * returns: a 2 dimensional array that gets the arrangement of the machine after the spin.
     */
    vector<vector<string> > getSlotMachineArrangment(int rows, int cols, map<string, int> symbols)
    {
        vector<string> allSymbols;
        for (auto const &symbol : symbols)
        {
            for (int i = 0; i < symbol.second; i++)
            {
                allSymbols.push_back(symbol.first);
            }
        }

        vector<vector<string> > columns;
        for (int i = 0; i < cols; i++)
        {
            vector<string> column;
            vector<string> currentSymbols = allSymbols;
            for (int j = 0; j < rows; j++)
            {
                int randomIndex = rand() % currentSymbols.size();
                string value = currentSymbols[randomIndex];
                currentSymbols.erase(currentSymbols.begin() + randomIndex);
                column.push_back(value);
            }
            columns.push_back(column);
        }
        return columns;
    }

    /**
     * displays our slot machine in a readable form for the user
     * columns: a 2 * 2 array storing the spin on the slot machine
     */
    void printSlotMachine(vector<vector<string> > columns)
    {
        for (int row = 0; row < columns[0].size(); row++)
        {
            for (int i = 0; i < columns.size(); i++)
            {
                if (i != columns.size() - 1)
                {
                    cout << columns[i][row] << " | ";
                }
                else
                {
                    cout << columns[i][row];
                }
            }
            cout << endl;
        }
    }

    /**
     * This checks if the user won anything
     * columns: A 2 * 2 array, representing the slot machine arrangments.
     * lines: An integer storing the number of lines the user bet on
     * bet: An integer representing the bet the user made
     * values: A dictionary storing each symbol and thier multiplier value.
     */
    pair<int, vector<int> > checkWinnings(vector<vector<string> > columns, int lines, int bet, map<string, int> values)
    {
        int winnings = 0;
        vector<int> winningLines;
        for (int line = 0; line < lines; line++)
        {
            string symbol = columns[0][line];
            bool isWinning = true;
            for (vector<string> column : columns)
            {
                string symbolToCheck = column[line];
                if (symbol != symbolToCheck)
                {
                    isWinning = false;
                    break;
                }
            }
            if (isWinning)
            {
                winnings += values[symbol] * bet;
                winningLines.push_back(line + 1);
            }
        }
        return make_pair(winnings, winningLines);
    }

    /**
     * This combines all the methods together
     * it spins the machine and checks if the user won anything and displayes it to the user.
     * userBalance: An integer storing how much the user has in his balance 
     */
    int spinMachine(int userBalance)
    {
        int slotLines = getNumberOfLines();
        int bet, totalBet;
        while (true)
        {
            bet = getPlayersBet();
            totalBet = bet * slotLines;
            if (totalBet > userBalance)
            {
                cout << "You do not have enough money in your balance to bet that amount, your current balance is $" << userBalance << endl;
            }
            else
            {
                break;
            }
        }
        cout << "You are putting $" << bet << " on " << slotLines << " lines. Total bet is " << totalBet << endl;
        vector<vector<string> > slots = getSlotMachineArrangment(ROWS, COLUMNS, symbolCount);
        printSlotMachine(slots);
        pair<int, vector<int> > winnings = checkWinnings(slots, slotLines, bet, symbolMultiplier);
        cout << "You won $" << winnings.first << endl;
        // cout << "On Lines: " << &winnings.second << endl;

        return winnings.first - totalBet;
    }
};

/**
 * Main functiion that initalizes the slot machine class
*/
int main()
{
    SlotMachine slotMachine;
    slotMachine.MAX_BET = 1000;
    slotMachine.MIN_BET = 1;
    slotMachine.MAX_LINES = 5;
    slotMachine.ROWS = 5;
    slotMachine.COLUMNS = 5;
    slotMachine.symbolCount["A"] = 8;
    slotMachine.symbolCount["B"] = 19;
    slotMachine.symbolCount["C"] = 38;
    slotMachine.symbolCount["D"] = 67;
    slotMachine.symbolMultiplier["A"] = 100;
    slotMachine.symbolMultiplier["B"] = 60;
    slotMachine.symbolMultiplier["C"] = 28;
    slotMachine.symbolMultiplier["D"] = 4;

    // Get and store user balance
    int userBalance = slotMachine.getDeposit();
    string spinMachine;

    // Loop until user balance is 0 or until the user quits
    while (true && userBalance > 0)
    {
        cout << " Your current balance is $" << userBalance << endl;
        cout << "press any key to spin (q to quit).";
        cin >> spinMachine;
        if (spinMachine == "q")
        {
            break;
        }
        // update user balance with each spin
        userBalance += slotMachine.spinMachine(userBalance);
    }

    return 0;
}