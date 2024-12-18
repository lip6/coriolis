#include <genlib.h>
#include <string.h>

char *FNAME = "sram_w2r2";

unsigned NBW;   // number of words
unsigned NBB;   // number of bits
char *PHINS_REF = NULL; // first instance of each column

char *M_NFF = "sram_w2r2nff";
char *M_CFF = "sram_w2r2nffct";
char *M_NOA = "sram_noa2a22";
char *M_COA = "sram_noa2a22ct";
char *M_MCT = "sram_mxct";
char *M_DEC = "sram_w2dec4";
char *M_INV = "sram_w2inv";

#define INAMEV(colname,cell,bit)  str("%s_%s_%d", colname,cell,bit)
#define INAME(colname,cell)       str("%s_%s", colname,cell)

#define DOSYMY   ((++symy)%2)?SYM_Y:NOSYM
#define NOSYMY   ((symy)%2)?SYM_Y:NOSYM
#define XFIRST  'x'
#define YFIRST  'y'

//--------------------------------------------------------------------------------------------------
// General Purpose Functions
//--------------------------------------------------------------------------------------------------

void usage(char *cmd)
{
    fprintf(stderr, "\nUsage: genlib %s <words> <bits>\n\n", cmd);
    fprintf(stderr, "    <words> : nb of words 2,4,8\n");
    fprintf(stderr, "    <bits>  : number of bits 1,4,8\n\n");
    exit(1);
}

char *inttostr(int entier)
{
    char *str = (char *) alloca(32 * sizeof(char));
    sprintf(str, "%d", entier);
    return namealloc(str);
}

char *bin(int n, int size) {
    char *str = alloca(size+1);
    str[size] = 0;
    for (int i = size - 1; i >= 0; i--) {
        str[i] = (n & 1) ? '1' : '0'; 
        n >>= 1; 
    }
    return namealloc(str);    
}

char *str(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char *str = (char *) alloca(256 * sizeof(char));
    vsnprintf(str, 256, fmt, ap);
    va_end(ap);
    return namealloc(str);
}

int in(int val, size_t size, ...)
{
    va_list ap;
    va_start(ap, size);
    int res=0;
    for(; size; size--) {
        int n = va_arg(ap, int);
        if (val == n) 
            res=1;
    }
    va_end(ap);
    return res;
}

//--------------------------------------------------------------------------------------------------
// Route DataPath Data & Addresses
//--------------------------------------------------------------------------------------------------

void PHSEG_WE_DT(char * cell, char * port[], size_t nbp, int nbr)
{
    long x1, y1, x2, y2;
    char *iname1 = str("w%d_w%d_%s", 0, 1, cell);
    char *iname2 = str("w%d_w%d_%s", NBW-2, NBW-1, cell);
    long xmin = GENLIB_GET_REF_X(iname1,"vss0");
    long xmax = GENLIB_GET_REF_X(iname2,"vss1");
    for (int p = 0; p < nbp; p++ ) {                                // foreach input port
       char *ref1 = str("w%s_0", port[p]);
       char *ref2 = str("w%s_1", port[p]);
       char *pname = str("w%s", port[p]);
       y1 = GENLIB_GET_REF_Y(iname1,ref1);
       y2 = GENLIB_GET_REF_Y(iname2,ref2);
       GENLIB_PHSEG(CALU2, 2, pname, xmin, y1, xmax, y2);            // PUT a CALU2 connecting all
    }
}

void PHSEG_CK_DT(char * cell, int nbr)
{
    char *iname1 = str("w%d_w%d_%s", 0, 1, cell);
    char *iname2 = str("w%d_w%d_%s", NBW-2, NBW-1, cell);
    long xmin = GENLIB_GET_REF_X(iname1,"vss0");
    long xmax = GENLIB_GET_REF_X(iname2,"vss1");
    long x, y;
    for (int w = 0; w < NBW ; w += 2) {                     // foreach word
        for (int r = 0; r < nbr ; r++) {                    // foreach ref
            char *iname = str("w%d_w%d_%s", w, w+1, cell);
            char *ref = str("ck%di", r);
            x = GENLIB_GET_REF_X(iname,ref);
            y = GENLIB_GET_REF_Y(iname,ref);
            GENLIB_PHVIA(CONT_VIA, x, y);                 // put a VIA
        }
    }
    GENLIB_PHSEG(CALU2, 2, "ck", xmin, y, xmax, y);      // PUT a CALU2 connecting all
}

