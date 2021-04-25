#include "enclave_u.h"
#include <errno.h>

typedef struct ms_ecall_double_num_t {
	int ms_retval;
	int* ms_num;
	sgx_status_t* ms_err;
} ms_ecall_double_num_t;

typedef struct ms_ecall_hash_natime_t {
	sgx_status_t ms_retval;
	const char* ms_nte1;
	size_t ms_nte1_len;
	const char* ms_nte2;
	size_t ms_nte2_len;
} ms_ecall_hash_natime_t;

typedef struct ms_ocall_show_who_wins_t {
	const char* ms_who;
} ms_ocall_show_who_wins_t;

typedef struct ms_ocall_show_double_num_t {
	int ms_num;
} ms_ocall_show_double_num_t;

static sgx_status_t SGX_CDECL enclave_ocall_show_who_wins(void* pms)
{
	ms_ocall_show_who_wins_t* ms = SGX_CAST(ms_ocall_show_who_wins_t*, pms);
	ocall_show_who_wins(ms->ms_who);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enclave_ocall_show_double_num(void* pms)
{
	ms_ocall_show_double_num_t* ms = SGX_CAST(ms_ocall_show_double_num_t*, pms);
	ocall_show_double_num(ms->ms_num);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[2];
} ocall_table_enclave = {
	2,
	{
		(void*)enclave_ocall_show_who_wins,
		(void*)enclave_ocall_show_double_num,
	}
};
sgx_status_t ecall_double_num(sgx_enclave_id_t eid, int* retval, int* num, sgx_status_t* err)
{
	sgx_status_t status;
	ms_ecall_double_num_t ms;
	ms.ms_num = num;
	ms.ms_err = err;
	status = sgx_ecall(eid, 0, &ocall_table_enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t ecall_hash_natime(sgx_enclave_id_t eid, sgx_status_t* retval, const char* nte1, const char* nte2)
{
	sgx_status_t status;
	ms_ecall_hash_natime_t ms;
	ms.ms_nte1 = nte1;
	ms.ms_nte1_len = nte1 ? strlen(nte1) + 1 : 0;
	ms.ms_nte2 = nte2;
	ms.ms_nte2_len = nte2 ? strlen(nte2) + 1 : 0;
	status = sgx_ecall(eid, 1, &ocall_table_enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

