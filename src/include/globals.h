#ifndef GLOBALS_H
#define GLOBALS_H
extern int is_interactive;

#ifndef RS_DEBUG
#if defined(VM_DEBUG) || defined(PARSE_DEBUG)
#define RS_DEBUG
#endif
#endif // RS_DEBUG

#ifdef RS_DEBUG
#define R_DEBUG(v, ...) printf(v, ##__VA_ARGS__)
#else
#define R_DEBUG(v, ...) 
#endif // RS_DEBUG

#ifndef R_STD_ERR
#define R_STD_ERR stderr
#endif // R_SRD_ERR


#define R_FATAL(v, ...) fprintf(R_STD_ERR, v, ##__VA_ARGS__) 


#endif // GLOBALS_H
