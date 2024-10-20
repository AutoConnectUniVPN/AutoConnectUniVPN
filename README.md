# Auto Connect Uni VPN
Hier gehts zu den [Downloads](#downloads).

## Aufgabe des Programms
**Kurz:** Mit nur einem Klick mit der Heidelberger Uni VPN verbinden.

Auto Connect Uni VPN entstand aus dem Problem heraus, dass Moodle nur noch über das Netwerk der Uni Heidelberg erreichbar 
gemacht wurde und es zu umständlich war, jedes Mal die Uni-Anmeldedaten händisch eingeben und einen 2FA Code generieren
lassen zu müssen.\
Dieses Programm speichert die Anmeldedaten auf wunsch des Nutzers und generiert durch einen eingebauten OTP Generator 
(der genauso funktioniert wie jede andere Authenticator-App) den 2FA Code. Die Anmeldedaten werden an den Cisco Secure 
Client der Uni weitergeleitet, wodurch automatisch die Verbindung mit der VPN hergestellt wird.

## Voraussetzungen
Damit die App funktioniert, braucht man Folgendes:
 - 'Cisco Secure Client - AnyConnect' muss installiert sein ([Download Link](https://vpn-ac.urz.uni-heidelberg.de/+CSCOE+/logon.html))
 - Cisco darf **_nicht_** im Autostart liegen oder vorher gestartet werden. Ansonsten kann es sein, dass Auto Connect 
   Uni VPN sich aufhängt! Nach dem Entfernen aus dem Autostart **_PC neu starten_** oder Cisco im Task Manager schließen.
 - Den Seed eines MFA Tokens, um den 2FA Code generieren zu können ([s. FAQ Seed finden](#wie-finde-ich-den-seed-meines-mfa-tokens))
 - Uni-ID und Uni-Passwort

## Downloads
Vorher bitte [Voraussetzungen](#voraussetzungen) lesen. **Laden Sie das Programm bitte nur über diese Webseite herunter!** Da es Open 
Source ist, kann jeder davon einen modifizierten Klon erstellen, der die Absicht hat, eure Anmeldedaten zu stehlen.

[Windows Setup Version 1.0.8](https://raw.githubusercontent.com/AutoConnectUniVPN/AutoConnectUniVPN/refs/heads/main/_Downloads/Auto_Connect_Uni_VPN-1.0.8-Setup.exe)
[Linux Appimage Version 1.0.8](https://raw.githubusercontent.com/AutoConnectUniVPN/AutoConnectUniVPN/refs/heads/main/_Downloads/Auto_Connect_Uni_VPN-1.0.8-x86_64.AppImage)

# FAQ
### Wie finde ich den Seed meines MFA Tokens?
Gehe in folgender [Anleitung](https://www.urz.uni-heidelberg.de/de/support/anleitungen/keepassxc-als-token-einrichten)
zum Kapitel "Token erstellen" und folge ihr bis zum letzten Punkt, wo das "secret" 
ausgelesen wird. Direkt unter der Zeile mit dem secret befindet sind der Seed, beginnend mit "seed://" 
(z.B. seed://b7ae5a6190dbf7f3632b44c81aecada9615c7ab0). Du kannst entweder die ganze Zeile oder nur den Teil hinter
"seed://" (dem eigentlichen seed) verwenden, um dich anzumelden.

### Wie aktualisiere ich Auto Connect Uni VPN?
Lade einfach die neue Setup-Installationsdatei herunter und installiere diese über die bestehende Installation (mit 
selbem Installationspfad wie die alte Installation). Dadurch wird die alte ersetzt.

### Wie werden meine Anmeldedaten gespeichert?
Deine Anmeldedaten werden immer verschlüsselt abgespeichert! Wenn du nur deine Uni-ID und den Seed abspeichern lässt, 
wird dein Uni-Passwort zum Verschlüsseln der Daten benutzt. Soll auch das Uni-Passwort gespeichert werden, wird 
empfohlen, ein neues Password zur Verschlüsselung anzugeben, AUCH wenn es möglich ist, das Passwortfeld 
freizulassen.

Wird das Passwortfeld freigelassen, wird ein Default-Passwort verwendet. Dieses ist zwar nicht dasselbe
wie das im Source Code (keine Garantie!), trotzdem kann jeder, der Zugriff auf euren PC und genügend 
Programmiererfahrung hat, ohne Probleme eure Anmeldedaten entschlüsseln, wenn das Default-Passwort verwendet wurde!

**Für Interessierte:**\
Die Daten werden mithilfe der OpenSSL Bibliothek (dem Quasi-Standard für C++ Verschlüsselung)
verschlüsselt. Als Verschlüsselungsverfahren wird AES (Advanced Encryption Standard) mit 256 Bit Schlüssellänge und
zufälligem 128 Bit IV (Initialization Vector) verwendet.\
Um den 256 Bit Schlüssel aus dem Password zu erzeugen wird PBKDF2 mit HMAC-SHA1 benutzt, 
was es sehr, sehr schwer macht, aus dem (eh geheimen und individuellem) Schlüssel das Passwort zurückzurechnen.\
Das hat folgenden schönen Vorteil: 
Wenn ihr euer Uni-Passwort nicht mit abspeichern lasst, wird dieses zum Verschlüsseln der Uni-ID und dem Seed verwendet.
Sollte der Hacker jetzt irgendwie euren Schlüssel knacken, bedeutet dass, dass er nur den Schlüssel, eure Uni-ID und 
euren Seed, aber nicht euer Uni-Passwort hat! Und den Seed kann man ggf. löschen und einen neuen generieren lassen.

### Mein Programm stürzt ab oder tut nicht was es soll, wat nu?
Es kann leider immer passieren, dass das Programm nicht tut, was es soll, weil der Cisco Secure Client ein Update
bekommen hat oder bei der Programmierung ein bestimmter Fall nicht abgedeckt wurde.

Das kann helfen:
 - Cisco Secure Client aus dem Autostart des Betriebssystems entfernen und PC neu starten
 - Cisco Secure Client nicht vorher manuell öffnen, da dabei ein Hintergrund Prozess gestartet wird, wodurch ein Konflikt 
   mit Auto Connect Uni VPN entsteht
 - Evt. muss Cisco ein Update machen, dann über Cisco manuell mit der Uni-VPN verbinden und danach PC neu starten

Ansonsten schreib gerne einen kurzen Bug-Report und ich werden mich hoffentlich irgendwann darum kümmern.

### Warum wird bei mir hin und wieder kurz ein schwarzes Fenster angezeigt?
Jedes Mal, wenn eine Interaktion mit dem Cisco Secure Client stattfindet, wird eine Konsole im Hintergrund
geöffnet, über die die Kommunikation stattfindet (z.B. laden des Verbindungsstatus). Leider liegt es an Windows, dass die Konsole nicht wie bei anderen 
Systemen sofort versteckt wird.

### Wie deinstalliere ich Auto Connect Uni VPN?
#### Windows
Das Programm lässt sich normal über die Systemsteuerungen deinstallieren. Falls das aus einem beliebigen Grund
nicht funktioniert kann man stattdessen auch den Installationsordner öffnen und dort die unins000.exe ausführen.

Um die verschlüsselten Anmeldedaten zu löschen, muss man jedoch vorher in der App auf Settings | Reset gehen.

# Auto Connect Uni VPN selbst kompilieren
### Windows
 1. Installiere: QT Creator, QT 6, LLCM-MinGW 17.* 64-bit, CMake >= 3.16, Ninja (Alles im QT Online Installer auswählbar).\
    Wenn du sicher gehen willst, installierst du dir auch noch den MinGW 13.* Compiler (auch mit im QT Online Installer)
 2. Projekt im QT Creator öffnen
 3. In Projektkonfiguration _nur_ Desktop QT 6.* llvm-mingw 64-bit auswählen (auch bei Release hacken setzen)
 4. Ganz unten auf Projekt konfigurieren gehen
 5. Links unten (3tes icon über dem Hammer) Release auswählen
 6. Programm ausführen: Oberes grünes Dreieck drücken (2tes Symbol über Hammer)
 7. Warten bis fertigerstellt wurde (Balken rechts unten, bzw. warten bis Auto Connect Uni VPN startet)
 8. Auto Connect Uni VPN schließen
 9. Projektordner im Explorer öffnen. Dort zu build -> Desktop_Qt_6_*_MinGW_64_bit-Release
10. Wenn man dort die AutoConnectUniVPN.exe ausführt, bekommt man die Meldung, dass DLL files fehlen, 
    daher müssen die noch eingebunden werden
11. LLVM-MinGW Compiler öffnen (z.B. in der Windowssuche nach 'llvm-mingw' suchen)
12. AutoConnectUniVPN.exe in neuen leeren Ordner kopieren (Ordner muss auf dem selben Datenträger liegen, wie die 
    LLVM-MinGW Installation. Der Installationspfad wird im bereits geöffneten LLVM-MinGW Compiler angezeigt)
13. In dem Terminal vom Compiler den Ordner mit der enthaltenen AutoConnectUniVPN.exe öffnen (Befehl: cd "Pfad\zum\Ordner", z.B. cd "C:\User\Me\Folder")
14. 'windeployqt AutoConnectUniVPN.exe' eingeben, um QT Bibliotheken (DLLs) hinzuzufügen
15. Jetzt sollten in dem Ordner, in dem vorher nur die AutoConnectUniVPN.exe lag, viele weitere Dateien und Ordner liegen
16. Durch das Ausführen der AutoConnectUniVPN.exe erfährt man, dass immer noch Bibliotheken fehlen
17. Um die restlichen DLL Dateien hinzuzufügen, müssen die .dll files aus dem [.dll](.dll) Ordner der GitHub Repository 
    in den Ordner mit der AutoConnectUniVPN.exe reinkopiert werden. Du kannst diese DLLs auch in deinem eigenen 
    Installationsordner der beiden Compiler finden, dann musst du aber beispielsweise die libcrypto*.dll noch umbenennen.
18. Jetzt sollte das Programm ausführbar sein. Der Ordner kann als ganzes an einen beliebigen Ort verschoben werden.
19. Sollten immer noch Bibliotheken fehlen, könnte es sein, dass du den MinGW und nicht den LLVM-MinGW Compiler in den 
    Projekteinstellungen des QT Creators ausgewählt hast (dann am besten nochmal alles löschen und von vorne beginnen)


# Lizenz
Siehe [LICENSE](LICENSE)
