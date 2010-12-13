
 CORIOLIS_TOP="/asim/coriolis2"
 if [ ! -d "$CORIOLIS_TOP" ]; then
   echo "[ERROR] Coriolis 2 root directory <$CORIOLIS_TOP> not found."
 else
   eval "`$CORIOLIS_TOP/etc/coriolis2/coriolisEnv.py --root=/users/outil/coriolis/coriolis-2.x --v2 --release --shared --python`"
 fi
