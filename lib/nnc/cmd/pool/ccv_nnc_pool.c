#include <ccv.h>
#include <nnc/ccv_nnc.h>
#include <nnc/ccv_nnc_easy.h>
#include <nnc/ccv_nnc_internal.h>

static int _ccv_nnc_max_pool_forw_bitmask(const uint64_t* const input_bitmasks, const int input_bitmask_size, const uint64_t* const output_bitmasks, const int output_bitmask_size)
{
	if ((input_bitmasks[0] & 1u) == 1u && output_bitmasks[0] == 1u)
		return 1;
	return 0;
}

static int _ccv_nnc_max_pool_back_bitmask(const uint64_t* const input_bitmasks, const int input_bitmask_size, const uint64_t* const output_bitmasks, const int output_bitmask_size)
{
	if ((input_bitmasks[0] & 7u) == ((1u << 0) | (1u << 1) | (1u << 2)) && output_bitmasks[0] == 1u)
		return 1;
	return 0;
}

static void _ccv_nnc_pool_tensor_auto_forw(const ccv_nnc_cmd_param_t cmd, const ccv_nnc_tensor_param_t* inputs, const int input_size, const ccv_nnc_hint_t hint, ccv_nnc_tensor_param_t* outputs, const int output_size)
{
	assert(output_size == 1);
	outputs[0].type = inputs[0].type;
	outputs[0].format = inputs[0].format;
	outputs[0].datatype = inputs[0].datatype;
	// Copy channels / batch size from the original input for now.
	memcpy(outputs[0].dim, inputs[0].dim, sizeof(inputs[0].dim));
	ccv_nnc_hint_tensor_forward(cmd, inputs[0], hint, outputs);
}

static void _ccv_nnc_pool_tensor_auto_back(const ccv_nnc_cmd_param_t cmd, const ccv_nnc_tensor_param_t* inputs, const int input_size, const ccv_nnc_hint_t hint, ccv_nnc_tensor_param_t* outputs, const int output_size)
{
	assert(output_size == 1);
	outputs[0].type = inputs[0].type;
	outputs[0].format = inputs[0].format;
	outputs[0].datatype = inputs[0].datatype;
	// Copy channels / batch size from the original input for now.
	memcpy(outputs[0].dim, inputs[0].dim, sizeof(inputs[0].dim));
	ccv_nnc_hint_tensor_backward(cmd, inputs[0], hint, outputs);
}

REGISTER_COMMAND(CCV_NNC_MAX_POOL_FORWARD)(ccv_nnc_cmd_registry_t* const registry)
	FIND_BACKEND(ccv_nnc_max_pool_cpu_ref.c)
{
	registry->bitmask = _ccv_nnc_max_pool_forw_bitmask;
	registry->tensor_auto = _ccv_nnc_pool_tensor_auto_forw;
}

REGISTER_COMMAND(CCV_NNC_MAX_POOL_BACKWARD)(ccv_nnc_cmd_registry_t* const registry)
	FIND_BACKEND(ccv_nnc_max_pool_cpu_ref.c)
{
	registry->bitmask = _ccv_nnc_max_pool_back_bitmask;
	registry->tensor_auto = _ccv_nnc_pool_tensor_auto_back;
}

static int _ccv_nnc_avg_pool_forw_bitmask(const uint64_t* const input_bitmasks, const int input_bitmask_size, const uint64_t* const output_bitmasks, const int output_bitmask_size)
{
	if ((input_bitmasks[0] & 1u) == 1u && output_bitmasks[0] == 1u)
		return 1;
	return 0;
}

static int _ccv_nnc_avg_pool_back_bitmask(const uint64_t* const input_bitmasks, const int input_bitmask_size, const uint64_t* const output_bitmasks, const int output_bitmask_size)
{
	if ((input_bitmasks[0] & 1u) == 1u && output_bitmasks[0] == 1u)
		return 1;
	return 0;
}

REGISTER_COMMAND(CCV_NNC_AVERAGE_POOL_FORWARD)(ccv_nnc_cmd_registry_t* const registry)
	FIND_BACKEND(ccv_nnc_avg_pool_cpu_ref.c)
{
	registry->bitmask = _ccv_nnc_avg_pool_forw_bitmask;
	registry->tensor_auto = _ccv_nnc_pool_tensor_auto_forw;
}

REGISTER_COMMAND(CCV_NNC_AVERAGE_POOL_BACKWARD)(ccv_nnc_cmd_registry_t* const registry)
	FIND_BACKEND(ccv_nnc_avg_pool_cpu_ref.c)
{
	registry->bitmask = _ccv_nnc_avg_pool_back_bitmask;
	registry->tensor_auto = _ccv_nnc_pool_tensor_auto_back;
}
