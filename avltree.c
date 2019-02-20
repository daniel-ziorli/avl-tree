#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>

#define HEIGHT 1

struct node {  
    char data[256];
    int count;
    struct node* left;
    struct node* right;
    int height;
};

//creates new node
struct node * get_node(char val[256]){
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->left = NULL;
    new_node->right = NULL;
    strcpy(new_node->data, val);
    new_node->height = HEIGHT;
    new_node->count = 1;
    return new_node;
}

//gets height
int get_height(struct node* root){
    if(!root){
        return 0;
    }
    else{
        return root->height;
    }
}

//gets balance number
int get_balance(struct node* root){
    if(!root){
        return 0;
    }
   return (get_height(root->left) - get_height(root->right));
}

//gets biggest number
int max(int a, int b){
    if (a > b){
        return a;
    }
    else{
        return b;
    }
}

//left rotation
struct node* left_rotate(struct node* root){
    struct node* right = root->right;
    struct node* left = right->left;

    right->left = root;
    root->right = left;

    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    right->height = max(get_height(right->left), get_height(right->right)) + 1;

    return right;
}

//right rotation
struct node* right_rotate(struct node* root){
    struct node* left = root->left;
    struct node* right = left->right;

    left->right = root;
    root->left = right;

    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    left->height = max(get_height(left->left), get_height(left->right)) + 1;

    return left;
 }

struct node* insert(struct node* root, char val[256]){
    if (!root){
        struct node* new_node = get_node(val);
        return new_node;
    }
    if (strcmp(root->data, val) == 0){
        root->count ++;
        return root;
    }
    else if (strcmp(root->data, val) > 0){
        root->left = insert(root->left, val);
    }
    else{
        root->right = insert(root->right, val);
    }
    root->height = max(get_height(root->left), get_height(root->right)) + 1;
    int balance = get_balance(root);
    if (balance > 1 && strcmp(root->left->data, val) > 0){
        root = right_rotate(root);
    }
    else if (balance < -1 && strcmp(root->right->data, val) < 0){
        root = left_rotate(root);
    }
    else if (balance > 1 && strcmp(root->left->data, val) < 0){
        root->left = left_rotate(root->left);
        root = right_rotate(root);
    }
    else if (balance < -1 && strcmp(root->right->data, val) > 0){
        root->right = right_rotate(root->right);
        root = left_rotate(root);
    }
    return root;
 }

 //balances tree
struct node* balance_tree(struct node* root){  
    struct node* temp_x;
    struct node* temp_y;  
    int left_height;
    int right_height;  
    left_height = get_height(root->left);  
    right_height = get_height(root->right);  
    if (left_height >= right_height){
        temp_x = root->left;  
    }
    else{
        temp_x = root->right;
    }

    left_height = get_height(temp_x->left);  
    right_height = get_height(temp_x->right);

    if (temp_x == root->left){
        if (left_height >= right_height){
            temp_y = temp_x->left;
        }
    }
    else{
        temp_y = temp_x->right;
    }
    if (temp_x == root->right){
        if (left_height > right_height){
            temp_y = temp_x->left;
        }
        else {
            temp_y = temp_x->right;
        }
    }
    if (root->left == temp_x && temp_x->left == temp_y){
        root = right_rotate(root);
    }
    else if (temp_x == root->right && temp_x->right == temp_y){
        root = left_rotate(root);
    }
    else if (temp_x == root->left && temp_y == temp_x->right){
        root->left = left_rotate(root->left);
        root = right_rotate(root);
    }
    else if (temp_x == root->right && temp_y == temp_x->left){
        root->right = right_rotate(root->right);
        root = left_rotate(root);
    }  
    return root;
}

 //find right trees successor   
struct node* right_successor(struct node* root){
    struct node* temp = root->right;  
    while(temp->left){  
        temp = temp->left;  
    }  
    return temp;  
}

//deletes and rebalances tree
struct node* deletion(struct node* root, char val[256]){
    if(!root)
        return NULL;  
    if(strcmp(root->data, val) > 0){  
        root->left = deletion(root->left, val);  
    }  
    else if(strcmp(root->data, val) < 0){  
        root->right = deletion(root->right, val);  
    }  
    else{  
        struct node* temp;

        if (root->count > 1){
            root->count--;
            return root;
        }
        else{
            if(root->left == NULL || root->right == NULL){  
            if(root->left)  
                temp = root->left;  
            else if(root->right)  
                temp = root->right;  
            else  
                temp = NULL;  
            root = NULL;  
            free(root);  
            return temp;  
            }  
            else{
                temp = right_successor(root);
                strcpy(root->data, temp->data);
                root->right = deletion(root->right,temp->data);  
            }
        }
    }
    if(root){  
        root->height = max(get_height(root->left), get_height(root->right)) + 1;  
        int balance = get_balance(root);  
        if(balance > 1 || balance < -1)  
            root = balance_tree(root);  
   }  
   return root;  
}

