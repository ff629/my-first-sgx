#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <string>

#include "enclave_u.h"
#include "errors.hpp"
#include "loader.hpp"
#include "ocalls.hpp"

using namespace std;

using defer = shared_ptr<void>;




int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "missing enclave path :(" << endl;
        return -1;
    }
    else if (argc <4)
    {
        cout <<"please input tow names"<<endl;
        return -1;
    }
    else if (argc <5){
        cout<<"please input a num at last"<<endl;
        return -1;
    }
    auto enclave_path = argv[1];
    string name1 = argv[2];
    string name2 = argv[3];
    int num = atoi(argv[4]);
    sgx_enclave_id_t enclave_id = 0;
    {
        sgx_status_t status;

        std::tie(enclave_id, status) = load_enclave(enclave_path);
        if (log_if_not_ok(status))
        {
            return -1;
        }
    }
    defer _1(nullptr, [&](...) { unload_enclave(enclave_id); });

    
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
    );
    string s1 = name1+to_string(ms.count());
    string s2 = name2+to_string(ms.count());
    const char * nte1=const_cast<char *> (s1.c_str());
    const char * nte2=const_cast<char *> (s2.c_str());

    
    auto err = SGX_SUCCESS;
    auto status = ecall_hash_natime(enclave_id, &err, nte1,nte2);
    if (log_if_not_ok(status))
    {
        cout<<"ecall_hash_natime err"<<endl;
        return -2;
    }
    else if (log_if_not_ok(err))
    {
        cout<<"ecall_hash_natime ocall err"<<endl;
        return -3;
    }

    int rt=0;
    status = ecall_double_num(enclave_id, &rt, &num, &err);
    if (log_if_not_ok(status))
    {
        cout<<"ecall_double_num err"<<endl;
        return -4;
    }
    else if(log_if_not_ok(err))
    {  
        cout<<"ecall_double_num ocall err"<<endl;
        return -5;
    }
    else if(rt!=0)
    {
        cout<<"oversize"<<endl;
        return -6;
    }

    return 0;
}