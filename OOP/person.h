#ifndef _PERSON_H
#define _PERSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Person Person;

// declaration of pointers to functions
typedef void    (*fptrDisplayInfo) (Person*);
typedef void    (*fptrWriteToFile) (Person*, const char*);
typedef void    (*fptrDelete)(Person*);

typedef struct _Person
{
    void* pDerivedObj; // for inheritance
    char* pFirstName;
    char* pLastName;
    //interface for function
    fptrDisplayInfo Display;
    fptrWriteToFile WriteToFile;
    fptrDelete  Delete;
}Person;

Person* new_Person(const char* pFirstName, const char* pLastName);  //constructor
void delete_Person(Person* const pPersonObj);    //destructor

void Person_DisplayInfo(Person* const pPersonObj);
void Person_WriteToFile(Person* const pPersonObj, const char* pFileName);

#endif