# Arduino Rear Fog Light Controller

**A traditional, reliable, and road-legal rear fog light control system for cars using an Arduino, relays, and a single pushbutton.**

---

## 🚦 Project Overview

This project provides a robust solution for controlling a vehicle’s rear fog light according to European (UNECE) road regulations.  
It ensures:
- The fog light can only be toggled ON or OFF with a pushbutton when the low or high beam headlights are ON.
- The fog light automatically turns OFF if the headlights are turned off (reset for safety).
- Uses only standard, inexpensive components.
- All logic, toggling, and reset are handled by the Arduino — no relays or flip-flop modules required.

---

## 🛠️ Features

- **Single pushbutton toggles fog light ON/OFF** (one press = ON, next press = OFF).
- **Only works if low beam or high beam is active** (prevents misuse).
- **Auto-reset:** Fog light always turns OFF when beams are switched OFF, and stays OFF when beams come back on.
- **All logic handled in software** — no complex relay wiring.
- **Debug output** via Serial Monitor for troubleshooting.

---

## ⚡ Hardware Requirements

| Item                | Example/Notes                  |
|---------------------|-------------------------------|
| Arduino board       | Nano, Uno, Mini, etc.          |
| 1-channel relay module | Active LOW type (e.g., Songle SRD-05VDC-SL-C) |
| Pushbutton switch   | Momentary, normally open (NO)  |
| Resistors           | 10kΩ + 4.7kΩ for each 12V beam input (voltage divider) |
| Diode (optional)    | 1N4007 or 1N4148 for protection |
| Wires/connectors    | Automotive rated recommended   |

---

## 🔌 Wiring Overview

1. **Pushbutton:**  
   - One leg to Arduino GND  
   - Other leg to Arduino D2

2. **Relay Module:**  
   - VCC to Arduino 5V  
   - GND to Arduino GND  
   - IN to Arduino D5  
   - Relay contacts switch the **12V supply** to rear fog light (do NOT power lamp from Arduino!)

3. **Beam Inputs (low/high):**  
   - Each beam wire (12V when ON, 0V when OFF) → [10kΩ] → [Arduino D3/D4] → [4.7kΩ] → GND  
   - (Arduino D3: low beam, D4: high beam)

4. **Optional:**  
   - Diode in series with 10kΩ to protect against voltage spikes  
   - 0.1uF capacitor from button pin to GND for hardware debounce

---

## 🚘 Operation Logic

- If either beam input is HIGH (beams ON), pushbutton toggles the fog lamp ON or OFF.
- If both beam inputs are LOW (OFF), fog lamp is forced OFF and stays OFF until the button is pressed again after beams return.
- Legal for EU road use: fog cannot be left ON accidentally when lights are cycled.

---

## 📝 Setup & Usage

1. Upload the provided Arduino sketch to your board.
2. Connect all hardware as per the diagram and table above.
3. Open Serial Monitor (9600 baud) for live debug info.
4. Test:  
   - With both beams OFF, button does nothing, fog is OFF.  
   - With either beam ON, button toggles fog.  
   - Turn beams OFF: fog is forced OFF/reset.

---

## ⚠️ Road Safety Disclaimer

This project is intended for educational and experimental use.  
**Always comply with local and EU automotive lighting laws.**  
Do not modify road vehicles unless you are qualified and understand legal requirements for rear fog lights.

---

## 🤝 Contributing

- Issues and improvements welcome via GitHub Issues or Pull Requests.
- Please maintain traditional, readable, and well-documented code style.

---

## 📜 License

MIT License.  
Please attribute and link back to this repository if you use or modify this project.

---

## 👨‍🔧 About

Created by Rihards Gailums (2025)  
For traditional automotive electronics and Arduino enthusiasts.


