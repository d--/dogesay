# dogesay

An attempt at writing a program that will simply shell out and call 'say' on
OSX as soon as the LCM broadcast packet is received.

Requires LCM

```bash
pushd src
lcm-gen -c say.lcm
popd
```
Alternatively, with Docker

```bash
docker build -t dogesay .
docker run --rm -v $PWD:/home/dogesay dogesay /bin/bash -c "cd src && lcm-gen -c say.lcm"
```
