FROM ubuntu:18.04

RUN apt -y update \
	&& apt install -y make \
	&& apt install -y gcc \
	&& apt install -y g++ \
	&& apt install -y xauth \
	&& touch ~/.Xauthority

RUN apt install -y qt4-default libqtwebkit-dev
ENV QT_X11_NO_MITSHM=1


# Install special packages
# COPY packages packages
RUN apt install -y libeigen3-dev 
#ENTRYPOINT [ "/workspace/start.sh" ]
