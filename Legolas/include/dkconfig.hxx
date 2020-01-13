/* fichier G36749@clau5ddp:dkconfig.hxx */
/* date : 2013-04-03 17:39:12 */
/* adresse IP : 130.98.90.55 */
/* user : G36749 (62002) */
/* configuration : --with-nbproc=8 --with-myprec=double --with-optim=opt3 --enable-x86timer */
#if ! defined( __dkconfig_hxx__ )
#define __dkconfig_hxx__

#define CONFIGURESTRING "--with-nbproc=8 --with-myprec=double --with-optim=opt3 --enable-x86timer"

typedef double DkReal		/* --with-myprec=double */;

#undef USING_MEMORY
#define USING_TBB 1
#define USING_X86_TIMER 1	/* --enable-x86timer */
#define HAS_PROC 1

#define sizeof_size_t 8
#define sizeof_unsigned_int 4
/* #define H5_USE_16_API */ /* IDM: portage HDF 1.8.8 */
#endif	/* #if ! defined( __dkconfig_hxx__ )*/
