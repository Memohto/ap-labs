#include <stdio.h>
#include <stdlib.h>

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday) {
    int leap;
    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;

    int day = 0;
    for (int i = 0; i < 13; i++) {
        day += daytab[leap][i];
        if(day >= yearday) {
            day -= daytab[leap][i];
            *pday = yearday - day;
            *pmonth = i;
            return;
        }
    }

}

char *month_name(int n) {

   static char *name[] = {
       "Illegal month",
       "Jan", "Feb", "Mar",
       "Apr", "May", "Jun",
       "Jul", "Aug", "Sep",
       "Oct", "Nov", "Dec"
   };

   return (n < 1 || n > 12) ? name[0] : name[n];
}

int main(int argc, char **argv) {
    if(argc < 3) {
        printf("Missing argument\n");
        printf("Usage: %s <year> <yearday>\n", argv[0]);
        return 0;
    }

    int month, day;
    month_day(atoi(argv[1]), atoi(argv[2]), &month, &day);

    if(month == 0 && day == 0) {
        printf("Error: Invalid year day\n");
    } else {
        printf("%s %02d, %s\n", month_name(month), day, argv[1]);
    }   
    
    return 0;
}