//find node with key
struct node* find(struct node* root, char val[256]){
    if (!root){
        return NULL;
    }
    else if (strcmp(root->data, val) == 0){
        return root;
    }
    else if (strcmp(root->data, val) > 0){
        return find(root->left, val);
    }
    else{
        return find(root->right, val);
    }
}

//delets node if found otherwise print No_such_key
struct node* delete(struct node* root, char val[256]){
    struct node* temp = find(root, val);
    if (temp == NULL){
        return NULL;
    }
    else{
        return deletion(root, val);
    }
}

// gets number of nodes in tree
int total_nodes(struct node* root){
    if (!root){
        return 0;
    }
    int sum = 1;
    sum += total_nodes(root->left);
    sum += total_nodes(root->right);
    return sum;
}

// gets number of counts in tree
int total_counts(struct node* root){
    if (!root){
        return 0;
    }
    int sum = root->count;
    sum += total_counts(root->left);
    sum += total_counts(root->right);
    return sum;
}

//find all keys with counts above input
void find_all(struct node* root, int thresh_hold){
    if (!root){
        return;
    }
    if (root->count >= thresh_hold){
        printf("key: %s, frequency: %d\n", root->data, root->count);
    }
    find_all(root->left, thresh_hold);
    find_all(root->right, thresh_hold);
}

//replaces said char with new char
void replace_chars(char * str, char oldChar, char newChar)
{
    int i = 0;
    while(str[i] != '\0')
    {
        if(str[i] == oldChar)
        {
            str[i] = newChar;
        }
        i++;
    }
}

//initializes from file
struct node* init(struct node* root){
    char * buffer = 0;
    long length;
    FILE * f = fopen ("A4_data_f18.txt", "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer)
        {
            fread (buffer, 1, length, f);
        }
        fclose (f);
    }

    if (buffer)
    {
        replace_chars(buffer, '\n', ' ');
        char * key = strtok (buffer," ");
        while (key != NULL)
        {
            if (strcmp(key, "") != 0){
                root = insert(root, key);
            }
            key = strtok (NULL, " ");
        }
    }
    return root;
}

int main(){
    struct node* root = NULL;  
    char option;
    int is_init = 0;
    while(1){
        printf("1. Initialization\n");
        printf("2. Find\n");
        printf("3. Insert\n");
        printf("4. Remove\n");
        printf("5. Check Height, Size, and Total Count\n");
        printf("6. Find All (above a given frequency)\n");
        printf("7. Exit\n");
        printf("Enter a code (1 – 7) and hit Return\n");
        printf("avl/> ");
        scanf("%c", &option);

        if (option == '1'){
            root = init(root);
            is_init = 1;
            printf("\n");
        }
        else if (option == '2'){
            if (is_init == 0){
                printf("Error Initialization Must Be Preformed First\n");
            }
            else{
                char input[256];
                printf("find key: ");
                scanf("%s", input);
                struct node* temp = find(root, input);
                if (temp == NULL){
                    printf("no_such_key\n\n");
                }
                else{
                    printf("key: %s, frequency: %d\n\n", temp->data, temp->count);
                } 
            }
            
        }
        else if (option == '3'){
            if (is_init == 0){
                printf("Error Initialization Must Be Preformed First\n");
            }
            else{
                char input[256];
                printf("insert key:\n");
                scanf("%s", input);
                root = insert(root, input);
                struct node* temp = find(root, input);
                printf("key: %s, frequency: %d\n\n", temp->data, temp->count);
            }
        }
        else if (option == '4'){
            if (is_init == 0){
                printf("Error Initialization Must Be Preformed First\n");
            }
            else{
                char input[256];
                printf("Please Enter Key:\n");
                scanf("%s", input);
                struct node* temp = find(root, input);
                if (!temp){
                    printf("no_such_key\n\n");
                }
                else{
                    root = delete(root, input);
                    temp = find(root, input);
                    if (!temp){
                        printf("key: %s, frequency: %d\n\n", input, 0);
                    }
                    else{
                        printf("key: %s, frequency: %d\n\n", temp->data, temp->count);
                    }
                }  
            }
        }
        else if (option == '5'){
            if (is_init == 0){
                printf("Error Initialization Must Be Preformed First\n");
            }
            else{
                printf("height: %d, size: %d, total count: %d \n", root->height, total_nodes(root), total_counts(root));
            }
        }
        else if (option == '6'){
            if (is_init == 0){
                printf("Error Initialization Must Be Preformed First\n");
            }
            else{
                int input;
                printf("Please Enter Posotive Number:\n");
                scanf("%d", &input);
                while(input < 0){
                    printf("Please Enter Posotive Number:\n");
                    scanf("%d", &input);
                }
                find_all(root, input);
            }
        }
        else if (option == '7'){
            return 0;
        }
        scanf("%c", &option);   
    }
    return 0;
}