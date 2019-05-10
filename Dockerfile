FROM ubuntu:latest

MAINTAINER Arkadiy Krava

RUN apt-get update && apt-get install -y git cmake libboost-all-dev libssl-dev

RUN git clone https://github.com/no1msd/mstch.git && cd mstch && cmake . && make install && cd .. && \
    git clone https://github.com/nlohmann/json.git && cd json && cmake . && make install && cd .. && \
    rm -rf mstch json

COPY . web-framework

RUN cd web-framework && \
    cmake . && \
    make install && \
    cd .. && \
    rm -rf web-framework

CMD ["bash"]