//
// Created by admin on 24.09.24.
//

#ifndef AUTOCONNECTUNIVPN_LOGPROGRESSPHRASES_H
#define AUTOCONNECTUNIVPN_LOGPROGRESSPHRASES_H

#include <string>

class LogProgressPhrases {
public:
	// XX -> exists only in english
	inline const static std::string XX_1 = "Cisco Secure Client (version";
	inline const static std::string XX_5 = "Copyright";
	inline const static std::string EN_8 = "state: Disconnected";
	inline const static std::string DE_8 = "state: Verbindung getrennt";
	inline const static std::string XX_10 = "contacting host";
	inline const static std::string EN_20 = "Username";
	inline const static std::string DE_20 = "Benutzername";
	inline const static std::string XX_30 = "OTP";
	inline const static std::string EN_40 = "Please respond to banner";
	inline const static std::string DE_40 = "Antworten Sie auf das Banne";
	inline const static std::string XX_44 = "[off-campus]";
	inline const static std::string XX_46 = "HD-Net";
	inline const static std::string XX_50 = "accept?";
	inline const static std::string XX_53 = "[y/n]";
	inline const static std::string EN_56 = "Downloader is analyzing this computer";	// no counterpart in german
	inline const static std::string XX_60 = "Downloader is performing update checks";
	inline const static std::string EN_64 = "Checking for profile updates";
	inline const static std::string DE_64 = "Es wird nach Profilaktualisierungen gesucht";
	inline const static std::string EN_68 = "Checking for product updates";
	inline const static std::string DE_68 = "Es wird nach Produktaktualisierungen gesucht";
	inline const static std::string EN_72 = "Checking for customization updates";
	inline const static std::string DE_72 = "Es wird nach Anpassungsaktualisierungen gesucht";
	inline const static std::string EN_76 = "Performing any required updates";
	inline const static std::string DE_76 = "Erforderliche Aktualisierungen werden durchgef";
	inline const static std::string EN_77_1 = "Exiting";
	inline const static std::string EN_77_2 = "Upgrade in progress";
	// Todo: Finde heraus, was das echte deutsche Gegenstück zu EN_77 ist
	inline const static std::string DE_77_1_unsure1 = "beende";
	inline const static std::string DE_77_1_unsure2 = "verlasse";
	inline const static std::string XX_80 = "update checks have been completed";
	inline const static std::string EN_84 = "state: Connecting";
	inline const static std::string DE_84 = "state: Wird verbunden";
	inline const static std::string EN_86 = "Initiating connection";
	inline const static std::string DE_86 = "VPN wird hergestellt";
	inline const static std::string EN_90 = "Examining system";
	inline const static std::string DE_90 = "System wird gepr";
	inline const static std::string EN_94 = "Activating VPN adapter";
	inline const static std::string DE_94 = "VPN-Adapter wird aktiviert";
	inline const static std::string EN_96 = "Configuring system";
	inline const static std::string DE_96 = "System wird konfiguriert";
	inline const static std::string EN_100_1 = "state: Connected";
	inline const static std::string DE_100_1 = "state: Verbunden";
	inline const static std::string XX_100_2 = "note: VPN Connection is still active";
};

#endif //AUTOCONNECTUNIVPN_LOGPROGRESSPHRASES_H
