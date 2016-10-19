#include <stdio.h>
int QT_main(int argc, char * const *argv);
int main(int argc, char *argv[])
{
    //printf("Hello World!\n");
    QT_main(argc, argv);
    printf("\npress ENTER key to exit\n");
    getchar();
    return 0;
}
