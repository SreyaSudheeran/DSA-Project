typedef struct Htree{
	char g;
	int len;
	int dec;
	struct Htree *f;
	struct Htree *r;
}Htree;


/* DATA STRUCTURE FOR TREE NODES */
typedef struct node{
    	char character;
    	int freq;
    	struct node *l, *r;
}node;
     
/* DATA STRUCTURE FOR MIN HEAP */
typedef struct heap{
    	int size;
    	node **array;
}heap;
     
typedef struct code{
	char k;
	int l;
	int code_arr[16];
	struct code *p;
}code;


int frequency(int fd, int freq[]);
void HuffmanCodes(int fd2, char arr[], int freq[], int size, int f);
node* buildHuffmanTree(char arr[], int freq[], int size);		
heap* createAndBuildheap(char arr[], int freq[], int size);
void buildheap(heap* heap);
void heapify(heap* heap, int idx);
node* extractMin(heap* heap);
void insertheap(heap* heap, node* node);
int isSizeOne(heap* heap);
void swapnode(node** a, node** b);
int isLeaf(node* root);
void printCodes(int fd2, node* root, int t[], int top);
void compressfile(int fd1, int fd2, unsigned char a);
void decimaltobinary(int bin[], int f, int len);
int binarytodecimal(int bin[], int len);
void ExtractCodes(int fd2);
void AgainBuildHuffmanTree(int fd1, int size);
void decompress(int fd1, int fd2, int f);
int isLeafTree(Htree* tree_temp);
extern Htree *tree, *tree_temp , *t ;
extern code *data, *rear , *front ;
