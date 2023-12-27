
# Boat v4

Source code to control VESC 6 (75/300) and servo along with associated safety

## How to build

```
$ get_idf
$ idf.py build; idf.py -p /dev/cu.usbserial-0001 flash monitor
```


Select the instructions depending on Espressif chip installed on your development board:

- [ESP32 Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/index.html)
