# RotaryEncoder library

A library for the Arduino environment for using a rotary encoder as an input.

Here you can find an Arduino compatible library for using rotary encoders.

I was searching a library for using a rotary encoder in my latest project and found a lot of information on this topic but none of the existing libraries did immediately match my expectations so I finally built my own.

It supports the type of rotary encoder that has a phase change on both input signals when rotating one `notch`.

The article on my web site explains the software mechanisms used in detail so you can understand
the coding and might be able to adjust it to your needs if you like:

<http://www.mathertel.de/Arduino/RotaryEncoderLibrary.aspx>

There are various aspects when writing a library for rotary encoders and you can also find a lot of the sources I analysed at the bottom of this article.


## updates

* 26.01.2020 Added debounce functionality for the encoder's push button. Based on the Bounce library by Thomas Fredericks' Bounce library: https://playground.arduino.cc/Code/Bounce/
