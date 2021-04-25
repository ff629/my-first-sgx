#include "enclave_t.h"
#include <functional>
#include <string>

using namespace std;

int ecall_double_num(int *num,sgx_status_t *err)
{
    if (*num>INT64_MAX/2)return -1;
    *num=*num * 2;
    *err = ocall_show_double_num(*num);
    return 0;
}

sgx_status_t ecall_hash_natime(const char* nte1,const char* nte2) {
    string s1=nte1;
    string s2=nte2;
    hash<string> hs;
    int x=hs(s1);
    int y=hs(s2);
    if (x==y)return ocall_show_who_wins("both");
    return x>y?ocall_show_who_wins(nte1):ocall_show_who_wins(nte2);
}