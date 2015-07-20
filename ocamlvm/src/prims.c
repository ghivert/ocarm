#include "caml/mlvalues.h"
#include "caml/prims.h"
extern value caml_abs_float();
extern value caml_acos_float();
extern value caml_add_float();
extern value caml_alloc_dummy();
extern value caml_alloc_dummy_float();
extern value caml_array_append();
extern value caml_array_blit();
extern value caml_array_concat();
extern value caml_array_get();
extern value caml_array_get_addr();
extern value caml_array_get_float();
extern value caml_array_set();
extern value caml_array_set_addr();
extern value caml_array_set_float();
extern value caml_array_sub();
extern value caml_array_unsafe_get();
extern value caml_array_unsafe_get_float();
extern value caml_array_unsafe_set();
extern value caml_array_unsafe_set_addr();
extern value caml_array_unsafe_set_float();
extern value caml_asin_float();
extern value caml_atan2_float();
extern value caml_atan_float();
extern value caml_bitvect_test();
extern value caml_blit_string();
extern value caml_bswap16();
extern value caml_ceil_float();
extern value caml_classify_float();
extern value caml_compare();
extern value caml_copysign_float();
extern value caml_cos_float();
extern value caml_cosh_float();
extern value caml_create_string();
extern value caml_div_float();
extern value caml_eq_float();
extern value caml_equal();
extern value caml_exp_float();
extern value caml_expm1_float();
extern value caml_fill_string();
extern value caml_float_compare();
extern value caml_float_of_int();
extern value caml_float_of_string();
extern value caml_floor_float();
extern value caml_fmod_float();
extern value caml_format_float();
extern value caml_format_int();
extern value caml_frexp_float();
extern value caml_ge_float();
extern value caml_get_public_method();
extern value caml_greaterequal();
extern value caml_greaterthan();
extern value caml_gt_float();
extern value caml_hash();
extern value caml_hash_univ_param();
extern value caml_hypot_float();
extern value caml_int32_add();
extern value caml_int32_and();
extern value caml_int32_bits_of_float();
extern value caml_int32_bswap();
extern value caml_int32_compare();
extern value caml_int32_div();
extern value caml_int32_float_of_bits();
extern value caml_int32_format();
extern value caml_int32_mod();
extern value caml_int32_mul();
extern value caml_int32_neg();
extern value caml_int32_of_float();
extern value caml_int32_of_int();
extern value caml_int32_of_string();
extern value caml_int32_or();
extern value caml_int32_shift_left();
extern value caml_int32_shift_right();
extern value caml_int32_shift_right_unsigned();
extern value caml_int32_sub();
extern value caml_int32_to_float();
extern value caml_int32_to_int();
extern value caml_int32_xor();
extern value caml_int64_add();
extern value caml_int64_and();
extern value caml_int64_bits_of_float();
extern value caml_int64_bswap();
extern value caml_int64_compare();
extern value caml_int64_div();
extern value caml_int64_float_of_bits();
extern value caml_int64_format();
extern value caml_int64_mod();
extern value caml_int64_mul();
extern value caml_int64_neg();
extern value caml_int64_of_float();
extern value caml_int64_of_int();
extern value caml_int64_of_int32();
extern value caml_int64_of_nativeint();
extern value caml_int64_of_string();
extern value caml_int64_or();
extern value caml_int64_shift_left();
extern value caml_int64_shift_right();
extern value caml_int64_shift_right_unsigned();
extern value caml_int64_sub();
extern value caml_int64_to_float();
extern value caml_int64_to_int();
extern value caml_int64_to_int32();
extern value caml_int64_to_nativeint();
extern value caml_int64_xor();
extern value caml_int_as_pointer();
extern value caml_int_compare();
extern value caml_int_of_float();
extern value caml_int_of_string();
extern value caml_lazy_follow_forward();
extern value caml_lazy_make_forward();
extern value caml_ldexp_float();
extern value caml_le_float();
extern value caml_lessequal();
extern value caml_lessthan();
extern value caml_log10_float();
extern value caml_log1p_float();
extern value caml_log_float();
extern value caml_lt_float();
extern value caml_make_array();
extern value caml_make_float_vect();
extern value caml_make_vect();
extern value caml_marshal_data_size();
extern value caml_md5_string();
extern value caml_ml_string_length();
extern value caml_modf_float();
extern value caml_mul_float();
extern value caml_nativeint_add();
extern value caml_nativeint_and();
extern value caml_nativeint_bswap();
extern value caml_nativeint_compare();
extern value caml_nativeint_div();
extern value caml_nativeint_format();
extern value caml_nativeint_mod();
extern value caml_nativeint_mul();
extern value caml_nativeint_neg();
extern value caml_nativeint_of_float();
extern value caml_nativeint_of_int();
extern value caml_nativeint_of_int32();
extern value caml_nativeint_of_string();
extern value caml_nativeint_or();
extern value caml_nativeint_shift_left();
extern value caml_nativeint_shift_right();
extern value caml_nativeint_shift_right_unsigned();
extern value caml_nativeint_sub();
extern value caml_nativeint_to_float();
extern value caml_nativeint_to_int();
extern value caml_nativeint_to_int32();
extern value caml_nativeint_xor();
extern value caml_neg_float();
extern value caml_neq_float();
extern value caml_notequal();
extern value caml_obj_add_offset();
extern value caml_obj_block();
extern value caml_obj_dup();
extern value caml_obj_is_block();
extern value caml_obj_set_tag();
extern value caml_obj_tag();
extern value caml_obj_truncate();
extern value caml_power_float();
extern value caml_set_oo_id();
extern value caml_sin_float();
extern value caml_sinh_float();
extern value caml_sqrt_float();
extern value caml_static_alloc();
extern value caml_static_free();
extern value caml_static_resize();
extern value caml_string_compare();
extern value caml_string_equal();
extern value caml_string_get();
extern value caml_string_get16();
extern value caml_string_get32();
extern value caml_string_get64();
extern value caml_string_greaterequal();
extern value caml_string_greaterthan();
extern value caml_string_lessequal();
extern value caml_string_lessthan();
extern value caml_string_notequal();
extern value caml_string_set();
extern value caml_string_set16();
extern value caml_string_set32();
extern value caml_string_set64();
extern value caml_sub_float();
extern value caml_tan_float();
extern value caml_tanh_float();
extern value caml_update_dummy();
c_primitive caml_builtin_cprim[] = {
	caml_abs_float,
	caml_acos_float,
	caml_add_float,
	caml_alloc_dummy,
	caml_alloc_dummy_float,
	caml_array_append,
	caml_array_blit,
	caml_array_concat,
	caml_array_get,
	caml_array_get_addr,
	caml_array_get_float,
	caml_array_set,
	caml_array_set_addr,
	caml_array_set_float,
	caml_array_sub,
	caml_array_unsafe_get,
	caml_array_unsafe_get_float,
	caml_array_unsafe_set,
	caml_array_unsafe_set_addr,
	caml_array_unsafe_set_float,
	caml_asin_float,
	caml_atan2_float,
	caml_atan_float,
	caml_bitvect_test,
	caml_blit_string,
	caml_bswap16,
	caml_ceil_float,
	caml_classify_float,
	caml_compare,
	caml_copysign_float,
	caml_cos_float,
	caml_cosh_float,
	caml_create_string,
	caml_div_float,
	caml_eq_float,
	caml_equal,
	caml_exp_float,
	caml_expm1_float,
	caml_fill_string,
	caml_float_compare,
	caml_float_of_int,
	caml_float_of_string,
	caml_floor_float,
	caml_fmod_float,
	caml_format_float,
	caml_format_int,
	caml_frexp_float,
	caml_ge_float,
	caml_get_public_method,
	caml_greaterequal,
	caml_greaterthan,
	caml_gt_float,
	caml_hash,
	caml_hash_univ_param,
	caml_hypot_float,
	caml_int32_add,
	caml_int32_and,
	caml_int32_bits_of_float,
	caml_int32_bswap,
	caml_int32_compare,
	caml_int32_div,
	caml_int32_float_of_bits,
	caml_int32_format,
	caml_int32_mod,
	caml_int32_mul,
	caml_int32_neg,
	caml_int32_of_float,
	caml_int32_of_int,
	caml_int32_of_string,
	caml_int32_or,
	caml_int32_shift_left,
	caml_int32_shift_right,
	caml_int32_shift_right_unsigned,
	caml_int32_sub,
	caml_int32_to_float,
	caml_int32_to_int,
	caml_int32_xor,
	caml_int64_add,
	caml_int64_and,
	caml_int64_bits_of_float,
	caml_int64_bswap,
	caml_int64_compare,
	caml_int64_div,
	caml_int64_float_of_bits,
	caml_int64_format,
	caml_int64_mod,
	caml_int64_mul,
	caml_int64_neg,
	caml_int64_of_float,
	caml_int64_of_int,
	caml_int64_of_int32,
	caml_int64_of_nativeint,
	caml_int64_of_string,
	caml_int64_or,
	caml_int64_shift_left,
	caml_int64_shift_right,
	caml_int64_shift_right_unsigned,
	caml_int64_sub,
	caml_int64_to_float,
	caml_int64_to_int,
	caml_int64_to_int32,
	caml_int64_to_nativeint,
	caml_int64_xor,
	caml_int_as_pointer,
	caml_int_compare,
	caml_int_of_float,
	caml_int_of_string,
	caml_lazy_follow_forward,
	caml_lazy_make_forward,
	caml_ldexp_float,
	caml_le_float,
	caml_lessequal,
	caml_lessthan,
	caml_log10_float,
	caml_log1p_float,
	caml_log_float,
	caml_lt_float,
	caml_make_array,
	caml_make_float_vect,
	caml_make_vect,
	caml_marshal_data_size,
	caml_md5_string,
	caml_ml_string_length,
	caml_modf_float,
	caml_mul_float,
	caml_nativeint_add,
	caml_nativeint_and,
	caml_nativeint_bswap,
	caml_nativeint_compare,
	caml_nativeint_div,
	caml_nativeint_format,
	caml_nativeint_mod,
	caml_nativeint_mul,
	caml_nativeint_neg,
	caml_nativeint_of_float,
	caml_nativeint_of_int,
	caml_nativeint_of_int32,
	caml_nativeint_of_string,
	caml_nativeint_or,
	caml_nativeint_shift_left,
	caml_nativeint_shift_right,
	caml_nativeint_shift_right_unsigned,
	caml_nativeint_sub,
	caml_nativeint_to_float,
	caml_nativeint_to_int,
	caml_nativeint_to_int32,
	caml_nativeint_xor,
	caml_neg_float,
	caml_neq_float,
	caml_notequal,
	caml_obj_add_offset,
	caml_obj_block,
	caml_obj_dup,
	caml_obj_is_block,
	caml_obj_set_tag,
	caml_obj_tag,
	caml_obj_truncate,
	caml_power_float,
	caml_set_oo_id,
	caml_sin_float,
	caml_sinh_float,
	caml_sqrt_float,
	caml_static_alloc,
	caml_static_free,
	caml_static_resize,
	caml_string_compare,
	caml_string_equal,
	caml_string_get,
	caml_string_get16,
	caml_string_get32,
	caml_string_get64,
	caml_string_greaterequal,
	caml_string_greaterthan,
	caml_string_lessequal,
	caml_string_lessthan,
	caml_string_notequal,
	caml_string_set,
	caml_string_set16,
	caml_string_set32,
	caml_string_set64,
	caml_sub_float,
	caml_tan_float,
	caml_tanh_float,
	caml_update_dummy,
	caml_do_nothing,
	0 };
