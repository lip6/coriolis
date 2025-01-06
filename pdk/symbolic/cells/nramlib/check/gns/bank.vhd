entity bank is
    generic (size, capacity, nbcolumn : integer);
    port ( data_in                  : in bit_vector(size - 1 downto 0);
           data_out                 : out bit_vector(size - 1 downto 0);
           com                      : in bit_vector(capacity - 1 downto 0);
           sel                      : in bit_vector(nbcolumn - 1 downto 0);
           wen                      : in bit;
           ck, cke, ckp             : in bit;
           vdd, vss                 : in bit);
    -- pragma symmetric com
    -- pragma symmetric data_in
    -- pragma symmetric data_out
    -- pragma symmetric sel
end;

architecture structural of bank is

    component bit_line 
        generic ( capacity, nbcolumn: integer);
        port    ( data_in           : in    bit;
                  data_out          : out   bit;
                  com               : in    bit_vector(capacity - 1 downto 0);
                  sel               : in    bit_vector(nbcolumn - 1 downto 0);
                  wen               : in    bit;
                  ck, cke, ckp      : in    bit;
                  vdd, vss          : in    bit);
    end component;

begin
    loop : for i in 0 to size - 1 generate
        bit_line_i : bit_line 
            generic map ( capacity, nbcolumn)
            port map    ( data_in(i), data_out(i),
                          com(capacity-1 downto 0), sel(nbcolumn-1 downto 0),
                          wen, ck, cke, ckp, vdd, vss);
    end generate;
end;
