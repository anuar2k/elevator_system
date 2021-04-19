FROM muslcc/x86_64:x86_64-linux-musl AS compiler
WORKDIR /usr/elevator_system
RUN apk update && apk add ncurses-dev make
ADD library library
ADD cli_client cli_client
RUN make -C cli_client

FROM alpine
RUN apk update && apk add ncurses
COPY --from=compiler /usr/elevator_system/cli_client/build/cli_client /usr/bin/cli_client

CMD ["/usr/bin/cli_client"]