/****************************************************************************/
/*                       Template bl_precharge                              */
/*                       behavioral actions                                 */
/*                                                                          */
/****************************************************************************/
void bl_precharge()
{
  char      *    q = _equiv("q");
  char      *   nq = _equiv("nq");
  char      *   ck = _equiv("ck");
  
  begCreateInterface();
  
  begAddMemDriver(nq, ck, "'1'", 0, NULL);
  begAddMemDriver(q,  ck, "'1'", 0, NULL);
  
  begKeepModel();
}
