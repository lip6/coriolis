--
-- Template mem_cell
-- Recognition rule
--
entity mem_cell is
    port    ( q, nq             : inout mux_bit bus;
              com               : in    bit;
              vdd, vss          : in    bit);
    -- pragma symmetric q nq
end mem_cell;

architecture structural of mem_cell is
    component tn
        port    ( gate          : in    bit;
                  source, drain : inout bit;
                  bulk          : in    bit);
    end component;

    component tp
        port    ( gate          : in    bit;
                  source, drain : inout bit;
                  bulk          : in    bit);
    end component;

    signal mem                  : bit;
    signal nmem                 : bit;

begin
    tn_1    : tn port map (nmem ,vss    ,mem    ,vss);
    tn_2    : tn port map (com  ,q      ,mem    ,vss);
    tn_3    : tn port map (mem  ,vss    ,nmem   ,vss);
    tn_4    : tn port map (com  ,nq     ,nmem   ,vss);
    tp_5    : tp port map (nmem ,vdd    ,mem    ,vdd);
    tp_6    : tp port map (mem  ,vdd    ,nmem   ,vdd);

end;
