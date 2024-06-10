# LetterBoxedSolver
A little program for solving New York Time's Letter Boxed Game.
A glorified word filter.

# Usage
1. Specify input file - this list of words will be filtered
2. Specify the available letters in the game. Do this by separating each side with a dash, like this: abc-def-ghi-jkl
3. Specify the output file - this will include the words after filtering.

The program's vocabulary is determined by the input file, so it might or might not know some words.
I've used [this](https://www.mit.edu/~ecprice/wordlist.10000) list when testing, but if you can find a better list of words separated by new line all the better.
