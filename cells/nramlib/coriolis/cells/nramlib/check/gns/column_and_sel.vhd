--
-- Template column_and_sel
-- Recognition rule
--
entity column_and_sel is
   generic  ( capacity              : integer);
   port     ( q, nq                 : inout mux_bit bus;
              com                   : in    bit_vector(capacity-1 downto 0);
              ck                    : in    bit;
              sel                   : in    bit;
              vdd, vss              : in    bit);
   -- pragma symmetric q nq
   -- pragma symmetric com
end;

architecture structural of column_and_sel is
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

    signal a, na, PCsig             : bit;

begin

    col : column
        generic map(capacity) 
        port map (a     ,na     ,com    ,vdd    ,vss);

    -- pass transistors
    t1 : TN port map (sel       ,a      ,q      ,vss);
    t2 : TN port map (sel       ,na     ,nq     ,vss);
 
    precharge_t1 : TN port map (ck, na, vdd, vss);
    precharge_t2 : TN port map (ck, a, vdd, vss);
    precharge_t3 : TN port map (ck, a, na, vss);
 
end;
