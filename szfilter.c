#include <netcdf.h>
#include <stdio.h>

#define FILE_NAME "example.nc"
#define VAR_NAME "temperature"
#define FILTER_ID 32017

int main() {
  int ncid;
  int var_id;
  unsigned int params[5] = {0, 0, 0, 0, 10}; // replace with your parameters

  int status;
  // Open the NetCDF file
  if (nc_open(FILE_NAME, NC_WRITE | NC_NETCDF4, &ncid)) {
    printf("Error opening file.\n");
    return 1;
  }

  // Get the ID of the variable
  if (nc_inq_varid(ncid, VAR_NAME, &var_id)) {
    printf("Error getting variable ID.\n");
    return 1;
  }

  // Call the nc_def_var_filter function
  // if ((status = nc_def_var_filter(ncid, var_id, FILTER_ID, 0, 0))) {
  // printf("Error setting filter: %s\n", nc_strerror(status));
  // return 1;
  // }

  if ((status = nc_def_var_deflate(ncid, var_id, 1, 1, 6))) {
    printf("Error deflating variable: %s\n", nc_strerror(status));
    return 1;
  }

  // Close the NetCDF file
  if (nc_close(ncid)) {
    printf("Error closing file.\n");
    return 1;
  }

  printf("Successfully defined variable filter.\n");
  return 0;
}
