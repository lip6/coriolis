--
-- Template bit_line
-- Recognition rule
--
entity bit_line is
    generic ( capacity, nbcolumn    : integer);
    port    ( data_in               : in    bit;
              data_out              : out   bit;
              com                   : in    bit_vector(capacity - 1 downto 0);
              sel                   : in    bit_vector(nbcolumn - 1 downto 0);
              wen                   : in    bit;
              ck, cke, ckp          : in    bit;
              vdd, vss              : in    bit);
    -- pragma symmetric com
    -- pragma symmetric sel
end;

architecture structural of bit_line is
    component column_and_sel
        generic ( capacity          : integer);
        port    ( q, nq             : inout mux_bit bus;
                  com               : in    bit_vector(capacity - 1 downto 0);
                  ck                : in    bit;
                  sel_column        : in    bit;
                  vdd, vss          : in    bit);
    end component;

    component sense_amplifier
        port    ( data, ndata       : in    bit;
                  cke, ckp          : in    bit;
                  data_out          : out   bit;
                  vdd, vss          : in    bit);
    end component;

    component column_input
        port    ( datain, wen, ckp  : in    bit;
                  q, nq             : inout bit;
                  vdd,vss           : in    bit);
    end component;

    signal nq, q : bit;

begin
    loop : for i in nbcolumn-1 downto 0 generate
        CSEL : column_and_sel generic map(capacity)
            port map (q, nq, com(capacity-1 downto 0), ck, sel(i), vdd, vss);
    end generate;

    SA : sense_amplifier
        port map (q, nq, cke, ckp, data_out, vdd, vss);

    CI : column_input
        port map (data_in, wen, ckp, q, nq, vdd, vss);
end;
