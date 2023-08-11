#include <netcdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sz.h>

#define FILE_NAME "example.nc"
#define NEW_FILE_NAME "example_new.nc"
#define VAR_NAME "temperature"
#define DIM1 1000
#define DIM2 1000

int main(void) {
  int ncid, new_ncid, varid, new_varid, status;
  int dimids[2];
  unsigned int params[4] = {2, SZ_FLOAT, 1000,
                            1000}; // Set actual filter parameters here
  unsigned int filter_id = 32017;  // Set actual filter ID here
  size_t start[2] = {0, 0};
  size_t count[2] = {DIM1, DIM2};

  float data[DIM1][DIM2];

  // Open the old file
  if ((status = nc_open(FILE_NAME, NC_NOWRITE, &ncid))) {
    printf("Error opening file: %s\n", nc_strerror(status));
    return 1;
  }

  // Get the variable id
  if ((status = nc_inq_varid(ncid, VAR_NAME, &varid))) {
    printf("Error getting variable id: %s\n", nc_strerror(status));
    return 1;
  }

  // Read the data
  if ((status = nc_get_vara_float(ncid, varid, start, count, &data[0][0]))) {
    printf("Error reading variable: %s\n", nc_strerror(status));
    return 1;
  }

  // Close the old file
  if ((status = nc_close(ncid))) {
    printf("Error closing file: %s\n", nc_strerror(status));
    return 1;
  }

  // Create the new file
  if ((status = nc_create(NEW_FILE_NAME, NC_NETCDF4, &new_ncid))) {
    printf("Error creating file: %s\n", nc_strerror(status));
    return 1;
  }

  // Define the dimensions in the new file
  if ((status = nc_def_dim(new_ncid, "dim1", DIM1, &dimids[0])) ||
      (status = nc_def_dim(new_ncid, "dim2", DIM2, &dimids[1]))) {
    printf("Error defining dimensions: %s\n", nc_strerror(status));
    return 1;
  }

  // Define the variable in the new file
  if ((status =
           nc_def_var(new_ncid, VAR_NAME, NC_FLOAT, 2, dimids, &new_varid))) {
    printf("Error defining variable: %s\n", nc_strerror(status));
    return 1;
  }

  // Set the filter
  if ((status = nc_def_var_filter(new_ncid, new_varid, filter_id, 0, params))) {
    printf("Error setting filter: %s\n", nc_strerror(status));
    return 1;
  }

  // Write the data
  if ((status =
           nc_put_vara_float(new_ncid, new_varid, start, count, &data[0][0]))) {
    printf("Error writing variable: %s\n", nc_strerror(status));
    return 1;
  }

  // Close the new file
  if ((status = nc_close(new_ncid))) {
    printf("Error closing file: %s\n", nc_strerror(status));
    return 1;
  }

  return 0;
}