FROM gcc

RUN mkdir -p /app

RUN mkdir -p /cinemas

WORKDIR /app

COPY ./cmake-build-release/cinema /app

COPY ./cinemas /cinemas

CMD ["./cinema"]