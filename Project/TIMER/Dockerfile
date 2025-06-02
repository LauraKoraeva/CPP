FROM ubuntu:latest AS builder


RUN apt-get update && apt-get install -y \
	cmake \
	g++ \
	&& rm -rf /var/lib/apt/lists/*


WORKDIR /app


COPY . .


RUN mkdir build


WORKDIR /app/build


RUN cmake .. && make




FROM alpine:latest AS runner


RUN apk add --no-cache libc6-compat && \
	apk add --no-cache libstdc++


COPY --from=builder /app/build/focus_timer /app/focus_timer


WORKDIR /app


CMD ["./focus_timer"]


