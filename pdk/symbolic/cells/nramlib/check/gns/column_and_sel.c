/****************************************************************************/
/*                       Template column_and_sel                            */
/*                       behavioral actions                                 */
/*                                                                          */
/****************************************************************************/
void column_and_sel()
{
  char      *   nq = _equiv("nq");
  char      *    q = _equiv("q");
  char      *   na = _equiv("na");
  char      *    a = _equiv("a");
  char      *   ck = _equiv("ck");
  char      *  sel = _equiv("sel");
  
  begCreateInterface();
  
  // pass-transistor
  begAddMemDriver(nq, sel, na, 0, NULL);
  begAddMemDriver(q,  sel, a,  0, NULL);
  begAddMemDriver(na, sel, nq, 0, NULL);
  begAddMemDriver(a,  sel, q,  0, NULL);
 
  // precharge
  begAddMemDriver(nq, ck, "'1'", 0, NULL);
  begAddMemDriver(q,  ck, "'1'", 0, NULL);
  
  begAddAllInstanceModels();

  begKeepModel();
}
