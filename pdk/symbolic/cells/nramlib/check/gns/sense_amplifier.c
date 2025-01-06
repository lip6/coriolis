/****************************************************************************/
/*                       Template sense_amplifier                           */
/*                       behavioral actions                                 */
/*                                                                          */
/****************************************************************************/
void sense_amplifier()
{
  char      *ndata = _equiv("ndata");
  char      * data = _equiv("data");
  char      *  cke = _equiv("cke");
  char      *    X = _equiv("X");
  char      *  ckp = _equiv("ckp");
  char        command[1024];

  
  begCreateInterface();

  sprintf(command,"%s and %s and not(%s)",cke,data,ndata);
  begAddMemDriver(X, command, "'1'", 0, NULL);
  
  sprintf(command,"%s and not(%s) and %s",cke,data,ndata);
  begAddMemDriver(X, command, "'0'", 0, NULL);

  begAddMemDriver(ndata, ckp, "'1'", 0, NULL);
  begAddMemDriver(data,  ckp, "'1'", 0, NULL);

  begKeepModel();
}
