/*-------------------------------------------
Program 5 Pt 1: Link Lint List - Find Shortest Word Ladder
Course: CS 211, Spring 2024, UIC
System: Advanced zyLab
Author: Mohammad Nusairat
UIN: 672559082
Description: This program takes in a starting
and ending word from the user, and builds a 
word ladder. A word ladder is a bridge between
one word and another, formed by changing one
letter at a time, with the constraint that at
each step the sequence of letters still forms
a valid word. The program finds the most efficient
possibile ladder and outputs it.
------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

int countWordsOfLength(char* filename, int wordSize) { 
    //---------------------------------------------------------
    // TODO - write countWordsOfLength()    
    //      open a file with name <filename> and count the 
    //      number of words in the file that are exactly 
    //      <wordSize> letters long, where a "word" is ANY set
    //      of characters that falls between two whitespaces 
    //      (or tabs, or newlines, etc.). 
    //          return the count, if filename is valid
    //          return -1 if the file cannot be opened
    //---------------------------------------------------------

    // open file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    } 

    // read each same size word from the file until the end of the file is reached
    int ct = 0;
    char word[100];
    while(fscanf(file, "%s", word) != EOF) {
        if (strlen(word) == wordSize) { // same size word
            ct++; // increment ct
        }
    }

    fclose(file);
    return ct;
}

bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    //---------------------------------------------------------
    // TODO - write buildWordArray()    
    //      open a file with name <filename> and fill the 
    //      pre-allocated word array <words> with only words
    //      that are exactly <wordSize> letters long;
    //      the file should contain exactly <numWords> words 
    //      that are the correct number of letters; thus, 
    //      <words> is pre-allocated as <numWords> char* ptrs, 
    //      each pointing to a C-string of legnth <wordSize>+1;
    //          return true iff the file is opened successfully
    //                      AND the file contains exactly 
    //                      <numWords> words of exactly 
    //                      <wordSize> letters, and those words
    //                      are stored in the <words> array
    //          return false otherwise
    //---------------------------------------------------------
    
    // open file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    // read each same size word from the file until the end of the file is reached
    int ind = 0; // index of words array
    char word[100];
    while(fscanf(file, "%s", word) != EOF && ind <= numWords) {
        if (strlen(word) == wordSize) { // same size word
            strcpy(words[ind], word); // copy word into word array
            ind++; // increment index
        }
    }

    fclose(file);
    // check if correct number of words were read
    if (ind == numWords) {
        return true;
    } else {
        return false;
    }

}

int findWord(char** words, char* aWord, int loInd, int hiInd) { 
    //---------------------------------------------------------
    // TODO - write findWord()
    //          binary search for string <aWord> in an 
    //          alphabetically sorted array of strings <words>, 
    //          only between <loInd> & <hiInd>
    //              return index of <aWord> if found
    //              return -1 if not found b/w loInd & hiInd
    //---------------------------------------------------------
    
    // binary search algorithm
    while (loInd <= hiInd) {
        // middle point between the low and high index
        int mid = loInd + (hiInd - loInd) / 2;
        
        // compare the words
        int cmp = strcmp(words[mid], aWord);
        
        // if word is found in the middle index
        if (cmp == 0) {
            return mid;
        }

        // word is in left half
        if (cmp > 0) {
            hiInd = mid - 1;
        } else { // word is in right half
            loInd = mid + 1;
        }

    }
    
    return -1; // word not found
}

void freeWords(char** words, int numWords) {
    //---------------------------------------------------------
    // TODO - write freeWords()
    //          free up all heap-allocated space for <words>,
    //          which is an array of <numWords> C-strings
    //           - free the space allocated for each C-string 
    //           - then, free the space allocated for the array
    //                  of pointers, <words>, itself
    //---------------------------------------------------------

    // Iterate through each C-string in the words array
    for (int i = 0; i < numWords; i++) {
        // Free the memory allocated for the current C-string
        free(words[i]);
    }
    // Free the memory allocated for the array of pointers
    free(words);
}

void insertWordAtFront(WordNode** ladder, char* newWord) {
    //---------------------------------------------------------
    // TODO - write insertWordAtFront()
    //          allocate space for a new [WordNode], set its 
    //          [myWord] subitem using <newWord> and insert
    //          it to the front of <ladder>, which is a 
    //          pointer-passed-by-pointer as the head node of 
    //          ladder changes inside this function; 
    //          <newWord> is a pointer to a C-string from the 
    //          full word array, already heap-allocated
    //---------------------------------------------------------

    // allocate memory for a new WordNode
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
    
    // set newNode's myWord subitem to newWord
    newNode->myWord = newWord;

    // set the next pointer of newNode to the current head of the ladder
    newNode->next = *ladder;

    // update head of ladder to be newNode
    *ladder = newNode;
}

int getLadderHeight(WordNode* ladder) {
    //---------------------------------------------------------
    // TODO - write getLadderHeight()
    //          find & return number of words in <ladder> list
    //---------------------------------------------------------
    
    int ct = 0;
    // loop through the ladder until the end is reached
    WordNode* current = ladder;
    while (current != NULL) {
        ct++; // increment ct
        current = current->next; // increment current word
    }
    return ct;
}

WordNode* copyLadder(WordNode* ladder) {
    //---------------------------------------------------------
    // TODO - write copyLadder()
    //          make a complete copy of <ladder> and return it;
    //          the copy ladder must have new space allocated
    //          for each [WordNode] in <ladder>, BUT the
    //          C-string pointers to elements of the full word  
    //          array can be reused; i.e. the actual words do 
    //          NOT need another allocation here
    //---------------------------------------------------------
    
    /* similiar to a deep copy, but no re-allocation of C-string pointers */

    // check if ladder is NULL
    if (ladder == NULL) {
        return NULL;
    }

    // intialize variables
    WordNode* originalCurrent = ladder;
    WordNode* copyOfOriginal = NULL;
    WordNode* copyOfOriginalCurrent = NULL;

    // loop through the original ladder
    while (originalCurrent != NULL) {
        // create a new node for the copied ladder
        WordNode* newNode = (WordNode*)malloc(sizeof(WordNode)); // alloc memory
        newNode->myWord = originalCurrent->myWord; // reuse C-string ptr
        newNode->next = NULL; // set next to NULL
    
        // if first node set as the head of the copied ladder
        if (copyOfOriginal == NULL) {
            copyOfOriginal = newNode; // set head
            copyOfOriginalCurrent = copyOfOriginal; // set copy's current ptr
        } else {
            // link to prev node
            copyOfOriginalCurrent->next = newNode; // set next to newNode
            copyOfOriginalCurrent = copyOfOriginalCurrent->next; // increment copy's current ptr
        }

        // increment orig's current ptr
        originalCurrent = originalCurrent->next;
    }

    return copyOfOriginal;

}

