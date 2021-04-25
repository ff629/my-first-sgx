#ifndef ENCLAVE_U_H__
#define ENCLAVE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */


#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OCALL_SHOW_WHO_WINS_DEFINED__
#define OCALL_SHOW_WHO_WINS_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_show_who_wins, (const char* who));
#endif
#ifndef OCALL_SHOW_DOUBLE_NUM_DEFINED__
#define OCALL_SHOW_DOUBLE_NUM_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_show_double_num, (int num));
#endif

sgx_status_t ecall_double_num(sgx_enclave_id_t eid, int* retval, int* num, sgx_status_t* err);
sgx_status_t ecall_hash_natime(sgx_enclave_id_t eid, sgx_status_t* retval, const char* nte1, const char* nte2);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
