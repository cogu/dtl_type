#include <stdio.h>
#include "dtl_type.h"
#include "adt_str.h"

int main(int argc, char **argv){
  dtl_sv_t *sv;

  /*** Scalar values ***/
  dtl_sv_t *sv1 = dtl_sv_make_i32(125);
  dtl_sv_t *sv2 = dtl_sv_make_cstr("Hello World");
  dtl_sv_t *sv3 = dtl_sv_make_dbl(10.2);

  printf("%d\n",dtl_sv_get_i32(sv1));
  printf("%s\n",dtl_sv_get_cstr(sv2));
  printf("%f\n",dtl_sv_get_dbl(sv3));
  printf("\n");


  /*** Array values ***/
  dtl_av_t *av = dtl_av_new();
  dtl_av_push(av,(dtl_dv_t*) sv1),dtl_ref_inc(sv1);
  dtl_av_push(av,(dtl_dv_t*) sv2),dtl_ref_inc(sv2);
  dtl_av_push(av,(dtl_dv_t*) sv3),dtl_ref_inc(sv3);
  //reference count for sv1,sv2 and sv3 is now 2

  int i;
  adt_str_t *str = adt_str_new();
  for(i=0;i<dtl_av_len(av);i++){
	  sv = *(dtl_sv_t**) dtl_av_get(av,i);
	  dtl_sv_to_string(sv,str);
	  printf("%s\n",adt_str_cstr(str));
  }
  adt_str_delete(str);
  dtl_ref_dec(av); //deletes av
  //reference count for sv1,sv2 and sv3 is now 1
  printf("\n");

  const char *key;
  uint32_t u32KeyLen;


  /*** Hash values ***/
  dtl_hv_t *hv = dtl_hv_new();
  dtl_hv_set(hv,"first",0,(dtl_dv_t*) sv1),dtl_ref_inc(sv1);
  dtl_hv_set(hv,"second",0,(dtl_dv_t*) sv2),dtl_ref_inc(sv2);
  dtl_hv_set(hv,"third",0,(dtl_dv_t*) sv3),dtl_ref_inc(sv3);
  //reference count for sv1,sv2 and sv3 is now 2

  dtl_hv_iter_init(hv);
  str = adt_str_new();
  while( (sv = (dtl_sv_t*) dtl_hv_iter_next(hv,&key,&u32KeyLen)) ){
	  dtl_sv_to_string(sv,str);
	  printf("%s: %s\n",key,adt_str_cstr(str));
  }
  adt_str_delete(str);
  dtl_ref_dec(hv); //deletes hv
  //reference count for sv1,sv2 and sv3 is now 1

  /*** cleanup ***/
  dtl_ref_dec(sv1); //deletes sv1 (reference count -> 0)
  dtl_ref_dec(sv2); //deletes sv2 (reference count -> 0)
  dtl_ref_dec(sv3); //deletes sv3 (reference count -> 0)

  return 0;
}
