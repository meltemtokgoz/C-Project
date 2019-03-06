#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define R (26)
#define CHAR_INDEX(x) ((int)x - (int)'a')

/********** TRIE STRUCTURE ********************************/
//Trie structure
struct trie
{
    struct trie *next[R];
    bool End;
    char *password;
    bool Leaf;
};
//This function create new node and return it
struct trie *createNode(void)
{
    struct trie *n = NULL;
    n = (struct trie *)malloc(sizeof(struct trie)); //dynamic memory allocation
    if (n)
    {
        int k;
        n->End = false;
        for (k=0;k<R;k++)
            n->next[k] = NULL;
    }
    n->Leaf = false;
    return n;
}
/********** ADD USER FUNCTION ********************************/
//This function check user name.If use name exist in trie return 3.
int searching(struct trie *root,char *user){
    struct trie *n = root;
    int size = strlen(user);

    int i;
    int j;
    for (j=0;j<size;j++)
    {
        i = CHAR_INDEX(user[j]);
        //level 1 after root node
        if(j==0){
            if (!n->next[i]){
                return 1;
            }
        }
        //level more than 1
        else if(j>0){
            if (!n->next[i]){
                return 2;
            }
        }
        n = n->next[i];
    }
    //not enough user name
    if(n->password == NULL){
        return 4;
    }
    //exist user in tree
    if(n != NULL && n->End){
        return 3;
    }
}
//This function adding user in trie.
void addUser(struct trie *root,char *user, char *password,FILE* f)
{
    if(searching(root,user)== 3){
        fprintf(f,"\"%s\" reserved username\n", user);
    }
    else{
        struct trie *n = root;
        int size = strlen(user);

        int i;
        int j;
        for (j=0;j<size;j++)
        {
            i = CHAR_INDEX(user[j]);
            if (!n->next[i])
                n->next[i] = createNode();

            n = n->next[i];
        }
        char *pass = (char *)malloc(sizeof(char));
        strcpy(pass, password);
        n->password = pass;
        n->End = true;
        n->Leaf = true;
        //printf("%s %s\n", user,n->password);
        fprintf(f,"\"%s\" was added\n", user);
    }
}
/********** SEARCH USER FUNCTION ********************************/

bool searchUser(struct trie *root,char *user,FILE* f)
{
    struct trie *n = root;
    int size = strlen(user);

    int i;
    int j;
    for (j=0;j<size;j++)
    {
        i = CHAR_INDEX(user[j]);
        if(j==0){
            if (!n->next[i]){
                fprintf(f,"\"%s\" no record\n", user);
                return false;
            }
        }
        else if(j>0){
            if (!n->next[i]){
                fprintf(f,"\"%s\" incorrect username\n", user);
                return false;
            }
        }
        n = n->next[i];
    }
    if(n->password==NULL){
        fprintf(f,"\"%s\" not enough username\n", user);
        return true;
    }
    if(n->End && n != NULL){
        fprintf(f,"\"%s\" password \"%s\"\n", user, n->password );
        return false;
    }
}

/********** LOGIN USER FUNCTION ********************************/
//This function check the user
int check(struct trie *root,char *user,char *password)
{
    struct trie *n = root;
    int size = strlen(user);

    int i;
    int j;
    for (j=0;j<size;j++)
    {
        i = CHAR_INDEX(user[j]);
        if(j==0){
            if (!n->next[i]){
                return 1; //no record
            }
        }
        else if(j>0){
            if (!n->next[i]){
                return 5; // incorrect user name
            }
        }
        n = n->next[i];
    }
    if(n->password==NULL){
        return 4; //not enough user name
    }
    if(n->End && n != NULL){
        if(!strcmp(n->password, password)){
            return 2; //successful login
        }
        else{
            return 3; //incorrect password
        }
    }
}
void loginUser(struct trie *root,char* username, char* password,FILE* f){
    int value = check(root, username,password);
    if(value == 1){
        fprintf(f,"\"%s\" no record\n",username);
    }
    else if(value == 2){
         fprintf(f,"\"%s\" successful login\n",username);
    }
    else if(value == 3){
        fprintf(f,"\"%s\" incorrect password\n",username);
    }
    else if(value == 4){
        fprintf(f,"\"%s\" not enough username\n", username);
    }
    else if(value == 5){
        fprintf(f,"\"%s\" incorrect username\n", username);
    }
}
/********** DELETE USER FUNCTION ********************************/

