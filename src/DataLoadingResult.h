//
// Created by admin on 30.08.24.
//

#ifndef AUTOCONNECTUNIVPN_DATALOADINGRESULT_H
#define AUTOCONNECTUNIVPN_DATALOADINGRESULT_H

enum class DataLoadingResult {
	UserCanceled,
	NoConfigFileAndUiInput,
	DecryptionFailed,
	LoadedFromFile,
	LoadedFromUi
};

#endif //AUTOCONNECTUNIVPN_DATALOADINGRESULT_H
