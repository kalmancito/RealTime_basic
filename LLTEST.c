/*
In the code above :

    The first node is always made accessible through a global ‘primero’ pointer. This pointer is adjusted when first node is deleted.
    Similarly there is a ‘ultimo’ pointer that contains the last node in the list. This is also adjusted when last node is deleted.
    Whenever a node is added to linked list, it is always checked if the linked list is empty then add it as the first node.


*/


#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct test_struct
{
    int val;
    struct test_struct *next;
};

struct test_struct *primero = NULL;
struct test_struct *ultimo = NULL;

struct test_struct* create_list(int val)
{
    printf("\n creating list with primeronode as [%d]\n",val);
    struct test_struct *ptr = (struct test_struct*)malloc(sizeof(struct test_struct));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->next = NULL;

    primero = ultimo = ptr;
    return ptr;
}

struct test_struct* add_to_list(int val, bool add_to_end)
{
    if(NULL == primero)
    {
        return (create_list(val));
    }

    /*if(add_to_end)
       printf("\n Adding node to end of list with value [%d]\n",val);
    else
        printf("\n Adding node to beginning of list with value [%d]\n",val);*/

    struct test_struct *ptr = (struct test_struct*)malloc(sizeof(struct test_struct));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->next = NULL;

    if(add_to_end)
    {
        ultimo->next = ptr;
        ultimo = ptr;
    }
    else
    {
        ptr->next = primero;
        primero = ptr;
    }
    return ptr;
}

int delete_from_list(int val)
{
    struct test_struct *prev = NULL;
    struct test_struct *del = NULL;

    printf("\n Deleting value [%d] from list\n",val);

    del = search_in_list(val,&prev);
    if(del == NULL)
    {
        return -1;
    }
    else
    {
        if(prev != NULL)
            prev->next = del->next;

        if(del == ultimo)
        {
            ultimo = prev;
        }
        else if(del == primero)
        {
            primero = del->next;
        }
    }

    free(del);
    del = NULL;

    return 0;
}

int remove_last(struct test_struct *primero) {
    int retval = 0;
   // if there is only one item in the list, remove it 
    if (primero->next == NULL) {
        retval = primero->val;
        free(primero);
        return retval;
    }

    // get to the last node in the list 
    struct test_struct * current = primero;
    while (current->next->next != NULL) {
        current = current->next;
    }

    // now current points to the last item of the list, so let's remove current->next 
    retval = current->next->val;
    free(current->next);
    current->next = NULL;
    return retval;

}


void print_list(void)
{
    struct test_struct *ptr = primero;

    printf("lista:");
    while(ptr != NULL)
    {
        printf("[%d] ;",ptr->val);
        ptr = ptr->next;
    }
    printf("\n");

    return;
}
//**-----------------------------------------------------------------------------------------------------------
int main(void)
{
    int i = 0, ret = 0; int valor=5;
    int numero=0;
    struct test_struct *ptr = NULL;

    /*for(i = 1; i<11; i++){
        add_to_list(0,true);
    }
    print_list();*/
   

    numero=rand()% 10 + 1;
    add_to_list(numero,true);
    numero=rand()% 10 + 1;
    add_to_list(numero,true);


    print_list();
    printf("lista inicializada \n" );

    struct test_struct *kk= ultimo;
    valor=kk->val;
    printf("ultimo valor: [%d] \n",valor);
       
        kk= primero;
        remove_last(kk);
    print_list();
    
   

    return 0;
}