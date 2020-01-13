// kw : EXIT_SUCCESS, macros, __FILE__, __LINE__, argv[0], std::cerr, std::flush, #error, assert, ASSERT, __cplusplus

#include <iomanip>

# if ! defined(  __UTILITES_HXX__ )
# define __UTILITES_HXX__


# include <cstdlib>


# if defined( ICI )
# error ICI est deja definie
# endif
# if defined( MESSAGE )
# error MESSAGE est deja definie
# endif
# if defined( DEBUT_DE )
# error DEBUT_DE est deja definie
# endif
# if defined( FIN_DE )
# error FIN_DE est deja definie
# endif
# if defined( SCRUTE )
# error SCRUTE est deja definie
# endif
# if defined( INFO_COMPILATION )
# error INFO_COMPILATION est deja definie
# endif

# define HEREWEARE    { std::cout<< std::flush ; std::cerr << __FILE__ << " [" << __LINE__ << "] : " << std::flush ; }
# define INFOS(chain) {HEREWEARE ; std::cerr << std::setprecision(10) << chain << std::endl ;}

# if defined( _DEBUG_ )

// la macro __cplusplus est un mot-cle standard du compilateur C++

# ifdef __cplusplus

// XXX : ON NE DOIT PAS FAIRE DE "using namespace dans un .hxx"
// using namespace std ;

// On vide le buffer de la sortie standard (std::cout << std::flush) avant d'ecrire sur
// la sortie des erreurs
# define ICI			{ std::cout << std::flush ; std::cerr << __FILE__ << " [" << __LINE__ << "] : " << std::flush ; }
# define MESSAGE(chain) {HEREWEARE ; std::cerr << chain << std::endl ;}
//# define MESSAGE(chaine)	{ ICI ; std::cerr << (chaine) << std::endl ;}
# define DEBUT_DE(fonction)	{ ICI ; std::cerr << "DEBUT de " << (fonction) << std::endl ;}
# define FIN_DE(fonction)	{ ICI ; std::cerr << "FIN de " << (fonction) << std::endl ;}
# define INFO_COMPILATION	{ ICI ; std::cerr << "Date de la compilation : " << __DATE__ << " a " << __TIME__ << std::endl ;}
# define SCRUTE(var)		{ ICI ; std::cerr << #var << "=" << std::flush << (var) << std::endl ;}

// on peut aussi utiliser assert( condition ), il faut alors inclure assert.h (# include <assert.h>)
# define ASSERT(condition)	{ if( !(condition) )\
					{\
						ICI ;\
						std::cerr << "Erreur de programmation, la condition : " ;\
						std::cerr << #condition  ;\
						std::cerr << " n'est pas respectee" ;\
						std::cerr << std::endl  ;\
						std::exit(1) ;\
					}\
				}
# define NON_DISPONIBLE(methode) {\
					ICI ;\
					std::cerr << "Methode " << std::flush ;\
					std::cerr << methode << std::flush ;\
					std::cerr << " PAS ENCORE PROGRAMMEE !!!" << std::endl ;\
					exit(EXIT_FAILURE) ;\
				 }

# else		/* # ifdef __cplusplus */
# define ICI	{ fflush(stdout) ; fprintf( stderr , "%s [%d] : " , __FILE__, __LINE__ ) ; }
# endif		/* # ifdef __cplusplus */

# else		/* # if defined( _DEBUG_ ) */

# define ICI
# define MESSAGE(chaine)
# define DEBUT_DE(fonction)
# define FIN_DE(fonction)
# define INFO_COMPILATION
# define SCRUTE(var)
# define ASSERT(condition)
# define NON_DISPONIBLE(methode)

# endif		/* # if defined( _DEBUG_ ) */

# endif		/* # if ! defined(  __UTILITES_HXX__ ) */
