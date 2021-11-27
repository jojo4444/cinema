FROM gcc

RUN mkdir -p /app

WORKDIR /app

COPY ./cmake-build-release/cinema /app

CMD ["./cinema"]