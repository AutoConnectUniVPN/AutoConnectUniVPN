# AutoConnectUniVPN
Hier gehts zu den [Downloads](#downloads).

## Aufgabe des Programms
**Kurz:** Mit nur einem Klick mit der Heidelberger Uni VPN verbinden.

AutoConnectUniVPN entstand aus dem Problem heraus, dass Moodle nur noch über das Netwerk der Uni Heidelberg erreichbar 
gemacht wurde und ich keine Lust hatte, jedes Mal meine Uni-Anmeldedaten händisch eingeben und einen 2FA Code generieren
lassen zu müssen.\
Dieses Programm speichert die Anmeldedaten auf wunsch des Nutzers und generiert durch einen eingebauten OTP Generator 
(der genauso funktioniert wie jede andere Authenticator-App) den 2FA Code. Die Anmeldedaten werden an den Cisco Secure 
Client der Uni weitergeleitet und die Verbindung mit der VPN wird automatisch hergestellt.

## Voraussetzungen
Damit die App funktioniert, braucht man Folgendes:
 - 'Cisco Secure Client - AnyConnect' muss installiert sein ([Download Link](https://vpn-ac.urz.uni-heidelberg.de/+CSCOE+/logon.html))
 - Cisco darf nicht im Autostart liegen
 - Den Seed eines MFA Tokens, um den 2FA Code generieren zu können ([s. FAQ Seed finden](#wie-finde-ich-den-seed-meines-mfa-tokens))
 - Uni-ID und Uni-Passwort

## Downloads
 - Fehlt


# FAQ
### Wie finde ich den Seed meines MFA Tokens?
Gehe in folgender [Anleitung](https://www.urz.uni-heidelberg.de/de/support/anleitungen/keepassxc-als-token-einrichten)
zum Kapitel "Token erstellen" und folge ihr bis zum letzten Punkt, wo das "secret" 
ausgelesen wird. Direkt unter der Zeile mit dem secret befindet sind der Seed, beginnend mit "seed://" 
(z.B. seed://b7ae5a6190dbf7f3632b44c81aecada9615c7ab0). Du kannst entweder die ganze Zeile oder nur den Teil hinter
"seed://" (dem eigentlichen seed) verwenden, um dich anzumelden.

### Wie werden meine Anmeldedaten gespeichert?
Deine Anmeldedaten werden immer verschlüsselt abgespeichert! Wenn du nur deine Uni ID und den Seed abspeichern lässt, 
wird dein Uni-Passwort zum Verschlüsseln der Daten benutzt. Soll auch das Uni-Passwort gespeichert werden, wird 
empfohlen, ein neues Password zur Verschlüsselung anzugeben, AUCH wenn es möglich ist, das Passwortfeld 
freizulassen.

Wird das Passwortfeld freigelassen, wird ein Default-Passwort verwendet. Dieses ist zwar nicht dasselbe
wie das im Source Code (123Zauberei), trotzdem ist es mit genügend krimineller Energie durch Reverse Engineering möglich
an dieses Passwort zu gelangen. Dann müsste der Hacker aber immer noch Zugriff auf euren PC bekommen, was hoffentlich
nicht so wahrscheinlich ist :)

**Für Interessierte:**\
Die Daten werden mithilfe der OpenSSL Bibliothek (dem Quasi-Standard für C++ Verschlüsselung)
verschlüsselt. Als Verschlüsselungsverfahren wird AES (Advanced Encryption Standard) mit 256 Bit Schlüssellänge und
zufälligem 128 Bit IV (Initialization Vector) verwendet.\
Um den 256 Bit Schlüssel aus dem Password zu erzeugen wird PBKDF2 mit HMAC-SHA1 und 100000 Iterationen benutzt, 
was es sehr, sehr schwer macht aus dem (eh geheimen und individuellem) Schlüssel das Passwort zu berechnen.\
Das hat folgenden schönen Vorteil: 
Wenn ihr euer Uni-Passwort nicht mit abspeichern lasst, wird dieses zum Verschlüsseln der Uni-ID und dem Seed verwendet.
Sollte der Hacker jetzt irgendwie euren Schlüssel knacken, bedeutet dass, dass er nur den Schlüssel, eure Uni-ID und 
euren Seed, aber nicht euer Uni-Passwort hat! Und den Seed kann man ggf. löschen und einen neuen generieren.

### Mein Programm stürzt ab oder tut nicht was es soll, wat nu?
Es kann leider immer passieren, dass das Programm nicht tut, was es soll, weil der Cisco Secure Client ein Update
bekommen hat oder ich einen bestimmten Fall bei der Programmierung nicht abgedeckt habe.

Das kann helfen:
 - Cisco Secure Client aus dem Autostart des Betriebssystems entfernen und PC neu starten
 - Cisco Secure Client nicht vorher manuell öffnen, da dabei ein Hintergrund Prozess gestartet wird, wodurch ein Konflikt 
   mit AutoConnectUniVPN entsteht
 - Evt. muss Cisco ein Update machen, dann einfach über Cisco manuell mit der Uni-VPN verbinden und danach PC neu starten

Ansonsten schreib gerne einen kurzen Bug-Report und ich werden mich vielleicht irgendwann darum kümmern.

### Warum wird bei mir hin und wieder kurz ein schwarzes Fenster angezeigt?
Jedes Mal, wenn eine Interaktion mit dem Cisco Secure Client stattfindet, wird eine Konsole im Hintergrund
geöffnet, über die die Kommunikation stattfindet. Leider liegt es an Windows, dass die Konsole nicht wie bei anderen 
Systemen gar nicht angezeigt wird. 

# License
Siehe [LICENSE.txt](LICENSE.txt)