void PHSEG_AXAY_DT(char * cell, char * port[], size_t nbp)
{
    char *iname1 = str("w%d_w%d_cff", 0, 1);
    char *iname2 = str("w%d_w%d_cff", NBW-2, NBW-1);
    long xmin = GENLIB_GET_REF_X(iname1,"vss0");
    long xmax = GENLIB_GET_REF_X(iname2,"vss1");
    for (int p = 0; p < nbp; p++ ) {                                // foreach input port
        char *iname = str("w%d_w%d_%s", 0, 1, cell);
        char *ref1 = str("a%s10", port[p]);
        char *ref2 = str("a%s20", port[p]);
        long y1 = GENLIB_GET_REF_Y(iname,ref1);
        long y2 = GENLIB_GET_REF_Y(iname,ref2);
        char * pname1 = str("a%s[1]", port[p]);
        char * pname2 = str("a%s[2]", port[p]);
        GENLIB_PHSEG(CALU2, 2, pname1, xmin, y1, xmax, y1);      // PUT a CALU2 connecting all
        GENLIB_PHSEG(CALU2, 2, pname2, xmin, y2, xmax, y2);      // PUT a CALU2 connecting all
    }
}

void PHSEG_AX0AY0_DT(char * cell, char * port[], size_t nbp)
{
    char *iname1 = str("w%d_w%d_%s", 0, 1, cell);
    char *iname2 = str("w%d_w%d_%s", NBW-2, NBW-1, cell);
    long xmin = GENLIB_GET_REF_X(iname1,"vss0");
    long xmax = GENLIB_GET_REF_X(iname2,"vss1");
    long x0, y0, x1, y1;
    for (int p = 0; p < nbp; p++ ) {                                // foreach input port
        for (int w = 0; w < NBW ; w += 2) {                     // foreach word
            char *iname = str("w%d_w%d_%s", w, w+1, cell);
            char *ref1 = str("a%s1", port[p]); 
            char *ref0 = str("a%s0", port[p]);
            x0 = GENLIB_GET_REF_X(iname,ref0);
            y0 = GENLIB_GET_REF_Y(iname,ref0);
            x1 = GENLIB_GET_REF_X(iname,ref1);
            y1 = GENLIB_GET_REF_Y(iname,ref1);
            GENLIB_PHVIA(CONT_VIA2, x0, y0);                 // put a VIA
            GENLIB_PHVIA(CONT_VIA2, x1, y1);                 // put a VIA
            GENLIB_PHVIA(CONT_VIA,  x1, y1);                 // put a VIA
            GENLIB_PHSEG(ALU3, 2, "", x0, y0, x1, y1);
        }
        char * pname = str("a%s[0]", port[p]);
        GENLIB_PHSEG(CALU2, 2, pname, xmin, y0, xmax, y0);      // PUT a CALU2 connecting all
    }
}

void PHSEG_AZ0AT0_DT(char * cell, char * port[], size_t nbp, int nbr)
{
    char *iname1 = str("w%d_w%d_cff", 0, 1, cell);
    char *iname2 = str("w%d_w%d_cff", NBW-2, NBW-1, cell);
    long xmin = GENLIB_GET_REF_X(iname1,"vss0");
    long xmax = GENLIB_GET_REF_X(iname2,"vss1");
    long x, y;
    for (int p = 0; p < nbp; p++ ) {                                // foreach input port
        for (int w = 0; w < NBW ; w += 2) {                     // foreach word
            for (int r = 0; r < nbr ; r++) {                    // foreach ref
                char *iname = str("w%d_w%d_%s", w, w+1, cell);
                char *ref = str("a%s_%d0", port[p], r);
                x = GENLIB_GET_REF_X(iname,ref);
                y = GENLIB_GET_REF_Y(iname,ref);
                GENLIB_PHVIA(CONT_VIA2, x, y);                 // put a VIA
                GENLIB_PHVIA(CONT_VIA,  x, y);                 // put a VIA
            }
        }
        char * pname = str("a%s[0]", port[p]);
        GENLIB_PHSEG(CALU2, 2, pname, xmin, y, xmax, y);      // PUT a CALU2 connecting all
    }
}

