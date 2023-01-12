//#define _POSIX_C_SOURCE 200809L
//#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Node {
    int data;
    struct Node* next; 
    struct Node* prev; 
};

void append(struct Node** head_ref, int new_data)
{
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    struct Node* last = *head_ref; 
 
    new_node->data = new_data;
 
    new_node->next = NULL;
 
    if (*head_ref == NULL) {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }
    while (last->next != NULL){
        last = last->next;
    }
    last->next = new_node;
    new_node->prev = last; 
 
    return;
}

struct Node* getNodeAt(struct Node* node,int index){
    int i = 0;
    for(struct Node* temp = node; temp != NULL; temp = temp->next){
        if(i == index){
            return temp;
        }
        i++;
    }
    return NULL;
}

void deleteNode(struct Node** head_ref, struct Node* del)
{
    if (*head_ref == NULL || del == NULL)
        return;

    if (*head_ref == del)
        *head_ref = del->next;
 
    if (del->next != NULL)
        del->next->prev = del->prev;
 
    if (del->prev != NULL)
        del->prev->next = del->next;
 
    free(del);
}

void deleteNodeAt(struct Node** head_ref, int n)
{
    if (*head_ref == NULL || n <= 0)
        return;
 
    struct Node* current = *head_ref;
    int i;
 
    for (i = 1; current != NULL && i < n; i++)
        current = current->next;
 
    if (current == NULL)
        return;
 
    deleteNode(head_ref, current);
}
 


struct Node* multiply(struct Node* multiplicand,int multiplicandLength,struct Node* multiplier,int multiplierLength){
    struct Node* result = NULL; 
    int size = multiplicandLength +multiplierLength;
int i;
    for(i = 0 ; i < size ;i++ ){
        append(&result,0);
    }
 
    int i_n1 = 0;
    int i_n2 = 0;
     
    // Go from right to left in num1
    for (struct Node* temp1 = multiplicand; temp1 != NULL; temp1 = temp1->next)
    {
        int carry = 0;
        int n1 = temp1->data;
 
        i_n2 = 0;
         
        // Go from right to left in num2            
        for (struct Node* temp2 = multiplier; temp2 != NULL;temp2 = temp2->next)
        {
            int n2 = temp2->data;
 
            int sum = n1*n2 + getNodeAt(result,i_n1 + i_n2)->data + carry;
 
            // Carry for next iteration
            carry = sum/10;
 
            // Store result
            getNodeAt(result,i_n1 + i_n2)->data = sum % 10;
 
            i_n2++;
        }
 
        // store carry in next cell
        if (carry > 0)
           getNodeAt(result,i_n1 + i_n2)->data += carry;
 
        // To shift position to left after every
        // multiplication of a digit in num1.
        i_n1++;
    }
 
    // ignore '0's from the right
    i = size - 1;
    while (i>=0 && getNodeAt(result,i)->data == 0){

        deleteNodeAt(&result,i+1);
        i--;

    }
 
    if (i == -1)
    return NULL;
 
    return result;
}


void printList(struct Node* node)
{
    struct Node* last = node;
    while (last->next != NULL)
    {
        last = last->next;
    }
    

    printf("\nNumber \n");
    for(struct Node* temp = last; temp != NULL; temp = temp->prev){
        printf("%d", temp->data);
    }
    printf("\n\n");
}



void printListToFile(struct Node* node, FILE *fp)
{
    struct Node* last = node;
    while (last->next != NULL)
    {
        last = last->next;
    }
    

    for(struct Node* temp = last; temp != NULL; temp = temp->prev){
        fprintf(fp, "%d",temp->data);
    }
    fprintf(fp,"\n");

}


int toInt(char c) {
    return c - '0';
}

void loadFromFile(const char* filename,struct Node** multiplicand,int*  multiplicandLength,struct Node** multiplier,int* multiplierLength){
    FILE * fp;
   	int bufferLength = 1024;
	char buffer[bufferLength];

    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    if(fgets(buffer, bufferLength, fp)) // read first line
    {
        
        char reverse[1024];
		strcpy(reverse,strrev(buffer));
        
		 for(int i = 0; reverse[i] != '\0';i++){
		 	if(reverse[i] < '0' || reverse[i] > '9'){
            	continue;
       		}
		 	
		 	(*multiplicandLength)++;
	        append(multiplicand,toInt(reverse[i]));
		 }
    }
    
    
    if(fgets(buffer, bufferLength, fp)) 
    {
    	char reverse[1024];
		strcpy(reverse,strrev(buffer));
    	
		 for(int i = 0; reverse[i] != '\0';i++){
		 	if(reverse[i] < '0' || reverse[i] > '9'){
	        	continue;
	    	}
	
	        (*multiplierLength)++;
	        append(multiplier,toInt(reverse[i]));
		 }
    }
    

  
    fclose(fp);
}

void saveToFile(const char* filename,struct Node* multiplicand,struct Node* multiplier,struct Node* result){
    FILE *fp;

    fp = fopen(filename, "w");

    printListToFile(multiplicand,fp);
    printListToFile(multiplier,fp);
    printListToFile(result,fp);
  

    fclose(fp);
}

int main() {

    struct Node* multiplicand = NULL;
    struct Node* multiplier = NULL;
    int multiplicandLength = 0;
    int multiplierLength = 0;
    
    loadFromFile("test.txt",&multiplicand,&multiplicandLength,&multiplier,&multiplierLength);
//
    printList(multiplicand);
    printList(multiplier);
    struct Node* result =  multiply(multiplicand,multiplicandLength,multiplier,multiplierLength);
    printList(result);

    saveToFile("output.txt",multiplicand,multiplier,result);

   

   return 0;
}

