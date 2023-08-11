#include <stdio.h>
#include <stdlib.h>
#include <netcdf.h>

void handle_error(int status) {
  if (status != NC_NOERR) {
    fprintf(stderr, "%s\n", nc_strerror(status));
    exit(1);
  }
}

size_t nc_type_size(nc_type xtype) {
  switch (xtype) {
    case NC_BYTE:
    case NC_CHAR:
      return sizeof(char);
    case NC_SHORT:
      return sizeof(short);
    case NC_INT:
      return sizeof(int);
    case NC_FLOAT:
      return sizeof(float);
    case NC_DOUBLE:
      return sizeof(double);
      // Add more types if necessary
    default:
      fprintf(stderr, "Unsupported type: %d\n", xtype);
      exit(1);
  }
}

// Function to copy attributes from one variable to another
void copy_attributes(int src_ncid, int src_varid, int dest_ncid, int dest_varid) {
  int status;
  int natts;
  status = nc_inq_varnatts(src_ncid, src_varid, &natts);
  handle_error(status);
  for (int attid = 0; attid < natts; attid++) {
    char attname[NC_MAX_NAME + 1];
    nc_type atttype;
    size_t attlen;

    status = nc_inq_attname(src_ncid, src_varid, attid, attname);
    handle_error(status);
    status = nc_inq_att(src_ncid, src_varid, attname, &atttype, &attlen);
    handle_error(status);

    // Allocate memory based on the attribute type and length
    void *attdata = malloc(attlen * nc_type_size(atttype)); // assuming nc_type_size() returns the size in bytes for the given type
    status = nc_get_att(src_ncid, src_varid, attname, attdata);
    handle_error(status);

    // Put the attribute to the new variable
    status = nc_put_att(dest_ncid, dest_varid, attname, atttype, attlen, attdata);
    handle_error(status);

    free(attdata);
  }
}

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Usage: %s [source-netcdf-file] [destination-netcdf-file]\n", argv[0]);
    return 1;
  }

  int old_ncid, new_ncid, varid, ndims, nvars, ngatts, unlimdimid;
  int status;

  // Open the old NetCDF file in read-only mode
  status = nc_open(argv[1], NC_NOWRITE, &old_ncid);
  handle_error(status);

  // Create a new NetCDF file using NetCDF-4 format
  status = nc_create(argv[2], NC_NETCDF4, &new_ncid);
  handle_error(status);

  // Handle dimensions: Simply copy all dimensions to the new file
  status = nc_inq_ndims(old_ncid, &ndims);
  handle_error(status);
  for (int i = 0; i < ndims; i++) {
    char dimname[NC_MAX_NAME + 1];
    size_t dimlen;
    status = nc_inq_dim(old_ncid, i, dimname, &dimlen);
    handle_error(status);

    status = nc_def_dim(new_ncid, dimname, dimlen, NULL); // Create dimension in new file
    handle_error(status);
  }

  // Handle variables
  status = nc_inq_nvars(old_ncid, &nvars);
  handle_error(status);

  for (int i = 0; i < nvars; i++) {
    varid = i;

    // Get details about the variable
    char varname[NC_MAX_NAME + 1];
    nc_type vartype;
    int varndims, vardimids[NC_MAX_VAR_DIMS], varnatts;
    status = nc_inq_var(old_ncid, varid, varname, &vartype, &varndims, vardimids, &varnatts);
    handle_error(status);

    // Define a similar variable in the new file
    int new_varid;
    status = nc_def_var(new_ncid, varname, vartype, varndims, vardimids, &new_varid);
    handle_error(status);

    copy_attributes(old_ncid, varid, new_ncid, varid);

    // Apply deflate compression to the variable in the new file
    status = nc_def_var_filter(new_ncid, new_varid, 32017, 0, 0); // Assuming shuffle filter, level 4 compression
    handle_error(status);

    // Get the size of the variable
    size_t varsize = 1;
    for (int d = 0; d < varndims; d++) {
      size_t len;
      status = nc_inq_dimlen(old_ncid, vardimids[d], &len);
      handle_error(status);
      varsize *= len;
    }

    // Copy data from old file to new file
    // Calculate the total size of the variable based on its type
    size_t typesize;
    switch (vartype) {
      case NC_BYTE:
      case NC_CHAR:
        typesize = sizeof(char);
        break;
      case NC_SHORT:
        typesize = sizeof(short);
        break;
      case NC_INT:
        typesize = sizeof(int);
        break;
      case NC_FLOAT:
        typesize = sizeof(float);
        break;
      case NC_DOUBLE:
        typesize = sizeof(double);
        break;
        // ... Add more types as necessary
      default:
        fprintf(stderr, "Unsupported type: %d\n", vartype);
        exit(1);
    }
    void *data = malloc(varsize * typesize);
    status = nc_get_var(old_ncid, varid, data);
    handle_error(status);
    status = nc_put_var(new_ncid, new_varid, data);
    handle_error(status);
    free(data);
  }

  // Close both files
  status = nc_close(old_ncid);
  handle_error(status);
  status = nc_close(new_ncid);
  handle_error(status);

  printf("Data copied and compressed successfully from %s to %s\n", argv[1], argv[2]);
  return 0;
}
