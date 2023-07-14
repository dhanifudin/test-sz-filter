#include <netcdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sz.h>
#include <vector>

#define FILE_NAME "cplfco.1d.mersea.grid_U.nc"
#define NEW_FILE_NAME "cplfco.1d.mersea.new_grid_U.nc"
#define DIM1 1000
#define DIM2 1000

int main(void) {
  int status;
  int ncid, new_ncid, nvars = 0, vartype;
  std::vector<int> varids, vartypes;
  std::vector<int> dimids(2);
  std::vector<unsigned int> params;
  unsigned int filter_id = 32017; // Set actual filter ID here
  std::vector<size_t> start = {0, 0};
  std::vector<size_t> count = {DIM1, DIM2};

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

    if ((status = nc_inq_ndims(ncid, &ndim))) {
      printf("Error getting number of dimensions: %s\n", nc_strerror(status));
      return 1;
    }

    vartypes.push_back(vartype);
  }

  // for (int i = 0; i < nvars; i++) {
  //   printf("Loop %d\n", i);
  //   // Get the variable id
  //   if ((status = nc_inq_vartype(ncid, i, &datatypes[i]))) {
  //     printf("Error getting datatype id: %d, %s\n", i, nc_strerror(status));
  //     return 1;
  //   }
  //   printf("datatype: %d\n", datatypes[i]);
  // }

  // For each variable
  // for (int i = 0; i < var_names.size(); i++) {
  //   // Get the variable id
  //   if ((status = nc_inq_varid(ncid[0], var_names[i].c_str(), &varid[i]))) {
  //     printf("Error getting variable id: %s\n", nc_strerror(status));
  //     return 1;
  //   }
  //
  //   // Read the data
  //   // NOTE: nc_get_vara_float needs to be updated to handle vectors of data
  // }
  //
  // // Close the old file
  // if ((status = nc_close(ncid[0]))) {
  //   printf("Error closing file: %s\n", nc_strerror(status));
  //   return 1;
  // }
  //
  // // Create the new file
  // if ((status = nc_create(NEW_FILE_NAME, NC_NETCDF4, &new_ncid[0]))) {
  //   printf("Error creating file: %s\n", nc_strerror(status));
  //   return 1;
  // }
  //
  // // Define the dimensions in the new file
  // for (int i = 0; i < dim_names.size(); i++) {
  //   if ((status =
  //            nc_def_dim(new_ncid[0], dim_names[i].c_str(), DIM1,
  //            &dimids[i]))) {
  //     printf("Error defining dimensions: %s\n", nc_strerror(status));
  //     return 1;
  //   }
  // }
  //
  // // Define the variables in the new file
  // for (int i = 0; i < var_names.size(); i++) {
  //   if ((status = nc_def_var(new_ncid[0], var_names[i].c_str(), NC_FLOAT,
  //                            dim_names.size(), dimids.data(),
  //                            &new_varid[i]))) {
  //     printf("Error defining variable: %s\n", nc_strerror(status));
  //     return 1;
  //   }
  //
  //   // Set the filter
  //   if ((status = nc_def_var_filter(new_ncid[0], new_varid[i], filter_id, 0,
  //                                   params.data()))) {
  //     printf("Error setting filter: %s\n", nc_strerror(status));
  //     return 1;
  //   }
  //
  //   // Write the data
  //   // NOTE: nc_put_vara_float needs to be updated to handle vectors of data
  // }
  //
  // // Close the new file
  if ((status = nc_close(ncid))) {
    printf("Error closing file: %s\n", nc_strerror(status));
    return 1;
  }

  return 0;
}