void PHSEG_AZ1AT1_DT(char * cell, char * port[], size_t nbp, int nbr)
{
    char *iname1 = str("w%d_w%d_cff", 0, 1, cell);
    char *iname2 = str("w%d_w%d_cff", NBW-2, NBW-1, cell);
    long xmin = GENLIB_GET_REF_X(iname1,"vss0");
    long xmax = GENLIB_GET_REF_X(iname2,"vss1");
    long x, y;
    for (int p = 0; p < nbp; p++ ) {                                // foreach input port
        char *iname, *refi, *refq;
        long y12i, x23i, y23i, x23q, y23q, x67i, y67i, x67q, y67q;
        for (int w = 0; w < NBW ; w += 2) {                     // foreach word
            for (int r = 0; r < nbr ; r++) {                    // foreach ref
                char *iname = str("w%d_w%d_dec", w, w+1);
                char *ref = str("a%s_%d1", port[p], r);
                x = GENLIB_GET_REF_X(iname,ref);
                y = GENLIB_GET_REF_Y(iname,ref);
                GENLIB_PHVIA(CONT_VIA,  x, y);                 // put a VIA
            }
        }
        if (NBW == 2) {
            iname = str("w0_w1_inv");
            refi = str("i0_%d",((strcmp(port[p],"z"))==0)?3:2);
            y12i = GENLIB_GET_REF_Y(iname, refi);
        }
        if (NBW > 2) {
            iname = str("w2_w3_inv");
            if (strcmp(port[p],"z")==0) {
                refi = "i0_3";
                refq = "nq0_3";
            } else {
                refi = "i1_2";
                refq = "nq1_2";
            }
            x23i = GENLIB_GET_REF_X(iname, refi);
            y23i = GENLIB_GET_REF_Y(iname, refi);
            x23q = GENLIB_GET_REF_X(iname, refq);
            y23q = GENLIB_GET_REF_Y(iname, refq);
            GENLIB_PHVIA(CONT_VIA,  x23i, y23i);
            GENLIB_PHVIA(CONT_VIA,  x23q, y23q);
        }  
        if (NBW > 4) {
            iname = str("w6_w7_inv");
            if (strcmp(port[p],"z")==0) {
                refi = "i0_3";
                refq = "nq0_3";
            } else {
                refi = "i1_2";
                refq = "nq1_2";
            }
            x67i = GENLIB_GET_REF_X(iname, refi);
            y67i = GENLIB_GET_REF_Y(iname, refi);
            x67q = GENLIB_GET_REF_X(iname, refq);
            y67q = GENLIB_GET_REF_Y(iname, refq);
            GENLIB_PHVIA(CONT_VIA,  x67i, y67i);
            GENLIB_PHVIA(CONT_VIA,  x67q, y67q);
        } 

        char * pname = str("a%s[1]", port[p]);
        switch (NBW) {
        case 2:   
            GENLIB_PHSEG(CALU2, 2, pname, xmin, y12i, xmax, y12i);
            break;
        case 4:
            GENLIB_PHSEG(CALU2, 2, pname, xmax, y23i, x23i, y23i);
            GENLIB_PHSEG(ALU2, 2, "", x23q, y23q, xmin, y23q);
            break;
        case 8:
            GENLIB_PHSEG(ALU2, 2, "", xmin, y23i, x23q, y23i);
            GENLIB_PHSEG(ALU2, 2, "", x23i, y23q, x67q, y23q);
            GENLIB_PHSEG(CALU2, 2, pname, x67i, y23q, xmax, y23q);
            break;
        }
    }
}

void PHSEG_AZ2AT2_DT(char * cell, char * port[], size_t nbp, int nbr)
{
    char *iname1 = str("w%d_w%d_cff", 0, 1, cell);
    char *iname2 = str("w%d_w%d_cff", NBW-2, NBW-1, cell);
    long xmin = GENLIB_GET_REF_X(iname1,"vss0");
    long xmax = GENLIB_GET_REF_X(iname2,"vss1");
    long x, y;
    for (int p = 0; p < nbp; p++ ) {                                // foreach input port
        char *iname, *refi, *refq;
        long y12i, x45i, y45i, x45q, y45q;
        for (int w = 0; w < NBW ; w += 2) {                     // foreach word
            for (int r = 0; r < nbr ; r++) {                    // foreach ref
                char *iname = str("w%d_w%d_dec", w, w+1);
                char *ref = str("a%s_%d2", port[p], r);
                x = GENLIB_GET_REF_X(iname,ref);
                y = GENLIB_GET_REF_Y(iname,ref);
                GENLIB_PHVIA(CONT_VIA,  x, y);                 // put a VIA
            }
        }
        if (NBW < 8) {
            iname = str("w0_w1_inv");
            refi = str("i0_%d",((strcmp(port[p],"z"))==0)?1:0);
            y12i = GENLIB_GET_REF_Y(iname, refi);
        }
        if (NBW > 4) {
            iname = str("w4_w5_inv");
            if (strcmp(port[p],"z")==0) {
                refi = "i0_1";
                refq = "nq0_1";
            } else {
                refi = "i1_0";
                refq = "nq1_0";
            }
            x45i = GENLIB_GET_REF_X(iname, refi);
            y45i = GENLIB_GET_REF_Y(iname, refi);
            x45q = GENLIB_GET_REF_X(iname, refq);
            y45q = GENLIB_GET_REF_Y(iname, refq);
            GENLIB_PHVIA(CONT_VIA,  x45i, y45i);
            GENLIB_PHVIA(CONT_VIA,  x45q, y45q);
        }  
        char * pname = str("a%s[2]", port[p]);
        if (NBW < 8) {
            GENLIB_PHSEG(CALU2, 2, pname, xmin, y12i, xmax, y12i);
        } else {
            GENLIB_PHSEG(ALU2, 2, "", xmin, y45q, x45q, y45q);
            GENLIB_PHSEG(CALU2, 2, pname, x45i, y45i, xmax, y45i);
        }
    }
}

