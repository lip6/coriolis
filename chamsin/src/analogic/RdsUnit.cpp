// ****************************************************************************************************
// File: RdsUnit.cpp
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************


#include "Error.h"
#include "stdio.h"
#include "regex.h"
#include "math.h"
#include "Commons.h"
#include "RdsUnit.h"




// ****************************************************************************************************
// Static data&function 
// ****************************************************************************************************

static long RDS_UNIT = -1;

static long RDS_PHYSICAL_GRID = 2;

static long RDS_LAMBDA = -1;



// -----------------------------------------------------------------------
// Function  :  "GetPattern()".
 
  /*  \static char * GetPattern (const string& str, const char* pattern)
   *  \param  str      the string to check for regex.
   *  \param  pattern  the pattern to find.
   *  
   *  Get and return the march substring from str according to the pattern.
   *
   *  \return NULL if Failure.
   *  \return the according substring if Success.
   */
 
static char* GetPattern(const string& str, const char* pattern)
// ************************************************************
{
    int err;
    regex_t preg;
    const char *str_request = str.c_str();
    const char *str_regex = pattern;

/* (1) */
    err = regcomp (&preg, str_regex, REG_EXTENDED);

    if (err == 0)
    {
       int match;
       size_t nmatch = 0;
       regmatch_t *pmatch = NULL;
       
       nmatch= 1;
       pmatch = (regmatch_t*)malloc (sizeof (*pmatch) * nmatch);

       if (pmatch)
       {
/* (2) */
          match = regexec (&preg, str_request, nmatch, pmatch, 0);

/* (3) */
          regfree (&preg);
 
/* (4) */
          if (match == 0)
          {
             char *site = NULL;
             int start = pmatch[0].rm_so;
             int end = pmatch[0].rm_eo;
             size_t size = end - start;
 
             site = (char*)malloc (sizeof (*site) * (size + 1));
             if (site)
             {
                strncpy (site, &str_request[start], size);
                site[size] = '\0';

		return site; 
             }
	     else {
                fprintf (stderr, "Memoire insuffisante\n");
                exit (EXIT_FAILURE);
	     }
          }
 /* (5) */
          else if (match == REG_NOMATCH)
          {
             printf ("%s doesn't accord to %s\n", str_request, str_regex);
	     return NULL;
          }
 /* (6) */
          else
          {
             char *text;
             size_t size;
 
 /* (7) */
             size = regerror (err, &preg, NULL, 0);
             text = (char*)malloc (sizeof (*text) * size);
             if (text)
             {
 /* (8) */
                regerror (err, &preg, text, size);
                fprintf (stderr, "%s\n", text);
                free (text);
             }
             else
             {
                fprintf (stderr, "Memoire insuffisante\n");
             }
             exit (EXIT_FAILURE);
          }
       }
       else
       {
          fprintf (stderr, "Memoire insuffisante\n");
          exit (EXIT_FAILURE);
       }
    }
    else {
          fprintf (stderr, "Regcomp fail\n");
          exit (EXIT_FAILURE);
    }

}  



// -------------------------------------------------------------------
// Function  :  "CalculateRdsUnit()".

  /* static void CalculateRdsUnit()
   *   
   * Open rds techno file who's name is MACRO RDS_TECHNO_FILE,
   * and get values technology : rds_physical_grid, rds_lambda.   
   * Calculate the RDS_UNIT according to these values. 
   *
   * If RDS_TECHNO_FILE is invalid or operaton of reading values technologys 
   * fail, a Hurricane Error will be throwed to explain the reason of failure.  
   */


static void CalculateRdsUnit()
// ***************************
{
  using Hurricane::Error;

  const char * rdsfilename = getenv("RDS_TECHNO_NAME");
  FILE * rdstechnofile;

  if(!rdsfilename) {
      throw Error("Can't not find macro RDS_TECHNO_FILE");
  }

  if( !(rdstechnofile = fopen(rdsfilename, "r")) ) {
      throw Error("Can't not open rds techno file : " + GetString(rdsfilename)); 
  }

  char buffer[80];                       // For stock a line of the rds techno file
  double rds_physical_grid = -0.1;       // For stock value of physical_grid
  double rds_lambda = -0.1;              // For stock value of lamba 

  while(fgets(buffer, 80, rdstechnofile )!=NULL){
    string tmp = buffer; 

    if( tmp.find("define physical_grid")!=string::npos) {  // Find the ligne begin with "define physical_grid"

	string pattern;

        if( (pattern = GetPattern(tmp, "[[:digit:]\\.]+")).size()==0 ) { // Get the value behind 
	  throw Error("Can't get rds_physical_grid : GetPattern(string&, char*) return NULL");
	} 

	if(!Scan(pattern, rds_physical_grid)){  // Convert from string to double
           throw Error("Can't get rds_physical_grid " + pattern + " : Hurricane::Scan(string&, double&) return false");
	}

    }

    if( tmp.find("define lambda")!=string::npos ) {

        string pattern; 

        if( (pattern = GetPattern(tmp, "[[:digit:]\\.]+")).size()==0 ) {
           throw Error("Can't get rds_lambda : GetPattern(string&, char*) return NULL");	
	}

	if(!Scan(pattern, rds_lambda)){
           throw Error("Can't get rds_lambda " + pattern + " : Hurricane::Scan(string&, double&) return false");
	}
    
    }

    // if rds_physical_grid and rds_lambda are finded, break the loop 
    // **************************************************************
    if( (rds_physical_grid!=-0.1) && (rds_lambda!=-0.1) )
       break;

  } // end of while 

  if( (rds_physical_grid==-0.1) && (rds_lambda==-0.1) ) {
     throw Error("Can't get rds_physical_grid and rds_lambda from rds techno file : \n" + string(rdsfilename));  
  }

  long rds_physical_grid_nano = (long)rint(rds_physical_grid*(double)1000);

  rds_physical_grid_nano = (long)rint( (double)1000/(double)rds_physical_grid_nano );

  RDS_UNIT = rds_physical_grid_nano << 1;

  long rds_lambda_nano = (long)rint( rds_lambda*(double)1000 );

  RDS_LAMBDA =  rds_lambda_nano * RDS_UNIT / 1000 ; 

  fclose(rdstechnofile);

}

BEGIN_NAMESPACE_HURRICANE




// ****************************************************************************************************
// Utilitarians 
// ****************************************************************************************************

const long& GetRdsUnit() 
// *********************
{
  if ( RDS_UNIT == -1) 
    CalculateRdsUnit();

  return RDS_UNIT;  
}

const long& GetRdsPhysicalGrid()
// *****************************
{
  return RDS_PHYSICAL_GRID;
}
  
const long& GetRdsLambda()
// ***********************
{
  if ( RDS_LAMBDA == -1) 
    CalculateRdsUnit();

  return RDS_LAMBDA;  
}  


END_NAMESPACE_HURRICANE
