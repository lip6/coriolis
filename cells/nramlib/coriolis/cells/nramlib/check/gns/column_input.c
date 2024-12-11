/****************************************************************************/
/*                       Template Column_input                              */
/*                       behavioral actions                                 */
/*                                                                          */
/****************************************************************************/
void column_input()
{
  char      *   nq = _equiv("nq");
  char      *    q = _equiv("q");
  char      *  wen = _equiv("wen");
  char      *  din = _equiv("datain");
  char      *  ckp = _equiv("ckp");
  char         data[1024];

  
  begCreateInterface();
  
  begAddMemDriver(q,  wen, din,   0, NULL);
  
  sprintf(data,"not(%s)",din);
  begAddMemDriver(nq, wen, data,  0, NULL);
  
  // precharge
  begAddMemDriver(q,  ckp, "'1'", 0, NULL);
  begAddMemDriver(nq, ckp, "'1'", 0, NULL);

  begKeepModel();
}
