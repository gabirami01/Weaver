/*-------------------------------------------
Program 2: Weaver
Course: CS 211, Spring 2023, UIC
System: Visual Studio Code
Starter Code Author: Ellen Kidane
Student Author: Gabriela Ramirez
------------------------------------------- */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function that sets up a menu and performs an action based on user input.
// Receives the number of options, the options (an array of strings), and max option length. Does not return a value.
void menuOptions(int, char **, int);

// Reads words of length specified by `wordNum` from a file, stores them in `wordArray` of 
// size `maxSize` and returns the count of words how many words were added to the array
int readWordsFromFile(int *wordNum, char ***wordArray, int *maxSize) {
int currentSize = 0;
char **newArray;
char inputString[81];
char filename[] = "words.txt";

  FILE *filePtr = fopen(filename, "r"); // "r" means we open the file for reading
  if (filePtr == NULL) { // Check that the file was able to be opened
    printf("Error: could not open %s for reading\n", filename);
    exit(-1);
  }
  // Allocate memory for the word array
  *wordArray = malloc(sizeof(char *) * (*maxSize));
  for (int i = 0; i < *maxSize; i++) {
    (*wordArray)[i] = malloc(sizeof(char) * (*wordNum + 1));
  }
  // Dynamically reallocating memory for wordArray if currentSize equals
  // maxSize while reading words from a file of a specific length
  while (fscanf(filePtr, "%s", inputString) != EOF) {
    if (strlen(inputString) == *wordNum) {
      if (currentSize == *maxSize) {
        newArray = malloc(sizeof(char *) * (*maxSize * 2));
        for (int j = 0; j < *maxSize * 2; j++) {
          newArray[j] = malloc(sizeof(char) * (*wordNum + 1));
        }
        // Copy the existing words to the new bigger array
        for (int k = 0; k < *maxSize; k++) {
          strcpy(newArray[k], (*wordArray)[k]);
        }
        // Free memory allocated to the old array
        for (int m = 0; m < *maxSize; m++) {
          free((*wordArray)[m]);
        }
        // Set the word array to the new bigger array
        free(*wordArray);
        (*wordArray) = newArray;
        *maxSize = *maxSize * 2;
      }
      // Add the new word to the word array
      strcpy((*wordArray)[currentSize], inputString);
      currentSize++;
    }
  }
  // Close the file and return the number of words in the word array
  fclose(filePtr);

  return currentSize;
}

// Functions that sets up a menu and performs an action based on user input
// Receives the number of options, the options (an array of strings), and max option length. Does not return a value.
void menuOptions(int, char **, int);

