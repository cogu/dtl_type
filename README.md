# dtl_type library

A Dynamically Typed Language (or DTL) is a generic term for a modern scripting language (such as Python, Perl or Javascript).
Such languages usually contains many parts:

* Language syntax
* Compiler
* Runtime Interpreter (or Virtual Machine)
  * Type system
* Language Libraries

In dtl_type I have implemnted only the type system without any of the other language components. It is inspired by the Perl type system and uses similar terminology.

Using this library you can easily build arbitrarily complex data structures in runtime using the C programming language.
All values are reference counted which significantly simplifies memory management. The initial reference count of newly created values is 1.

In some programming languages (or libraries) you might have something called "Variant". This is a similar idea to what this library is trying to achieve.

## Dynamic Value (DV)

This is the base class which all other value types inherits from. (This is sort of a pseudo-statement since C have neither classes or inheritance).

A dynamic value can be any of the following types:

* Scalar Value (SV)
* Array Value (AV)
* Hash Value (HV)

## Scalar Values (SV)

A scalar contains a single unit of data.

Example of scalar types:

* Integer
* Double
* String
* Boolean
* NoneType (this name is actually borrowed from Python)

## Array Values (AV)

Array values are managed arrays containing dynamic values (DVs).

Examples:

* Array of scalar values
* Array of array values
* Array of hash values
* Array of mixed values (any of the above)


## Hash Values (HV)

Hash values are key-value lookup tables where the key is a string and the value is any dynamic value (DV).

# Dependencies

* [cogu/adt](https://github.com/cogu/adt)

This repo depends on some of my other repos. The unit test project(s) assumes that repos are cloned (separately) into a common directory.

* adt
* dtl_type (this repo)

# Usage

``` C
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
   dtl_hv_set_cstr(hv, "first", (dtl_dv_t*) sv1, true);
   dtl_hv_set_cstr(hv, "second",(dtl_dv_t*) sv2, true);
   dtl_hv_set_cstr(hv, "third", (dtl_dv_t*) sv3, true);
   //reference count for sv1,sv2 and sv3 is now 2

   dtl_hv_iter_init(hv);
   while ( (sv = (dtl_sv_t*) dtl_hv_iter_next_cstr(hv, &key)) )
   {
      sv = (dtl_sv_t*) dtl_hv_get_cstr(hv, key);
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
```









