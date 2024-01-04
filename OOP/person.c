#include "person.h"

Person* new_Person(const char* const pFirstName, const char* const pLastName){
    Person* pObj = NULL;
    //allocating memory
    pObj = (Person*) malloc(sizeof(Person));
    if(pObj==NULL) return NULL;

    pObj->pDerivedObj = pObj; //pointing to itself
    pObj->pFirstName = malloc(sizeof(char)*(strlen(pFirstName+1)));
    if(pObj->pFirstName==NULL) return NULL;
    strcpy(pObj->pFirstName,pFirstName);

    pObj->pLastName = malloc(sizeof(char)*(strlen(pLastName+1)));
    if(pObj->pLastName==NULL) return NULL;
    strcpy(pObj->pLastName,pLastName);

    //Initializing interface for access to functions
    pObj->Delete = delete_Person;
    pObj->Display = Person_DisplayInfo;
    pObj->WriteToFile = Person_WriteToFile;

    return pObj;
}

void delete_Person(Person* const pPersonObj){
    if(pPersonObj!=NULL){
        free(pPersonObj->pFirstName);
        free(pPersonObj->pLastName);
        free(pPersonObj);
    }
}

void Person_DisplayInfo(Person* const pPersonObj){
    printf("FirstName: %s\n",pPersonObj->pFirstName);
    printf("LastName: %s\n",pPersonObj->pLastName);
}

void Person_WriteToFile(Person* const pPersonObj, const char* pFileName){
    // code for writing Person info to the file
}