// Prompts users to enter starting and ending words, and then requires them to enter a new word that differs by one character and is in the dictionary
// The function validates input, selects random words if desired, and allows users to quit by typing 'q 
// It takes in starting and ending words, a number of words, a word array, a count, and a maximum size as parameters
void playWeaver(char startingWord[], char endingWord[], int wordNum, char **wordArray, int count, int maxSize) {

  char userInput[81];
  char previousWord[81];
  char quitGame[2];
  int countRow = 1;
  int goFoward = 0;
  int flagOne = 0;
  int flagTwo = 0;

  int openFile = readWordsFromFile(&wordNum, &wordArray, &maxSize); // Read words from file and set number of words to openFile
  // Prompts the user to enter starting and ending words, allowing the option to enter "r" for a random word. It validates the entered 
  // words and prompts the user to try again if necessary. Once valid words are entered, it prints them out and exits the loop.
  while (true) {
    printf("Enter starting and ending words, or 'r' for either for a random word: ");
    scanf("%s %s", startingWord, endingWord);
    // If user enters 'r' for starting word, select a random word
    if (strcmp(startingWord, "r") == 0) { 
      int random = rand() % openFile;
      strcpy(startingWord, wordArray[random]);
    }
    // If user enters 'r' for ending word, select a random word
    if (strcmp(endingWord, "r") == 0) { 
      int random = rand() % openFile;
      strcpy(endingWord, wordArray[random]);
    }
    // If starting word is not of the correct length, prompt user to try again
    if (strlen(startingWord) != wordNum) { 
      printf("Your word, '%s', is not a %d-letter word. Try again.\n", startingWord, wordNum);
    // If endingword is not of the correct length, ask user to try again
    } else if (strlen(endingWord) != wordNum) {
      printf("Your word, '%s', is not a %d-letter word. Try again.\n", endingWord, wordNum); 
    } else if ((strlen(startingWord) == wordNum) && (strlen(endingWord) == wordNum)) {
      // Loop through all words in dictionary and check if starting word is in dictionary
      for (int j = 0; j < openFile; j++) { 
        if (strcmp(startingWord, wordArray[j]) == 0) {
          flagOne = 1;
        }
      }
      // Loop through all words in dictionary check if ending word is in dictionary
      for (int i = 0; i < openFile; i++) { 
        if (strcmp(endingWord, wordArray[i]) == 0) { 
          flagTwo = 1; 
        }
      }
      // If starting word is not in dictionary, prompt user to try again 
      if (flagOne == 0) {  
        printf("Your word, '%s', is not a valid dictionary word. Try again.\n", startingWord);
        playWeaver(startingWord, endingWord, wordNum, wordArray, count, maxSize);
      }
      // If ending word is not in dictionary, prompt user to try again
      if (flagTwo == 0) { 
        printf("Your word, '%s', is not a valid dictionary word. Try again.\n", endingWord);
        playWeaver(startingWord, endingWord, wordNum, wordArray, count, maxSize);
      }
      printf("Your starting word is: %s.\n", startingWord);
      printf("Your ending word is: %s.\n", endingWord);
      printf("\n");

      break;
    }
    else {
      flagOne = 0;
      // Check if the user entered a valid dictionary word and if it is, then increment count of rows (words) entered
      for (int i = 0; i < openFile; i++) { 
        if (strcmp(startingWord, wordArray[i]) == 0) {
          flagOne = 1; 
          countRow++; 
        }
      }
      // If word is not in dictionary, prompt user to try again
      if (flagOne == 0) {  
        printf("Your word, '%s', is not a valid dictionary word. Try again.\n", startingWord);
      }
    }
  }
  // The code prompts the user to enter a word of the same length that is at most one 
  // character different and in the dictionary, and sets the previous word to the starting word
  printf("On each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
  printf("You may also type in 'q' to quit guessing.\n");
  strcpy(previousWord, startingWord); 
  // This code is prompting the user to enter a word that is the same length as wordNum and is not 'q'
  // If the user enters a word of the wrong length, it prints an error message and continues the loop
  while (strcmp(previousWord, endingWord) != 0) { 
    printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", countRow, previousWord, endingWord);
    scanf("%s", userInput);
    if (strcmp(userInput, "q") == 0) { 
     menuOptions(wordNum, wordArray, maxSize);
    }
    if (strlen(userInput) != wordNum) {
      printf("Your word, '%s', is not a %d-letter word. Try again.\n", userInput, wordNum); 
      continue;
    }
    count = 0;
    flagOne = 0;
    // This code checks if the word entered by the user is a valid word in the dictionary and if it is exactly one character
    // different from the previous word entered by the user. If either condition fails, it prompts the user to try again.
    for (int i = 0; i < openFile; i++) { 
      if (strcmp(userInput, wordArray[i]) == 0) { 
        flagOne = 1;
        for (int t = 0; t < strlen(userInput); t++) {
          if (userInput[t] != previousWord[t]) {
            count++;
          }
        }
        if (count == 1) { 
          countRow++;
        }
      }
    }
    if (flagOne == 0) { 
      printf("Your word, '%s', is not a valid dictionary word. Try again.\n", userInput);
      continue;
    }
    if (count != 1) { 
      printf( "Your word, '%s', is not exactly 1 character different. Try again.\n", userInput);
      continue;
    }
    // Updates the previous word, increments the number of moves, and checks if the game is won
    // If so, it displays a congratulations message and prompts the user for their next action.
    strcpy(previousWord, userInput); 
    goFoward++;
    if (strcmp(previousWord, endingWord) == 0) { 
      printf("Congratulations! You changed '%s' into '%s' in %d moves.\n", startingWord, previousWord, goFoward);
       menuOptions(wordNum, wordArray, maxSize);
    } 
  }
}

// Shows the user a menu with three choices (play the game again, change the word length and play again, or exit the program)
// It takes as parameters the number of letters in the words, an array of words, and the maximum size of the array.
// After the user chooses, the function runs the appropriate function based on what they chose.
void menuOptions(int wordNum, char **wordArray, int maxSize) {
  int numPlay; // Store user's menu choice
  char quitGame[2];  // Holds user's choice to quit the game
  char startingWord[81];
  char endingWord[81];
  int count;  // Keep track of the number of words played
  
  while (true) { // Start of an infinite loop that continues until the user chooses to exit
    printf("\nEnter: \t1 to play again,\n");
    printf("   \t2 to change the number of letters in the words and then play again, or\n");
    printf("   \t3 to exit the program.\n");
    printf("Your choice --> ");
    scanf("%d", &numPlay);
    if (numPlay == 1) { // If the user enters 1, call the playWeaver function with the same parameters
    playWeaver(startingWord, endingWord, wordNum, wordArray, count, maxSize);
    // Prompts the user to enter the number of letters they want in the words, reads the user's input, 
    // calls the readWordsFromFile function to get words with the desired number of letters, and displays 
    // the number of words found  and calls playWeaver to run game.
    } else if (numPlay == 2) {  
      printf("How many letters do you want to have in the words? ");
      scanf("%d", &wordNum);
      int openFile = readWordsFromFile(&wordNum, &wordArray, &maxSize); 
      printf("Number of %d-letter words found: %d.\n\n", wordNum, openFile); 
      playWeaver(startingWord, endingWord, wordNum, wordArray, count, maxSize); 
      // If the user enters 3, exit the program
    } else if (numPlay == 3) { 
      printf("\nThanks for playing!\n");
      printf("Exiting...");
      exit(0);
    }
  }
}

// This function prints instructions for the Weaver game. It then it then prompts
// the user to input the desired number of letters and calls the playWeaver function.
// It returns 0 to indicate successful execution of the program.
int main() {
  srand(1); // Initialize random number generator
  char startingWord[81];
  char endingWord[81];
  char inputString[81];
  char random[2]; // Holds a random character
  char quitGame[2];
  char **wordArray;
  int maxSize = 10; // Holds the maximum size of the word array
  int currentSize = 0; // Holds the current size of the word array
  int numPlay;
  int wordNum;
  int count = 0;
  int flagOne = 0;
 
  // Prints intro message for the game, reads in the user's desired number of letters, call the readWordsFromFile function
  // to get the words with the desired number of letters and calls the playWeaver function with the starting word,
  // ending word, wordNum, wordArray, count, and maxSize. It returns 0 to indicate successful execution of the program.
  printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\n");
  printf("You can change only one letter at a time, and each word along the way must be a valid word.\n");
  printf("Enjoy!\n\n");
  printf("How many letters do you want to have in the words? ");
  scanf("%d", &wordNum);
  int openFile = readWordsFromFile(&wordNum, &wordArray, &maxSize); 
  printf("Number of %d-letter words found: %d.\n\n", wordNum, openFile);
  playWeaver(startingWord, endingWord, wordNum, wordArray, count, maxSize); 

  return 0;
}