void PHSEG_IN_DT(char * cell, char * port[], size_t nbp, int nbr)
{
    char *iname1 = str("w%d_w%d_%s_0", 0, 1, cell);
    char *iname2 = str("w%d_w%d_%s_0", NBW-2, NBW-1, cell);
    long xmin = GENLIB_GET_REF_X(iname1,"vss0");
    long xmax = GENLIB_GET_REF_X(iname2,"vss1");
    long x, y;
    for (int p = 0; p < nbp; p++ ) {                                // foreach input port
        for (int b = 0; b < NBB ; b++) {                            // foreach bit   
            for (int w = 0; w < NBW ; w += 2) {                     // foreach word
                for (int r = 0; r < nbr ; r++) {                    // foreach ref
                    char *iname = str("w%d_w%d_%s_%d", w, w+1, cell, b);
                    char *ref = str("i%s%d", port[p], r);
                    x = GENLIB_GET_REF_X(iname,ref);
                    y = GENLIB_GET_REF_Y(iname,ref);
                    GENLIB_PHVIA(CONT_VIA, x, y);                 // put a VIA
                }
            }
            char * pname = str("i%s[%d]", port[p], b);
            GENLIB_PHSEG(CALU2, 2, pname, xmin, y, xmax, y);      // PUT a CALU2 connecting all
        }
    }
}

void PHSEG_OUT_DT(char * cnff, char *cnoa, char * port[], size_t nbp)
{
    char *iname1 = str("w%d_w%d_%s_0", 0, 1, cnff);
    char *iname2 = str("w%d_w%d_%s_0", NBW-2, NBW-1, cnff);
    long xmin = GENLIB_GET_REF_X(iname1,"vss0");
    long xmax = GENLIB_GET_REF_X(iname2,"vss1");
    for (int p = 0; p < nbp; p++ ) {                                // foreach output port
        for (int b = 0; b < NBB ; b++) {                            // foreach bit   
            long x01, x23, x45, x67;
            long x12, x34, x56;
            long x120, x121, x340, x341, x560, x561;
            long y01, y23, y45, y67;
            long y12, y34, y56;
            long y120, y121, y340, y341, y560, y561;
            char * inff;
            char * inoa;
            char * nffname = str("nq%s", port[p]);
            char * noaname = str("i%s",  port[p]);
            switch (NBW) {                                          // Put VIAs on nff cells
            case 8:
                inff = str("w6_w7_%s_%d", cnff, b);
                x67 = GENLIB_GET_REF_X(inff,nffname);
                y67 = GENLIB_GET_REF_Y(inff,nffname);
                GENLIB_PHVIA(CONT_VIA, x67, y67);
                inff = str("w4_w5_%s_%d", cnff, b);
                x45 = GENLIB_GET_REF_X(inff,nffname);
                y45 = GENLIB_GET_REF_Y(inff,nffname);
                GENLIB_PHVIA(CONT_VIA, x45, y45);
            case 4:
                inff = str("w2_w3_%s_%d", cnff, b);
                x23 = GENLIB_GET_REF_X(inff,nffname);
                y23 = GENLIB_GET_REF_Y(inff,nffname);
                GENLIB_PHVIA(CONT_VIA, x23, y23);
            case 2:    
                inff = str("w0_w1_%s_%d", cnff, b);
                x01 = GENLIB_GET_REF_X(inff,nffname);
                y01 = GENLIB_GET_REF_Y(inff,nffname);
                GENLIB_PHVIA(CONT_VIA, x01, y01);
            }
            switch (NBW) {                                           // Put VIAs on noa cells
            case 8:
                inoa = str("mx_56_%s_%d", cnoa, b);
                x560 = GENLIB_GET_REF_X(inoa,str("%s0",noaname));
                y560 = GENLIB_GET_REF_Y(inoa,str("%s0",noaname));
                GENLIB_PHVIA(CONT_VIA, x560, y560); 
                x561 = GENLIB_GET_REF_X(inoa,str("%s1",noaname));
                y561 = GENLIB_GET_REF_Y(inoa,str("%s1",noaname));
                GENLIB_PHVIA(CONT_VIA, x561, y561);
                x56 = GENLIB_GET_REF_X(inoa,nffname);
                y56 = GENLIB_GET_REF_Y(inoa,nffname);
                GENLIB_PHVIA(CONT_VIA, x56, y56);
                inoa = str("mx_34_%s_%d", cnoa, b);
                x340 = GENLIB_GET_REF_X(inoa,str("%s0o",noaname));
                y340 = GENLIB_GET_REF_Y(inoa,str("%s0o",noaname));
                GENLIB_PHVIA(CONT_VIA, x340, y340); 
                x341 = GENLIB_GET_REF_X(inoa,str("%s1o",noaname));
                y341 = GENLIB_GET_REF_Y(inoa,str("%s1o",noaname));
                GENLIB_PHVIA(CONT_VIA, x341, y341);
                x34 = GENLIB_GET_REF_X(inoa,str("%so",nffname));
                y34 = GENLIB_GET_REF_Y(inoa,str("%so",nffname));
                GENLIB_PHVIA(CONT_VIA, x34, y34);
                if (((y34-y340) % 5) == 3) {
                    GENLIB_PHSEG(ALU2, 2, "", x34, y34, x34, y34+2);
                    y34 = y34 + 2;
                } else if (((y34-y340) % 5) == -3) {
                    GENLIB_PHSEG(ALU2, 2, "", x34, y34, x34, y34-2);
                    y34 = y34 - 2;
                } 
            case 4:
                inoa = str("mx_12_%s_%d", cnoa, b);
                x120 = GENLIB_GET_REF_X(inoa,str("%s0",noaname));
                y120 = GENLIB_GET_REF_Y(inoa,str("%s0",noaname));
                GENLIB_PHVIA(CONT_VIA, x120, y120); 
                x121 = GENLIB_GET_REF_X(inoa,str("%s1",noaname));
                y121 = GENLIB_GET_REF_Y(inoa,str("%s1",noaname));
                GENLIB_PHVIA(CONT_VIA, x121, y121);
                x12 = GENLIB_GET_REF_X(inoa,nffname);
                y12 = GENLIB_GET_REF_Y(inoa,nffname);
                GENLIB_PHVIA(CONT_VIA, x12, y12);
            }
            switch (NBW) {
            case 8:
                GENLIB_PHSEG(ALU2, 2, "", x12, y12, x340, y340);
                GENLIB_PHSEG(ALU2, 2, "", x56, y56, x341, y341);
                GENLIB_PHSEG(ALU2, 2, "", x45, y45, x560, y560);
                GENLIB_PHSEG(ALU2, 2, "", x67, y67, x561, y561);
            case 4:
                GENLIB_PHSEG(ALU2, 2, "", x01, y01, x120, y120);
                GENLIB_PHSEG(ALU2, 2, "", x23, y23, x121, y121);
            }
            char * pname = str("nq%s[%d]", port[p], b);
            long y = (NBW == 2) ? y01 : (NBW == 4) ? y12 : y34;
            GENLIB_PHSEG(CALU2, 2, pname, xmin, y, xmax, y);      // PUT a CALU2 connecting all
        }
    }
}

