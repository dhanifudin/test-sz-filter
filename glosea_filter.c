#include <netcdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sz.h>
#include <vector>

#define FILE_NAME "cplfco.1d.mersea.grid_U.nc"
#define NEW_FILE_NAME "cplfco.1d.mersea.new_grid_U.nc"
#define SZ_FILTER 32017
#define DIM1 1000
#define DIM2 1000

int main(void) {
  int status;
  int ncid, new_ncid, nvars = 0, vartype;
  std::vector<int> vartypes, ndims;
  std::vector<std::vector<int>> ndimlens;

  std::vector<int> dimids(2);
  std::vector<unsigned int> params;
  // std::vector<size_t> start = {0, 0};
  // std::vector<size_t> count = {DIM1, DIM2};
  std::vector<size_t> start;
  std::vector<size_t> count;

  std::vector<std::vector<float>> data(DIM1, std::vector<float>(DIM2));

  std::vector<std::string> var_names = {"temperature", "humidity"};
  std::vector<std::string> dim_names = {"dim1", "dim2"};

  // Open the old file
  if ((status = nc_open(FILE_NAME, NC_NOWRITE, &ncid))) {
    printf("Error opening file: %s\n", nc_strerror(status));
    return 1;
  }

  if ((status = nc_inq_nvars(ncid, &nvars))) {
    printf("Error getting number of variable: %s\n", nc_strerror(status));
  }

  printf("Number of variables: %d\n", nvars);

  // Loop over all variables.
  for (int i = 0; i < nvars; i++) {
    // Get the type of the variable.
    if ((status = nc_inq_vartype(ncid, i, &vartype))) {
      printf("Error getting variable type: %s\n", nc_strerror(status));
      return 1;
    }

    int ndim = 0;
    int dimids[6];
    if ((status = nc_inq_varndims(ncid, i, &ndim)) ||
        (status = nc_inq_vardimid(ncid, i, dimids))) {
      printf("Error getting number of dimensions: %s\n", nc_strerror(status));
      return 1;
    }

    size_t dimlen = 0;
    std::vector<int> temp;
    for (int j = 0; j < ndim; j++) {
      if ((status = nc_inq_dimlen(ncid, dimids[j], &dimlen))) {
        printf("Error getting number of dimensions: %s\n", nc_strerror(status));
        return 1;
      }
      temp.push_back(dimlen);
      ndimlens.push_back(temp);
    }

    ndims.push_back(ndim);
    vartypes.push_back(vartype);
  }

  // TODO: Read the data

  if ((status = nc_close(ncid))) {
    printf("Error closing file: %s\n", nc_strerror(status));
    return 1;
  }

  for (int i = 0; i < nvars; i++) {
    printf("%d ndim: %d\n", i, ndims[i]);
    printf("%d vartype: %d\n", i, vartypes[i]);
    printf("Dim: ");
    for (int dim : ndimlens[i]) {
      printf("%d\t", dim);
    }
    printf("\n");
  }

  // Create the new file
  if ((status = nc_create(NEW_FILE_NAME, NC_NETCDF4, &new_ncid))) {
    printf("Error creating file: %s\n", nc_strerror(status));
    return 1;
  }

  // TODO: Define the dimensions in the new file
  // for (int i = 0; i < 6; i++) {
  //
  // }

  // Define the dimensions in the new file
  // if ((status = nc_def_dim(new_ncid, "dim1", DIM1, &dimids[0])) ||
  // (status = nc_def_dim(new_ncid, "dim2", DIM2, &dimids[1]))) {
  // printf("Error defining dimensions: %s\n", nc_strerror(status));
  // return 1;
  // }

  for (int i = 0; i < nvars; i++) {
    if ((status = nc_def_var_filter(new_ncid, new_varid, SZ_FILTER, 0, 0))) {
      printf("Error setting filter: %s\n", nc_strerror(status));
      return 1;
    }
  }

  // TODO: write the data

  // Close the new file
  if ((status = nc_close(ncid))) {
    printf("Error closing file: %s\n", nc_strerror(status));
    return 1;
  }

  return 0;
}