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


typedef struct ms_ecall_init_oram_controller_t {
	int ms_retval;
} ms_ecall_init_oram_controller_t;

typedef struct ms_ecall_seal_t {
	sgx_status_t ms_retval;
	const uint8_t* ms_plaintext;
	size_t ms_plaintext_len;
	sgx_sealed_data_t* ms_sealed_data;
	size_t ms_sealed_size;
} ms_ecall_seal_t;

typedef struct ms_ecall_unseal_t {
	sgx_status_t ms_retval;
	const sgx_sealed_data_t* ms_sealed_data;
	size_t ms_sealed_size;
	uint8_t* ms_plaintext;
	size_t ms_plaintext_len;
} ms_ecall_unseal_t;

typedef struct ms_ecall_begin_DHKE_t {
	sgx_status_t ms_retval;
} ms_ecall_begin_DHKE_t;

typedef struct ms_ecall_sample_key_pair_t {
	sgx_status_t ms_retval;
	uint8_t* ms_pub_key;
	size_t ms_pubkey_size;
} ms_ecall_sample_key_pair_t;

typedef struct ms_ecall_compute_shared_key_t {
	sgx_status_t ms_retval;
	const uint8_t* ms_pub_key;
	size_t ms_pubkey_size;
} ms_ecall_compute_shared_key_t;

typedef struct ms_enclave_init_ra_t {
	sgx_status_t ms_retval;
	int ms_b_pse;
	sgx_ra_context_t* ms_p_context;
} ms_enclave_init_ra_t;

typedef struct ms_enclave_ra_close_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
} ms_enclave_ra_close_t;

typedef struct ms_verify_att_result_mac_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
	uint8_t* ms_message;
	size_t ms_message_size;
	uint8_t* ms_mac;
	size_t ms_mac_size;
} ms_verify_att_result_mac_t;

typedef struct ms_put_secret_data_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
	uint8_t* ms_p_secret;
	uint32_t ms_secret_size;
	uint8_t* ms_gcm_mac;
} ms_put_secret_data_t;

typedef struct ms_sgx_ra_get_ga_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
	sgx_ec256_public_t* ms_g_a;
} ms_sgx_ra_get_ga_t;

typedef struct ms_sgx_ra_proc_msg2_trusted_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
	const sgx_ra_msg2_t* ms_p_msg2;
	const sgx_target_info_t* ms_p_qe_target;
	sgx_report_t* ms_p_report;
	sgx_quote_nonce_t* ms_p_nonce;
} ms_sgx_ra_proc_msg2_trusted_t;

typedef struct ms_sgx_ra_get_msg3_trusted_t {
	sgx_status_t ms_retval;
	sgx_ra_context_t ms_context;
	uint32_t ms_quote_size;
	sgx_report_t* ms_qe_report;
	sgx_ra_msg3_t* ms_p_msg3;
	uint32_t ms_msg3_size;
} ms_sgx_ra_get_msg3_trusted_t;

typedef struct ms_ocall_printf_t {
	const char* ms_str;
} ms_ocall_printf_t;

typedef struct ms_ocall_get_slot_t {
	const char* ms_slot_fingerprint;
} ms_ocall_get_slot_t;

typedef struct ms_ocall_write_slot_t {
	const char* ms_slot_finger_print;
	const uint8_t* ms_data;
	size_t ms_data_len;
} ms_ocall_write_slot_t;

typedef struct ms_ocall_exception_handler_t {
	const char* ms_err_msg;
} ms_ocall_exception_handler_t;

