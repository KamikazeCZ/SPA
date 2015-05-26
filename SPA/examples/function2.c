int f(int *p, int *q){
  *p = (*q)++;
  return 42;
}

int main(void){
  int a = 0;
  a = f(&a,&a);
}