void freeLadder(WordNode* ladder) {
    //---------------------------------------------------------
    // TODO - write freeLadder()
    //          free up all heap-allocated space for <ladder>;
    //          this does NOT include the actual words, 
    //          instead just free up the space that was 
    //          allocated for each [WordNode]
    //---------------------------------------------------------

    WordNode* current = ladder;
    while (current != NULL) {
        WordNode* temp = current;
        current = current->next;
        free(temp);
    }
}

void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    //---------------------------------------------------------
    // TODO - write insertLadderAtBack()
    //          allocate space for a new [LadderNode], set its 
    //          [topWord] subitem using <newLadder>; then, find
    //          the back of <list> and append the newly created
    //          [LadderNode] to the back; Note that <list> is a 
    //          pointer-passed-by-pointer, since this function 
    //          must handle the edge case where <list> is empty
    //          and the new [LadderNode] becomes the head node
    //---------------------------------------------------------

    // create a new LadderNode
    LadderNode* newLadderNode = (LadderNode*)malloc(sizeof(LadderNode));
    newLadderNode->topWord = newLadder;
    newLadderNode->next = NULL;

    // edge case: <list> is empty and the new [LadderNode] must become the head node
    if (*list == NULL) {
        *list = newLadderNode;
        return;
    }

    // find back of list
    LadderNode* temp = *list;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    // append newLadderNode to back of list
    temp->next = newLadderNode;
}

WordNode* popLadderFromFront(LadderNode** list) {
    //---------------------------------------------------------
    // TODO - write popLadderFromFront()
    //          pop the first ladder from the front of the list
    //          by returning the pointer to the head node of 
    //          the ladder that is the subitem of the head node
    //          of <list> AND updating the head node of <list>
    //          to the next [LadderNode]; Note that <list> is a 
    //          pointer-passed-by-pointer, since this function 
    //          updates the head node to be one down the list;
    //          the [LadderNode] popped off the front must have
    //          its memory freed in this function, since it 
    //          will go out of scope, but the ladder itself, 
    //          i.e. the head [WordNode], should NOT be freed. 
    //---------------------------------------------------------
    
    // edge case: list is empty
    if (*list == NULL) {
        return NULL;
    }

    // get the head of the list
    LadderNode* head = *list;

    // get topWord from the head
    WordNode* topWordToReturn = head->topWord;

    // update head to next node
    *list = head->next;

    // free the head
    free(head);

    return topWordToReturn;
}

