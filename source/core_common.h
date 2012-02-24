#ifndef SBOL_CORE_COMMON
#define SBOL_CORE_COMMON

# ifndef SBOLAPIEXPORTS
#  if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#    if defined(STATIC_LINKED)
#          define SBOLAPIEXPORTS
#    else
#   if defined(SBOL_EXPORTS)
#              if defined(USE_STDCALL)
#                   define SBOLAPIEXPORTS __stdcall __declspec(dllexport)
#              else
#                   define SBOLAPIEXPORTS __declspec(dllexport)
#              endif
#          else
#              define SBOLAPIEXPORTS
#          endif
#     endif
#  else
#    if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#      define SBOLAPIEXPORTS __attribute__ ((visibility("default")))
#    else
#      define SBOLAPIEXPORTS
#    endif
#  endif
# endif

struct _DNASequence;
struct _SequenceAnnotation;
struct _DNAComponent;
struct _Collection;

// TODO these should return the new objects
SBOLAPIEXPORTS void createDNASequence(const char* id);
SBOLAPIEXPORTS void createSequenceAnnotation(const char* id);
SBOLAPIEXPORTS void createDNAComponent(const char* id);
SBOLAPIEXPORTS void createCollection(const char* id);

// TODO registerDNASequence?
void registerSequenceAnnotation(_SequenceAnnotation* ann);
void registerComponent(_DNAComponent* com);
void registerCollection(_Collection* col);

SBOLAPIEXPORTS void deleteDNASeqeuence(_DNASequence* seq);
SBOLAPIEXPORTS void deleteSequenceAnnotation(_SequenceAnnotation* ann);
SBOLAPIEXPORTS void deleteDNAComponent(_DNAComponent* com);
SBOLAPIEXPORTS void deleteCollection(_Collection* col);

void cleanup();

#endif
