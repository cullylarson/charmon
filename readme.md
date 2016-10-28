# charmon

> Remember that sequence? It's on the tip of my fingers!


## Ingredients

* [ATMega328P](https://www.sparkfun.com/products/9061)

## Install

_NOTE: The makefile uses `avr-gcc` and `avrdude` to compile and upload. It also assumes you're using a `usbtiny` programmer, like the Sparkfun Pocket AVR Programmer._

1.  Connect your programmer

1.  Compile and upload

    ```
    $ make upload
    ```

## Resources

* [AVR Programming](https://github.com/cullylarson/avr-programming)
* [C Constants for ATMega328P](https://github.com/vancegroup-mirrors/avr-libc/blob/master/avr-libc/include/avr/iom328p.h)
* [ATMega328 Datasheet](http://www.sparkfun.com/datasheets/Components/SMD/ATMega328.pdf)
* [Debounce Guide](assets/debounce-guide.pdf)
* [Video on Debouncing](https://www.youtube.com/watch?v=RzVc3o0iedM)
* [Arduino C Code on Github](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/cores/arduino)

### Pin Layouts

![ATMega328 Colorful Pin Layout](assets/atmega328-colorful.jpg)
