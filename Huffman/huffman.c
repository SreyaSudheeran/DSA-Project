#include "huffman.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<math.h>

#define MAX 32
#define MAX_TREE_HT 100

Htree *tree, *tree_temp , *t ;
code *data, *rear , *front ;

/* FUNCTION TO INITIALIZE NEW NODE */   
node* newNode(char character, int freq) {
        node* temp = (node*) malloc(sizeof(node));
        temp->l = temp->r = NULL;
        temp->character = character;
        temp->freq = freq;
        return temp;
}
     
/* FUNCTION TO INITIALIZE A MIN HEAP */   
heap* createheap(int size) {
        heap* new_heap = (heap*)malloc(sizeof(heap));
        new_heap->size = size;
        new_heap->array = (node**)malloc(new_heap->size * sizeof(node*));
        return new_heap;
}


/* CREATION OF FREQUENCY ARRAY FOR THE INPUT FILE CHARACTERS */
int frequency(int fd, int freq[]) {
	char n;
	int k, p = 0;
	while(read(fd, &n, sizeof(char)) != 0){
		p++;
		k = n;
		freq[k]++;
	}
	return p;
}   

/* FUNCTION TO GENERATE UNIQUE CODES FOR ALL CHARACTERS IN TEXT */
void HuffmanCodes(int fd2, char arr[], int freq[], int size, int f) {
	int k = 0, i;
	for(i = 0; i < 128; i++){
		if(freq[i] != 0){
			freq[k] = freq[i];
			arr[k++] = arr[i];
		}
	}

	/* WRITING NUMBER OF UNIQUE CHARACTERS IN A FILE */
	write(fd2, &size, sizeof(int));//change

	/* WRITING TOTAL NUMBER OF CHARACTERS IN A FILE */
	write(fd2, &f, sizeof(int));

	node* root = buildHuffmanTree(arr, freq, size);
	int t[MAX_TREE_HT], top = 0;
	printCodes(fd2, root, t, top);
}
    
/* FUNCTION TO BUILD AND CREATE A HUFFMAN TREE */
node* buildHuffmanTree(char arr[], int freq[], int size) {
	node *l, *r, *top;
	heap* heap = createAndBuildheap(arr, freq, size);
	while (!isSizeOne(heap))
	{
		l = extractMin(heap);
		r = extractMin(heap);
		top = newNode('$', l->freq + r->freq);
		top->l = l;
		top->r = r;
		insertheap(heap, top);
	}
	return extractMin(heap);
}   


heap* createAndBuildheap(char arr[], int freq[], int size) {
    	int i;

	/* INITIALIZING HEAP */
        heap* heap = createheap(size);

	/* INITIALZING THE ARRAY OF POINTERS IN MIN HEAP. POINTERS POINTING TO NEW NODES OF CHARACTER AND THEIR FREQUENCY */
        for (i = 0; i < size; ++i)
		heap->array[i] = newNode(arr[i], freq[i]);

        buildheap(heap);
        return heap;
} 

/* FUNCTION TO BUILD MIN HEAP */
void buildheap(heap* heap) {
        int n = heap->size - 1;
        int i;
        for (i = (n - 1) / 2; i >= 0; --i)
    		heapify(heap, i);
}

/* FUNCTION TO ORGANIZE ELEMENTS IN HEAP STRUCTURE */
void heapify(heap* heap, int idx) {
        int smallest = idx;
        int l = 2 * idx + 1;
        int r = 2 * idx + 2;
     
        if (l < heap->size && heap->array[l]->freq < heap->array[smallest]->freq)
    		smallest = l;
    		
    	if (r < heap->size && heap->array[r]->freq < heap->array[smallest]->freq)
    		smallest = r;
    		
        if (smallest != idx) {
    		swapnode(&heap->array[smallest], &heap->array[idx]);
    		heapify(heap, smallest);
        }
}

