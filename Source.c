#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#include<time.h>
#define SIZE 26
FILE* fp;
char decipher(char c, int shift);
char encipher(char c, int shift);
bool check_pass(char* pass);
bool check_username(char* user);
void sign_in();
bool logged(char* user, char* pass);
void login();
void main() {
    fp = fopen("info.txt", "r+");
    if (!fp) {
        printf("error");
        exit(1);
    }
    printf("enter 1 for login, 2 for sign in\n");
    int n;
    scanf("%d", &n);
    getchar();
    switch (n) {
    case 1: login();
        break;
    case 2: sign_in();
        break;
    default: printf("invalid input");
    }
}
char encipher(char c, int shift)
{
    if ('A' <= c && c <= 'Z')
        c = 'A' + (c - 'A' - shift + SIZE) % SIZE;
    else if ('a' <= c && c <= 'z')
        c = 'a' + (c - 'a' - shift + SIZE) % SIZE;

    return c;
}
char decipher(char c, int shift)
{
    if ('A' <= c && c <= 'Z')
        c = 'A' + (c - 'Z' + shift - 1 + SIZE) % SIZE;
    else if ('a' <= c && c <= 'z')
        c = 'a' + (c - 'z' + shift - 1 + SIZE) % SIZE;

    return c;
}
bool check_pass(char* pass) {
    int flag = 0;
    int sp = 0, c = 0, s = 0, n = 0;
    if (strlen(pass) < 8)
        flag = 1;
    for (int i = 0; i < strlen(pass); i++)
    {
        if (pass[i] >= 'a' && pass[i] <= 'z') {
            s++;
        }
        else if (pass[i] >= 'A' && pass[i] <= 'Z')
            c++;
        else if (pass[i] >= '0' && pass[i] <= '9')
            n++;
        else
            sp++;
    }
    if (!s || !n || !c || !sp)
        flag = 1;
    if (flag)
        return false;
    return true;
}
bool check_username(char* user) {
    char fuser[50], fpass[50];
    rewind(fp);
    while (!feof(fp)) {
        fscanf(fp, "%s %s", fuser, fpass);
        if (!strcmp(user, fuser)) {
            return false;
        }
    }
    return true;
}
void sign_in() {
    int n = 0, shift;
    char user[50], pass[50];
    while (1) {                             //takes username
        printf("enter user name\n");
        gets(user);
        if (check_username(user)) {        //checks username
            break;
        }
        printf("username already taken\n");
    }
    while (1) {                            //takes password 
        printf("enter password\n");
        gets(pass);
        if (check_pass(pass))            //checks password
            break;
        printf("password should be atleast 8 char long .\nIt should contain atleast one special char,capital char,small char and number\n");
    }
    srand(time(0));
    shift = (rand() % 9) + 1;
    do {                                    //encrypting password
        if (isalpha(pass[n]))
            pass[n] = encipher(pass[n], shift);
    } while (pass[n++]);
    fprintf(fp, "\n%s %s %d", user, pass, shift);  //storing username encrypted password and shift
    printf("SIGNED IN");
}
void login() {
    int i = 0;
    char user[50], pass[50];
    while (i < 3) {
        i++;
        printf("enter user name\n");
        gets(user);
        printf("enter password\n");
        gets(pass);
        if (logged(user, pass)) {
            printf("LOGGED IN\n");
            return;
        }
        printf("incorrect user name or password\n");
    }
    printf("wrong credentials have been entered too many times. access denied\n");
}
bool logged(char* user, char* pass) {
    char fuser[50], fpass[50], ffpass[50];
    int shift;
    rewind(fp);
    while (!feof(fp)) {
        int n = 0;
        fscanf(fp, "%s %s %d", fuser, fpass, &shift);
        do {
            if (isalpha(fpass[n]))
                ffpass[n] = decipher(fpass[n], shift);
            else
                ffpass[n] = fpass[n];
        } while (fpass[n++]);
        if (!strcmp(user, fuser) && !strcmp(pass, ffpass))
            return true;
    }
    return false;
}