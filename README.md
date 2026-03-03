# HoloCube3D 🧊✨

A compact, animated desktop companion powered by the **ESP32-S3-WROOM-1 (N16R8)**. This project uses a high-speed IPS display and a **Beam Splitting Cube** to create a pseudo-holographic 3D animation effect.

![HoloCube Device](https://m.media-amazon.com/images/I/418SW+HhY1L._AC_UF894,1000_QL80_.jpg)

---

## 🛠 Materials Used
To build this version of the HoloCube3D, the following hardware is required:

* **Microcontroller:** ESP32-S3-WROOM-1 (N16R8) with 16MB Flash and 8MB PSRAM.
* **Display:** IPS 1.3 Inch TFT LCD Display (High contrast for better "floating" effects).
* **Optics:** Beam Splitting Cube (Splits light to create the holographic reflection).

---

## 🚀 Overview
The **HoloCube3D** renders smooth, high-frame-rate animations by cycling through pre-processed image frames stored directly in the ESP32-S3's memory. 

### Featured Animation
The repository includes a 18-frame looping animation:

![Animation Demo](https://media.tenor.com/d6YbAaUX9BQAAAAM/zero-two-smile.gif)

---

## 📂 File Structure
* `main.ino`: The primary Arduino sketch handling the LCD drivers and the animation loop.
* `frame_xxx.h`: Header files containing the raw hex pixel data for each frame.

---

## ⚙️ Quick Start
1.  **Library Requirement:** Install the [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) library in your Arduino IDE.
2.  **Configuration:** Edit `User_Setup.h` in the TFT_eSPI folder to match the pins for your **ESP32-S3** and **1.3" IPS LCD**.
3.  **Flash Settings:** * **Board:** ESP32S3 Dev Module
    * **PSRAM:** OPI (Required for N16R8)
    * **Flash Mode:** QIO/OPI 80MHz
4.  **Upload:** Open `main.ino` and upload. Ensure all `.h` files are in the same directory.

---

## 📜 License
This project is open-source. Feel free to swap the frame files to display your own custom 3D holograms!
