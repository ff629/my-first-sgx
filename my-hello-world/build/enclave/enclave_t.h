#ifndef ENCLAVE_T_H__
#define ENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

int ecall_double_num(int* num, sgx_status_t* err);
sgx_status_t ecall_hash_natime(const char* nte1, const char* nte2);

sgx_status_t SGX_CDECL ocall_show_who_wins(const char* who);
sgx_status_t SGX_CDECL ocall_show_double_num(int num);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
