//Project: Morse Code Puzzle
//Code Written by: Cameron Wong & Pranav Sisodia
//Group 128

#include <string.h>
#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions
#include "ece198.h"
#include <time.h>

//global variables

char characters[] = "abcdefghijklmnopqrstuvwxyz1234567890 ";
//                      A  B    C    D   E F    G   H    I  J    K   L    M  N  O   P    Q    R   S   T U   V    W   X    Y    Z    1     2     3     4     5     6     7     8     9     0    " "
char morseAlphabet[] = "/.-/-.../-.-./-.././..-./--./..../../.---/-.-/.-../--/-./---/.--./--.-/.-./.../-/..-/...-/.--/-..-/-.--/--../.----/..---/...--/....-/...../-..../--.../---../----./-----/_/";

//function declarations:

char *strToMorse(char str[]);
void output(char* morse);
int findSize(char arr[]);
char *getInput(void);
void isCorrect(void);
void isNotCorrect(void);
char* retArr(char* m);

//functions:

//strToMorse is responsible for translating a char string value into it's respective 
//morse code translation according to International Morse Code
//This function is called when outputting the question and when checking if the answer (input) is correct
char *strToMorse(char str[])
{
    int morseTracker = 0;
    int counter = -1;
    char *morse;
    int size = findSize(str);
    morse = (char*)calloc((size*6) + 1, sizeof(char));
    
    //The nested for loops are used to iterate through each character in the string to find the corresponding
    //morse code translation, then, char morse is updated with the morse code translation of the
    //original string (question/answer)
    
    for(int i = 0; i < size; i++)
    {
        for(int k = 0; k < 37; k++){
            if(str[i] == characters[k]){
                counter = -1;
                for(int j = 0; j < 171; j++)
                {
                    if(morseAlphabet[j] == '/')
                    {
                        counter++;
                    }

                    if(counter == k)
                    {
                        j++;
                        while(morseAlphabet[j] != '/')
                        {
                            morse[morseTracker] = morseAlphabet[j];
                            morseTracker++;
                            j++;                          
                        }
                        if(i != size-1 && (characters[k] == ' ' || str[i+1] == ' '))
                        {}
                        else
                        {
                            morse[morseTracker] = '/';
                            morseTracker++;    
                        }  
                        break;
                    }
                }
            }
        }
    }

    morse[morseTracker] = '\0';
    
    return morse;
}

//The output function is used to output the initial question to the user
//This function is called at the beginning of the program

void output(char* morse)
{
    int size = findSize(morse);

    for(int j = 0; j < size; j++)
    {
        HAL_Delay(200);
        if(morse[j] == '-')
        {//0.6 seconds of the light being on to indicate a 'dash' in morse code
            SerialPuts("-");
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            HAL_Delay(600);
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        }
        else if(morse[j] == '.')
        {//0.2 seconds of the light being on to indicate a 'dot' in morse code
            SerialPuts(".");
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            HAL_Delay(200);
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        }
        else if(morse[j] == '/')
        {
            SerialPuts("/");
            HAL_Delay(200);//time between each dot or dash 
        }
        else
        {//2 seconds of no light to indicate a space in between words
            SerialPuts("_");
            HAL_Delay(1800);
        }
    }
}

//findSize is a helper function used in the other functions to find the size of the char string

int findSize(char arr[])
{
    int counter = 0;

    while(arr[counter] != '\0')
    {
        counter++;
    }

    return counter;
}

//getInput is responsible for taking user input and storing the values
//The function waits until the user presses the button, then it starts recording the input
//and storing it into and char input array

char *getInput(void)
{
    int counter = 0;
    char *input;
    input = (char*)calloc(600, sizeof(char));
    int time = 0;
    int timeDiff = 0;

    while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
    {}
    while (true)  // wait for button press
    {
        if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
        {
            time = HAL_GetTick();
            while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
            {
            }
            timeDiff = HAL_GetTick()-time;

            if(timeDiff < 400)
            {
                input[counter] = '.';
                counter++;
                SerialPuts(".");
            }
            else
            {
                input[counter] = '-';
                counter++;
                SerialPuts("-");
            }
        }
        else
        {
            time = HAL_GetTick();
            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) && HAL_GetTick()-time < 4000)
            {
            }
            timeDiff = HAL_GetTick()-time;

            if(timeDiff < 400)
            {
            }
            else if(timeDiff > 400 && timeDiff < 1800)
            {
                input[counter] = '/';
                counter++;
                SerialPuts("/");
            }
            else if(timeDiff < 4000)
            {
                input[counter] = '_';
                counter++;
                SerialPuts("_");
            }
            else
            {
                input[counter] = '/';
                SerialPuts("/");
                counter++;
                input[counter] = '\0';
                break;
            }
        }
    }
    return input;
}

//isCorrect is a function that is called if the user input is correct
//The function turns on the LED (solid)

void isCorrect(void)
{
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

//isNotCorrect is a function that is called if the user input is not correct
//The function flashes the LED (blinks)

void isNotCorrect(void)
{
    while(true)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(250);
    }
}

int main(void)
{
    HAL_Init(); 

    __HAL_RCC_GPIOA_CLK_ENABLE(); 
    __HAL_RCC_GPIOB_CLK_ENABLE(); 
    __HAL_RCC_GPIOC_CLK_ENABLE(); 

    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED
    SerialSetup(9600);
    
    ///

    //variable for any question we want to set for escape room puzzle
    char question[] = "2 plus 2";
    char answer[] = "4";

    //The following code is commented out for testing reasons
    //The following lines are responsible for randomly selecting a question for the output
    //whenever the reset button is pressed

    /*
    srand(time(NULL));
    int random = rand() % 3;

    char question_array[3] = {"hello","how are you?","2 plus 2"};
    char answer_array[3] = {"hi", "good", "4"};

    int index = random;

    char question[] = question_array[index];
    char answer[] = answer_array[index];
    */


    //STEPS
    //1. Translate question
    char *morseOutput = strToMorse(question);
    //2. Output question
    output(morseOutput);
    //3. Get input
    char *input = getInput();
    printf(input);
    //4. Indicate if input is correct or incorrect

    if(strcmp((strToMorse(answer)),input) == 0)
        isCorrect(); //LED will turn on (solid)
    else
        isNotCorrect(); //LED will turn on (blink)
}


void SysTick_Handler(void)
{
    HAL_IncTick();

}