#include <malloc.h>
#include <stdio.h>
#include <ffi.h>

int main()
{
  printf("before it all\n");
  ffi_cif cif;
  ffi_type *args[4];
  void *values[4];
  char *s1;
  char *s2;
  char *s3;
  char *s4;
  long rc;
  args[0] = &ffi_type_pointer;
  args[1] = &ffi_type_pointer;
  args[2] = &ffi_type_pointer;
  args[3] = &ffi_type_pointer;
  values[0] = &s1;
  values[1] = &s2;
  values[2] = &s3;
  values[3] = &s4;
  void* sym = fprintf;
  printf("before prep\n");
  if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 4, &ffi_type_pointer, args) == FFI_OK)
    {
      s1 = "testing %s %s %s\n";
      s2 = "BLA";
      s3 = "BLI";
      s4 = "BLO";
      printf("before call\n");
      ffi_call(&cif, sym, &rc, values);
      printf("after call\n");
    }

  return 0;
}