char * caml_names_of_builtin_cprim[] = {
	"caml_abs_float",
	"caml_acos_float",
	"caml_add_float",
	"caml_alloc_dummy",
	"caml_alloc_dummy_float",
	"caml_array_append",
	"caml_array_blit",
	"caml_array_concat",
	"caml_array_get",
	"caml_array_get_addr",
	"caml_array_get_float",
	"caml_array_set",
	"caml_array_set_addr",
	"caml_array_set_float",
	"caml_array_sub",
	"caml_array_unsafe_get",
	"caml_array_unsafe_get_float",
	"caml_array_unsafe_set",
	"caml_array_unsafe_set_addr",
	"caml_array_unsafe_set_float",
	"caml_asin_float",
	"caml_atan2_float",
	"caml_atan_float",
	"caml_bitvect_test",
	"caml_blit_string",
	"caml_bswap16",
	"caml_ceil_float",
	"caml_classify_float",
	"caml_compare",
	"caml_copysign_float",
	"caml_cos_float",
	"caml_cosh_float",
	"caml_create_string",
	"caml_div_float",
	"caml_eq_float",
	"caml_equal",
	"caml_exp_float",
	"caml_expm1_float",
	"caml_fill_string",
	"caml_float_compare",
	"caml_float_of_int",
	"caml_float_of_string",
	"caml_floor_float",
	"caml_fmod_float",
	"caml_format_float",
	"caml_format_int",
	"caml_frexp_float",
	"caml_ge_float",
	"caml_get_public_method",
	"caml_greaterequal",
	"caml_greaterthan",
	"caml_gt_float",
	"caml_hash",
	"caml_hash_univ_param",
	"caml_hypot_float",
	"caml_int32_add",
	"caml_int32_and",
	"caml_int32_bits_of_float",
	"caml_int32_bswap",
	"caml_int32_compare",
	"caml_int32_div",
	"caml_int32_float_of_bits",
	"caml_int32_format",
	"caml_int32_mod",
	"caml_int32_mul",
	"caml_int32_neg",
	"caml_int32_of_float",
	"caml_int32_of_int",
	"caml_int32_of_string",
	"caml_int32_or",
	"caml_int32_shift_left",
	"caml_int32_shift_right",
	"caml_int32_shift_right_unsigned",
	"caml_int32_sub",
	"caml_int32_to_float",
	"caml_int32_to_int",
	"caml_int32_xor",
	"caml_int64_add",
	"caml_int64_and",
	"caml_int64_bits_of_float",
	"caml_int64_bswap",
	"caml_int64_compare",
	"caml_int64_div",
	"caml_int64_float_of_bits",
	"caml_int64_format",
	"caml_int64_mod",
	"caml_int64_mul",
	"caml_int64_neg",
	"caml_int64_of_float",
	"caml_int64_of_int",
	"caml_int64_of_int32",
	"caml_int64_of_nativeint",
	"caml_int64_of_string",
	"caml_int64_or",
	"caml_int64_shift_left",
	"caml_int64_shift_right",
	"caml_int64_shift_right_unsigned",
	"caml_int64_sub",
	"caml_int64_to_float",
	"caml_int64_to_int",
	"caml_int64_to_int32",
	"caml_int64_to_nativeint",
	"caml_int64_xor",
	"caml_int_as_pointer",
	"caml_int_compare",
	"caml_int_of_float",
	"caml_int_of_string",
	"caml_lazy_follow_forward",
	"caml_lazy_make_forward",
	"caml_ldexp_float",
	"caml_le_float",
	"caml_lessequal",
	"caml_lessthan",
	"caml_log10_float",
	"caml_log1p_float",
	"caml_log_float",
	"caml_lt_float",
	"caml_make_array",
	"caml_make_float_vect",
	"caml_make_vect",
	"caml_marshal_data_size",
	"caml_md5_string",
	"caml_ml_string_length",
	"caml_modf_float",
	"caml_mul_float",
	"caml_nativeint_add",
	"caml_nativeint_and",
	"caml_nativeint_bswap",
	"caml_nativeint_compare",
	"caml_nativeint_div",
	"caml_nativeint_format",
	"caml_nativeint_mod",
	"caml_nativeint_mul",
	"caml_nativeint_neg",
	"caml_nativeint_of_float",
	"caml_nativeint_of_int",
	"caml_nativeint_of_int32",
	"caml_nativeint_of_string",
	"caml_nativeint_or",
	"caml_nativeint_shift_left",
	"caml_nativeint_shift_right",
	"caml_nativeint_shift_right_unsigned",
	"caml_nativeint_sub",
	"caml_nativeint_to_float",
	"caml_nativeint_to_int",
	"caml_nativeint_to_int32",
	"caml_nativeint_xor",
	"caml_neg_float",
	"caml_neq_float",
	"caml_notequal",
	"caml_obj_add_offset",
	"caml_obj_block",
	"caml_obj_dup",
	"caml_obj_is_block",
	"caml_obj_set_tag",
	"caml_obj_tag",
	"caml_obj_truncate",
	"caml_power_float",
	"caml_set_oo_id",
	"caml_sin_float",
	"caml_sinh_float",
	"caml_sqrt_float",
	"caml_static_alloc",
	"caml_static_free",
	"caml_static_resize",
	"caml_string_compare",
	"caml_string_equal",
	"caml_string_get",
	"caml_string_get16",
	"caml_string_get32",
	"caml_string_get64",
	"caml_string_greaterequal",
	"caml_string_greaterthan",
	"caml_string_lessequal",
	"caml_string_lessthan",
	"caml_string_notequal",
	"caml_string_set",
	"caml_string_set16",
	"caml_string_set32",
	"caml_string_set64",
	"caml_sub_float",
	"caml_tan_float",
	"caml_tanh_float",
	"caml_update_dummy",
	0 };

value caml_do_nothing(void) {
  printf("Primitive non disponible.\n");
  return 0;
}
