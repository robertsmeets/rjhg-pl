#include <malloc.h>
#include <stdio.h>
#include <ffi.h>

int main()
{
  ffi_cif cif;
  ffi_type *args[2];
  void *values[2];
  char *s1;
  char *s2;
  long rc;
  args[0] = &ffi_type_pointer;
  args[1] = &ffi_type_pointer;
  values[0] = &s1;
  values[1] = &s2;
  void* sym = fopen;
  if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 2, &ffi_type_pointer, args) == FFI_OK)
    {
      s1 = "f1.txt";
      s2 = "w";
      ffi_call(&cif, sym, &rc, values);
    }

  return 0;
}

