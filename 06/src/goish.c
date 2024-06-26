#include "goish.h"
static char inputbuffer[256];
char goishbuffer[2048] = "";
static uint8_t ibuffer = 0;
static inline uint8_t isChar(char c){
    if ((c >= 32) && (c <= 126)) return 1;
    return 0;
}
static uint8_t strcmp(char* c1, char* c2){
    int i = 0;
    while (c1[i] * c2[i]){
        if (c1[i] != c2[i]) return 0;
        i ++;
    }
    if (c1[i] != c2[i]) return 0;
    return 1;
}

static goishfunction(char* str){
    printf("\n\r");
    if (strcmp(str, "WhoAmI")) print_WhoAmI();
    else printf(str);
    #ifndef MYPRINT
	printf("\033[1A");
    #endif
}
void addGoishBuffer(char c){
    if (isChar(c)) {
        inputbuffer[ibuffer] = c;
        ibuffer ++;
        inputbuffer[ibuffer] = 0;
    } else {
        switch (c){
            case GOISH_BACKSPACE:
                if (ibuffer > 0){
                    ibuffer --;
                    inputbuffer[ibuffer] = 0;
                }
                break;
            case GOISH_TAB:
                //do something                
                break;
            case GOISH_ENTER:
                goishfunction(inputbuffer);
                //do something
                ibuffer = 0;
                inputbuffer[ibuffer] = 0;
                break;
            default:
                inputbuffer[ibuffer] = '^';
                ibuffer ++;
                inputbuffer[ibuffer] = (c / 100) + '0';
                ibuffer ++;
                inputbuffer[ibuffer] = ((c % 100) / 10) + '0';
                ibuffer ++;
                inputbuffer[ibuffer] = ((c % 10)) + '0';
                ibuffer ++;
                inputbuffer[ibuffer] = '^';
                ibuffer ++;
        }
        
    }
    printf("\033[33m\rgoish>>$\033[0m%s\r", inputbuffer);

}
