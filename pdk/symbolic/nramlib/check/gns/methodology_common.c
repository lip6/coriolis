/****************************************************************************/
/*                 Template for use of progammed timing models              */
/*                       METHODOLOGY FUNCTIONS                              */
/*                                                                          */
/****************************************************************************/

int METHODOLOGY_ENABLE_ROUNDING=0;
int METHODOLOGY_TRACE=0;
double METHODOLOGY_SLOPE_ROUNDING=5e-12;
double METHODOLOGY_CAPA_ROUNDING=1e-15;

/****************************************************************************/
/*                 Table based cache with interpolation to                  */
/*                 reduce computations                                      */
/****************************************************************************/

void METHODOLOGY_CommonModel_TABLE()
{
  double threshold = 20.0;
  double input_slope=GET_INPUT_SLOPE(), output_capa=GET_OUTPUT_CAPA();
  
  if (METHODOLOGY_ENABLE_ROUNDING)
    {
      input_slope=stm_arround(input_slope, METHODOLOGY_SLOPE_ROUNDING);
      output_capa=stm_arround(output_capa, METHODOLOGY_CAPA_ROUNDING);
    }

  if (!stm_delayThresholdOK(input_slope, output_capa, threshold)
      && !stm_slopeThresholdOK(input_slope, output_capa, threshold))
    {
      CALL_SIMULATION();
      stm_sup_storeDelay(GET_DELAY());
      stm_sup_storeSlope(GET_SLOPE());
    } 
  else
    {
      SET_DELAY(stm_sup_getDelay());
      SET_SLOPE(stm_sup_getSlope());
      if (METHODOLOGY_TRACE)
        printf("CACHE Request(InputSlope=%g OutputCapa=%g) => delay=%g, slope=%g\n",GET_INPUT_SLOPE(), GET_OUTPUT_CAPA(),GET_DELAY(), GET_SLOPE());
    }
}

/****************************************************************************/
/*                 Simple cache based on stored values                      */
/*                 for previously computed values                           */
/****************************************************************************/

void METHODOLOGY_CommonModel_SIMPLECACHE(char *ID)
{
  double input_slope=GET_INPUT_SLOPE(), output_capa=GET_OUTPUT_CAPA();
  double res_slope, res_delay;
  char bufs[128];
  char bufd[128];

  if (METHODOLOGY_ENABLE_ROUNDING)
    {
      input_slope=stm_arround(input_slope, METHODOLOGY_SLOPE_ROUNDING);
      output_capa=stm_arround(output_capa, METHODOLOGY_CAPA_ROUNDING);
    }

  dtb_Create("METHODOLOGY_SIMPLECACHE");

  sprintf(bufs,"%s%g%g_slope", ID, input_slope, output_capa);
  sprintf(bufd,"%s%g%g_slope", ID, input_slope, output_capa);
  res_slope=dtb_GetDouble("METHODOLOGY_SIMPLECACHE", bufs);
  res_delay=dtb_GetDouble("METHODOLOGY_SIMPLECACHE", bufd);

  if (res_slope!=0.0)
  {
    SET_DELAY(res_delay);
    SET_SLOPE(res_slope);
    if (METHODOLOGY_TRACE)
      printf("CACHE Request(InputSlope=%g OutputCapa=%g) => delay=%g, slope=%g\n",GET_INPUT_SLOPE(), GET_OUTPUT_CAPA(),GET_DELAY(), GET_SLOPE());
  }
  else
  {
    CALL_SIMULATION();
    dtb_SetDouble("METHODOLOGY_SIMPLECACHE", bufs, GET_SLOPE());
    dtb_SetDouble("METHODOLOGY_SIMPLECACHE", bufd, GET_DELAY());
  }
}

