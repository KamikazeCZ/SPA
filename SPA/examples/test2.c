int f(int *a){
    (*a)++;
    return *a;
}

int g(int *b){
    (*b) *= 2;
    return *b;
}

int main(){
    int i = 1;
    int j;
    i = f(&j) + g(&j); // !!!
    return i;
}
