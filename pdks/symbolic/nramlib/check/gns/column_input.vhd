--
-- Template column_input
-- Recognition rule
--
entity column_input is
    port    ( datain, wen, ckp      : in    bit;
              q, nq                 : inout bit;
              vdd,vss               : in    bit);
end;

architecture precharge_included of column_input is
    component tn
        port    ( gate              : in    bit;
                  source            : inout bit;
                  drain             : inout bit;
                  bulk              : in    bit);
    end component;
    
    component tp
        port    ( gate              : in    bit;
                  source            : inout bit;
                  drain             : inout bit;
                  bulk              : in    bit);
    end component;

    signal nsig, nsig2, sig, PCsig  : bit;

begin
    inv_tn          : tn port map ( datain   ,vss    ,nsig   ,vss);
    inv_tp          : tp port map ( datain   ,vdd    ,nsig   ,vdd);
    
    buf_tn1         : tn port map ( datain   ,vss    ,nsig2  ,vss);
    buf_tp1         : tp port map ( datain   ,vdd    ,nsig2  ,vdd);
    buf_tn2         : tn port map ( nsig2    ,vss    ,sig    ,vss);
    buf_tp2         : tp port map ( nsig2    ,vdd    ,sig    ,vdd);
    
    precharge_tn1   : tn port map ( ckp      ,nq     ,vdd    ,vss);
    precharge_tn2   : tn port map ( ckp      ,q      ,vdd    ,vss);
    precharge_tn3   : tn port map ( ckp      ,nq     ,q      ,vss);
    
    write_q         : tn port map ( wen      ,nq     ,nsig   ,vss);
    write_nq        : tn port map ( wen      ,q      ,sig    ,vss);
    
end;
