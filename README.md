# ESP32-TesseraOS
![TesseraOS logo](Logo.png)
### **[ENG ver.]**
## What is this?
Firmware for ESP32 paired with a couple of displays. In my implementation, this will be a small 3D-printed monitor case. It includes weather functionality (from both an onboard sensor and API), a task manager (in my case, from the YouGille CRM), time zones for different regions, a mini-game, and settings. The project features a fairly simple system for adding new pages. All you need to do is create two files inherited from Page.h and write your logic. All of this will be described in more detail later, once the code actually starts looking like something functional.

## Project Goal
I started all of this for work and for self-development. I've always been interested in learning programming but never had a clear goal — now I do.
The final destination of this project will be a full ESP32 OS.
I'd be glad to receive your comments, corrections, and commits. You'll not only support the project but also make an invaluable contribution to raising yet another self-taught programmer.

## Hardware Used
1. ESP32 DevKit V1
2. ST7735 TFT 1.8" (SPI)
3. SSD1306 OLED 0.91" (i2c)
4. DHT 22

## What already works
- [x] Menu
- [x] Page system (PageManager)
- [x] Return to home
- [ ] Screen Saver (in progress)
- [ ] Weather page (in progress)
- [ ] Tasks page (in progress)
- [ ] Time zones (in progress)
- [ ] Mini-games (in progress)
- [x] Settings
- [ ] DHT22 integration (in progress)
## =============================================
### **[RUS ver.]**
## Что это такое? 
Прошивка для ESP32 в связке с парочкой экранов. В моей реализации это будет небольшой напечатанный на 3д принтере корпус монитора. Она имеет в себе функционал погоды (С датчика в корпусе и из API), менеджера задач (В моём случае из CRM YouGille, Часовые пояса по разным регионам, мини игра, настройки. В проекте реализована достаточно простая система добавления страниц. Всё что вам необходимо это создать два файла из родителя Page.h и написать логику. Всё это будет описано чуть позже, когда сам код будет хоть немного походить на что то работоспособное.

## Цель проекта
Всё это я затеял для работы и для своего саморазвития. Мне всегда было интересно изучить программирование но у меня не было цели, теперь она есть.
Окончательная точка этого проекта будет ESP32 OS.
Буду рад вашим комментариям, поправкам, коммитам. Вы окажете не только поддержку проекту но и создадите неоценимый вклад в воспитании ещё одного программиста самоучки.

## Используемые устройства 
1. ESP32 DevKit V1
2. ST7735 TFT 1.8" (SPI)
3. SSD1306 OLED 0.91" (i2c)
4. DHT 22

## Что уже работает 
- [x] Меню
- [x] Система страниц (PageManager)
- [x] Возврат на главную из любой страницы
- [ ] Заставка (Screen Saver) (в работе)
- [ ] Страница погоды (в работе)
- [ ] Страница задач (в работе)
- [ ] Часовые пояса (в работе)
- [ ] Мини-игры (в работе)
- [x] Настройки
- [ ] Интеграция DHT22 (в работе)

## Libaries Used // Используемые библиотеки
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit ST7735 Library](https://github.com/adafruit/Adafruit-ST7735-Library)
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)
- [OneButton](https://github.com/mathertel/OneButton)