/* FUNCTION TO EXTRACT MINIMUM ELEMENT AND REORGANIZE THE MIN HEAP */
node* extractMin(heap* heap) {
        node* temp = heap->array[0];
        heap->array[0] = heap->array[heap->size - 1];
        --heap->size;
        heapify(heap, 0);
        return temp;
}

/* FUNCTION TO INSERT A NEW ELEMENT TO MIN HEAP */
void insertheap(heap* heap, node* node) {
	int i = heap->size;
	++heap->size;
	while (i && node->freq < heap->array[(i - 1)/2]->freq) {
		heap->array[i] = heap->array[(i - 1)/2];
		i = (i - 1)/2;
	}
	heap->array[i] = node;
}

/* FUNCTION TO CHECK IF HEAP SIZE = 1 */
int isSizeOne(heap* heap) {
	return (heap->size == 1);
}

/* FUNCTION TO SWAP TWO GIVEN NODES */
void swapnode(node** a, node** b) {
	node* t = *a;
	*a = *b;
	*b = t;
}

     
/* FUNCTION TO CHECK IF GIVEN NODE IS A LEAF NODE */   
int isLeaf(node* root) {
	return !(root->l) && !(root->r);
}

int k = 0;

void printCodes(int fd2, node* root, int t[], int top) {
	int i;
	if(root->l) {
		t[top] = 0;
		printCodes(fd2, root->l, t, top + 1);
	}
     
	if(root->r) {
		t[top] = 1;
		printCodes(fd2, root->r, t, top + 1);
	}
     
	if(isLeaf(root)) {
		data = (code*)malloc(sizeof(code));
		data->k = root->character;
		write(fd2, &data->k, sizeof(char));
		
		for(i = 0; i < top; i++){
			data->code_arr[i] = t[i];
		}
		
		write(fd2, &top, sizeof(int));
		int decimal = binarytodecimal(data->code_arr, top);
		write(fd2, &decimal, sizeof(int));
		data->l = top;
		data->p = NULL;      
		if(k == 0){
			front = rear = data;
			k++;
		}
		else{
			rear->p = data;
			rear = rear->p;
		}
	}
}

/* FUNCTION TO COMPRESS THE FILE BY SUBSTITUTING CHARACTERS WITH THEIR HUFFMAN CODES */
void compressfile(int fd1, int fd2, unsigned char a){
	char n;
	int h = 0, i;
	/* CODES ARE WRITTEN INTO FILE IN BIT BY BIT FORMAT */
	while(read(fd1, &n, sizeof(char)) != 0) {
		rear = front;
		while(rear->k != n && rear->p != NULL) {
			rear = rear->p;
		}
		if(rear->k == n) {
			for(i = 0; i < rear->l; i++) {
				if(h < 7) {
					if(rear->code_arr[i] == 1) {
						a++;
						a = a << 1;
						h++;
					}
					else if(rear->code_arr[i] == 0) {
						a = a << 1;
						h++;
					}
				}
				else if(h == 7) {
					if(rear->code_arr[i] == 1) {
						a++;
						h = 0;
					}
					else {
						h = 0;
					}
				    write(fd2, &a, sizeof(char));
				    a = 0;
				}
			}
		}
	}
	for(i = 0; i < 7 - h; i++){
		a = a << 1;
	}
	write(fd2, &a, sizeof(char));
}
				
/* FUNCTION TO CONVERT DECIMAL NUMBER TO ITS BINARY EQUIVALENT*/
void decimaltobinary(int bin[], int f, int len) {
	int i = 0, t;
	for(i = 0;i < len; i++) {
		bin[i] = 0;
	}
	i = 0;
	while(f != 0 && i < len) {
		if(f % 2 == 0){
		
			bin[i++] = 0;
			f = f / 2;
		}
		else if(f % 2 == 1) {
			bin[i++] = 1;
			f = f / 2;
		}		
	}
	for(i = 0; i < (len)/2; i++) {
		t = bin[i];
		bin[i] = bin[(len - 1) - i];
		bin[(len - 1) - i] = t;
	}
}

