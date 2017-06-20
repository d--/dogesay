# dogesay

An attempt at writing a program that will simply shell out and call 'say' on
OSX as soon as the LCM broadcast packet is received.

Take five minutes and do the following...

Install the XCode Command Line Tools

```bash
xcode-select --install
```

Install Homebrew

```bash
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

Install pkg-config

```bash
brew install pkg-config
```

Install LCM

```bash
brew install lcm
```

Generate the message marshalling code

```bash
pushd src
lcm-gen -c say.lcm
popd
```

Run the magic incantation

```bash
make
make install
```
