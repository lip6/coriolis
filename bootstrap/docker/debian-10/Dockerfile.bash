
FROM debian10.coriolis

COPY root/dot.bashrc  /root/.bashrc

CMD [ "/bin/bash", "-i" ]
