

---

````markdown
# 🔐 STM32 Smart Lock System (Bare-Metal C)

## 📌 Project Overview
This project is a **Protected Door Lock System** implemented on an **STM32F407G-DISC1** board, programmed in **pure bare-metal C** (no HAL, no RTOS in this version).  
It uses:
- **16x2 LCD** in 4-bit mode (for user messages)
- **4x3 Keypad** (for password entry)
- **IR Sensor** (for detecting user presence)

The system enforces **increasing delays** for wrong password attempts and locks completely after 4 failed tries.

---

## 🎯 Features
- **Active LOW IR sensor detection** to start password entry
- **4-digit password authentication**
- **Delay penalties**:
  - 1st wrong: 10 sec
  - 2nd wrong: 20 sec
  - 3rd wrong: 30 sec
  - 4th wrong: Permanent lock
- **Masked password entry** on LCD
- **Pure register-level programming** (no HAL, no libraries)

---

## 🛠 Components Used
| Component            | Description |
|----------------------|-------------|
| STM32F407G-DISC1     | Cortex-M4 development board |
| 16x2 LCD Display     | HD44780 compatible, 4-bit mode |
| 4x3 Matrix Keypad    | For user password input |
| IR Sensor (Active Low)| Detects presence for password prompt |
| Jumper Wires         | For connections |

---

## 🔌 Pin Connections
### LCD (4-bit mode)
| LCD Pin | STM32 Pin |
|---------|-----------|
| RS      | PA0       |
| EN      | PA1       |
| D4      | PA4       |
| D5      | PA5       |
| D6      | PA6       |
| D7      | PA7       |

### Keypad (4x3)
| Keypad Pin | STM32 Pin |
|------------|-----------|
| R1         | PB0       |
| R2         | PB1       |
| R3         | PB2       |
| R4         | PB3       |
| C1         | PB4       |
| C2         | PB5       |
| C3         | PB6       |

### IR Sensor
| IR Sensor Pin | STM32 Pin |
|---------------|-----------|
| OUT           | PC0       |

---

## 🧠 Methodology
1. **Hardware Initialization**
   - Configure GPIO for LCD, keypad, and IR sensor.
   - Initialize SysTick for delays.
2. **LCD & Keypad Setup**
   - LCD in 4-bit mode.
   - Keypad scanning via row/column method.
3. **IR Sensor Integration**
   - Monitor IR sensor state.
   - Trigger password entry on detection.
4. **Password Authentication**
   - Accept 4-digit password.
   - Compare with predefined password.
5. **Security Feedback**
   - Access granted or denied messages.
   - Delay penalties for wrong attempts.
6. **System Lock**
   - After 4 wrong attempts, lock permanently.

---

## 🖥 Block Diagram
```plaintext
   +--------------+
   |   IR Sensor  |
   +------+-------+
          |
          v
   +--------------+
   | STM32F407G   |
   |              |
   |  +--------+  |
   |  | Keypad |  |
   |  +--------+  |
   |  +--------+  |
   |  |  LCD   |  |
   |  +--------+  |
   +--------------+
          |
          v
     Door Lock Logic
````

---

## ▶ How to Run

1. Open project in **STM32CubeIDE**.
2. Remove HAL includes (`stm32f4xx_hal.h`) if present.
3. Paste provided **main.c** and **smart\_lock.c** into `Core/Src/`.
4. Build and flash to STM32F407G-DISC1.
5. Connect LCD, keypad, and IR sensor as per above table.
6. Power the board and test.

---

## 📂 Project Structure

```
STM32_Smart_Lock/
├── Core/
│   ├── Src/
│   │   ├── main.c
│   │   ├── smart_lock.c
│   ├── Inc/
│       ├── smart_lock.h
├── README.md
```

---

## 📹 Demo Video

*i will add video here*

---

## 📌 Future Improvements

* Add **FreeRTOS** for multitasking keypad, LCD, and IR sensor tasks.
* Implement **buzzer** for alerts.
* Use **EEPROM** to store wrong attempts count.
* Integrate **Bluetooth / Wi-Fi** for remote unlock.
* Add **biometric authentication** (fingerprint).

---

## 👨‍💻 Author

**Rajagiri Naresh Kumar**
Embedded Systems Enthusiast | Bare-metal C Programmer
