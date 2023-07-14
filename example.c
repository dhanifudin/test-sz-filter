#include <stdio.h>
#include <netcdf.h>

#define FILE_NAME "example.nc"
#define VAR_NAME "temperature"
#define DIM_NAME "time"
#define DIM_LENGTH 10

int main() {
    int status, ncid, varid, dimid;
    float data[DIM_LENGTH] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    
    /* Create a new NetCDF-4 file */
    if ((status = nc_create(FILE_NAME, NC_NETCDF4, &ncid))) {
        printf("Error creating file: %s\n", nc_strerror(status));
        return 1;
    }

    /* Define the dimension */
    if ((status = nc_def_dim(ncid, DIM_NAME, DIM_LENGTH, &dimid))) {
        printf("Error defining dimension: %s\n", nc_strerror(status));
        return 1;
    }

    /* Define the variable */
    if ((status = nc_def_var(ncid, VAR_NAME, NC_FLOAT, 1, &dimid, &varid))) {
        printf("Error defining variable: %s\n", nc_strerror(status));
        return 1;
    }

    /* Leave the define mode */
    if ((status = nc_enddef(ncid))) {
        printf("Error ending define mode: %s\n", nc_strerror(status));
        return 1;
    }

    /* Write data to the variable */
    if ((status = nc_put_var_float(ncid, varid, data))) {
        printf("Error writing data: %s\n", nc_strerror(status));
        return 1;
    }

    /* Close the file */
    if ((status = nc_close(ncid))) {
        printf("Error closing file: %s\n", nc_strerror(status));
        return 1;
    }

    return 0;
}

