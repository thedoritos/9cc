# 9cc

Study to write C compiler based on [低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook).

## Getting Started

```sh
$ docker build -t compilerbook .
```

## Make

```sh
$ docker run --rm -v $PWD/src:/9cc -w /9cc compilerbook make test
```

## Formatting

```sh
# Run on your machine.
$ brew install clang-format
$ clang-format -style=llvm -dump-config > .clang-format
$ clang-format -i $PWD/src/**/*.c

# Or run on Docker image.
$ docker run --rm -v $PWD/src:/9cc -w /9cc compilerbook make format
```
