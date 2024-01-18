#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{

    char sentence []="Rudolph is 12 423 years old";
    char str [20];
    int i;

    // sscanf (sentence,"%*[^0-9]%d", &i);
    // printf ("%d\n", (int)i);

    const char* current = sentence;

    while (sscanf(current, "%*[^0-9]%d", &i) == 1)
    {
        // Move the pointer to the next character after the integer
        current += strspn(current, "0123456789");
        // Skip non-numeric characters
        current += strspn(current, " ,");
    }

  
  return 0;
}