--
-- Template column
-- Recognition rule
--
-- capacity is the number of mem_cell composing column
--
entity column is
    generic ( capacity              : integer);
    port    ( q,nq                  : inout mux_bit bus;
              com                   : in    bit_vector(capacity-1 downto 0);
              vdd,vss               : in    bit);
    -- pragma symmetric q nq
    -- pragma symmetric com
end;


architecture structural of column is
    component mem_cell 
        port    ( q, nq             : inout mux_bit bus;
                  com               : in    bit;
                  vdd, vss          : in    bit);
    end component;

begin
    loop: for i in 0 to capacity-1 generate
        mem     : mem_cell port map ( q ,nq ,com(i) ,vdd ,vss);
    end generate;

end;
