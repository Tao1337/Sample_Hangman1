#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <string>
using namespace std;

enum STATE{ INIT, PICKED_ALREADY, NOT_FOUND, FOUND_ONE, FOUND_MANY, USED_ALL, WINNER, TRY_AGAIN };
// Constant declarion used later
const int WORD_NUM = 25;
// Global declarions used later
int max_guess;
STATE mState;
int mGuess;

// These function declarions will be in the code.
void readWords( string* );
void outputKnown( string, string );
string getSpaces( string, char );
char updateGameState( string, string* );
bool outputGameState( string );

// Main function
int
main(void) {
    // init
    STATE mState = INIT;
    string words[ 25 ];
    string known = "";
    mGuess = 0;
    // read words
    readWords( &words[ 0 ] );
    // get random word
    srand(time(NULL));
    string word = words[ (int)round( rand() % WORD_NUM ) ];
    max_guess = word.length() * 2;
    // GAME LOOP
    cout << "The actual word is: " << word << endl;
    for(;;)
    {
        cout << "Guess a letter of the unknown word. You have " << ( max_guess - mGuess ) << " guesses left." << endl;
        outputKnown( word, known );
        char input = updateGameState( word, &known );
        string sp = getSpaces( word, input );
        if( outputGameState( sp ) ) break;
    }
}

void
//Read input from the file
readWords( string* arr)
{
    ifstream in( "guess.txt" );
    // Change above line to ifstream in("guess.txt"); if needed
    for( int i = 0; i < WORD_NUM; i++ )
        in >> arr[ i ];
    in.close();
}

void
//Replace underscores with correct letters, otherwise reprint underscore
outputKnown( string word, string known )
{
    for( char c : word )
    {
        bool k = true;
        for( char h : known )
        {
            if( h == c )
            {
                k = false;
                break;
            }
        }
        if( k )
            cout << "_";
        else
            cout << c;
    }
    cout << endl;
}

string
//Get the position of the letter(s) in the word
getSpaces( string word, char c )
{
    string sp = "";
    for( int i = 0; i < word.length(); i++ )
    {
        char h = word[ i ];
        if( h == c ) sp += ( sp.length() > 0 ? ", " : "" ) +  to_string(i + 1 );
    }
    return sp;
}

char
//Calculate and print game states
updateGameState( string word, string *known )
{
    mState = INIT;
    char c;
    while (true) {
        cin >> c; cin.ignore();
        if (isalpha(c))
            break;
        cout << "Not a letter, try again." << endl;
    }
    for( char h : word )
        if( c == h ) mState = mState == INIT ? FOUND_ONE : FOUND_MANY;
    for( char h : *known )
        if( c == h ) mState = PICKED_ALREADY;
    if( mState == INIT )
    {
        mState = NOT_FOUND;
        mGuess++;
    }
    if( mState == FOUND_ONE || mState == FOUND_MANY )
    {
        mGuess++;
        *known += string( 1, c );
        bool winner = true;
        for( char h : word )
        {
            bool in = false;
            for( char x : *known )
                if( h == x ) in = true;
            if( !in ) winner = false;
        }
        if( winner ) mState = WINNER;
    }
    if( mGuess == max_guess ) mState = USED_ALL;
    return c;
}

bool
// Switch statements of possible game states
outputGameState( string sp )
{
    switch( mState )
    {
        case PICKED_ALREADY:
            cout << "You already tried this letter, pick another." << endl;
            return false;
        case NOT_FOUND:
            cout << "Your letter was not found in the phrase." << endl;
            return false;
        case FOUND_ONE:
            cout << "Your letter is at space " << sp << " of the phrase." << endl;
            return false;
        case FOUND_MANY:
            cout << "Your letter is at the following spaces of the phrase: " << sp << endl;
            return false;
        case USED_ALL:
            cout << "Game over. You have used all of your letter guesses, try again." << endl;
            return true;
        case WINNER:
            cout << "Congratulations, you have won!!!" << endl;
            return true;
            /*case TRY_AGAIN:
             cout << "Game over. Sorry, that is not the phrase, try again." << endl; // don't understand how this is suppose to be implemented when USED_ALL exists
             return true;*/
    }
    return false;
}
