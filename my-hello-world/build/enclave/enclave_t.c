#include "enclave_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define ADD_ASSIGN_OVERFLOW(a, b) (	\
	((a) += (b)) < (b)	\
)


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

static sgx_status_t SGX_CDECL sgx_ecall_double_num(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_double_num_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_double_num_t* ms = SGX_CAST(ms_ecall_double_num_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	int* _tmp_num = ms->ms_num;
	size_t _len_num = 1 * sizeof(int);
	int* _in_num = NULL;
	sgx_status_t* _tmp_err = ms->ms_err;
	size_t _len_err = 1 * sizeof(sgx_status_t);
	sgx_status_t* _in_err = NULL;

	if (sizeof(*_tmp_num) != 0 &&
		1 > (SIZE_MAX / sizeof(*_tmp_num))) {
		return SGX_ERROR_INVALID_PARAMETER;
	}

	if (sizeof(*_tmp_err) != 0 &&
		1 > (SIZE_MAX / sizeof(*_tmp_err))) {
		return SGX_ERROR_INVALID_PARAMETER;
	}

	CHECK_UNIQUE_POINTER(_tmp_num, _len_num);
	CHECK_UNIQUE_POINTER(_tmp_err, _len_err);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_num != NULL && _len_num != 0) {
		if ( _len_num % sizeof(*_tmp_num) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_num = (int*)malloc(_len_num);
		if (_in_num == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_num, _len_num, _tmp_num, _len_num)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_err != NULL && _len_err != 0) {
		_in_err = (sgx_status_t*)malloc(_len_err);
		if (_in_err == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_err, _len_err, _tmp_err, _len_err)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ms->ms_retval = ecall_double_num(_in_num, _in_err);

err:
	if (_in_num) free(_in_num);
	if (_in_err) free(_in_err);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_hash_natime(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_hash_natime_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_hash_natime_t* ms = SGX_CAST(ms_ecall_hash_natime_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const char* _tmp_nte1 = ms->ms_nte1;
	size_t _len_nte1 = ms->ms_nte1_len ;
	char* _in_nte1 = NULL;
	const char* _tmp_nte2 = ms->ms_nte2;
	size_t _len_nte2 = ms->ms_nte2_len ;
	char* _in_nte2 = NULL;

	CHECK_UNIQUE_POINTER(_tmp_nte1, _len_nte1);
	CHECK_UNIQUE_POINTER(_tmp_nte2, _len_nte2);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_nte1 != NULL && _len_nte1 != 0) {
		_in_nte1 = (char*)malloc(_len_nte1);
		if (_in_nte1 == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_nte1, _len_nte1, _tmp_nte1, _len_nte1)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

		_in_nte1[_len_nte1 - 1] = '\0';
		if (_len_nte1 != strlen(_in_nte1) + 1)
		{
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}
	if (_tmp_nte2 != NULL && _len_nte2 != 0) {
		_in_nte2 = (char*)malloc(_len_nte2);
		if (_in_nte2 == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_nte2, _len_nte2, _tmp_nte2, _len_nte2)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

		_in_nte2[_len_nte2 - 1] = '\0';
		if (_len_nte2 != strlen(_in_nte2) + 1)
		{
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

	ms->ms_retval = ecall_hash_natime((const char*)_in_nte1, (const char*)_in_nte2);

err:
	if (_in_nte1) free(_in_nte1);
	if (_in_nte2) free(_in_nte2);
	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[2];
} g_ecall_table = {
	2,
	{
		{(void*)(uintptr_t)sgx_ecall_double_num, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_hash_natime, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[2][2];
} g_dyn_entry_table = {
	2,
	{
		{0, 0, },
		{0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_show_who_wins(const char* who)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_who = who ? strlen(who) + 1 : 0;

	ms_ocall_show_who_wins_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_show_who_wins_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(who, _len_who);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (who != NULL) ? _len_who : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_show_who_wins_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_show_who_wins_t));
	ocalloc_size -= sizeof(ms_ocall_show_who_wins_t);

	if (who != NULL) {
		ms->ms_who = (const char*)__tmp;
		if (_len_who % sizeof(*who) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, who, _len_who)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_who);
		ocalloc_size -= _len_who;
	} else {
		ms->ms_who = NULL;
	}
	
	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_show_double_num(int num)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_show_double_num_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_show_double_num_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_show_double_num_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_show_double_num_t));
	ocalloc_size -= sizeof(ms_ocall_show_double_num_t);

	ms->ms_num = num;
	status = sgx_ocall(1, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

