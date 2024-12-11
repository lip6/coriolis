--
-- Template sense_amplifier
-- Recognition rule
--
entity sense_amplifier is
    port    ( data, ndata       : in    bit;
              cke, ckp          : in    bit;
              X                 : out   bit;
              vdd, vss          : in    bit);
end;

architecture template of sense_amplifier is
    component differential_amplifier
        port    ( i_1, i_2      : in    bit;
                  g             : in    bit;
                  x             : out   bit;
                  vdd, vss      : in    bit);
    end component;

    component TN
       port     ( gate          : in    bit;
                  source,drain  : inout bit;
                  bulk          : in    bit);
    end component;

    signal g1, g2, g3, x1, x2 : bit;

begin
    tn_1 : TN port map (cke ,g1 ,vss ,vss);
    DA_1 : differential_amplifier port map ( ndata ,data  ,g1 ,x1 ,vdd ,vss);

    tn_2 : TN port map (cke ,g2 ,vss ,vss);
    DA_2 : differential_amplifier port map ( data  ,ndata ,g2 ,x2 ,vdd ,vss);

    tn_3 : TN port map (cke ,g3 ,vss ,vss);
    DA_3 : differential_amplifier port map ( x1, x2, g3, X, vdd, vss);

    link : TN port map (ckp ,x2 ,x1  ,vss);

end;

architecture mixt of sense_amplifier is
    component differential_amplifier
        port    ( i_1, i_2      : in    bit;
                  g             : in    bit;
                  x             : out   bit;
                  vdd, vss      : in    bit);
    end component;

    component TP
        port     ( gate          : in    bit;
                   source,drain  : inout bit;
                   bulk          : in    bit);
    end component;

    component TN
        port     ( gate          : in    bit;
                   source,drain  : inout bit;
                   bulk          : in    bit);
    end component;

    signal g1, g2, g3, x1, x2, sig1, sig2 : bit;

begin
    tn_1 : TN port map (cke ,g1 ,vss ,vss);
    custom_amp_1_tp1 : TP port map  (sig1   ,sig1   ,vdd    ,vdd);
    custom_amp_1_tp2 : TP port map  (sig1   ,x1     ,vdd    ,vdd);
    custom_amp_1_tn1 : TN port map  (ndata  ,g1     ,sig1   ,vss);
    custom_amp_1_tn2 : TN port map  (data   ,x1     ,g2     ,vss);

    tn_2 : TN port map (cke ,g2 ,vss ,vss);
    custom_amp_2_tp1 : TP port map  (sig2   ,sig2   ,vdd    ,vdd);
    custom_amp_2_tp2 : TP port map  (sig2   ,x2     ,vdd    ,vdd);
    custom_amp_2_tn1 : TN port map  (data   ,g1     ,sig2   ,vss);
    custom_amp_2_tn2 : TN port map  (ndata  ,x2     ,g2     ,vss);

    tn_3 : TN port map (cke ,g3 ,vss ,vss);
    DA   : differential_amplifier port map ( x1, x2, g3, X, vdd, vss);

    -- pass transistor
    PT : TN port map (ckp ,x2 ,x1  ,vss);

end;
