FROM akrava/web-framework:latest

WORKDIR /usr/src/app

RUN apt-get install -y libsqlite3-dev

COPY . .

RUN cmake . && make

EXPOSE 80

CMD ["sh", "-c", "./simple-site 0.0.0.0:$PORT"]