BAOMEI MB-8001B

Библиотека управления дисплеем от пульта квадрокоптера.

Библиотека написана под микроконтроллер STM32F030. Используется библиотека HAL, так что портировать под любой другой микроконтроллер STM32 не составить сложностей.
Тестировал на тактовых частотах тактирования GPIO от 8 MHz до 48 MHz.
Протокол управления софтверный, по этому библеотеке достаточно 3 любых выхода микроконтроллера настроенных в режиме push/pull и по умолчанию выходы подтянуты к питанию. Запитывать микроконтроллер и дисплей нужно от 3 вольт.


BAOMEI MB-8001B

Library control the display from the remote quadrocopter.

The library is written under the microcontroller STM32F030. The HAL library is used, so porting to any other STM32 microcontroller is not difficult.
Tested on GPIO clock speeds from 8 MHz to 48 MHz.
Software control protocol, according to this library, there are enough 3 any outputs of the microcontroller configured in the push / pull mode and by default the outputs are pulled to the power supply. The microcontroller and the display need to be powered from 3 volts.