void RouteDT()
{
    char * in_port[]  = { "z", "t" };
    char * out_port[] = { "x", "y" };
    PHSEG_CK_DT  ("cff", 2); 
    PHSEG_AZ0AT0_DT  ("dec", in_port, sizeof(in_port)/sizeof(char*), 2); 
    PHSEG_AZ1AT1_DT  ("dec", in_port, sizeof(in_port)/sizeof(char*), 2); 
    PHSEG_AZ2AT2_DT  ("dec", in_port, sizeof(in_port)/sizeof(char*), 2); 
    PHSEG_AX0AY0_DT  ("cff", out_port, sizeof(in_port)/sizeof(char*)); 
    PHSEG_AXAY_DT  ("cff", out_port, sizeof(in_port)/sizeof(char*)); 
    PHSEG_WE_DT  ("cff", in_port, sizeof(in_port)/sizeof(char*), 2); 
    PHSEG_IN_DT  ("nff", in_port, sizeof(in_port)/sizeof(char*), 2); 
    PHSEG_OUT_DT ("nff", "noa", out_port, sizeof(out_port)/sizeof(char*)); 
}

//--------------------------------------------------------------------------------------------------
// Route DataPath Control
//--------------------------------------------------------------------------------------------------

void PHSEG_CT(char *iname, char *c1, char *c2, char *ref1, char *ref2, int metal)
{
    char *ins1 = INAME(iname, c1);
    long x1 = GENLIB_GET_REF_X(ins1, ref1);
    long y1 = GENLIB_GET_REF_Y(ins1, ref1);

	char *ins2 = INAME(iname, c2);
	long x2 = GENLIB_GET_REF_X(ins2, ref2);
	long y2 = GENLIB_GET_REF_Y(ins2, ref2);

    if (metal == 1) GENLIB_PHVIA(CONT_VIA, x1, y1);
	GENLIB_PHVIA(CONT_VIA, x2, y2);
    GENLIB_PHVIA(CONT_VIA2, x1, y1);
    GENLIB_PHSEG(ALU3, 2, "", x1, y1, x1, y2);

    if (x1 == x2) {
	    GENLIB_PHVIA(CONT_VIA2, x2, y2);
    } else {
	    GENLIB_PHVIA(CONT_VIA2, x1, y2);
        GENLIB_PHSEG(ALU2, 2, "", x1, y2, x2, y2);
    }
}

void PHSEG_DL(char *iname, char *c1, char *c2, char *ref1, char* ref2, int leg)
{
    char *ins1 = INAME(iname, c1);
    long x1 = GENLIB_GET_REF_X(ins1, ref1);
    long y1 = GENLIB_GET_REF_Y(ins1, ref1);

    char *ins2 = INAME(iname, c2);
	long x2 = GENLIB_GET_REF_X(ins2, ref2);
	long y2 = GENLIB_GET_REF_Y(ins2, ref2);

    long x3 = x1 + leg;
    
    GENLIB_PHVIA(CONT_VIA, x1, y1);
    GENLIB_PHSEG(ALU2, 2, "", x1, y1, x3, y1);
	GENLIB_PHVIA(CONT_VIA2, x3, y1);
    GENLIB_PHSEG(ALU3, 2, "", x3, y1, x3, y2);
	GENLIB_PHVIA(CONT_VIA2, x3, y2);
    GENLIB_PHSEG(ALU2, 2, "", x3, y2, x2, y2);
    GENLIB_PHVIA(CONT_VIA2, x2, y2);
    GENLIB_PHVIA(CONT_VIA, x2, y2);
}

