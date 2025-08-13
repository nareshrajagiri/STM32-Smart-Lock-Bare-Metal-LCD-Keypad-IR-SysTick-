# STM32 Smart Lock — Bare-Metal (LCD + Keypad + IR + SysTick)

A protected door lock system on **STM32F407G-DISC1** written in **pure bare-metal C** (no HAL, no FreeRTOS).  
Uses a **16x2 LCD (4-bit)**, **4x3 matrix keypad**, **IR sensor (active LOW)**, and a **progressive penalty** (10s → 20s → 30s) with lockout on the 4th failure.

> Demo:  
> - Idle → `Waiting for user`  
> - IR detects presence → `Enter Password`  
> - Wrong password → `Wrong Password / Wait 10 sec` (countdown)  
> - Next wrong → `20 sec`, then `30 sec`  
> - 4th wrong → `System Locked!`

---

## Features
- Bare-metal register programming (RCC, GPIO, SysTick)
- LCD 16×2 in 4-bit mode (PA0..PA5)
- 4×3 Keypad scanning (PB0..PB6)
- IR sensor trigger on PC0 (**active LOW**)
- Password auth (`"1234"`, masked `****`)
- Progressive penalties + final lockout
- Single-file firmware (`Src/main.c`) for clarity

---

## Hardware

### Bill of Materials
- STM32F407G-DISC1 board
- LCD 16×2 (HD44780 compatible)
- 4×3 matrix keypad
- IR sensor module (digital output, active LOW)
- 10k potentiometer (LCD contrast)
- Jumper wires, breadboard
- (Optional) series resistor for LCD backlight

### Connections

**LCD (16×2, 4-bit mode) — GPIOA**
| LCD Pin | Signal | STM32 Pin | Mode   |
|--------:|--------|-----------|--------|
| 1 (VSS) | GND    | —         | —      |
| 2 (VDD) | +5V    | —         | —      |
| 3 (V0)  | Contrast (pot wiper, ends to +5V/GND) | — | — |
| 4       | RS     | **PA0**   | Output |
| 5       | RW     | **GND**   | —      |
| 6       | EN     | **PA1**   | Output |
| 11      | D4     | **PA2**   | Output |
| 12      | D5     | **PA3**   | Output |
| 13      | D6     | **PA4**   | Output |
| 14      | D7     | **PA5**   | Output |
| 15 (A)  | LED+   | +5V (via resistor if needed) | — |
| 16 (K)  | LED−   | GND       | —      |

**Keypad (4×3) — GPIOB**
| Keypad | STM32 Pin | Mode                 |
|--------|-----------|----------------------|
| R1     | **PB0**   | Input + Pull-Up      |
| R2     | **PB1**   | Input + Pull-Up      |
| R3     | **PB2**   | Input + Pull-Up      |
| R4     | **PB3**   | Input + Pull-Up      |
| C1     | **PB4**   | Output               |
| C2     | **PB5**   | Output               |
| C3     | **PB6**   | Output               |

**IR Sensor — GPIOC**
| IR Pin | STM32 Pin | Mode  | Note              |
|--------|-----------|-------|-------------------|
| OUT    | **PC0**   | Input | **Active LOW = detected** |
| VCC    | +5V       | —     |                   |
| GND    | GND       | —     |                   |

> Note: LCD is powered at 5V; STM32 I/O is 3.3V. Most HD44780 modules accept 3.3V as HIGH. Ensure **common GND**. If characters are unstable, add level shifting.


## High-Level Block Diagram



<img width="1101" height="628" alt="image" src="https://github.com/user-attachments/assets/5f2d7aab-5896-4771-a4ca-e00f18200e87" />




## Methodology (What the firmware does)
1. Hardware Init
- Enable clocks (RCC) for GPIOA/B/C. Configure:

- PA0–PA5 → LCD outputs

- PB0–PB3 → keypad row inputs with pull-ups; PB4–PB6 → column outputs

- PC0 → IR input (active LOW)

2. Timing with SysTick
- Configure SysTick for a 1 ms tick using SystemCoreClock.

3. LCD & Keypad
- Initialize LCD in 4-bit mode; scan keypad by driving columns and reading rows.

4. IR-Triggered Auth
- Idle: Waiting for user. On IR LOW → show Enter Password.

5. Password Handling
- Read 4 keys, display as ****, compare with "1234".

6. Security & Feedback

- Correct → Access Granted (3s)

- Wrong → penalties: 10s, 20s, 30s (LCD countdown)

- 4th wrong → System Locked! (no input)

7. Continuous Loop
- Monitor → Authenticate → Feedback → repeat.

## Build & Flash (STM32CubeIDE, No HAL)
1. Create Project: New STM32 Project → MCU STM32F407VG (DISC1).

2. No HAL: Choose empty project or delete HAL sources if auto-generated. Keep:

 - startup_stm32f407xx.s

 - system_stm32f4xx.c (provides SystemCoreClock)

3. Add Firmware: Create Src/main.c and paste this repo’s main.c.

4. Build & Flash: Connect board (ST-Link), click Run/Debug.

5. What you should see: LCD → Waiting for user.

## Troubleshooting
- LCD blank → adjust contrast (V0), check RW→GND, wiring PA0..PA5, common GND.

- Garbled text → verify 4-bit pins D4..D7 only; check delays.

- Keypad unresponsive → PB0..PB3 rows with pull-ups, PB4..PB6 columns as outputs.

- IR not detected → this code expects LOW = detected on PC0.

- Delays weird → check SystemCoreClock value in system_stm32f4xx.c.

## Future Improvements
- Buzzer + relay/servo for actual door control

- Change-password menu, store in Flash/EEPROM

- UART logs; BLE/Wi-Fi connectivity

- FreeRTOS tasks (IR, keypad, LCD, buzzer) + MISRA-C cleanup
