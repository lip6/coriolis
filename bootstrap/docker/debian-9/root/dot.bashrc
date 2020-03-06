
 echo "Running /root/.bashrc"

 for archDir in `ls /root/coriolis-2.x/`; do
   if [ "$archDir" = "src" ]; then continue; fi
   break
 done
 echo "Found Coriolis architecture directory \"${archDir}\"."

 installDir="/root/coriolis-2.x/${archDir}/Release.Shared/install"
 . ${installDir}/etc/profile.d/alc_env.sh
 eval `${installDir}/etc/coriolis2/coriolisEnv.py`

 export QT_X11_NO_MITSHM=1
