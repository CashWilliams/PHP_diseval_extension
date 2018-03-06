/*
 * phpize
 * ./configure
 * make
 * php -dextension=modules/diseval.so -r "diseval_info();";
 **/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "diseval.h"
#include "zend.h"
#include "zend_compile.h"
#include "zend_API.h"
#include "ext/standard/info.h"

#ifdef COMPILE_DL_DISEVAL
ZEND_GET_MODULE(diseval)
#endif

void (*zend_execute_old)(zend_execute_data *execute_data TSRMLS_DC);

zend_function_entry diseval_functions[] = {
	PHP_FE(diseval_info,   0)
	PHP_FE_END
};

zend_module_entry diseval_module_entry = {
	STANDARD_MODULE_HEADER,
	"diseval",
	diseval_functions,
	PHP_MINIT(diseval),
	PHP_MSHUTDOWN(diseval),
	PHP_RINIT(diseval),    
	PHP_RSHUTDOWN(diseval),
	PHP_MINFO(diseval),    
	"0.1",
	STANDARD_MODULE_PROPERTIES
};


PHP_MINIT_FUNCTION(diseval)
{
	zend_execute_old = zend_execute_ex;
	zend_execute_ex = diseval_execute_ex;
	return SUCCESS;
}

PHP_MINFO_FUNCTION(diseval)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Diseval support", "enabled");
	php_info_print_table_end();
}

PHP_MSHUTDOWN_FUNCTION(diseval)
{
	zend_execute_ex = zend_execute_old;
	return SUCCESS;
}

PHP_RINIT_FUNCTION(diseval)
{
	return SUCCESS;
}
PHP_RSHUTDOWN_FUNCTION(diseval)
{
	return SUCCESS;
}

PHP_FUNCTION(diseval_info)
{
	php_printf("DISEVAL Version 0.1\n");
	RETURN_FALSE;
}

void diseval_execute_ex(zend_execute_data *execute_data TSRMLS_DC)
{
#if PHP_VERSION_ID >= 70000
	const zend_op_array *op_array = &execute_data->func->op_array;
#else
	const zend_op_array *op_array = execute_data->op_array;
#endif

	if (op_array->type == ZEND_EVAL_CODE) {
    // allowed flag
    bool allow_eval = false;

    zval return_value; 
    php_printf("Generating backtrace.\n");
    zend_fetch_debug_backtrace(&return_value, 0, 0, 0);
    php_var_dump(&return_value, 0);

    if (Z_TYPE(return_value) == IS_ARRAY) {
      HashTable *myht;
      myht = Z_ARRVAL(return_value);
      zval *zv_parent;

      if ( (zv_parent = zend_hash_index_find(myht, 1)) ) {
        if (Z_TYPE_P(zv_parent) == IS_ARRAY) {
          HashTable *myht_parent = NULL;
          myht_parent = Z_ARRVAL_P(zv_parent);

          zval *function;
          function = zend_hash_str_find_ind(myht_parent, "function", sizeof("function")-1);
          // TODO: hard coded "allow_eval" for now, will be array from php.ini later.
          if (strcmp(Z_STRVAL_P(function),"allow_eval") == 0) {
            allow_eval = true;
          }

          zend_hash_destroy(myht_parent);
          efree(myht_parent);
        }
      }
      
      zend_hash_destroy(myht);
      efree(myht);
    }
    _zval_ptr_dtor(&return_value);

    if (!allow_eval) {
      zend_error(E_ERROR, "DISEVAL - Use of eval is forbidden");
      zend_bailout();
    }
	}
	zend_execute_old(execute_data TSRMLS_CC);
}