/* FUNCTION TO CONVERT BINARY NUMBER TO ITS DECIMAL EQUIVALENT */
int binarytodecimal(int bin[], int len) {
	int i = 0, sum = 0;
	for(i = 0; i < len; i++) {
		sum = sum + bin[i]*pow(2, (len - 1) - i);
	}
	return sum;
}
				
/* FUNCTION TO EXTRACT HUFFMAN CODES FROM A COMPRESSED FILE */
void ExtractCodes(int fd1) {
	read(fd1, &t->g, sizeof(char));
	read(fd1, &t->len, sizeof(int));
	read(fd1, &t->dec, sizeof(int));
}
    
/* FUNCTION TO BUILD HUFFMAN TREE FROM DATA EXTRACTED FROM COMPRESSED FILE */
void AgainBuildHuffmanTree(int fd1, int size){
	int i = 0, j, k;
	tree = (Htree *)malloc(sizeof(Htree));
	tree_temp = tree;
	tree->f = NULL;
	tree->r = NULL;
	t = (Htree*)malloc(sizeof(Htree));
	t->f = NULL;
	t->r = NULL;
	for(k = 0; k < size; k++) { 
		tree_temp = tree;
		ExtractCodes(fd1);
		int bin[MAX], bin_con[MAX];
		for(i = 0; i < MAX; i++) {
			bin[i] = bin_con[i] = 0;
		}
		decimaltobinary(bin, t->dec, t->len);
                for(i = 0; i < t->len; i++) {
	            	bin_con[i] = bin[i];
	          }
	            
	        for(j = 0; j < t->len; j++) {
            		if(bin_con[j] == 0) {
            			if(tree_temp->f == NULL) {
            				tree_temp->f = (Htree*)malloc(sizeof(Htree));
            			}
            			tree_temp = tree_temp->f;
            		}
            		else if(bin_con[j] == 1) {
            			if(tree_temp->r == NULL) {
            				tree_temp->r = (Htree*)malloc(sizeof(Htree));
            			}
            			tree_temp = tree_temp->r;
		          }
	        }
	        tree_temp->g = t->g;
	        tree_temp->len = t->len;
            	tree_temp->dec = t->dec;
         	tree_temp->f = NULL;
            	tree_temp->r = NULL;
            	tree_temp = tree;
	}  	
}

/* FUNCTION TO GET BACK ORIGINAL FILE WITHOUT LOSING ANY DATA */
void decompress(int fd1, int fd2, int f) {
        tree_temp = tree;
	int inp[8], i, k = 0;
	unsigned char p;
	read(fd1, &p, sizeof(char));
	decimaltobinary(inp, p, 8);
	tree_temp = tree;
	for(i = 0; i < 8 && k < f; i++){
		if(!isLeafTree(tree_temp)){//is it root or leaf?
			if(i != 7){
				if(inp[i] == 0){
					tree_temp = tree_temp->f;
				}
				if(inp[i] == 1){
					tree_temp = tree_temp->r;
				}
			}
			else{
				if(inp[i] == 0){
					tree_temp = tree_temp->f;
				}
				if(inp[i] == 1){
					tree_temp = tree_temp->r;
				}
				if(read(fd1, &p, sizeof(char)) != 0){
					decimaltobinary(inp, p, 8);
					i = -1;
				}
				else{
					break;
				}
			}
		}
		else{
			k++;
			write(fd2, &tree_temp->g, sizeof(char));
			tree_temp = tree;
			i--;
		}
	}
}
	    
/* FUNCTION TO CHECK IF A GIVEN NODE IS A ROOT NODE */
int isLeafTree(Htree* tree_temp){
	return !(tree_temp->f) && !(tree_temp->r);
}			
