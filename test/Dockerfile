# set the base image
FROM ubuntu

# author
MAINTAINER Nakano Masaki

# update sources list
RUN apt-get clean
RUN apt-get update
RUN apt-get install -qy apt-utils
RUN apt-get install -qy git
RUN apt-get install -qy clang
RUN apt-get install -qy gcc
RUN apt-get install -qy curl
RUN apt-get install -qy libcurl4-gnutls-dev
RUN apt-get install -qy xz-utils
RUN apt-get install -qy tar
RUN apt-get install -qy wget
RUN apt-get install -qy curl
RUN apt-get install -qy libcurl4-gnutls-dev
RUN apt-get install -qy libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev
RUN curl -fsS https://dlang.org/install.sh | bash -s dmd
RUN sh ~/dlang/dmd-2.076.0/activate
RUN apt-get install -qy dub
RUN apt-get install -qy python3
RUN apt-get install -qy python3-pip
RUN pip3 install --upgrade pip
RUN pip3 install PyYAML
RUN pip3 install jsonschema
RUN pip3 install pyqt5
RUN apt-get install -qy m4
RUN apt-get install -qy unzip
RUN wget https://raw.github.com/ocaml/opam/master/shell/opam_installer.sh -O - | sh -s /usr/local/bin
ENV OPAMDOWNLOADJOBS 1
RUN opam init -y -a --comp=4.04.0
RUN opam install -y yojson omake

RUN git clone https://github.com/google/googletest
RUN cd googletest

RUN echo 'eval `opam config env`' > ~/.bashrc
RUN echo 'source ~/dlang/dmd-2.076.0/activate' >> ~/.bashrc

RUN mkdir ci

RUN cd ci/gtest
RUN wget https://googletest.googlecode.com/files/gtest-1.7.0.zip
RUN 
WORKDIR ci

ENTRYPOINT ["/bin/bash"]
