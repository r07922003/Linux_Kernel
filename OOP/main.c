#include "encapsulation.h"
#include "person.h"
#include "employee.h"

int main(int argc, char*argv[]){
    Object *cur_object = NULL;
    int a = 10;
    int b = 3;
    init_object(&cur_object,a,b);
    printf("a:%d b:%d\n",a,b);
    printf("sum:%d\n",cur_object->add(cur_object));
    printf("sub:%d\n",cur_object->sub(cur_object));

    // test Person
    Person* Jack = new_Person("Jack","Annlank");
    Jack->Display(Jack);
    Jack->WriteToFile(Jack,"Jack.txt");
    Jack->Delete(Jack);

    // test Employee
    Person* Lisa = new_Employee("Lisa","Huang","Software Engineer","TSMC",2000000);
    Lisa->Display(Lisa);
    Lisa->WriteToFile(Lisa,"Lisa.txt");
    Lisa->Delete(Lisa);
    return 0;
}