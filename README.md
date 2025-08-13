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

```mermaid
flowchart LR
    IR[IR Sensor (PC0, Active LOW)] -->|Presence| MCU[(STM32F407)]
    Keypad[4x3 Keypad (PB0..PB6)] -->|Key Presses| MCU
    MCU -->|4-bit Data + RS/EN| LCD[LCD 16x2 (PA0..PA5)]
    MCU -->|1ms tick| SysTick[SysTick Timer]


<img width="1101" height="628" alt="image" src="https://github.com/user-attachments/assets/5f2d7aab-5896-4771-a4ca-e00f18200e87" />