void PHSEG_DP(char *iname, char *c1, char *c2, char *ref1, char* ref2, char direction)
{
    long x1, y1, x2, y2;
    char *ins1 = INAMEV(iname, c1, 0);
    char *ins2 ;

    x1 = GENLIB_GET_REF_X(ins1, ref1);
    y1 = GENLIB_GET_REF_Y(ins1, ref1);
    GENLIB_PHVIA(CONT_VIA, x1, y1);
    GENLIB_PHVIA(CONT_VIA2, x1, y1);

    for (int bit = 1; bit != NBB; bit++) {
        ins2 = INAMEV(iname, c1, bit);
	    x2 = GENLIB_GET_REF_X(ins2, ref1);
	    y2 = GENLIB_GET_REF_Y(ins2, ref1);
	    GENLIB_PHVIA(CONT_VIA, x2, y2);
	    GENLIB_PHVIA(CONT_VIA2, x2, y2);
    }

    GENLIB_PHSEG(ALU3, 2, "", x1, y1, x2, y2);
    x1 = x2;
    y1 = y2;

	ins2 = INAME(iname, c2);
	x2 = GENLIB_GET_REF_X(ins2, ref2);
	y2 = GENLIB_GET_REF_Y(ins2, ref2);

    if (x1 == x2) {
        GENLIB_PHSEG(ALU3, 2, "", x1, y1, x2, y2);
	    GENLIB_PHVIA(CONT_VIA2, x2, y2);
    } else {
        if (direction == XFIRST) {
	        GENLIB_PHVIA(CONT_VIA2, x1, y1);
            GENLIB_PHSEG(ALU2, 2, "", x1, y1, x2, y1);
	        GENLIB_PHVIA(CONT_VIA2, x2, y1);
            GENLIB_PHSEG(ALU3, 2, "", x2, y1, x2, y2);
	        GENLIB_PHVIA(CONT_VIA2, x2, y2);
        } else {
            GENLIB_PHSEG(ALU3, 2, "", x1, y1, x1, y2);
	        GENLIB_PHVIA(CONT_VIA2, x1, y2);
            GENLIB_PHSEG(ALU2, 2, "", x1, y2, x2, y2);
        }
    }
	GENLIB_PHVIA(CONT_VIA, x2, y2);
}


void RouteMem(char *iname)
{
    PHSEG_DP (iname, "nff", "cff", "ck0",  "ck0",  YFIRST);
    PHSEG_DP (iname, "nff", "cff", "ck1",  "ck1",  YFIRST);
    PHSEG_DP (iname, "nff", "cff", "nck0", "nck0", YFIRST);
    PHSEG_DP (iname, "nff", "cff", "nck1", "nck1", YFIRST);
    PHSEG_DP (iname, "nff", "cff", "nwt1", "nwt1", YFIRST);
    PHSEG_DP (iname, "nff", "cff", "wt1",  "wt1",  YFIRST);
    PHSEG_DP (iname, "nff", "cff", "nwt0", "nwt0", YFIRST);
    PHSEG_DP (iname, "nff", "cff", "wt0",  "wt0",  YFIRST);
    PHSEG_DP (iname, "nff", "cff", "nwd0", "nwd0", YFIRST);
    PHSEG_DP (iname, "nff", "cff", "wd0",  "wd0",  YFIRST);
    PHSEG_DP (iname, "nff", "cff", "wd1",  "wd1",  YFIRST);
    PHSEG_DP (iname, "nff", "cff", "nwd1", "nwd1", YFIRST);
    PHSEG_DP (iname, "nff", "cff", "nwz0", "nwz0", XFIRST);
    PHSEG_DP (iname, "nff", "cff", "wz0",  "wz0",  YFIRST);
    PHSEG_DP (iname, "nff", "cff", "nwz1", "nwz1", XFIRST);
    PHSEG_DP (iname, "nff", "cff", "wz1",  "wz1",  YFIRST);

    // words are missnamed 0 is 1, and 0 is 0
    // I do not want to change the layout, thus it is simpler to change the addr routing

    PHSEG_DP (iname, "nff", "cff", "rx1",  "rx0",  YFIRST);
    PHSEG_DP (iname, "nff", "cff", "rx0",  "rx1",  YFIRST);
    PHSEG_DP (iname, "nff", "cff", "ry1",  "ry0",  YFIRST);
    PHSEG_DP (iname, "nff", "cff", "ry0",  "ry1",  YFIRST);

    //if (NBW > 2) {
        PHSEG_CT (iname, "cff", "dec", "wt_0", "wt_0", 2); 
        PHSEG_CT (iname, "cff", "dec", "wt_1", "wt_1", 2); 
        PHSEG_CT (iname, "cff", "dec", "wz_0", "wz_0", 2); 
        PHSEG_CT (iname, "cff", "dec", "wz_1", "wz_1", 2); 
                                              
        PHSEG_DL (iname, "cff", "dec", "at0",  "at0",  6); 
        PHSEG_CT (iname, "cff", "dec", "at1",  "at1",  1); 
        PHSEG_CT (iname, "cff", "dec", "az0",  "az0",  1); 
        PHSEG_CT (iname, "cff", "dec", "az1",  "az1",  1); 
   // }
}

