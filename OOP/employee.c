#include "employee.h"

Person *new_Employee(const char* pFirstName,
                     const char* pLastName,
                     const char* pDepartment,
                     const char* pCompany,
                     int salary)
{
    Employee* pEmpyObj;
    Person* pObj;
    pObj = new_Person(pFirstName,pLastName);
    //allocating memory
    pEmpyObj = malloc(sizeof(Employee));
    if(pEmpyObj==NULL){
        pObj->Delete(pObj);
        return NULL;
    }
    pObj->pDerivedObj = pEmpyObj; //pointing to derived object

    pEmpyObj->pDepartment = malloc(sizeof(char)*(strlen(pDepartment)+1));
    strcpy(pEmpyObj->pDepartment,pDepartment);
    pEmpyObj->pCompany = malloc(sizeof(char)*(strlen(pCompany)+1));
    strcpy(pEmpyObj->pCompany,pCompany);
    pEmpyObj->salary = salary;

    pObj->Delete = delete_Employee;
    pObj->Display = Employee_DisplayInfo;
    pObj->WriteToFile = Employee_WriteToFile;

    return pObj;
}

void delete_Employee(Person * const pPersonObj){
    Employee *pEmpObj;
    pEmpObj = pPersonObj->pDerivedObj;
    
    free(pEmpObj->pCompany);
    free(pEmpObj->pDepartment);
    free(pEmpObj);
    delete_Person(pPersonObj);
}

void Employee_DisplayInfo(Person* const pPersonObj){
    Employee* pEmpObj;
    pEmpObj = pPersonObj->pDerivedObj;

    Person_DisplayInfo(pPersonObj);
    printf("Department: %s\n", pEmpObj->pDepartment);
    printf("Company: %s\n", pEmpObj->pCompany);
    printf("salary: %d\n", pEmpObj->salary);
}
void Employee_WriteToFile(Person* const pPersonObj, const char* pFileName){
    Person_WriteToFile(pPersonObj,pFileName);
}