# PIC16F84-Simulator
Simulator for the PIC16F84

## Components
- **Parser**: Parses `.LST` files into executable instructions.
- **Decoder**: Decodes instructions into operations.
- **Simulator VM**: Executes the instructions using a virtual machine.

## Usage
1. Compile the project using a C++ compiler.
2. Run the simulator with a `.LST` file as an argument:
   ```
   ./simulator TPicSim1.LST
   ```
3. View the output and debug information in the console.

## Example
To test the simulator, use one of the provided `.LST` files in the `testprog` directory.
```
./simulator ../testprog/TPicSim1.LST
```

| Programm | Inhalt | Max. Erreich. | Erledigt |
|----------|--------|---------------|----------|
| 1 (MUSS) | Die einfachen Literalbefehle, u.a. MOVLW, ADDLW, SUBLW | 6 |    |
| 2 / 21 (MUSS) | u.a. CALL, GOTO (vereinfacht, ohne Rücksicht auf PCLATH) | 1 |
| 3 (MUSS) | u.a. MOVWF, MOVF, SUBWF (nur direkte Adressierung, aber mit d-Bit Auswert.) | 1 |
| 4 (MUSS) | u.a. DCFSZ, INCFSZ, RLF, RRF (nur direkte Adressierung, aber mit d-Bit Auswert.) | 1 |
| 5 (MUSS) | u.a. BSF, BCF, BTFSC, BTFSS (direkt und indirekt Adressierung) | 2 |
| 6 (MUSS) | u.a. Bytebefehle, (direkte und indirekte Adressierung) | 2 |
| 7 (MUSS) | Timerfunktion ohne / mit Berücksichtigung der Bits im OPTION-Register (e / o)2 | 2 / 5 |
| 8 (MUSS) | Interrupt für Timer 0, Interrupt für INT (RB0) mind. 1 Interrupt muss realisiert werden, Interrupt für RB4-RB7 | 5, 4, 3 |
| 9 | u.a. SLEEP | 3 |
| 10 / 101 (MUSS) | ADDWF PCL mit Berücksichtigung von PCLATH (TPicSim101) (theor. Für CALL und GOTO; geprüft an Hand des Codes) | 5 (6), 4 |
| 11 | Watchdog ohne Vorteiler / u.a. Watchdog mit Vorteiler (e / o)2 | 3 / 5 |
| 12 | EEPROM ohne Programmierzeit / EEPROM inc. 1ms Programmierzeit (e / o)2 | 3 / 5 |
| 13 | Lauflicht | 1 |
| 14 | Leuchtband | 1 |
| 15 (MUSS) | I/O Ausgangslatch (Wirkung internes TRIS-Register auf Ausgangstreiber) | 4 |


| Pflicht | Eigenschaft | eigene GUI | Erledigt |
|---------|-------------|------------|----------|
| MUSS | Breakpoints | 3 |    |
| MUSS | Laufzeitzähler (visualisiert) | 2 |    |
| MUSS | Stimulation der I/O-Pins per Maus (Toggle-Funktion, Click-Ereignis) | 4 |    |
| | Frei wählbare Quarzfrequenz (im Zusammenhang mit dem Laufzeitzähler) | 3 |    |
| MUSS | Markieren des aktuellen (nächsten) Befehls im LST-Fenster | 2 |    |
| MUSS | Fenster für LST, SFR und GPR ohne / mit Reseteinstellungen in den SFRs | 1 / 2 |    |
| | Editmöglichkeit von SFR und GPR | 2 |    |
| MUSS | Visualisierung des Stacks | 2 |    |
| | Ggf. eigene Ideen realisieren incl. passendes Testprogr. (nur nach Absprache) | - |    |
| | Hardwareansteuerung (5 zusätzliche Punkte) | - |    |
| MUSS | Doku mit ausführlichem persönlichem Fazit | 4 |    |
| | Vorzeitige Abgabe: bis = 5 Pkte bzw. bis = 2 Pkte | - |    |
