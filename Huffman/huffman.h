typedef struct MH_Tree{
	char g;
	int len;
	int dec;
	struct MH_Tree *f;
	struct MH_Tree *r;
}MH_Tree;

// DATA STRUCTURE FOR TREE NODES
struct MH_Node{
    	char character;
    	int freq;
    	struct MH_Node *l, *r;
};
     
// DATA STRUCTURE FOR MIN HEAP     
struct M_Heap{
    	int size;
    	struct MH_Node **array;
};
     
typedef struct code{
	char k;
	int l;
	int code_arr[16];
	struct code *p;
}code;

/*All functions*/

int frequency(int fd, int freq[]);
void HuffmanCodes(int fd2, char arr[], int freq[], int size, int f);
struct MH_Node* buildHuffmanTree(char arr[], int freq[], int size);		
struct M_Heap* createAndBuildM_Heap(char arr[], int freq[], int size);
void buildM_Heap(struct M_Heap* M_Heap);
void M_Heapify(struct M_Heap* M_Heap, int idx);
struct MH_Node* extractMin(struct M_Heap* M_Heap);
void insertM_Heap(struct M_Heap* M_Heap, struct MH_Node* MH_Node);
int isSizeOne(struct M_Heap* M_Heap);
void swapMH_Node(struct MH_Node** a, struct MH_Node** b);
void printArr(int arr[], int n);
int isLeaf(struct MH_Node* root);
void printCodes(int fd2, struct MH_Node* root, int t[], int top);
void compressfile(int fd1, int fd2, unsigned char a);
void decimaltobinary(int bin[], int f, int len);
int binarytodecimal(int bin[], int len);
void ExtractCodes(int fd2);
void AgainBuildHuffmanTree(int fd1, int size);
void decompress(int fd1, int fd2, int f);
int isroot(struct MH_Tree* tree_temp);

extern MH_Tree *tree, *tree_temp, *t;
extern code *data, *rear, *front;
