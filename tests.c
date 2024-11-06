#define SCL_IMPLEMENTATION
#include "scl.h"
#include "assert.h"

SCL_SARR_DEFINE(int);
SCL_SARR_IMPLEMENT(int)

SCL_SLIST_DEFINE(int);
SCL_SLIST_IMPLEMENT(int)

SCL_SHTABLE_DEFINE(int);
SCL_SHTABLE_IMPLEMENT(int)

int main() {
  // sarr_
  sarr_int* arrint = sarr_int_new(10);
  assert(arrint);
  assert(arrint->data);
  assert(arrint->len == 10);
  for(size_t i = 0; i < arrint->len; i++) arrint->data[i] = i;
  for(size_t i = 0; i < arrint->len; i++) assert(arrint->data[i] == i);
  sarr_int_del(arrint);
  fprintf(stderr, "sarr_: all assertions passed\n");

  // sarr_
  sarr* arrptr = sarr_new(10);
  assert(arrptr);
  assert(arrptr->data);
  assert(arrptr->len == 10);
  for(size_t i = 0; i < arrptr->len; i++) {
    int* x = malloc(sizeof(int));
    *x = i;
    arrptr->data[i] = x;
  }
  for(size_t i = 0; i < arrptr->len; i++) assert(*(int*)arrptr->data[i] == i);
  for(size_t i = 0; i < arrptr->len; i++) free(arrptr->data[i]);
  sarr_del(arrptr);
  fprintf(stderr, "sarr_: all assertions passed\n");

  // slist_
  slist_int* listint = slist_int_new(16);
  assert(listint);
  assert(listint->cap == 16);
  assert(listint->data);
  assert(listint->len == 0);
  for(size_t i = 0; i < 15; i++) assert(slist_int_add(listint, i));
  assert(listint->len == 15);
  assert(listint->cap == 16);
  for(size_t i = 0; i < 15; i++) assert(listint->data[i] == i);
  assert(slist_int_add(listint, 15));
  assert(listint->len == 16);
  for(size_t i = 0; i < 16; i++) assert(listint->data[i] == i);
  assert(slist_int_add(listint, 16));
  assert(listint->len == 17);
  for(size_t i = 0; i < 17; i++) assert(listint->data[i] == i);
  assert(listint->cap == 32);
  slist_int_del(listint);
  fprintf(stderr, "slist_: all assertions passed\n");

  // slist_
  slist* listptr = slist_new(16);
  assert(listptr);
  assert(listptr->cap == 16);
  assert(listptr->data);
  assert(listptr->len == 0);
  for(size_t i = 0; i < 15; i++) {
    int* x = malloc(sizeof(int));
    *x = i;
    assert(slist_add(listptr, x));
  }
  assert(listptr->len == 15);
  assert(listptr->cap == 16);
  for(size_t i = 0; i < 15; i++) assert(*(int*)listptr->data[i] == i);
  {
    int* x = malloc(sizeof(int));
    *x = 15;
    assert(slist_add(listptr, x));
  }
  assert(listptr->len == 16);
  for(size_t i = 0; i < 16; i++) assert(*(int*)listptr->data[i] == i);
  {
    int* x = malloc(sizeof(int));
    *x = 16;
    assert(slist_add(listptr, x));
  }
  assert(listptr->len == 17);
  for(size_t i = 0; i < 17; i++) assert(*(int*)listptr->data[i] == i);
  assert(listptr->cap == 32);
  for(size_t i = 0; i < listptr->len; i++) free(listptr->data[i]);
  slist_del(listptr);
  fprintf(stderr, "slist_: all assertions passed\n");

  // shtable_
  shtable_int* htint = shtable_int_new(100);
  assert(htint);
  assert(htint->data0);
  assert(htint->data1);
  assert(htint->len == 100);
  char* words[100] = {
"zbDJfDpSAuBv", "KBaeDYvXRp4V", "Swen8XrrxLYK", "NUkyxB7L2YXf", "9ehgxjp7kYDx",
"24txREB3AwHV", "KF4SgaPNgFFP", "QMzwjyRnUAtZ", "pBtLnUkRrD26", "53rnmt2ABJcP",
"m8p7u6w6L9T3", "XRYSBRVAGJ6p", "VMvmHPHpdXtM", "zY8aLp5vpApZ", "K89FxyfaFxgz",
"YcuPqKyJrguW", "zTKGwuTEeyjd", "5AJ5ZMCMnK69", "x6RwGmpfcCpG", "tDjHpw8EGTrj",
"t9YEuKWMzkZv", "JcCxp8EXRjWm", "UaYEpyBqu23D", "aev5xuDzx8Pb", "tEuam2M5a3Ck",
"cMhESDKvmZ83", "8EtEZtnSY9Gs", "sqFAXT3TDPaW", "4CaABaQArvXZ", "cyMgXKpBECqw",
"Vq7CaNf9w8P2", "pbfetGdRU6vQ", "vTWHHwYdBLD4", "tzT2MbSc7wzc", "Y26sdwL5mreu",
"FxBaHsAEbDbu", "t3MAWBbhatPW", "D3n6WWfDD889", "YZK9XCn7JCw9", "tcZpYe3Lm5yC",
"x59mUhgNjzcN", "vunZW2bbRxRT", "taj4xzdw9TcU", "373PazV9xDbr", "ftCA2gxst5XB",
"fSMd6B4kz5h3", "Mh8rfaKugqYZ", "SzMuhBE8r7wa", "gV7SrH3L4Aw9", "rZ4kJPsEpsx6",
"NRA4vbxY6XLW", "BC3ycdBhNsWg", "nnUNRT72s5Yw", "czr9WYtqNVxF", "uANxzP2wjSGp",
"WwnEgUJ4mGNZ", "vfu9yxhm4c5V", "x8gJ3p6eG45Z", "tqtjLkm8MByM", "FBxm4wwXXfuR",
"kJNPUGDJXMZJ", "g6ESCTtHR3xp", "cxTWdryXrYRz", "mdMum9RDyYrk", "TYXz4F78fRpk",
"HYWLTcnMtNJK", "bEypvmHzPtjr", "9zsLPTttMvKd", "SV7yLWaZuXsU", "bvw9AJZUCMA2",
"MXn5w2K3478Y", "624dPJha3w8V", "N9Tf76dmv2J5", "jHKVhYkJbDqB", "SC3aKbUkh5Np",
"6vmU7tXZh9H4", "jx2rT5HxB2uK", "Kue4EcnzjcwG", "7wCCzzx47qdA", "fw2phVFahh6Z",
"e2y5f4SSrUzK", "K26Pkptk4ShJ", "tYhCa6uXPWP9", "d72ZqATcJ5pq", "3c9ksxuNz2wL",
"sVJysAXHwGhc", "Z5494jpRzcdN", "RWUkSthy4JZ9", "p8fRrRQKQZ7U", "Tvm2Kt3D4VuM",
"nPaH5kEUvNXD", "GYFvcJx3LkmH", "6ak8V4cq4ABJ", "SUtRDwEh3nxm", "sf4K7MQrCbyh",
"rGxKU62ynqL3", "jdsTs63m244v", "2k3us4tyWCXK", "nwRHWFWnuyLQ", "aK33X9N996D6",
  };

  for(size_t i=0; i<100; i++) assert(shtable_int_add(htint, words[i], i));
  for(size_t i=0; i<100;i++) {
    int x = -1;
    shtable_int_get(htint, words[i], &x);
    assert(x != -1);
  }
  shtable_int_del(htint);
  fprintf(stderr, "shtable_: all assertions passed\n");

  // shtable_
  shtable* htptr = shtable_new(100);
  assert(htptr);
  assert(htptr->data0);
  assert(htptr->data1);
  assert(htptr->len == 100);
  for(size_t i=0; i<100; i++) assert(shtable_add(htptr, words[i], words[i]));
  for(size_t i=0; i<100;i++) {
    char* x = NULL;
    shtable_get(htptr, words[i], (void**)&x);
    assert(x);
  }
  shtable_del(htptr);
  fprintf(stderr, "shtable_: all assertions passed\n");
}
