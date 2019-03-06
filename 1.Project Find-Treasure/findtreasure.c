#include <stdio.h>
#include <stdlib.h>

 /*
===============Step 4===================================
In this section, the find function is implemented
to find the treasure.This function is recursive.Determines
in which direction the key matrix go according to given
rules.Key matrix go to opposite direction
if there is a limit.Prints the mid point of the
sub matrix each time it passes in output file.
=========================================================
    */

void find(int** map,int** key,int msize1,int msize2,int ksize, int ka, int kb ,int mx,int my,FILE* f){

    int sum = 0;
    int s,t;
    for(s=0; s<ksize; s++){
        for(t=0; t<ksize; t++){
            sum = sum + (map[mx][my]*key[ka][kb]);
            kb = kb +1;
            my = my +1;
        }
        kb = kb - ksize;
        my = my - ksize;
        ka=ka+1;
        mx=mx+1;
    }

    int result = (sum %5);
    ka = 0;
    kb = 0;
    mx = mx -ksize;

    fprintf(f,"%d;%d;%d",(mx+((ksize-1)/2)),(my+((ksize-1)/2)),sum);
    fprintf(f,"\n");
    if(result == 0){
    }
    else if(result == 1){
        if(mx == 0 ){
            find(map,key,msize1,msize2,ksize,ka,kb,(mx+ksize),my,f);
        }
        else{
            find(map,key,msize1,msize2,ksize,ka,kb,(mx-ksize),my,f);
        }
    }
    else if(result == 2){
        if(mx+ksize == msize1 ){
            find(map,key,msize1,msize2,ksize,ka,kb,(mx-ksize),my,f);
        }
        else{
            find(map,key,msize1,msize2,ksize,ka,kb,(mx+ksize),my,f);
        }
    }
    else if(result == 3){
        if(my+ksize == msize2){
            find(map,key,msize1,msize2,ksize,ka,kb,mx,(my-ksize),f);
        }
        else{
            find(map,key,msize1,msize2,ksize,ka,kb,mx,(my+ksize),f);
        }
    }
    else if(result == 4){
        if(my == 0){
             find(map,key,msize1,msize2,ksize,ka,kb,mx,(my+ksize),f);
        }else{
             find(map,key,msize1,msize2,ksize,ka,kb,mx,(my-ksize),f);
        }
    }
}

int main(int argc, char *argv[])
{
    /*
    ============Step 1===================================
    In this section, the arguments are taken and assigned.
    Also read as map matrix and key matrix file with argument,
    output file is opened with argument.
    =====================================================
    */
    int size[2];
    int plen = strlen(argv[1]);
    char *argument = (char *)malloc((plen+1) * sizeof(char));
    strcpy(argument, argv[1]);
    char *Sep = "x";
    char *pToken = strtok(argument, Sep);
    int i;
    for(i= 0 ; i<2; i++){
            if(pToken == NULL)
                    break;
            size[i] = atoi(pToken);
            pToken = strtok(NULL,Sep);
    }
    int msize1 = size[0];
    int msize2 = size[1];
    int ksize = atoi(argv[2]);
    FILE* mapfile = fopen(argv[3],"r");
    FILE* keyfile = fopen(argv[4],"r");
    FILE* outfile = fopen(argv[5],"w");
    /*
    ============Step 2===================================
    In this section, dynamic memory allocation was made
    for the two matrices that I created.The name of these
    matrices is map and key.Then the values read from the
    appropriate files were assigned to the matrices.
    =====================================================
    */
    //************for map matrix ************************
    int** map = (int**)malloc(sizeof(int*)*(msize1));
    int j;
    for(j = 0 ; j < msize1 ; j++){
        int* temp = (int*)malloc(sizeof(int)*(msize2));
        map[j] = temp;
    }
    int a,b;
    for(a = 0 ; a < msize1 ; a++){
        for(b = 0 ; b< msize2 ; b++)
            fscanf(mapfile , "%d" , &map[a][b]);
        fgetc(mapfile);
    }
    //************for key matrix *************************
    int** key = (int**)malloc(sizeof(int*)*(ksize));
    int x;
    for(x = 0 ; x < ksize ; x++){
        int* temp2 = (int*)malloc(sizeof(int)*(ksize));
        key[x] = temp2;
    }
    int r,c;
    for(r = 0 ; r < ksize ; r++){
        for(c = 0 ; c< ksize ; c++)
            fscanf(keyfile , "%d" , &key[r][c]);
        fgetc(keyfile);
    }
    /*
    ============Step 3===================================
    In this section, the treasure discovery function is
    called for the first time with necessary parameters
    and files are closed.
    =====================================================
    */
    int ka =0;
    int kb =0;
    int mx =0;
    int my =0;
    find(map,key,msize1,msize2,ksize,ka,kb,mx,my,outfile);

    fclose(mapfile);
    fclose(keyfile);
    fclose(outfile);
    return 0;
}
