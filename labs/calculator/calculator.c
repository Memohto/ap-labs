#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
long calc(int operator, int nValues, int *values) {
  int result = values[0];
  if(nValues == 1) {
    printf("%d = %d\n", result, result);
    return result;
  }
  if(operator == 1) {
    printf("%d + ", result);
    for(int i = 1; i < nValues; i++) {
      if(i != nValues-1) {
        printf("%d + ", values[i]);
      } else {
        printf("%d = ", values[i]);
      }
      result += values[i];
    }
    printf("%d\n", result);
  } else if(operator == 2) {
    printf("%d - ", result);
    for(int i = 1; i < nValues; i++) {
      if(i != nValues-1) {
        printf("%d - ", values[i]);
      } else {
        printf("%d = ", values[i]);
      }
      result -= values[i];
    }
    printf("%d\n", result);
  } else {
    printf("%d * ", result);
    for(int i = 1; i < nValues; i++) {
      if(i != nValues-1) {
        printf("%d * ", values[i]);
      } else {
        printf("%d = ", values[i]);
      }
      result *= values[i];
    }
    printf("%d\n", result);
  }

  return result;
}

int main(int argc, char** argv) {
  if(argc < 3) {
    printf("Missing argument\n");
    printf("Usage: %s <operator> <numbers separated by space>\n", argv[0]);
    return -1;
  }

  char* strOperator = argv[1];
  int operator;
  if(strcmp(strOperator, "add") == 0) {
    operator = 1;
  } else if(strcmp(strOperator, "sub") == 0) {
    operator = 2;
  } else if(strcmp(strOperator, "mult") == 0) {
    operator = 3;
  } else {
    printf("Invalid opartor (Acepted values: add, sub, mult)\n");
    return -1;
  }

  int nValues = argc - 2;
  int values[nValues];
  for(int i = 2; i < argc; i++) {
    values[i-2] = atoi(argv[i]);
  }

  calc(operator, nValues, values);

  return 0;
}