static sgx_status_t SGX_CDECL sgx_ecall_init_oram_controller(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_init_oram_controller_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_init_oram_controller_t* ms = SGX_CAST(ms_ecall_init_oram_controller_t*, pms);
	sgx_status_t status = SGX_SUCCESS;



	ms->ms_retval = ecall_init_oram_controller();


	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_seal(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_seal_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_seal_t* ms = SGX_CAST(ms_ecall_seal_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const uint8_t* _tmp_plaintext = ms->ms_plaintext;
	size_t _tmp_plaintext_len = ms->ms_plaintext_len;
	size_t _len_plaintext = _tmp_plaintext_len;
	uint8_t* _in_plaintext = NULL;
	sgx_sealed_data_t* _tmp_sealed_data = ms->ms_sealed_data;
	size_t _tmp_sealed_size = ms->ms_sealed_size;
	size_t _len_sealed_data = _tmp_sealed_size;
	sgx_sealed_data_t* _in_sealed_data = NULL;

	CHECK_UNIQUE_POINTER(_tmp_plaintext, _len_plaintext);
	CHECK_UNIQUE_POINTER(_tmp_sealed_data, _len_sealed_data);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_plaintext != NULL && _len_plaintext != 0) {
		if ( _len_plaintext % sizeof(*_tmp_plaintext) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_plaintext = (uint8_t*)malloc(_len_plaintext);
		if (_in_plaintext == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_plaintext, _len_plaintext, _tmp_plaintext, _len_plaintext)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_sealed_data != NULL && _len_sealed_data != 0) {
		if ((_in_sealed_data = (sgx_sealed_data_t*)malloc(_len_sealed_data)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_sealed_data, 0, _len_sealed_data);
	}

	ms->ms_retval = ecall_seal((const uint8_t*)_in_plaintext, _tmp_plaintext_len, _in_sealed_data, _tmp_sealed_size);
	if (_in_sealed_data) {
		if (memcpy_s(_tmp_sealed_data, _len_sealed_data, _in_sealed_data, _len_sealed_data)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_plaintext) free(_in_plaintext);
	if (_in_sealed_data) free(_in_sealed_data);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_unseal(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_unseal_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_unseal_t* ms = SGX_CAST(ms_ecall_unseal_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const sgx_sealed_data_t* _tmp_sealed_data = ms->ms_sealed_data;
	size_t _tmp_sealed_size = ms->ms_sealed_size;
	size_t _len_sealed_data = _tmp_sealed_size;
	sgx_sealed_data_t* _in_sealed_data = NULL;
	uint8_t* _tmp_plaintext = ms->ms_plaintext;
	size_t _tmp_plaintext_len = ms->ms_plaintext_len;
	size_t _len_plaintext = _tmp_plaintext_len;
	uint8_t* _in_plaintext = NULL;

	CHECK_UNIQUE_POINTER(_tmp_sealed_data, _len_sealed_data);
	CHECK_UNIQUE_POINTER(_tmp_plaintext, _len_plaintext);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_sealed_data != NULL && _len_sealed_data != 0) {
		_in_sealed_data = (sgx_sealed_data_t*)malloc(_len_sealed_data);
		if (_in_sealed_data == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_sealed_data, _len_sealed_data, _tmp_sealed_data, _len_sealed_data)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_plaintext != NULL && _len_plaintext != 0) {
		if ( _len_plaintext % sizeof(*_tmp_plaintext) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_plaintext = (uint8_t*)malloc(_len_plaintext)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_plaintext, 0, _len_plaintext);
	}

	ms->ms_retval = ecall_unseal((const sgx_sealed_data_t*)_in_sealed_data, _tmp_sealed_size, _in_plaintext, _tmp_plaintext_len);
	if (_in_plaintext) {
		if (memcpy_s(_tmp_plaintext, _len_plaintext, _in_plaintext, _len_plaintext)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_sealed_data) free(_in_sealed_data);
	if (_in_plaintext) free(_in_plaintext);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_begin_DHKE(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_begin_DHKE_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_begin_DHKE_t* ms = SGX_CAST(ms_ecall_begin_DHKE_t*, pms);
	sgx_status_t status = SGX_SUCCESS;



	ms->ms_retval = ecall_begin_DHKE();


	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_sample_key_pair(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_sample_key_pair_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_sample_key_pair_t* ms = SGX_CAST(ms_ecall_sample_key_pair_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	uint8_t* _tmp_pub_key = ms->ms_pub_key;
	size_t _tmp_pubkey_size = ms->ms_pubkey_size;
	size_t _len_pub_key = _tmp_pubkey_size;
	uint8_t* _in_pub_key = NULL;

	CHECK_UNIQUE_POINTER(_tmp_pub_key, _len_pub_key);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_pub_key != NULL && _len_pub_key != 0) {
		if ( _len_pub_key % sizeof(*_tmp_pub_key) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_pub_key = (uint8_t*)malloc(_len_pub_key)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_pub_key, 0, _len_pub_key);
	}

	ms->ms_retval = ecall_sample_key_pair(_in_pub_key, _tmp_pubkey_size);
	if (_in_pub_key) {
		if (memcpy_s(_tmp_pub_key, _len_pub_key, _in_pub_key, _len_pub_key)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_pub_key) free(_in_pub_key);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_compute_shared_key(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_compute_shared_key_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_compute_shared_key_t* ms = SGX_CAST(ms_ecall_compute_shared_key_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const uint8_t* _tmp_pub_key = ms->ms_pub_key;
	size_t _tmp_pubkey_size = ms->ms_pubkey_size;
	size_t _len_pub_key = _tmp_pubkey_size;
	uint8_t* _in_pub_key = NULL;

	CHECK_UNIQUE_POINTER(_tmp_pub_key, _len_pub_key);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_pub_key != NULL && _len_pub_key != 0) {
		if ( _len_pub_key % sizeof(*_tmp_pub_key) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_pub_key = (uint8_t*)malloc(_len_pub_key);
		if (_in_pub_key == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_pub_key, _len_pub_key, _tmp_pub_key, _len_pub_key)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ms->ms_retval = ecall_compute_shared_key((const uint8_t*)_in_pub_key, _tmp_pubkey_size);

err:
	if (_in_pub_key) free(_in_pub_key);
	return status;
}

static sgx_status_t SGX_CDECL sgx_enclave_init_ra(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_enclave_init_ra_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_enclave_init_ra_t* ms = SGX_CAST(ms_enclave_init_ra_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	sgx_ra_context_t* _tmp_p_context = ms->ms_p_context;
	size_t _len_p_context = sizeof(sgx_ra_context_t);
	sgx_ra_context_t* _in_p_context = NULL;

	CHECK_UNIQUE_POINTER(_tmp_p_context, _len_p_context);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_p_context != NULL && _len_p_context != 0) {
		if ((_in_p_context = (sgx_ra_context_t*)malloc(_len_p_context)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_p_context, 0, _len_p_context);
	}

	ms->ms_retval = enclave_init_ra(ms->ms_b_pse, _in_p_context);
	if (_in_p_context) {
		if (memcpy_s(_tmp_p_context, _len_p_context, _in_p_context, _len_p_context)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_p_context) free(_in_p_context);
	return status;
}

static sgx_status_t SGX_CDECL sgx_enclave_ra_close(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_enclave_ra_close_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_enclave_ra_close_t* ms = SGX_CAST(ms_enclave_ra_close_t*, pms);
	sgx_status_t status = SGX_SUCCESS;



	ms->ms_retval = enclave_ra_close(ms->ms_context);


	return status;
}

static sgx_status_t SGX_CDECL sgx_verify_att_result_mac(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_verify_att_result_mac_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_verify_att_result_mac_t* ms = SGX_CAST(ms_verify_att_result_mac_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	uint8_t* _tmp_message = ms->ms_message;
	size_t _tmp_message_size = ms->ms_message_size;
	size_t _len_message = _tmp_message_size;
	uint8_t* _in_message = NULL;
	uint8_t* _tmp_mac = ms->ms_mac;
	size_t _tmp_mac_size = ms->ms_mac_size;
	size_t _len_mac = _tmp_mac_size;
	uint8_t* _in_mac = NULL;

	CHECK_UNIQUE_POINTER(_tmp_message, _len_message);
	CHECK_UNIQUE_POINTER(_tmp_mac, _len_mac);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_message != NULL && _len_message != 0) {
		if ( _len_message % sizeof(*_tmp_message) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_message = (uint8_t*)malloc(_len_message);
		if (_in_message == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_message, _len_message, _tmp_message, _len_message)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_mac != NULL && _len_mac != 0) {
		if ( _len_mac % sizeof(*_tmp_mac) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_mac = (uint8_t*)malloc(_len_mac);
		if (_in_mac == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_mac, _len_mac, _tmp_mac, _len_mac)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ms->ms_retval = verify_att_result_mac(ms->ms_context, _in_message, _tmp_message_size, _in_mac, _tmp_mac_size);

err:
	if (_in_message) free(_in_message);
	if (_in_mac) free(_in_mac);
	return status;
}

static sgx_status_t SGX_CDECL sgx_put_secret_data(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_put_secret_data_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_put_secret_data_t* ms = SGX_CAST(ms_put_secret_data_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	uint8_t* _tmp_p_secret = ms->ms_p_secret;
	uint32_t _tmp_secret_size = ms->ms_secret_size;
	size_t _len_p_secret = _tmp_secret_size;
	uint8_t* _in_p_secret = NULL;
	uint8_t* _tmp_gcm_mac = ms->ms_gcm_mac;
	size_t _len_gcm_mac = 16 * sizeof(uint8_t);
	uint8_t* _in_gcm_mac = NULL;

	if (sizeof(*_tmp_gcm_mac) != 0 &&
		16 > (SIZE_MAX / sizeof(*_tmp_gcm_mac))) {
		return SGX_ERROR_INVALID_PARAMETER;
	}

	CHECK_UNIQUE_POINTER(_tmp_p_secret, _len_p_secret);
	CHECK_UNIQUE_POINTER(_tmp_gcm_mac, _len_gcm_mac);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_p_secret != NULL && _len_p_secret != 0) {
		if ( _len_p_secret % sizeof(*_tmp_p_secret) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_p_secret = (uint8_t*)malloc(_len_p_secret);
		if (_in_p_secret == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_p_secret, _len_p_secret, _tmp_p_secret, _len_p_secret)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_gcm_mac != NULL && _len_gcm_mac != 0) {
		if ( _len_gcm_mac % sizeof(*_tmp_gcm_mac) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_gcm_mac = (uint8_t*)malloc(_len_gcm_mac);
		if (_in_gcm_mac == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_gcm_mac, _len_gcm_mac, _tmp_gcm_mac, _len_gcm_mac)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ms->ms_retval = put_secret_data(ms->ms_context, _in_p_secret, _tmp_secret_size, _in_gcm_mac);

err:
	if (_in_p_secret) free(_in_p_secret);
	if (_in_gcm_mac) free(_in_gcm_mac);
	return status;
}

static sgx_status_t SGX_CDECL sgx_sgx_ra_get_ga(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_sgx_ra_get_ga_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_sgx_ra_get_ga_t* ms = SGX_CAST(ms_sgx_ra_get_ga_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	sgx_ec256_public_t* _tmp_g_a = ms->ms_g_a;
	size_t _len_g_a = sizeof(sgx_ec256_public_t);
	sgx_ec256_public_t* _in_g_a = NULL;

	CHECK_UNIQUE_POINTER(_tmp_g_a, _len_g_a);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_g_a != NULL && _len_g_a != 0) {
		if ((_in_g_a = (sgx_ec256_public_t*)malloc(_len_g_a)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_g_a, 0, _len_g_a);
	}

	ms->ms_retval = sgx_ra_get_ga(ms->ms_context, _in_g_a);
	if (_in_g_a) {
		if (memcpy_s(_tmp_g_a, _len_g_a, _in_g_a, _len_g_a)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_g_a) free(_in_g_a);
	return status;
}

static sgx_status_t SGX_CDECL sgx_sgx_ra_proc_msg2_trusted(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_sgx_ra_proc_msg2_trusted_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_sgx_ra_proc_msg2_trusted_t* ms = SGX_CAST(ms_sgx_ra_proc_msg2_trusted_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const sgx_ra_msg2_t* _tmp_p_msg2 = ms->ms_p_msg2;
	size_t _len_p_msg2 = sizeof(sgx_ra_msg2_t);
	sgx_ra_msg2_t* _in_p_msg2 = NULL;
	const sgx_target_info_t* _tmp_p_qe_target = ms->ms_p_qe_target;
	size_t _len_p_qe_target = sizeof(sgx_target_info_t);
	sgx_target_info_t* _in_p_qe_target = NULL;
	sgx_report_t* _tmp_p_report = ms->ms_p_report;
	size_t _len_p_report = sizeof(sgx_report_t);
	sgx_report_t* _in_p_report = NULL;
	sgx_quote_nonce_t* _tmp_p_nonce = ms->ms_p_nonce;
	size_t _len_p_nonce = sizeof(sgx_quote_nonce_t);
	sgx_quote_nonce_t* _in_p_nonce = NULL;

	CHECK_UNIQUE_POINTER(_tmp_p_msg2, _len_p_msg2);
	CHECK_UNIQUE_POINTER(_tmp_p_qe_target, _len_p_qe_target);
	CHECK_UNIQUE_POINTER(_tmp_p_report, _len_p_report);
	CHECK_UNIQUE_POINTER(_tmp_p_nonce, _len_p_nonce);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_p_msg2 != NULL && _len_p_msg2 != 0) {
		_in_p_msg2 = (sgx_ra_msg2_t*)malloc(_len_p_msg2);
		if (_in_p_msg2 == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_p_msg2, _len_p_msg2, _tmp_p_msg2, _len_p_msg2)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_p_qe_target != NULL && _len_p_qe_target != 0) {
		_in_p_qe_target = (sgx_target_info_t*)malloc(_len_p_qe_target);
		if (_in_p_qe_target == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_p_qe_target, _len_p_qe_target, _tmp_p_qe_target, _len_p_qe_target)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_p_report != NULL && _len_p_report != 0) {
		if ((_in_p_report = (sgx_report_t*)malloc(_len_p_report)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_p_report, 0, _len_p_report);
	}
	if (_tmp_p_nonce != NULL && _len_p_nonce != 0) {
		if ((_in_p_nonce = (sgx_quote_nonce_t*)malloc(_len_p_nonce)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_p_nonce, 0, _len_p_nonce);
	}

	ms->ms_retval = sgx_ra_proc_msg2_trusted(ms->ms_context, (const sgx_ra_msg2_t*)_in_p_msg2, (const sgx_target_info_t*)_in_p_qe_target, _in_p_report, _in_p_nonce);
	if (_in_p_report) {
		if (memcpy_s(_tmp_p_report, _len_p_report, _in_p_report, _len_p_report)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}
	if (_in_p_nonce) {
		if (memcpy_s(_tmp_p_nonce, _len_p_nonce, _in_p_nonce, _len_p_nonce)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_p_msg2) free(_in_p_msg2);
	if (_in_p_qe_target) free(_in_p_qe_target);
	if (_in_p_report) free(_in_p_report);
	if (_in_p_nonce) free(_in_p_nonce);
	return status;
}

static sgx_status_t SGX_CDECL sgx_sgx_ra_get_msg3_trusted(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_sgx_ra_get_msg3_trusted_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_sgx_ra_get_msg3_trusted_t* ms = SGX_CAST(ms_sgx_ra_get_msg3_trusted_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	sgx_report_t* _tmp_qe_report = ms->ms_qe_report;
	size_t _len_qe_report = sizeof(sgx_report_t);
	sgx_report_t* _in_qe_report = NULL;
	sgx_ra_msg3_t* _tmp_p_msg3 = ms->ms_p_msg3;

	CHECK_UNIQUE_POINTER(_tmp_qe_report, _len_qe_report);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_qe_report != NULL && _len_qe_report != 0) {
		_in_qe_report = (sgx_report_t*)malloc(_len_qe_report);
		if (_in_qe_report == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_qe_report, _len_qe_report, _tmp_qe_report, _len_qe_report)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}

	ms->ms_retval = sgx_ra_get_msg3_trusted(ms->ms_context, ms->ms_quote_size, _in_qe_report, _tmp_p_msg3, ms->ms_msg3_size);

err:
	if (_in_qe_report) free(_in_qe_report);
	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[13];
} g_ecall_table = {
	13,
	{
		{(void*)(uintptr_t)sgx_ecall_init_oram_controller, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_seal, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_unseal, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_begin_DHKE, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_sample_key_pair, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_compute_shared_key, 0, 0},
		{(void*)(uintptr_t)sgx_enclave_init_ra, 0, 0},
		{(void*)(uintptr_t)sgx_enclave_ra_close, 0, 0},
		{(void*)(uintptr_t)sgx_verify_att_result_mac, 0, 0},
		{(void*)(uintptr_t)sgx_put_secret_data, 0, 0},
		{(void*)(uintptr_t)sgx_sgx_ra_get_ga, 0, 0},
		{(void*)(uintptr_t)sgx_sgx_ra_proc_msg2_trusted, 0, 0},
		{(void*)(uintptr_t)sgx_sgx_ra_get_msg3_trusted, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[4][13];
} g_dyn_entry_table = {
	4,
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_printf(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_printf_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_printf_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_printf_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_printf_t));
	ocalloc_size -= sizeof(ms_ocall_printf_t);

	if (str != NULL) {
		ms->ms_str = (const char*)__tmp;
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}
	
	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_get_slot(const char* slot_fingerprint)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_slot_fingerprint = slot_fingerprint ? strlen(slot_fingerprint) + 1 : 0;

	ms_ocall_get_slot_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_get_slot_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(slot_fingerprint, _len_slot_fingerprint);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (slot_fingerprint != NULL) ? _len_slot_fingerprint : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_get_slot_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_get_slot_t));
	ocalloc_size -= sizeof(ms_ocall_get_slot_t);

	if (slot_fingerprint != NULL) {
		ms->ms_slot_fingerprint = (const char*)__tmp;
		if (_len_slot_fingerprint % sizeof(*slot_fingerprint) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, slot_fingerprint, _len_slot_fingerprint)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_slot_fingerprint);
		ocalloc_size -= _len_slot_fingerprint;
	} else {
		ms->ms_slot_fingerprint = NULL;
	}
	
	status = sgx_ocall(1, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_write_slot(const char* slot_finger_print, const uint8_t* data, size_t data_len)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_slot_finger_print = slot_finger_print ? strlen(slot_finger_print) + 1 : 0;
	size_t _len_data = data_len;

	ms_ocall_write_slot_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_write_slot_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(slot_finger_print, _len_slot_finger_print);
	CHECK_ENCLAVE_POINTER(data, _len_data);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (slot_finger_print != NULL) ? _len_slot_finger_print : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (data != NULL) ? _len_data : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_write_slot_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_write_slot_t));
	ocalloc_size -= sizeof(ms_ocall_write_slot_t);

	if (slot_finger_print != NULL) {
		ms->ms_slot_finger_print = (const char*)__tmp;
		if (_len_slot_finger_print % sizeof(*slot_finger_print) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, slot_finger_print, _len_slot_finger_print)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_slot_finger_print);
		ocalloc_size -= _len_slot_finger_print;
	} else {
		ms->ms_slot_finger_print = NULL;
	}
	
	if (data != NULL) {
		ms->ms_data = (const uint8_t*)__tmp;
		if (_len_data % sizeof(*data) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, data, _len_data)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_data);
		ocalloc_size -= _len_data;
	} else {
		ms->ms_data = NULL;
	}
	
	ms->ms_data_len = data_len;
	status = sgx_ocall(2, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_exception_handler(const char* err_msg)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_err_msg = err_msg ? strlen(err_msg) + 1 : 0;

	ms_ocall_exception_handler_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_exception_handler_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(err_msg, _len_err_msg);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (err_msg != NULL) ? _len_err_msg : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_exception_handler_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_exception_handler_t));
	ocalloc_size -= sizeof(ms_ocall_exception_handler_t);

	if (err_msg != NULL) {
		ms->ms_err_msg = (const char*)__tmp;
		if (_len_err_msg % sizeof(*err_msg) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_s(__tmp, ocalloc_size, err_msg, _len_err_msg)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_err_msg);
		ocalloc_size -= _len_err_msg;
	} else {
		ms->ms_err_msg = NULL;
	}
	
	status = sgx_ocall(3, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}
