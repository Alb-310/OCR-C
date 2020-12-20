//
//  main.c
//  OCR
//
//  Created by Adrien Hicaubert on 23/11/2020.
//

#include <stdio.h>
#include "../.h/NN.h"
#include "../.h/nn_tools.h"
#include "../.h/matrice.h"

char array_char_2[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 
                'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                '!', '"', '$', '%', '&', '\'', '(', ')', ',', '.', ':', ';', '?', '@'};

char* array_police_2[] = {"arial", "calibri", "tnr", "apple-c", "bradley-h", "courier", "comic"};

int len_police = 7;
int len_char = 75;

int main(int argc, char *argv[])
{
    printf("[\033[31mBEGIN\033[0m] - InitializeNetwork\n");
    struct NN *net = InitializeNetwork();
    printf("[\033[32mEND\033[0m] - InitializeNetwork\n");

    //print_NN(net);

    printf("[\033[31mBEGIN\033[0m] - Training_NN\n");
    
    for (int i = 0; i < len_police; i++)
    {
        for (int j = 0; j < len_char; j++)
        {
            int len = 16;
	        int len_2 = 75;
            double* goal = malloc(len_2 * sizeof(double));
            int array[len][len];

	        for(int v = 0; v < len_2; v++)
            {
                goal[v] = 0.0;
            }

            int index = search(array_char_2[j]) - 1;

            goal[index] = 1.0;

            //fill the array with 0
            for(int v = 0; v < len; v++)
            {
                for(int x = 0; x < len; x++)
                {
                    array[v][x] = 0;
                }
            }

            get_matrice(array_char_2[j], array_police_2[i], 16, array);

            double array_2[len][len];

			//fill the array with 0
			for(int v = 0; v < len; v++)
			{
				for(int x = 0; x < len; x++)
				{
					array_2[v][x] = (double) array[v][x];
				}
			}

            //print_array_double(len, array_2);
            //printf("expected char: %c\n", array_char_2[j]);
            char c = TRAINING_NN(net, len, array_2, goal);
            //printf("got: %c\n", c);
        }
    } 

    printf("[\033[32mEND\033[0m] - Training_NN\n");

    printf("[\033[31mBEGIN\033[0m] - SaveData\n");

    saveData(net);

    printf("[\033[32mEND\033[0m] - SaveData\n");
    return 0;
}
