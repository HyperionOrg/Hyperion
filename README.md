# Hyperion
![](https://img.shields.io/badge/license-MIT-yellow)

This repository houses the source code for Hyperion.

## Installing
In order to even build the minecraft server you have to install OpenSSL.<br>
You can download the OpenSSL binaries <a href="https://wiki.openssl.org/index.php/Binaries">here</a>.<br>

## Building
In order to build the minecraft server, you will need to follow these steps:
1.  Start cloning the repository by typing in the command prompt of your choice:<br><code>git clone --recurse-submodules <a href="https://github.com/SkillerRaptor/Hyperion">https://github.com/SkillerRaptor/Hyperion</a></code>.
2.  Inside the repository directory you have to run `cmake -G "Visual Studio 16 2019" -A x64 -H. -Bbuild`

## Dependencies & License
- [asio](https://github.com/chriskohlhoff/asio/blob/master/asio/COPYING) Copyright (c) Boost Software License, Cristopher M. Kohlhoff 2003-2020
- [json](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT) Copyright (c) MIT, Niels Lohmann 2013-2021
- [spdlog](https://github.com/gabime/spdlog/blob/v1.x/LICENSE) Copyright (c) MIT, Gabi Melman 2016

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
