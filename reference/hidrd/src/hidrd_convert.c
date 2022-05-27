#include "hidrd/util/str.h"
#include "hidrd/util/fd.h"
#include "hidrd/fmt.h"
#include "hidrd_convert.h"

hidrd_error hidrd_convert(const char *inputData,
                          size_t inputLen,
                          char **outputData,
                          size_t *outputLen)
{
    hidrd_error         result          = HIDRD_SUCCESS;

    const void         *input_buf       = inputData;
    size_t              input_size      = inputLen;
    const char         *input_fmt_name  = "natv";
    const hidrd_fmt    *input_fmt       = NULL;
    hidrd_src          *input           = NULL;
    const char         *input_options   = "";

    void               *output_buf      = NULL;
    size_t              output_size     = 0;
    const char         *output_fmt_name = "spec";
    const hidrd_fmt    *output_fmt      = NULL;
    hidrd_snk          *output          = NULL;
    const char         *output_options  = "";

    const hidrd_item   *item;

    char               *err             = NULL;

    /*
     * Lookup and initialize input and output formats
     */
    input_fmt = hidrd_fmt_list_lkp(input_fmt_name);
    if (input_fmt == NULL)
    {
        result = HIDRD_UNKNOWN_INPUT_FORMAT;
        goto cleanup;
    }
    if (!hidrd_fmt_readable(input_fmt))
    {
        result = HIDRD_FORMAT_NOT_SUPPORT;
        goto cleanup;
    }
    if (!hidrd_fmt_init(input_fmt))
    {
        result = HIDRD_FMT_LIB_INIT_FAILED;
        goto cleanup;
    }

    output_fmt = hidrd_fmt_list_lkp(output_fmt_name);
    if (output_fmt == NULL)
    {
        result = HIDRD_UNKNOWN_OUTPUT_FORMAT;
        goto cleanup;
    }
    if (!hidrd_fmt_writable(output_fmt))
    {
        result = HIDRD_FORMAT_NOT_SUPPORT;
        goto cleanup;
    }
    if (!hidrd_fmt_init(output_fmt))
    {
        result = HIDRD_FMT_LIB_INIT_FAILED;
        goto cleanup;
    }

    /*
     * Open input and output streams
     */
    input = hidrd_src_new_opts(input_fmt->src, &err,
                               input_buf, input_size, input_options);
    if (input == NULL)
    {
        result = HIDRD_OPEN_STEAM_FAILED;
        goto cleanup;
    }
    free(err);
    err = NULL;

    output = hidrd_snk_new_opts(output_fmt->snk, &err,
                                &output_buf, &output_size, output_options);
    if (output == NULL)
    {
        result = HIDRD_OPEN_STEAM_FAILED;
        goto cleanup;
    }
    free(err);
    err = NULL;

    /*
     * Transfer the stream
     */
    while (hidrd_src_getpos(input),
           ((item = hidrd_src_get(input)) != NULL))
        if (!hidrd_snk_put(output, item))
        {
            result = HIDRD_WRITE_STEAM_FAILED;
            goto cleanup;
        }

    if (hidrd_src_error(input))
    {
        result = HIDRD_READ_STEAM_FAILED;
        goto cleanup;
    }

    /*
     * Close input and output streams
     */
    hidrd_src_delete(input);
    input = NULL;
    if (!hidrd_snk_close(output))
    {
        result = HIDRD_CLOSE_STEAM_FAILED;
        goto cleanup;
    }
    output = NULL;

    *outputData = output_buf;
    *outputLen = output_size;

    /* Success! */
    result = HIDRD_SUCCESS;

cleanup:
    free(err);

    hidrd_src_delete(input);
    hidrd_snk_delete(output);

    if (result != HIDRD_SUCCESS)
        free(output_buf);

    if (output_fmt != NULL)
        hidrd_fmt_clnp(output_fmt);
    if (input_fmt != NULL)
        hidrd_fmt_clnp(input_fmt);

    return result;
}