void RouteMux(char *iname, int ref)
{
    PHSEG_DP (iname, "noa", "coa", "rx0", "rx0",  XFIRST);
    PHSEG_DP (iname, "noa", "coa", "rx1", "rx1",  YFIRST);
    PHSEG_DP (iname, "noa", "coa", "ry0", "ry0",  YFIRST);
    PHSEG_DP (iname, "noa", "coa", "ry1", "ry1",  XFIRST);
    PHSEG_CT (iname, "coa", "coa", str("ay%d", ref), "ay0", 2);
    PHSEG_CT (iname, "coa", "coa", str("ax%d", ref), "ax0", 2);
}

//--------------------------------------------------------------------------------------------------
// Place
//--------------------------------------------------------------------------------------------------

void PlaceMem(char sym, char *iname)
{
    int symy = (sym == NOSYM) ? 1 : 0;
    
    if (PHINS_REF == NULL) {
	    GENLIB_PLACE(M_NFF, INAMEV(iname,"nff",0), DOSYMY, 0, 0);
    } else {
        GENLIB_DEF_PHINS(PHINS_REF);
        GENLIB_PLACE_RIGHT(M_NFF, INAMEV(iname,"nff",0), DOSYMY);
    }
    for (int bit = 1; bit != NBB; bit ++) {
	    GENLIB_PLACE_TOP(M_NFF, INAMEV(iname,"nff",bit), DOSYMY);
    }        
	GENLIB_PLACE_TOP(M_CFF, INAME(iname,"cff"), DOSYMY);

    //if (NBW > 2) {
	    GENLIB_PLACE_TOP(M_INV, INAME(iname,"inv"), DOSYMY);
	    GENLIB_PLACE_RIGHT(M_DEC, INAME(iname,"dec"), NOSYMY);
    //}
    PHINS_REF = INAMEV(iname,"nff",0);
}

void PlaceMux(char sym, char *iname)
{
    int symy = (sym == NOSYM) ? 1 : 0;

    GENLIB_DEF_PHINS(PHINS_REF);
	GENLIB_PLACE_RIGHT(M_NOA, INAMEV(iname,"noa",0), DOSYMY);
    for (int bit = 1; bit != NBB; bit ++) {
	    GENLIB_PLACE_TOP(M_NOA, INAMEV(iname,"noa",bit), DOSYMY);
    }        
	GENLIB_PLACE_TOP(M_COA, INAME(iname,"coa"), DOSYMY);
    // if (NBW > 2)
	    GENLIB_PLACE_TOP(M_MCT, INAME(iname,"mct"), DOSYMY);
    PHINS_REF = INAMEV(iname,"noa",0);
}

//--------------------------------------------------------------------------------------------------
// Route Supply Both Horizotal and Vertical
//--------------------------------------------------------------------------------------------------

void RouteSupply(void)
{
    int b, y;
    char * iname = str("w%d_w%d_%s", NBW-2, NBW-1, "cff");
    long xmax = GENLIB_GET_REF_X(iname,"vss1");

    for(b = 0, y = 0; b < NBB + 2; b+=2, y+=100) {
        GENLIB_PHSEG (CALU1, 4, "vdd", 0, y, xmax, y);
        GENLIB_PHSEG (CALU1, 4, "vss", 0, y+50, xmax, y+50);
    }
    GENLIB_PHSEG (CALU1, 4, "vdd", 0, y, xmax, y);
}

//--------------------------------------------------------------------------------------------------
// Create Figure
//--------------------------------------------------------------------------------------------------

