/****************************************************************************/
/*                       Template column_and_prech                          */
/*                       behavioral actions                                 */
/*                                                                          */
/****************************************************************************/
void column_and_prech()
{
  char      *   nq = _equiv("nq");
  char      *    q = _equiv("q");
  char      *   ck = _equiv("ck");
  
  begCreateInterface();
  
  // precharge
  begAddMemDriver(nq  ,ck  ,"'1'" ,0  ,NULL);
  begAddMemDriver(q   ,ck  ,"'1'" ,0  ,NULL);
  
  begAddAllInstanceModels();

  begKeepModel();
}
