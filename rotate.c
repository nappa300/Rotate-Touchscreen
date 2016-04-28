#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // sleep()

/*
Purpose : Rotate the screen {left, right, inverted, normal} and rotate the touchscreen input to go along with it.
*/


void runThisCommand(char* command){
  FILE *fp;
  char path[1035];

  /* Open the command for reading. */
  fp = popen(command, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
    printf("%s", path);
  }
  /* close */
  pclose(fp);   
}

char* runCommandAndReturnString(char* command){
    
    FILE *fp;
    char path[1035];

    /* Open the command for reading. */
    fp = popen(command, "r");
    if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
    }
    /* Read the output a line at a time - output it. */
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        path[strlen(path)-1] = '\0'; // replace the \n character with null terminator 
        //printf("--%s--", path);
    }
    /* close */
    pclose(fp);   

    //printf("%lu",strlen(path));
    char* result = malloc(strlen(path) + 1); //+1 for null terminator
    strcpy(result, path);
    result[strlen(path)] = '\0'; // because we used malloc we must explicity set the last element to \0

    return result;
}

void orientScreenInDirection(char* direction){
    char base[] = "xrandr -o ";
    char* command = calloc(1, strlen(base) + strlen(direction) + 1); //+1 for null terminator
    command = strcat(base, direction);

    printf("\n%s\n",command);
    runThisCommand(command);
}

void orientTouchInputInDirection(char* direction){
    //For those interested - Linear Algebra; Rotation Matrix - https://en.wikipedia.org/wiki/Rotation_matrix

    // ⎡ 1 0 0 ⎤
    // ⎜ 0 1 0 ⎥
    // ⎣ 0 0 1 ⎦
    char normal[] = "1 0 0 0 1 0 0 0 1";

    // ⎡ -1  0 1 ⎤
    // ⎜  0 -1 1 ⎥
    // ⎣  0  0 1 ⎦
    char inverted[] = "-1 0 1 0 -1 1 0 0 1";

    //  90° to the counter-clockwise (left) 
    //  ⎡ 0 -1 1 ⎤
    //  ⎜ 1  0 0 ⎥
    //  ⎣ 0  0 1 ⎦
    char left[] = "0 -1 1 1 0 0 0 0 1";

    //  90° to the clockwise (right)
    // ⎡  0 1 0 ⎤
    // ⎜ -1 0 1 ⎥
    // ⎣  0 0 1 ⎦
    char right[] = "0 1 0 -1 0 1 0 0 1";


    char* keys[4] = {"normal", "inverted", "left", "right"};
    char* matrix[4] = {normal, inverted, left, right };

    int index;
    int position = -1;
    for(index = 0; index < 4; index++){
        if(!strcmp(direction, keys[index])){
            position = index;
        }
    }

    if(position == -1){
        printf("\nSorry, that is not a valid orientation. Please choose from normal, inverted, left, right \n\n");
        exit(1);
    }
    
    char* rotationMatrix = matrix[position];
    //printf("%s", rotationMatrix);
    
    char* touchscreenID = runCommandAndReturnString("xinput --list | grep -m3 'Touchscreen' | tail -n1 | grep -o 'id=[0-9]\\+' | grep -o '[0-9]\\+'");
    //printf("\n%s\n", touchscreenID);
    
    
    
    //xinput set-prop 11 'Coordinate Transformation Matrix' -1 0 1 0 -1 1 0 0 1
    char base1[] = "xinput set-prop ";
    //touchscreenID
    char base2[] = " 'Coordinate Transformation Matrix' ";
    //rotationMatrix
    int commandLength = strlen(base1)+strlen(base2)+strlen(rotationMatrix)+strlen(touchscreenID)+1;  // +1 for \0
    char* command = malloc(commandLength);
    strcpy(command, base1);
    strcat(command, touchscreenID);
    strcat(command, base2);
    strcat(command, rotationMatrix);
    command[commandLength] = '\0'; //explicitly define termination of string
    
    printf("%s", command);
    
    runThisCommand(command);
    
    
    free(touchscreenID);
    free(command);
    
}

int main( int argc, char *argv[] ){
    char* direction = argv[1];
    
    orientTouchInputInDirection(direction); // Run this guy first - simultaneously checks direction is valid and determines a value
    orientScreenInDirection(direction);
    
}