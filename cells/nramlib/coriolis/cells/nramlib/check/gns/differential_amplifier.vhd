--
-- Template differential_amplifier
-- Recognition rule
--
--       ^          ^
--       |          | 
--        ]o--o---o[
--       |    |     |
--   sig o----+     o-- x
--       |          |
-- i_1 -[            ]- i_2
--       |          |
--       o----------+
--       |
-- g ----+
--
entity differential_amplifier is
    port    ( i_1, i_2          : in    bit;
              g                 : in    bit;
              x                 : out   bit;
              vdd, vss          : in    bit);
end;

architecture structural of differential_amplifier is
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

    signal sig                  : bit;
    
begin
    tn_1 : TN port map (i_1    ,sig    ,g    ,vss);
    tn_2 : TN port map (i_2    ,x      ,g    ,vss);
    tp_1 : TP port map (sig    ,x      ,vdd  ,vdd);
    tp_2 : TP port map (sig    ,sig    ,vdd  ,vdd);

end;


