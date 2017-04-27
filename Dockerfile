FROM ubuntu:14.04

RUN apt-get update && apt-get install -y \
   build-essential \
   curl \
   wget \
   libglib2.0-dev \
   autoconf \
   libtool \
   unzip \
   git \
   valgrind \
   pkg-config

RUN wget https://github.com/lcm-proj/lcm/releases/download/v1.3.1/lcm-1.3.1.zip && \
   unzip lcm-1.3.1.zip && \
   cd lcm-1.3.1 && ./configure && \
   make && \ 
   sudo make install && \
   sudo ldconfig  

RUN curl -sL https://github.com/libcheck/check/releases/download/0.11.0/check-0.11.0.tar.gz | tar xz

RUN cd check-0.11.0 && autoreconf --install && ./configure && make && make install && cd ..

RUN ldconfig

CMD ["/bin/bash"]
