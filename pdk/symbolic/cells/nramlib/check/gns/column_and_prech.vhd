--
-- Template column_and_prech
-- Recognition rule
--
entity column_and_prech is
   generic  ( capacity              : integer);
   port     ( q, nq                 : inout mux_bit bus;
              com                   : in    bit_vector(capacity-1 downto 0);
              ck                    : in    bit;
              vdd, vss              : in    bit);
   -- pragma symmetric q nq
   -- pragma symmetric com
end;

architecture structural of column_and_prech is
    component column 
       generic  ( capacity          : integer);
       port     ( q, nq             : inout mux_bit bus;
                  com               : in    bit_vector(capacity-1 downto 0);
                  vdd, vss          : in    bit);
    end component;      
    
    component TN
       port     ( gate              : in    bit;
                  source, drain     : inout bit;
                  bulk              : in    bit);
    end component;

begin

    col : column
        generic map(capacity) 
        port map (q     ,nq     ,com    ,vdd    ,vss);

    precharge_t1 : TN port map (ck, nq, vdd, vss);
    precharge_t2 : TN port map (ck, q, vdd, vss);
    precharge_t3 : TN port map (ck, q, nq, vss);
 
end;
