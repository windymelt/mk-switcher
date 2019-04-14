# mk-switcher
ATMEGA168-20PUによるスイッチャーのソース

## build

```shell
$ docker-compose run --rm builder bash
# cd bench/
# make
```

## write into AVR

- Build hex file before write.

```shell
$ make write
```

## board schematic

Fritzing用のソースを用意しています．(`switcher.fzz`)
