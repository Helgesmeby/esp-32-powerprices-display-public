# ESP32 e-paper powerprices display

Handy e-paperdisplay that updates every hour and displays power prices in an easily readable format. 

Created in Arduino, utilizes [GxEPD2](https://github.com/ZinggJM/GxEPD2) and [Adafruit GFX (Fonts)](https://github.com/adafruit/Adafruit-GFX-Library). 

### Items

* [ESP32 Dev Module](https://www.aliexpress.com/item/1005004268911484.html?spm=a2g0o.productlist.main.1.4e792e2aCbh6W3&algo_pvid=4d8414a7-8d67-4a7c-955c-ebdace48d5fa&algo_exp_id=4d8414a7-8d67-4a7c-955c-ebdace48d5fa-0&pdp_ext_f=%7B%22sku_id%22%3A%2212000028838235689%22%7D&pdp_npi=3%40dis%21EUR%213.52%212.85%21%21%21%21%21%40211bd3cb16802582304613136d0709%2112000028838235689%21sea%21NO%21198746603&curPageLogUid=CrUzD1s0la0t) 
* [Waveshare 4.2" e-paper display](https://www.aliexpress.com/item/32829561526.html?spm=a2g0o.productlist.main.5.721227bbt9xr9q&algo_pvid=f38b5b35-9099-463c-b4c7-aaeec421cf3b&algo_exp_id=f38b5b35-9099-463c-b4c7-aaeec421cf3b-2&pdp_ext_f=%7B%22sku_id%22%3A%2264968275656%22%7D&pdp_npi=3%40dis%21EUR%2138.47%2134.62%21%21%21%21%21%40212272e216802578308782883d071b%2164968275656%21sea%21NO%21198746603&curPageLogUid=2TRaQnf0JONY)

## Getting started

### Pin-connections:
| Display | ESP32  |  
|---|---|
| VCC  | 3V3  |
| GND  | GND  |
| DIN  | D23  |
| CLK  | D18  |
| CS   | D5   |
| DC   | D22  |
| RST  | D21  |
| BUSY | D4   |

### Running

1. Use the Arduino extension for vscode
2. Use Library Manager to load GxEPD and Adafruit GFX-libraries
2. The whole program is in the `src.ino`-file