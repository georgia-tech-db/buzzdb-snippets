
#include <algorithm>
#include <fstream>
#include <iostream>

namespace buzzdb {
namespace utils {

// #define UNUSED(p) ((void)(p))

// void hello_world(){
//   printf("Hello world");
// }

struct MinHeapNode {
  int elem;
  int x;
};

void swap(MinHeapNode* a, MinHeapNode *b);

class MinHeap{

  MinHeapNode* node;
  int sizeHeap;

  public: 
    MinHeap(MinHeapNode x[], int size);
    void MinHeapify(int);
    int right (int x) {
      return (2 * x + 2);
    }
    int left (int x) {
      return (2 * x + 1);
    }

    MinHeapNode getMinHeap() {
      return node[0];
    }

    void replaceMinNode(MinHeapNode x) {
      node[0] = x;
      MinHeapify(0);
    }
};

MinHeap::MinHeap(MinHeapNode x[], int size) {
  sizeHeap = size;
  node = x;
  for (int a = (sizeHeap - 1) / 2; a >= 0; a--) {
    MinHeapify(a);
  }

}

void MinHeap::MinHeapify(int x) {
  int r = right(x);
  int l = left(x);
  int smaller = x;
  if (l < sizeHeap && node[l].elem < node[x].elem)
      smaller = l;
  if (r < sizeHeap && node[r].elem < node[smaller].elem)
      smaller = r;
  if (smaller != x) {
      swap(&node[x], &node[smaller]);
      MinHeapify(smaller);
  }
}

void swap(MinHeapNode* a, MinHeapNode* b)
{
   MinHeapNode temp = *a;
   *a = *b;
   *b = temp;
}

void merge(int arr[], int left, int mid, int right)
{
   
   int i1 = mid - left + 1;
   int i2 = right - mid;

   int L[i1], R[i2];
   int a = 0;
   while (a < i1) {
    L[a] = arr[left + a];
    a++;
   }
   int b = 0;
   while (b < i2) {
    R[b] = arr[mid + 1 + b];
    b++;
   }
      
   a = 0;
   b = 0;
   int c = l;
   while (a < i1 && b < i2) {
      if (L[a] <= R[b])
         arr[c++] = L[a++];
      else
         arr[c++] = R[b++];
   }
   while (a < i1) {
      arr[c++] = L[a++];
   }
   while (b < i2) {
      arr[c++] = R[b++];
   }
}

void mergeSort(int arr[], int left, int right) {
   
   if (left < right) {
      int mid = left + (right - left) / 2;
      mergeSort(arr, left, mid);
      mergeSort(arr, mid + 1, right);
      merge(arr, left, mid, right);
   }
}

FILE* openFile(char* fileName, char* mode)
{
   FILE* file = fopen(fileName, mode);
   if (file == NULL) {
      perror("Error: Cannot open file");
      exit(EXIT_FAILURE);
   }
   return file;
}
void mergeData(char* openedFile, int n, int k) {
   
   FILE* in[k];
   int x = 0;
   while ( x < k) {
      char fileName[2];
      snprintf(fileName, sizeof(fileName), "%d", x);
      in[x] = openFile(fileName, "r");
      x++;
   }
   FILE* out = openFile(openedFile, "w");
   MinHeapNode* node = new MinHeapNode[k];
   int y = 0;
   while (y < k) {
      if (fscanf(in[y], "%d ", &node[y].elem) != 1)
         break;
      node[y].y = y;
      y++;
   }
   MinHeap hp(node, y);
   int count = 0;
   while (count != y) {
      MinHeapNode root = hp.getMin();
      fprintf(out, "%d ", root.elem);
      if (fscanf(in[root.i], "%d ", &root.elem) != 1) {
         root.elem = INT_MAX;
         count++;
      }
      hp.replaceMin(root);
   }
   int z = 0;
   while (z < k) {
      fclose(in[z]);
   }
   fclose(out);
}
void initialiseData( char* inputFile, int memory, int num_ways) {
   
   FILE* in = openFile(inputFile, "r");
   FILE* out[num_ways];
   char fileName[2];
   int x = 0;
   while (x <num_ways){

      snprintf(fileName, sizeof(fileName), "%d", x);
      out[x] = openFile(fileName, "w");
      x++;

   }

   int* arr = (int*)malloc( memory * sizeof(int));
   bool inputing = true;
   int nextOutput = 0;

  
   while (inputing) {
    int y = 0;

      while (y < memory) {
         if (fscanf(in, "%d ", &arr[y]) != 1) {
            inputing = false;
            break;

         }
         y++;
      }
      mergeSort(arr, 0, y - 1);
      int z = 0;
      while (z < y){
         fprintf(out[nextOutput], "%d ", arr[z]);
         z++;
      }
      nextOutput++;
   }
   int a = 0;
   while (a < num_ways) {
      fclose(out[a]);
      a++;
   }
   fclose(in);
}
void externalSort( char* inputFile, char* outputFile, int diffways, int mem) {
   
   initialiseData(inputFile, mem, diffways);
   mergeData(outputFile, mem, diffways);
}


}  // namespace utils
}  // namespace buzzdb
