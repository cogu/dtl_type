#include <stdio.h>
#include <stdlib.h>
#include "dtl_type.h"
#include "adt_str.h"

int main(int argc, char **argv)
{
   int i;
   dtl_sv_t *sv;

   /*** Scalar values ***/
   dtl_sv_t *sv1 = dtl_sv_make_i32(125);
   dtl_sv_t *sv2 = dtl_sv_make_cstr("Hello World");
   dtl_sv_t *sv3 = dtl_sv_make_dbl(10.2);

   printf("%d\n", dtl_sv_to_i32(sv1, NULL));
   printf("%s\n", dtl_sv_to_cstr(sv2));
   printf("%f\n", dtl_sv_to_dbl(sv3, NULL));
   printf("\n");

   /*** Array Values ***/
   dtl_av_t *av = dtl_av_new();
   dtl_av_push(av, (dtl_dv_t*) sv1, true);
   dtl_av_push(av, (dtl_dv_t*) sv2, true);
   dtl_av_push(av, (dtl_dv_t*) sv3, true);
   //reference count for sv1, sv2 and sv3 are now set to 2

   /*** Printing Array Values***/
   for (i = 0; i < dtl_av_length(av); i++)
   {
      sv = (dtl_sv_t*) dtl_av_value(av, i);
      const char *cstr = dtl_sv_to_cstr(sv);
      printf("%s\n", cstr );
   }
   dtl_dec_ref(av); //deletes av. Reference count for sv1,sv2 and sv3 is now 1
   printf("\n");

   const char *key;

   /*** Hash Values ***/
   dtl_hv_t *hv = dtl_hv_new();
   dtl_hv_set(hv, "first", (dtl_dv_t*) sv1, true);
   dtl_hv_set(hv, "second",(dtl_dv_t*) sv2, true);
   dtl_hv_set(hv, "third", (dtl_dv_t*) sv3, true);
   //reference count for sv1,sv2 and sv3 is now 2

   dtl_hv_iter_init(hv);
   while ( (sv = (dtl_sv_t*) dtl_hv_iter_next(hv, &key)) )
   {
      sv = (dtl_sv_t*) dtl_hv_get(hv, key);
      printf("%s: %s\n", key, dtl_sv_to_cstr(sv));
   }
   dtl_dec_ref(hv); //deletes hv
   //reference count for sv1,sv2 and sv3 is now 1

   /*** cleanup ***/
   dtl_dec_ref(sv1); //deletes sv1 (reference count -> 0)
   dtl_dec_ref(sv2); //deletes sv2 (reference count -> 0)
   dtl_dec_ref(sv3); //deletes sv3 (reference count -> 0)

   return 0;
}
