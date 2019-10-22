#include <php.h>
#include <sys/ioctl.h>
#include "ioctl.h"
#include <stdlib.h>
#include <errno.h>

// PHP ioctl extension
// by dj.thd <dj.thd@hotmail.com>
// UNSAFE! PHP developer is responsible of passing correct arguments to function

// Adapted for PHP 5.x by David Lublink
// PHP 5 support ended on 31 December 2018
// Very limited testing done

zend_function_entry ioctl_functions[] = {
            PHP_FE(ioctl, NULL)
        PHP_FE_END
};

zend_module_entry ioctl_module_entry = {
            STANDARD_MODULE_HEADER,
        PHP_IOCTL_EXTNAME,
        ioctl_functions,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        PHP_IOCTL_VERSION,
        STANDARD_MODULE_PROPERTIES,
};

ZEND_GET_MODULE(ioctl);

typedef struct {
         FILE *file;
         int fd;                       /* underlying file descriptor */
         unsigned is_process_pipe:1;   /* use pclose instead of fclose */
         unsigned is_pipe:1;           /* don't try and seek */
         unsigned cached_fstat:1; /* sb is valid */
         unsigned is_pipe_blocking:1; /* allow blocking read() on pipes, currently Windows only */
         unsigned _reserved:28;
      
         int lock_flag;           /* stores the lock state */
         char *temp_name;  /* if non-null, this is the path to a temporary file that
                                   * is to be deleted when the stream is closed */
     #if HAVE_FLUSHIO
              char last_op;
     #endif
          
          #if HAVE_MMAP
              char *last_mapped_addr;
         size_t last_mapped_len;
     #endif
          #ifdef PHP_WIN32
              char *last_mapped_addr;
         HANDLE file_mapping;
     #endif
          
     } php_stdio_stream_data;

PHP_FUNCTION(ioctl) {

	zval *res;
	int request;
	char *data;
	zend_bool is_ptr;

     int res_len;
     int request_len;
     int data_len;
     int is_ptr_len;


	php_stream *stream;

	int fd;

     if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rlsb", &res, &request, &data, &data_len, &is_ptr )  == FAILURE) 
     {
          RETURN_NULL();
     }

	php_stream_from_zval(stream, &res);
	php_stdio_stream_data *stream_data = (php_stdio_stream_data*)stream->abstract;
	fd = stream_data->fd;

	int result = ioctl(fd, (int)request, *data ) ;

	if(result == -1)
	{
          printf( "Error no was %i", errno );
		RETURN_FALSE;
	}
	else
	{
		RETURN_LONG(result);
	}

};