void create_vbe(void) 
{
    FILE * vbe = fopen (str("sram_w2r2_%dx%d.vbe", NBW, NBB), "w");

    int s = (NBW==2) ? 6 : (NBW==4) ? 4 : 0 ;
    int logNBW = (NBW==2) ? 1 : (NBW==4) ? 2 : 3 ;

    fprintf(vbe,
    "ENTITY sram_w2r2_%dx%d IS\n"
    "PORT (\n"
    "\n"
    "    at : in bit_vector(2 DOWNTO 0) ;\n"
    "    az : in bit_vector(2 DOWNTO 0) ;\n"
    "    ax : in bit_vector(2 DOWNTO 0) ;\n"
    "    ay : in bit_vector(2 DOWNTO 0) ;\n"
    "    wt : in BIT;\n"
    "    wz : in BIT;\n"
    "\n"
    "    it : in bit_vector(%d DOWNTO 0) ;\n"
    "    iz : in bit_vector(%d DOWNTO 0) ;\n"
    "\n"
    "    nqx : out bit_vector(%d DOWNTO 0) ;\n"
    "    nqy : out bit_vector(%d DOWNTO 0) ;\n"
    "\n"
    "    ck : in BIT;\n"
    "\n"
    "    vdd : in BIT;\n"
    "    vss : in BIT\n"
    ");\n"
    "END sram_w2r2_%dx%d;\n"
    "\n"
    "ARCHITECTURE behaviour_data_flow OF sram_w2r2_%dx%d IS\n"
    "\n" 
    ,NBW,NBB,NBB-1,NBB-1,NBB-1,NBB-1,NBW,NBB,NBW,NBB);
    
    for (int w = 0; w < NBW; w++) 
    fprintf(vbe, 
    "    SIGNAL w%dm, w%ds : REG_VECTOR(%d DOWNTO 0) REGISTER;\n", w, w, NBB-1);

    fprintf(vbe, 
    "\n"
    "BEGIN\n"
    "\n"
    "    Masters : BLOCK (NOT(ck))\n"
    "    BEGIN\n"
    );

    for (int w = 0; w < NBW; w++) 
    fprintf(vbe, 
    "        w%dm <= GUARDED NOT(it) when (wt AND at = \"%s\") else NOT(iz) when (wz AND az = \"%s\") else NOT(w%ds);\n"
    , w, bin(s+w,3), bin(s+w,3), w);

    fprintf(vbe, 
    "    END BLOCK Masters;\n"
    "\n"
    "    Slaves : BLOCK (ck)\n"
    "    BEGIN\n"
    );

    for (int w = 0; w < NBW; w++) 
    fprintf(vbe, 
    "        w%ds <= GUARDED NOT(w%dm);\n"
    , w, w);

    fprintf(vbe, 
    "    END BLOCK Slaves;\n"
    "\n"
    "    nqx <=   %s w0s when (ax(%d downto 0) = \"%s\")\n"
    , (NBW == 4) ? "" : "NOT", logNBW-1, bin(0,logNBW));

    for (int w = 1; w < NBW-1; w++) 
    fprintf(vbe, 
    "        else %s w%ds when (ax(%d downto 0) = \"%s\")\n"
    , (NBW == 4) ? "" : "NOT", w, logNBW-1, bin(w,logNBW));

    fprintf(vbe, 
    "        else %s w%ds;\n"
    "\n"
    , (NBW == 4) ? "" : "NOT", NBW-1);
 
    fprintf(vbe, 
    "    nqy <=   %s w0s when (ay(%d downto 0) = \"%s\")\n"
    , (NBW == 4) ? "" : "NOT", logNBW-1, bin(0,logNBW));

    for (int w = 1; w < NBW-1; w++) 
    fprintf(vbe, 
    "        else %s w%ds when (ay(%d downto 0) = \"%s\")\n"
    ,  (NBW == 4) ? "" : "NOT", w, logNBW-1, bin(w,logNBW));

    fprintf(vbe, 
    "        else %s w%ds;\n"
    "\n"
    , (NBW == 4) ? "" : "NOT", NBW-1);

    fprintf(vbe, 
    "END;\n");
}

int main(int argc, char *argv[])
{

//- Check Arguments

    if (argc != 3)
	usage(argv[0]);

    NBW = atoi(argv[1]);
    NBB = atoi(argv[2]);

    int sym = SYM_Y;

    if (!in(NBW,3,2,4,8)) usage(argv[0]);
    if (!in(NBB,5,2,4,8,16,32)) usage(argv[0]);

    create_vbe();

//- Create Figure

    GENLIB_DEF_PHFIG(str("%s_%dx%d", FNAME, NBW, NBB));

//- Place Cells

    PlaceMem (sym, "w0_w1");
    if (NBW > 2) {
        PlaceMux (sym, "mx_12");
        PlaceMem (sym, "w2_w3");
    }
    if (NBW > 4) {
        PlaceMux (sym, "mx_34");
        PlaceMem (sym, "w4_w5");
        PlaceMux (sym, "mx_56");
        PlaceMem (sym, "w6_w7");
    }

//- Route Cells Most of Control Signals

    RouteMem ("w0_w1");
    if (NBW > 2) {
        RouteMux ("mx_12",1);
        RouteMem ("w2_w3");
    }
    if (NBW > 4) {
        RouteMux ("mx_34",2);
        RouteMem ("w4_w5");
        RouteMux ("mx_56",1);
        RouteMem ("w6_w7");
    }

//- Create Data And Addresses

    RouteDT ();
    RouteSupply();

//- Create Abutment Box and Save figure

    GENLIB_DEF_AB(0, 0, 0, 0);
    GENLIB_SAVE_PHFIG();
}
