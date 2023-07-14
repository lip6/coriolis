
#ifndef  __HMETIS_H__
#define  __HMETIS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void HMETIS_PartRecursive( int nvtxs, int nhedges, int *vwgts, int *eptr
                         , int *eind, int *hewgts, int nparts, int ubfactor
                         , int *options, int *part, int *edgecut );

void HMETIS_PartKway( int nvtxs, int nhedges, int *vwgts
                    , int *eptr, int *eind, int *hewgts, int nparts
                    , int ubfactor, int *options, int *part, int *edgecut);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __HMETIS_H__