bool isEmptyTrie(struct  trie* root)
{
    int i;
    for (i = 0; i < R; i++)
        if (root->next[i])
            return false;
    return true;
}
struct trie* deleteThisUser(struct trie* root, char* user, int level)
{
    int i = CHAR_INDEX(user[level]);

    if (!root)
    return NULL;

    if (level == strlen(user)) {

        if (root->End)
            root->End = false;

        if (isEmptyTrie(root)) {
            free(root);
            root = NULL;
        }
        return root;
    }
    root->next[i] = deleteThisUser(root->next[i], user, level + 1);

    if (root->End == false && isEmptyTrie(root)) {
        free(root);
        root = NULL;
    }
    return root;
}

void deleteUser(struct trie* root, char* user, int depth,FILE* f){
    if(searching(root,user)== 1){
        fprintf(f,"\"%s\" no record\n",user);
    }
    else if(searching(root,user)== 2){
        fprintf(f,"\"%s\" incorrect username\n",user);
    }
    else if(searching(root,user)== 3){
        deleteThisUser(root,user,depth);
        fprintf(f,"\"%s\" deletion is successful\n",user);
    }
    else if(searching(root,user)== 4){
        fprintf(f,"\"%s\" not enough username\n",user);
    }
}
/********** LIST TRIE FUNCTION ********************************/

bool isLeafNode(struct trie* root)
{
    return root->Leaf != false;
}

void listUser(struct trie* root, char str[], int level,FILE* f){

    if (isLeafNode(root))
    {
        str[level] = '\0';
        fprintf(f,"%s\n", str);
    }
    int i;
    for (i = 0; i < R; i++)
    {
        if (root->next[i])
        {
            str[level] = i + 'a';
            listUser(root->next[i], str, level + 1,f);
        }
    }
}

/********** MAIN FUNCTION ********************************/

int main(int argc, char *argv[])
{
    FILE *inputFile, *outputFile;
    inputFile = fopen(argv[1],"r");
    outputFile = fopen("output.txt","w");

    struct trie *root = createNode();

    size_t len = 200;
    char *line = malloc(sizeof(char) * len);
    while(fgets(line, len, inputFile) != NULL) {
        char* token = strtok(line, " ");
        if(!strcmp(token, "-a")){
            char* parameter[3];
            int j;
            for(j= 0 ; j<3; j++){
                if(token == NULL)
                    break;
                parameter[j] = token;
                token = strtok(NULL," ");
            }
            char* tokenP = strtok(parameter[2], "\n");
            addUser(root, parameter[1],parameter[2],outputFile);
        }

        else if(!strcmp(token, "-s")){
            char* parameter[2];
            int j;
            for(j= 0 ; j<2; j++){
                if(token == NULL)
                    break;
                parameter[j] = token;
                token = strtok(NULL," ");
            }
            char* tokenS = strtok(parameter[1], "\n");
            searchUser(root,tokenS,outputFile);
        }
        else if(!strcmp(token, "-q")){
            char* parameter[3];
            int j;
            for(j= 0 ; j<3; j++){
                if(token == NULL)
                    break;
                parameter[j] = token;
                token = strtok(NULL," ");
            }
            char* tokenL = strtok(parameter[2], "\n");
            loginUser(root,parameter[1],tokenL,outputFile);
        }
        else if(!strcmp(token, "-d")){
            char* parameter[2];
            int j;
            for(j= 0 ; j<2; j++){
                if(token == NULL)
                    break;
                parameter[j] = token;
                token = strtok(NULL," ");
            }
            char* token1 = strtok(parameter[1], "\n");
            deleteUser(root,token1,0,outputFile);
        }
        else if(!strcmp(token, "-l")){
            char str[20];
            listUser(root, str, 0,outputFile);
        }
        else{
            fprintf(outputFile,"invalid command");
        }
    }
}
/********** END ******************************************/

