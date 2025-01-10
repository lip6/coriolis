--
-- Template column_xn
-- Recognition rule
--
-- capacity is the number of mem_cell composing column
-- width is the number of column composing column_xn
--
--
entity column_xn is
    generic ( size, width       : integer);
    port    ( com               : in    bit_vector(size-1 downto 0);
              q, nq             : inout mux_vector(width-1 downto 0) bus;
              vdd,gnd           : in    bit);
    -- pragma symmetric com
    -- pragma symmetric q(i) nq(i)
end;

architecture structural of column_xn is
    component column
        generic ( capacity      : integer);
        port    ( q, nq         : inout mux_bit bus;
                  com           : in    bit_vector(capacity-1 downto 0);
                  vdd, gnd      : in    bit);
    end component;

begin
    loop: for i in width-1 downto 0 generate
        column_n : column
            generic map (size)
            port map (q(i) ,nq(i) ,com ,vdd ,gnd);
    end generate;

end;
