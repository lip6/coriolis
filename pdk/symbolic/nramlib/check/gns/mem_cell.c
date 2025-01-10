/****************************************************************************/
/*                       Template mem_cell                                  */
/*                       behavioral actions                                 */
/*                                                                          */
/*  This action describe the behavior of a mem_cell                         */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void mem_cell()
{
  char *           mem = _equiv("mem");
  char *          nmem = _equiv("nmem");
  char *             q = _equiv("q");
  char *            nq = _equiv("nq");
  char *           com = _equiv("com");
  char * din_mem_delay = _equiv("din_mem_delay");
  char *com_dout_delay = _equiv("com_dout_delay");
  char command[1024];

  begCreateInterface ();

  // IF (com AND (q AND NOT nq))
  //   mem <= '1' AFTER din_mem_delay;
  // IF (com AND (NOT q AND nq))
  //   mem <= '0' AFTER din_mem_delay;
  sprintf(command,"%s and (%s and not %s)", com, q, nq);
  begAddMemDriver(mem , command, "'1'", 0, din_mem_delay);
  sprintf(command,"%s and (not %s and %s)", com, q, nq);
  begAddMemElse  (mem , command, "'0'", 0, din_mem_delay);

  // IF (com AND com'EVENT)
  //   IF (mem)
  //     q <= '1' AFTER com_dout_delay;
  //   ELSIF (NOT mem)
  //     q <= '0'
  sprintf(command,"%s and %s and %s'event", com, mem, com);
  begAddMemDriver(q   , command, "'1'", 0, com_dout_delay);
  sprintf(command,"%s and not %s and %s'event", com, mem, com);
  begAddMemElse  (q   , command, "'0'", 0, com_dout_delay);

  // IF (com AND com'EVENT)
  //   IF (mem)
  //     nq <= '0' AFTER com_dout_delay;
  //   ELSIF (NOT mem)
  //     nq <= '1'
  sprintf(command,"%s and %s and %s'event", com, mem, com);
  begAddMemDriver(nq  ,command , "'0'", 0, com_dout_delay);
  sprintf(command,"%s and not %s and %s'event", com, mem, com);
  begAddMemElse  (nq  , command, "'1'", 0, com_dout_delay);

  begKeepModel   ();
}

void mem_cell_noevent()
{
  char *           mem = _equiv("mem");
  char *          nmem = _equiv("nmem");
  char *             q = _equiv("q");
  char *            nq = _equiv("nq");
  char *           com = _equiv("com");
  char * din_mem_delay = _equiv("din_mem_delay");
  char *com_dout_delay = _equiv("com_dout_delay");
  char command[1024];

  begCreateInterface ();

  // IF (com AND (q AND NOT nq))
  //   mem <= '1' AFTER din_mem_delay;
  // IF (com AND (NOT q AND nq))
  //   mem <= '0' AFTER din_mem_delay;
  sprintf(command,"%s and (%s and not %s)", com, q, nq);
  begAddMemDriver(mem , command, "'1'", 0, din_mem_delay);
  sprintf(command,"%s and (not %s and %s)", com, q, nq);
  begAddMemElse  (mem , command, "'0'", 0, din_mem_delay);

  // IF (com AND com'EVENT)
  //   IF (mem)
  //     q <= '1' AFTER com_dout_delay;
  //   ELSIF (NOT mem)
  //     q <= '0'
  sprintf(command,"%s and %s", com, mem);
  begAddMemDriver(q   , command, "'1'", 0, com_dout_delay);
  sprintf(command,"%s and not %s", com, mem);
  begAddMemElse  (q   , command, "'0'", 0, com_dout_delay);

  // IF (com AND com'EVENT)
  //   IF (mem)
  //     nq <= '0' AFTER com_dout_delay;
  //   ELSIF (NOT mem)
  //     nq <= '1'
  sprintf(command,"%s and %s", com, mem);
  begAddMemDriver(nq  ,command , "'0'", 0, com_dout_delay);
  sprintf(command,"%s and not %s", com, mem);
  begAddMemElse  (nq  , command, "'1'", 0, com_dout_delay);

  begKeepModel   ();
}
