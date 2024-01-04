#include "person.h"

typedef struct _Employee Employee;

typedef struct _Employee
{
    Person* pBaseObj;
    char* pDepartment;
    char* pCompany;
    int salary;
}Employee;

Person* new_Employee(const char*,const char*,const char*,const char* pCompany,int salary);
void delete_Employee(Person *);

void Employee_DisplayInfo(Person* const);
void Employee_WriteToFile(Person* const, const char*);
