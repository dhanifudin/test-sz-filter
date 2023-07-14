#include <netcdf.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "example.nc"
#define VAR_NAME "temperature"

#define DIM1_LEN 1000
#define DIM2_LEN 1000

int main() {
    int status, ncid, dimids[2], varid;
    float* data;
    size_t i, j;

    /* Create the file. */
    if ((status = nc_create(FILE_NAME, NC_NETCDF4, &ncid))) {
        printf("Error creating file: %s\n", nc_strerror(status));
        return 1;
    }

    /* Define dimensions. */
    if ((status = nc_def_dim(ncid, "dim1", DIM1_LEN, &dimids[0])) ||
        (status = nc_def_dim(ncid, "dim2", DIM2_LEN, &dimids[1]))) {
        printf("Error defining dimensions: %s\n", nc_strerror(status));
        return 1;
    }

    /* Define the variable. */
    if ((status = nc_def_var(ncid, VAR_NAME, NC_FLOAT, 2, dimids, &varid))) {
        printf("Error defining variable: %s\n", nc_strerror(status));
        return 1;
    }

    /* End define mode. */
    if ((status = nc_enddef(ncid))) {
        printf("Error ending define mode: %s\n", nc_strerror(status));
        return 1;
    }

    /* Allocate and assign data. */
    data = (float*) malloc(DIM1_LEN * DIM2_LEN * sizeof(float));
    for (i = 0; i < DIM1_LEN; i++) {
        for (j = 0; j < DIM2_LEN; j++) {
            data[i * DIM2_LEN + j] = (float) i * j; // Modify this as necessary
        }
    }

    /* Write the data. */
    if ((status = nc_put_var_float(ncid, varid, data))) {
        printf("Error writing data: %s\n", nc_strerror(status));
        return 1;
    }

    /* Close the file. */
    if ((status = nc_close(ncid))) {
        printf("Error closing file: %s\n", nc_strerror(status));
        return 1;
    }

    /* Free the data. */
    free(data);

    return 0;
}