void freeLadderList(LadderNode* myList) {
    //---------------------------------------------------------
    // TODO - write freeLadderList()
    //          free up all heap-allocated space for <myList>;
    //          for each ladder in <myList>:
    //           - free the space allocated for each [WordNode] 
    //                  in the ladder using freeLadder()
    //           - then, free the space allocated for the 
    //                  [LadderNode] itself 
    //---------------------------------------------------------

    // Traverse through the list of ladders
    while (myList != NULL) {
        // Get a reference to the current ladder
        LadderNode* currentLadder = myList;
        // Move to the next ladder before freeing the current one
        myList = myList->next;

        // Free the words in the ladder
        freeLadder(currentLadder->topWord);

        // Free the ladder node itself
        free(currentLadder);
    }
}

WordNode* findNeighbors(char** words, int numWords, int wordSize, char* word) {
    WordNode* neighbors = NULL;
    // loop over each character index in the word
    for (int i = 0; i < wordSize; i++) {
        // loop over the alphabet
        for (char c = 'a'; c < 'z'; c++) {
            // create a possible neighbor word by replacing the current at index i with c
            char* possibleWord = (char*)malloc((wordSize + 1) * sizeof(char)); // Allocate memory
            strcpy(possibleWord, word); // Copy the original word
            possibleWord[i] = c;

            // check if the possible word is a valid english word
            int index = findWord(words, possibleWord, 0, numWords - 1);
            if (index != -1) {
                // allocate memory for the neighbor WordNode
                WordNode* neighborNode = (WordNode*)malloc(sizeof(WordNode));
                neighborNode->myWord = words[index];
                neighborNode->next = neighbors;

                // update neighbors to point to newly created neighbor node
                neighbors = neighborNode;
            }

            // free memory alloced for the possible word
            free(possibleWord);
        }
    }

    return neighbors;
}

WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord, 
                                    char* finalWord ) {
    //---------------------------------------------------------
    // TODO - write findShortestWordLadder()
    //          run algorithm to find the shortest word ladder
    //          from <startWord> to <finalWord> in the <words>
    //          word array, where each word is <wordSize> long 
    //          and there are <numWords> total words;
    //          <usedWord> also has size <numWords>, such that
    //          usedWord[i] is only true if words[i] has 
    //          previously be entered into a ladder, and should
    //          therefore not be added to any other ladders; 
    //          the algorithm creates partial word ladders, 
    //          which are [WordNode] linked lists, and stores 
    //          them in a [LadderNode] linked list. 
    //              return a pointer to the shortest ladder;
    //              return NULL if no ladder is possible;
    //                  before return, free all heap-allocated 
    //                  memory that is created here that is not 
    //                  the returned ladder
    //---------------------------------------------------------
    
    // Create myList, an empty list of LadderNode structs
    LadderNode* myList = NULL;

    // Create myLadder, an empty list of WordNode structs
    WordNode* myLadder = NULL;

    // Prepend startWord to the front of myLadder
    insertWordAtFront(&myLadder, startWord);

    // Append myLadder to the back of myList
    insertLadderAtBack(&myList, myLadder);

    while (myList != NULL) { // While myList is not empty:
        // Pop the head LadderNode off the front of myList, call it myLadder
        myLadder = popLadderFromFront(&myList);
        
        // Find neighbors of the last word in myLadder
        WordNode* neighbors = findNeighbors(words, numWords, wordSize, myLadder->myWord);

        while (neighbors != NULL) {
            int index = findWord(words, neighbors->myWord, 0, numWords); // find the index of the neighbor's word
            if (!usedWord[index]) { // If the neighbor word has not already been used in a ladder to this point:
                if (strcmp(neighbors->myWord, finalWord) == 0) { // If the neighbor word is w2:
                    // Prepend w2 to the front of myLadder
                    insertWordAtFront(&myLadder, finalWord);
                    // free myList and remaining neighbors
                    freeLadderList(myList);
                    freeLadder(neighbors);
                    // Hooray! We found the shortest word ladder, so return myLadder
                    return myLadder;
                } else {
                    // Copy myLadder to anotherLadder
                    WordNode* anotherLadder = copyLadder(myLadder);
                    // Prepend neighbor word to the front of anotherLadder
                    insertWordAtFront(&anotherLadder, neighbors->myWord);
                    // Append anotherLadder to the back of myList
                    insertLadderAtBack(&myList, anotherLadder);
                    // mark the word as used
                    usedWord[index] = true;
                }
            }
            // free current neighbor and increment to next neighbor
            WordNode* nextNeighbor = neighbors->next;
            free(neighbors);
            neighbors = nextNeighbor;
        }
        // free anything left in neighbors and myLadder
        freeLadder(neighbors);
        freeLadder(myLadder);
    }

    // If no ladder was returned, then no ladder is possible
    return NULL;

}

// interactive user-input to set a word; 
//  ensures sure the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize); 
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}


//-----------------------------------------------------
// The primary application is fully provided in main(); 
//  no changes should be made to main()
//-----------------------------------------------------
int main() {
    srand((int)time(0));
    
    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n"); 
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever 
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i] 
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize); 
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n"); 

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array, 
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);    
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    return 0;
}
